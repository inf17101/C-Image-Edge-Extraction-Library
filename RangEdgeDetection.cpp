#include "RangEdgeDetection.h"
#include "Sobel.h"
#include <memory>
#include <map>
#include <algorithm>


bool RangEdgeDetection::isConfigValid(Config& c, float& threshold_value, unsigned int& surrounding)
{
    bool validConfig = true;
    if(auto it = c.find("rang_threshold"); it != c.end())
    {
        if(auto p = dynamic_cast<TypedImageProperty<float>*>(it->second); p)
            threshold_value = p->getProperty();
        else
            validConfig = false;
    }else
        validConfig = false;

    if(auto it1 = c.find("surrounding"); it1 != c.end())
    {
        if(auto ptr = dynamic_cast<TypedImageProperty<std::uint8_t>*>(it1->second); ptr)
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

void RangEdgeDetection::calculate_rang_position(float array[], const std::size_t len)
{
    float **parray = new float*[len];
    if(parray == nullptr) {std::cout << "ERROR: cannot allocate memory to calculate rang position." << std::endl; exit(1);}
    for(size_t i=0;i<len; ++i)
        parray[i] = array + i;
    std::sort(parray, parray + len, [](float* left, float* right){ return (*left) < (*right); });
    size_t pos = 0;
    int first = -1;
    int last = -1;
    float item = (*parray[0]);
    for(pos=0; pos < len; ++pos)
    {
        if((*parray[pos]) != item)
        {
            item = (*parray[pos]);
            if(first < last)
            {
                int rang = (first+last)/2;
                for(;first<=last; ++first)
                    (*parray[first-1]) = static_cast<float>(rang);
            }else
            {
                (*parray[pos-1]) = static_cast<float>(first);
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
            (*parray[first-1]) = static_cast<float>(rang);
        }else
        {
            (*parray[pos-1]) = static_cast<float>(first);
        }

        delete[] parray;
        delete parray;
}

void RangEdgeDetection::replace_by_threshold(PicturePGM* pic, float threshold)
{
    for(uint32_t i = 0; i<pic->height; ++i)
        for(uint32_t j = 0; j<pic->width; ++j)
        {
            pic->map[i][j] = (pic->map[i][j] > threshold) ? 255.0f : 0.0f;
        }
    pic->max_value = 255;
}

std::unique_ptr<PicturePGM> RangEdgeDetection::processImage(PicturePGM* pic, Config& c)
{
    float threshold_value;
    unsigned int surrounding;
    if (!isConfigValid(c, threshold_value, surrounding))
    {
        std::cout << "error in config." << std::endl;
        return std::make_unique<PicturePGM>();
    }

    auto sobel = std::make_unique<Sobel>();
    Config c_sobel;
    c_sobel["gradient_only"] = new TypedImageProperty<bool>("gradient_only", true);
    auto GradientPicture = sobel->processImage(pic, c_sobel);
    delete c_sobel["gradient_only"];
    GradientPicture->make_padding();

    float** new_map = new float*[pic->height];
    if(new_map == nullptr) return std::make_unique<PicturePGM>();
    for(uint32_t i=0; i<pic->height; ++i)
    {
        new_map[i] = new float[pic->width];
        if(new_map[i] == nullptr) return std::make_unique<PicturePGM>();
    }
    auto rang_pic = std::make_unique<PicturePGM>(pic->height, pic->width, pic->height*pic->width, pic->max_value, new_map);

    const unsigned int surrounding_size = surrounding * surrounding;
    float* pixel_surrounding = new float[surrounding_size];
    //float pixel_surrounding[surrounding_size];
    for(uint32_t row=0; row<GradientPicture->height-(surrounding-1); ++row)
    {
        for(uint32_t col=0; col<GradientPicture->width-(surrounding-1); ++col)
        {
            for(unsigned int i=0; i<surrounding; ++i)
                for(unsigned int j=0; j<surrounding; ++j)
                    pixel_surrounding[i*surrounding + j] = GradientPicture->map[row+i][col+j];
            
            calculate_rang_position(pixel_surrounding, surrounding_size);
            for(unsigned int i=0; i<surrounding; ++i)
                for(unsigned int j=0; j<surrounding; ++j)
                {
                    rang_pic->map[row+i][col+j] = pixel_surrounding[i*surrounding + j];
                }
        }
    }

    delete [] pixel_surrounding;
    replace_by_threshold(rang_pic.get(), threshold_value);
    rang_pic->removePadding();
    return rang_pic;
}