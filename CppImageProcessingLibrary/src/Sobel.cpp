#include "Sobel.h"
#include <cmath>
#include <algorithm>

Sobel::Sobel(const float thresholdValue, const bool gradientOnly):thresholdValue_(thresholdValue), gradientOnly_(gradientOnly) 
{
}

Sobel::Sobel() : gradientOnly_(true), thresholdValue_(0.0f)
{
}

PicturePGM Sobel::processImage(PicturePGM& pic) noexcept
{
    if (pic.isEmpty())
        return PicturePGM{};

    std::uint8_t max_value = gradientOnly_ ? pic.getMaxValue() : 255;
    std::uint32_t newHeight = pic.getHeight() - 2;
    std::uint32_t newWidth = pic.getWidth() - 2;
    std::uint32_t new_size = newHeight * newWidth;

    PicturePGM sobelPicture{
        newHeight, newWidth, new_size, max_value,
            std::vector<std::vector<float>>(newHeight, std::vector<float>(newWidth)) };

    for(uint32_t i=0; i<pic.getHeight()-2; i++)
    {
        for(uint32_t j=0; j<pic.getWidth()-2; j++)
        {
            float Sx_i = 0, Sy_i = 0;
            for(int n=0; n<3; ++n)
                for(int m=0; m<3; ++m)
                {
                    Sx_i += sobel_Dx_[n][m] * pic.get(i+n,j+m);
                    Sy_i += sobel_Dy_[n][m] * pic.get(i+n,j+m);
                }
            if (gradientOnly_)
                sobelPicture.set(static_cast<float>(sqrt(Sx_i * Sx_i + Sy_i * Sy_i)), i, j);
            else
                sobelPicture.set(static_cast<float>(sqrt(Sx_i * Sx_i + Sy_i * Sy_i)) > thresholdValue_ ? 255.0f : 0.0f, i, j);
        }        
    }
    return sobelPicture;
}
