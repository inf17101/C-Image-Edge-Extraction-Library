#include "Sobel.h"
#include <cmath>
#include <memory>
#include <algorithm>
#include "ArrayFunctions.h"

bool Sobel::isConfigValid(Config& c, float& threshold_value, bool& gradient_only)
{
    bool validConfig = true;
    auto it = c.find("sobel_threshold");
    if(it != c.end())
    {
        if(auto p = dynamic_cast<TypedImageProperty<float>*>(it->second); p)
            threshold_value = p->getProperty();
        else
            validConfig = false;       
    }

    if(auto it1 = c.find("gradient_only"); it1 != c.end())
    {
        if(auto ptr = dynamic_cast<TypedImageProperty<bool>*>(it1->second); ptr)
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

std::unique_ptr<PicturePGM> Sobel::processImage(PicturePGM* pic, Config& c)
{
    if (pic->isEmpty())
        return std::make_unique<PicturePGM>();

    float threshold_value;
    bool gradient_only;
    if (!isConfigValid(c, threshold_value, gradient_only))
    {
        std::cout << "error in config." << std::endl;
        return std::make_unique<PicturePGM>();
    }

    static float sobel_Dx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    static float sobel_Dy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    std::uint8_t max_value = gradient_only ? pic->max_value : 255;
    std::uint32_t new_size = pic->height-2 * pic->width-2;
    std::unique_ptr<PicturePGM> sobelPicture = std::make_unique<PicturePGM>(pic->height-2, pic->width-2, new_size, max_value, nullptr);

    bool result = create2dArray<float, uint32_t>(&(sobelPicture->map), sobelPicture->height, sobelPicture->width);
    if (!result)
        return sobelPicture;

    for(uint32_t i=0; i<pic->height-2; i++)
    {
        for(uint32_t j=0; j<pic->width-2; j++)
        {
            float Sx_i = 0, Sy_i = 0;
            for(int n=0; n<3; ++n)
                for(int m=0; m<3; ++m)
                {
                    Sx_i += sobel_Dx[n][m] * pic->map[i+n][j+m];
                    Sy_i += sobel_Dy[n][m] * pic->map[i+n][j+m];
                }
            if (gradient_only)
                sobelPicture->map[i][j] = static_cast<float>(sqrt(Sx_i*Sx_i + Sy_i*Sy_i));
            else
                sobelPicture->map[i][j] = static_cast<float>(sqrt(Sx_i*Sx_i + Sy_i*Sy_i)) > threshold_value ? 255.0f : 0.0f;
        }        
    }
    return sobelPicture;
}
