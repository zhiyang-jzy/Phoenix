//
// Created by jzy99 on 2021/4/13.
//

#include<phoenix/core/texture.h>
#include<phoenix/core/common.h>


#include<stb/stb_image.h>

PHOENIX_NAMESPACE_BEGIN
    ImageTexture::ImageTexture(unsigned char *_data, int width, int height, int bytes_per_scanline) : data(_data),
                                                                                                      width(width),
                                                                                                      height(height),
                                                                                                      bytes_per_scanline(
                                                                                                              bytes_per_scanline) {
    }

    Color3f ImageTexture::GetColor(Point2f uv) const {
        float u = uv.x(), v = uv.y();
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (data == nullptr)
            return Color3f(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Clamp(u, 0.0, 1.0);
        v = 1.0 - Clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return Color3f(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

    bool ImageTexture::LoadFromFile(string path) {
        int nrChannels;
        data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);
        if (data)
            return true;
        return false;
    }

PHOENIX_NAMESPACE_END