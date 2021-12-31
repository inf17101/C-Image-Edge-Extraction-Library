#include "Sobel.h"
#include <cmath>
#include <algorithm>

bool Sobel::isConfigValid(Config& c, float& threshold_value, bool& gradient_only)
{
    bool validConfig = true;
    auto it = c.find("sobel_threshold");
    if(it != c.end())
    {
        if(auto p = dynamic_cast<TypedImageProperty<float>*>(it->second.get()); p)
            threshold_value = p->getProperty();
        else
            validConfig = false;       
    }

    if(auto it1 = c.find("gradient_only"); it1 != c.end())
    {
        if(auto ptr = dynamic_cast<TypedImageProperty<bool>*>(it1->second.get()); ptr)
        {
            gradient_only = ptr->getProperty();
            if(!gradient_only && (it == c.end()))
                validConfig = false;
        }
        else
            validConfig = false;
    }else
        validConfig = false;

    return validConfig;
}

PicturePGM Sobel::processImage(PicturePGM& pic, Config& c) noexcept
{
    if (pic.isEmpty())
        return PicturePGM{};

    float threshold_value;
    bool gradient_only;
    if (!isConfigValid(c, threshold_value, gradient_only))
    {
        std::cout << "error in config." << std::endl;
        return PicturePGM{};
    }

    std::uint8_t max_value = gradient_only ? pic.getMaxValue() : 255;
    std::uint32_t newHeight = pic.getHeight() - 2;
    std::uint32_t newWidth = pic.getWidth() - 2;
    std::uint32_t new_size = pic.getHeight()-2 * pic.getWidth()-2;


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
            if (gradient_only)
                sobelPicture.set(static_cast<float>(sqrt(Sx_i * Sx_i + Sy_i * Sy_i)), i, j);
            else
                sobelPicture.set(static_cast<float>(sqrt(Sx_i * Sx_i + Sy_i * Sy_i)) > threshold_value ? 255.0f : 0.0f, i, j);
        }        
    }
    return sobelPicture;
}
