#include "RangEdgeDetection.h"
#include "Sobel.h"
#include <map>
#include <algorithm>


bool RangEdgeDetection::isConfigValid(Config& c, float& threshold_value, unsigned int& surrounding) noexcept
{
    bool validConfig = true;
    if(auto it = c.find("rang_threshold"); it != c.end())
    {
        if(auto p = dynamic_cast<TypedImageProperty<float>*>(it->second.get()); p)
            threshold_value = p->getProperty();
        else
            validConfig = false;
    }else
        validConfig = false;

    if(auto it1 = c.find("surrounding"); it1 != c.end())
    {
        if(auto ptr = dynamic_cast<TypedImageProperty<std::uint8_t>*>(it1->second.get()); ptr)
        {
            if((ptr->getProperty() == 3) || (ptr->getProperty() == 5) || (ptr->getProperty() == 7))
            {
                surrounding = ptr->getProperty();
            }else
                validConfig = false;
        }else
            validConfig = false;
    }else
        validConfig = false;
    
    return validConfig;
}

void RangEdgeDetection::calculate_rang_position(std::vector<float>& pixelVector) noexcept
{
    std::sort(pixelVector.begin(), pixelVector.end(), [](const float& lhs, const float& rhs) { return lhs < rhs; });
    size_t pos{ 0 };
    const size_t sizeOfVector{ pixelVector.size() };
    int first{ -1 };
    int last{ -1 };
    float item = pixelVector[0];
    for(; pos < sizeOfVector; ++pos)
    {
        if(pixelVector[pos] != item)
        {
            item = pixelVector[pos];
            if(first < last)
            {
                int rang = (first+last)/2;
                for(;first<=last; ++first)
                    pixelVector[first-1] = static_cast<float>(rang);
            }else
            {
                pixelVector[pos-1] = static_cast<float>(first);
            }

            first = -1;
            last = -1;
        }

        if(first == -1)
                first = pos+1;

        last = pos+1;
    }

        if(first < last)
        {
        int rang = (first+last)/2;
        for(;first<=last; ++first)
            pixelVector[first-1] = static_cast<float>(rang);
        }else
        {
            pixelVector[pos-1] = static_cast<float>(first);
        }
}

void RangEdgeDetection::replace_by_threshold(PicturePGM& pic, float threshold) noexcept
{
    for(uint32_t i = 0; i<pic.getHeight(); ++i)
        for(uint32_t j = 0; j<pic.getWidth(); ++j)
        {
            pic.set((pic.get(i,j) > threshold) ? 255.0f : 0.0f, i,j);
        }
    pic.setMaxValue(255);
}

PicturePGM RangEdgeDetection::processImage(PicturePGM& pic, Config& c) noexcept
{
    if (pic.isEmpty())
        return PicturePGM{};

    float threshold_value;
    unsigned int surrounding;
    if (!isConfigValid(c, threshold_value, surrounding))
    {
        std::cout << "error in config." << std::endl;
        return PicturePGM{};
    }

    Sobel sobel;
    Config c_sobel;
    c_sobel["gradient_only"] = std::make_unique<TypedImageProperty<bool>>("gradient_only", true);
    auto GradientPicture = sobel.processImage(pic, c_sobel);

    PicturePGM rang_pic{ pic.getHeight(), pic.getWidth(),
                    pic.getHeight() * pic.getWidth(), pic.getMaxValue(),
                    std::vector<std::vector<float>>(pic.getHeight(), std::vector<float>(pic.getWidth())) };

    const unsigned int surrounding_size = surrounding * surrounding;
    std::vector<float> pixel_surrounding(surrounding_size);
    for(uint32_t row=0; row<GradientPicture.getHeight()-(surrounding-1); ++row)
    {
        for(uint32_t col=0; col<GradientPicture.getWidth()-(surrounding-1); ++col)
        {
            for(unsigned int i=0; i<surrounding; ++i)
                for(unsigned int j=0; j<surrounding; ++j)
                    pixel_surrounding[(i*surrounding) + j] = GradientPicture.get(row+i, col+j);
            
            calculate_rang_position(pixel_surrounding);
            for(unsigned int i=0; i<surrounding; ++i)
                for(unsigned int j=0; j<surrounding; ++j)
                {
                    rang_pic.set(pixel_surrounding[(i * surrounding) + j], row+i, col+j);
                }
        }
    }

    replace_by_threshold(rang_pic, threshold_value);
    rang_pic.removePadding();
    return rang_pic;
}