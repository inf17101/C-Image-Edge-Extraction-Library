#pragma once

#include <cstdint>
#include <iostream>
#include <vector>


class PicturePGM
{
    public:
        explicit PicturePGM(const std::uint32_t h, const std::uint32_t w, const std::uint32_t s, const std::uint16_t max_v, const std::vector<std::vector<float>>& m)
        : height_(h), width_(w), size_(s), maxValue_(max_v), map_(m) 
        {
        }

        explicit PicturePGM() : height_(0), width_(0), size_(0), maxValue_(0), map_({{}}) {}

        PicturePGM(const PicturePGM& p) noexcept : height_(p.height_), width_(p.width_), size_(p.size_), maxValue_(p.maxValue_), map_(p.map_)
        {
        }

        PicturePGM(PicturePGM&& p) noexcept : height_(p.height_), width_(p.width_), size_(p.size_), maxValue_(p.maxValue_), map_(std::move(p.map_))
        {
            p.map_ = {{}};
        }

        ~PicturePGM() noexcept
        {
        }

        PicturePGM& operator =(const PicturePGM& p) noexcept
        {
            // handle self assignment
            if (this == &p)
                return *this;

            height_ = p.height_;
            width_ = p.width_;
            size_ = p.size_;
            map_ = p.map_;
            maxValue_ = p.maxValue_;
        
            return *this;
        }

        PicturePGM& operator=(PicturePGM&& p) noexcept
        {
            if (this == &p)
                return *this;

            map_ = std::exchange(p.map_, {});
            maxValue_ = std::exchange(p.maxValue_, 0);
            height_ = std::exchange(p.height_, 0);
            width_ = std::exchange(p.width_, 0);
            size_ = std::exchange(p.size_, 0);
            return *this;
        }

        void print() const noexcept
        {
            std::cout << "Picture: " << "\n" << "width: " << width_ << "\nheight: " << height_ << std::endl;
            std::cout << "size: " << size_ << "\nmax value: " << static_cast<unsigned int>(maxValue_) << std::endl;

            for(uint32_t i=0; i<height_; ++i)
            {
                for(uint32_t j=0; j<width_;++j)
                {
                    printf("%.2f ", map_[i][j]);
                }
                printf("\n");
            }
        }

        void makePadding(std::uint8_t padding=1) noexcept
        {
            if(padding < 1 || padding > 7)
            {
                std::cerr << "Wrong padding. Expected padding between 1 and 7." << std::endl;
                return;
            }

            height_ += (padding*2);
            width_ += (padding*2);
            size_ = height_ * width_;
            std::vector<std::vector<float>> newMap(height_, std::vector<float>(width_, 0));

            for(uint32_t i=padding; i<height_-padding; ++i)
                for(uint32_t j=padding; j<width_-padding; ++j)
                    newMap[i][j] = map_[i-padding][j-padding];
 
            map_ = std::move(newMap);
        }

        void removePadding(std::uint8_t padding=1) noexcept
        {
            int heightDifference = height_ - padding;
            int widthDifference = width_ - padding;
            if(heightDifference < 1 || widthDifference < 1)
            {
                std::cerr << "removing padding does eliminate the picture. Padding size is too big." << std::endl;
                return;
            }

            height_ -= padding*2;
            width_ -= padding*2;
            size_ = height_ * width_;

            std::vector<std::vector<float>> newMap(height_, std::vector<float>(width_));

            for(uint32_t row=padding; row<height_+padding*2-padding; ++row)
                for(uint32_t col=padding; col<width_+padding*2-padding; ++col)
                    newMap[row-padding][col-padding] = map_[row][col];
        
            map_ = std::move(newMap);
        }

        bool reinitWithValue(float value, uint32_t width, uint32_t height) noexcept
        {
            std::vector<std::vector<float>> newMap(height, std::vector<float>(width, value));
            map_ = std::move(newMap); 
            return true;
        }

        const bool isEmpty() const noexcept
        {
            return (height_ == 0 || width_ == 0 || size_ == 0);
        }

        const float get(const std::uint32_t row, const std::uint32_t column) const noexcept
        {
            if (row >= height_ || column >= width_)
                return -1.0f;

            return map_[row][column];
        }

        void set(const float value, const std::uint32_t row, const std::uint32_t column) noexcept
        {
            if (row >= height_ || column >= width_)
                return;

            map_[row][column] = value;
        }

        void setMaxValue(const std::uint8_t newMaxValue) noexcept
        {
            maxValue_ = newMaxValue;
        }

        const auto getHeight() const noexcept
        {
            return height_;
        }

        const auto getWidth() const noexcept
        {
            return width_;
        }

        const auto getSize() const noexcept
        {
            return size_;
        }

        const auto getMaxValue() const noexcept
        {
            return maxValue_;
        }

    private:
        std::uint32_t height_;
        std::uint32_t width_;
        std::uint32_t size_;
        std::uint16_t maxValue_;
        std::vector<std::vector<float>> map_;
};
