//
// Created by 焦智洋 on 2021/4/21.
//

#ifndef PHOENIX_ENVMAP_H
#define PHOENIX_ENVMAP_H

#include<phoenix/core/phoenix.h>
#include<phoenix/core/bitmap.h>

PHOENIX_NAMESPACE_BEGIN

    typedef Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> FloatMat;

    class Envmap : public Bitmap {
    public:
        // ---------- CONSTRUCTORS
        Envmap(const std::string &path);

        Envmap();

        // ---------- MAIN FUNCTIONS
        //eval
        Color3f Eval(const Vector3f &dir) const;

        //sample
        Color3f Sample(Vector3f &dir, const Point2f &sample) const;

        //pdf
        float Pdf(const Vector3f &dir) const;

        // ---------- HELPER FUNCTIONS
        // samples a point
        Point2f SamplePixel(const Point2f &sample) const;

        // finds pdf of a point
        float FindPdf(const Point2f &point) const;

        // point (u,v) to color on map
        Color3f FindColor(const Point2f &point) const;

        //find pixel from direction
        Point2f DirToPixel(const Vector3f &vec) const;

        //find direction from pixel
        Vector3f PixelToDir(const Point2f &p) const;

    private:
        FloatMat luminance_,pdf_,cdf_,pmarg_,cmarg_;
    };


PHOENIX_NAMESPACE_END

#endif //PHOENIX_ENVMAP_H
