#pragma once
#include "ImageAlgorithmStrategy.h"
#include "Picture.h"

class Sobel : public ImageAlgorithmStrategy
{
    public:
        PicturePGM processImage(PicturePGM& pic, Config& c) noexcept override;

    private:
        bool isConfigValid(Config& c, float& threshold_value, bool& gradient_only);
        constexpr static float sobel_Dx_[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
        constexpr static float sobel_Dy_[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

};