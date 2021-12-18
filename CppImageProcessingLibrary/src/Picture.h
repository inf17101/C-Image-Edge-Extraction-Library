#pragma once

#include <cstdint>
#include <iostream>
#include "ArrayFunctions.h"

struct PicturePGM
{

    std::uint32_t height;
    std::uint32_t width;
    std::uint32_t size;
    std::uint8_t max_value;
    float** map;

    PicturePGM(const std::uint32_t h, const std::uint32_t w, const std::uint32_t s, const std::uint8_t max_v, float** m)
    : height(h), width(w), size(s), max_value(max_v), map(m) {}

    PicturePGM() : height(0), width(0), size(0), max_value(0), map(nullptr) {}

    PicturePGM(const PicturePGM& p) : height(p.height), width(p.width), size(p.size), max_value(p.max_value), map(nullptr)
    {
        bool result = create2dArray<float, uint32_t>(&map, p.height, p.width);
        if (result)
        {
            for (uint32_t i = 0; i < p.height; ++i)
                for (uint32_t j = 0; j < p.width; ++j)
                    map[i][j] = p.map[i][j];
        }
    }

    PicturePGM(PicturePGM&& p) noexcept : height(p.height), width(p.width), size(p.size), max_value(p.max_value)
    {
        delete2dArray(&map, height);
        map = std::exchange(p.map, nullptr);
    }

    ~PicturePGM()
    {
        delete2dArray(&map, height);
    }

    PicturePGM& operator =(const PicturePGM& p) noexcept
    {
        // handle self assignment
        if (this == &p)
            return *this;

        if (map != nullptr)
        {
            delete2dArray(&map, height);
        }

        height = p.height;
        width = p.width;
        size = p.size;
        map = nullptr;
        map = new float* [p.height];

        for (uint32_t r = 0; r < p.height; ++r)
        {
            map[r] = new float[p.width];
            for (uint32_t c = 0; c < p.width; ++c)
            {
                if(map[r] != nullptr)
                    map[r][c] = p.map[r][c];
            }
        }

        max_value = p.max_value;
        
        return *this;
    }

    PicturePGM& operator=(PicturePGM&& p) noexcept
    {
        if (this == &p)
            return *this;
        if (map != nullptr)
        {
            delete2dArray(&map, height);
        }
        map = std::exchange(p.map, nullptr);
        max_value = std::exchange(p.max_value, 0);
        height = std::exchange(p.height, 0);
        width = std::exchange(p.width, 0);
        size = std::exchange(p.size, 0);
        return *this;
    }

    void printPic() const
    {
        std::cout << "Picture: " << "\n" << "width: " << width << "\nheight: " << height << std::endl;
        std::cout << "size: " << size << "\nmax value: " << static_cast<unsigned int>(max_value) << std::endl;

        for(uint32_t i=0; i<height; ++i)
        {
            for(uint32_t j=0; j<width;++j)
            {
                printf("%.2f ", map[i][j]);
            }
            printf("\n");
        }
    }

    void make_padding(std::uint8_t padding=1)
    {
        if(padding < 1 || padding > 7)
        {
            std::cout << "Wrong padding. Expected padding between 1 and 7." << std::endl;
            return;
        }
        uint32_t oldHeight = height;
        height += (padding*2);
        width += (padding*2);
        size = height * width;
        float** new_map = new float*[height];
        if(new_map == nullptr) {std::cout << "cannot allocate new memory for making padding." << std::endl; return;}

        for(uint32_t i=0; i<height; ++i)
        {
            new_map[i] = new float[width];
            if(new_map[i] == nullptr) {std::cout << "cannot allocate new memory for making padding." << std::endl; return;}
            for(uint32_t j=0; j<width; ++j)
                new_map[i][j] = 0;
        }

        for(uint32_t i=padding; i<height-padding; ++i)
            for(uint32_t j=padding; j<width-padding; ++j)
                new_map[i][j] = map[i-padding][j-padding];
 
        delete2dArray(&map, oldHeight);
        map = new_map;
    }

    void removePadding(std::uint8_t padding=1)
    {
        int heightDifference = height - padding;
        int widthDifference = width - padding;
        if(heightDifference < 1 || widthDifference < 1)
        {
            std::cout << "removing padding does eliminate the picture. Padding size is too big." << std::endl;
            return;
        }

        height -= padding*2;
        width -= padding*2;
        size = height * width;

        float** new_map = nullptr;
        bool result = create2dArray<float, uint32_t>(&new_map, height, width);
        if (!result)
            return;

        for(uint32_t row=padding; row<height+padding*2-padding; ++row)
            for(uint32_t col=padding; col<width+padding*2-padding; ++col)
                new_map[row-padding][col-padding] = map[row][col];
        
        if(map != nullptr)
        {
            delete2dArray(&map, height + padding * 2);
        }

        map = new_map;
    }

    bool reinitWithValue(float value, uint32_t width, uint32_t height)
    {
        float** new_map = new float*[height];
        if(new_map == nullptr) return false;

        for(uint32_t row=0; row<height; ++row)
        {
            new_map[row] = new float[width];
            if(new_map[row] == nullptr) return false;
            for(uint32_t col=0; col<width; ++col)
                new_map[row][col] = value;
        }

        if(map != nullptr)
        {
            delete2dArray(&map, height);
        }
        map = new_map;
        return true;
    }

    bool isEmpty()
    {
        return (height == 0 || width == 0 || map == nullptr);
    }

};
