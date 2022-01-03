#pragma once
#include "ImageAlgorithmStrategy.h"
#include "Picture.h"

class Sobel : public ImageAlgorithmStrategy
{
    public:
        Sobel(const float thresholdValue, const bool gradientOnly);
        Sobel();
        PicturePGM processImage(PicturePGM& pic) noexcept override;

    private:
        float thresholdValue_;
        bool gradientOnly_;
        constexpr static float sobel_Dx_[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
        constexpr static float sobel_Dy_[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

};