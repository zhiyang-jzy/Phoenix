//
// Created by 焦智洋 on 2021/4/21.
//

#include<phoenix/core/envmap.h>
#include<phoenix/core/frame.h>
#include<algorithm>
#include<cmath>

PHOENIX_NAMESPACE_BEGIN

    float Precompute1D(int row, const FloatMat &f, FloatMat &pf, FloatMat &Pf) {
        float nf = f.cols();

        float I = 0;
        for (int i = 0; i < nf; ++i)
            I += f(row, i);

        // otherwise we divide by 0
        if (I == 0)
            return I;

        for (int i = 0; i < nf; ++i)
            pf(row, i) = f(row, i) / I;

        Pf(row, 0) = 0;
        for (int i = 1; i < nf; ++i)
            Pf(row, i) = Pf(row, i - 1) + pf(row, i - 1);
        Pf(row, nf) = 1;

        return I;
    }

//helper function
    void Sample1D(int row, const FloatMat &pf, const FloatMat &Pf, const float &unif, float &x, float &p) {
        //binary search
        int i = 0;

        for (; i < Pf.cols(); ++i) {
            //std::cout << Pf(row, i) << "\n";
            if (unif >= Pf(row, i) && unif < Pf(row, i + 1))
                break;
        }

        float t = (Pf(row, i + 1) - unif) / (Pf(row, i + 1) - Pf(row, i));

        x = (1 - t) * i + t * (i + 1);
        p = pf(row, i);
    }

//helper function
/// Linearly interpolate between two colors
    Color3f ColLerp(float t, Color3f v1, Color3f v2) {
        return (1 - t) * v1 + t * v2;
    }

// bilinear interpolation
    Color3f Bilerp(float tx, float ty, Color3f v00, Color3f v01, Color3f v10, Color3f v11) {
        Color3f up = ColLerp(tx, v00, v01);
        Color3f bottom = ColLerp(tx, v10, v11);
        return ColLerp(ty, up, bottom);
    }

// ---------- CONSTRUCTORS
    Envmap::Envmap(const std::string &path)
            : Bitmap(path) {
        //resizing
        luminance_ = FloatMat(rows(), cols());
        pdf_ = FloatMat(rows(), cols());
        cdf_ = FloatMat(rows(), cols() + 1);
        pmarg_ = FloatMat(1, rows());
        cmarg_ = FloatMat(1, rows() + 1);

        //compute luminance matrix
        for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                Color3f color = (*this)(i, j);
                luminance_(i, j) = .3 * color.x() + .6 * color.y() + .1 * color.z() + EPSILON / 10000000;
            }
        }

        //precomputing the pdf and cdf values
        FloatMat colsum(1, rows());
        for (int i = 0; i < pdf_.rows(); ++i) {
            colsum(0, i) = Precompute1D(i, luminance_, pdf_, cdf_);
        }
        Precompute1D(0, colsum, pmarg_, cmarg_);
    }

    Envmap::Envmap()
            : Bitmap() {}

// ---------- MAIN FUNCTIONS
//eval
    Color3f Envmap::Eval(const Vector3f &dir) const {
        //std::cout << dir << "||";
        Point2f pixel = DirToPixel(dir);

        //std::cout << pixelToDir(pixel) << "\n \n \n";

        return FindColor(pixel);
    }

//sample
    Color3f Envmap::Sample(Vector3f &dir, const Point2f &sample) const {
        Point2f pixel = SamplePixel(sample);
        dir = PixelToDir(pixel);
        float jac = (cols() - 1) * (rows() - 1) / (2 * std::pow(M_PI, 2) * Frame::SinTheta(dir));
        float pdf = FindPdf(pixel) * jac;
        return FindColor(pixel) / pdf;
    }

//pdf
    float Envmap::Pdf(const Vector3f &dir) const {
        Point2f pixel = DirToPixel(dir);
        return FindPdf(pixel);
    }

// ---------- HELPER FUNCTIONS
    Point2f Envmap::SamplePixel(const Point2f &sample) const {
        float u, v;
        float pdfu, pdfv;
        Sample1D(0, pmarg_, cmarg_, sample.x(), u, pdfu);
        Sample1D(std::min(std::round(u), (float) rows() - 1), pdf_, cdf_, sample.y(), v, pdfv);

        return Point2f(std::min(u, (float) rows() - 1), v);
    }

    float Envmap::FindPdf(const Point2f &point) const {
        /*
        float u = point.x();
        float theta = u * M_PI / (rows() -1);

        float jac = (cols() -1) * (rows() -1) / (2 * std::pow(M_PI, 2) * std::sin(theta));

        */
        int i = std::floor(point.x());
        int j = std::floor(point.y());

        //std::cout << "(" << point.x() << "," << point.y() <<")  -- pmarg: " << m_pmarg(0, i) << " pdf: " << m_pdf(i, j) << " jac: " << jac <<" final: " << m_pmarg(0, i) * m_pdf(i, j) * jac << " \n";

        return pmarg_(0, i) * pdf_(i, j) /** jac*/;
        //float theta = M_PI * point.x() / rows();
        //return m_pmarg(0, i) * m_pdf(i, j) * rows() * cols() * std::pow(INV_PI, 2) / (2 * std::sin(theta));
    }

    Color3f Envmap::FindColor(const Point2f &point) const {
        int u = std::floor(point.x());
        int v = std::floor(point.y());

        //in corners and on borders
        if (u >= rows() - 1 || v >= cols() - 1) {
            return (*this)(u, v);
        }

        float du = point.x() - u;
        float dv = point.y() - v;

        Color3f v00 = (*this)(u, v);
        Color3f v01 = (*this)(u, v + 1);
        Color3f v10 = (*this)(u + 1, v);
        Color3f v11 = (*this)(u + 1, v + 1);

        return Bilerp(du, dv, v00, v01, v10, v11);
    }

    Point2f Envmap::DirToPixel(const Vector3f &vec) const {
        //float theta = std::acos(Frame::cosTheta(vec));
        //float phi = std::atan2(vec.y(), vec.x());

        Point2f coords = SphericalCoordinates(vec);

        float theta = coords.x();
        float phi = coords.y();

        float u = theta * INV_PI * (rows() - 1);
        float v = phi * INV_PI * 0.5 * (cols() - 1);

        //std::cout << u << "," << v << "\n";

        //if (u < 0 || v < 0)
        //	std::cout << "error negative pixel \n";

        if (std::isnan(u) || std::isnan(v)) {
            spdlog::error("No corresponding pixel for direction");
            return Point2f(0, 0);
        }

        return Point2f(u, v);
    }

    Vector3f Envmap::PixelToDir(const Point2f &p) const {
        float u = p.x();
        float v = p.y();

        float theta = u * M_PI / (rows() - 1);
        float phi = v * 2 * M_PI / (cols() - 1);

        return Vector3f(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta)).normalized();
    }


PHOENIX_NAMESPACE_END