#include "RangEdgeDetection.h"
#include "Sobel.h"
#include <algorithm>

RangEdgeDetection::RangEdgeDetection(const std::uint8_t surrounding, const float thresholdValue) 
    : surrounding_(surrounding), thresholdValue_(thresholdValue)
{
}

std::vector<float> RangEdgeDetection::calculate_rang_position(std::vector<std::pair<float, std::uint8_t>>& pixelVector) noexcept
{
    std::sort(pixelVector.begin(), pixelVector.end(), [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });
    size_t pos{ 0 };
    const size_t sizeOfVector{ pixelVector.size() };
    int first{ -1 };
    int last{ -1 };
    auto item = pixelVector[0].first;
    for(; pos < sizeOfVector; ++pos)
    {
        if(pixelVector[pos].first != item)
        {
            item = pixelVector[pos].first;
            if(first < last)
            {
                int rang = (first+last)/2;
                for(;first<=last; ++first)
                    pixelVector[first-1].first = static_cast<float>(rang);
            }else
            {
                pixelVector[pos-1].first = static_cast<float>(first);
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
            pixelVector[first-1].first = static_cast<float>(rang);
        }else
        {
            pixelVector[pos-1].first= static_cast<float>(first);
        }
    std::vector<float> tmp(sizeOfVector);
    for (const auto& rang : pixelVector)
    {
        tmp[rang.second] = rang.first;
    }
    return tmp;
}

void RangEdgeDetection::replace_by_threshold(PicturePGM& pic) noexcept
{
    for (uint32_t i = 0; i < pic.getHeight(); ++i)
    {
        for (uint32_t j = 0; j < pic.getWidth(); ++j)
        {
            pic.set((pic.get(i, j) > thresholdValue_) ? 255.0f : 0.0f, i, j);
        }
    }
    pic.setMaxValue(255);
}

PicturePGM RangEdgeDetection::processImage(PicturePGM& pic) noexcept
{
    if (pic.isEmpty())
        return PicturePGM{};

    Sobel sobel;
    auto GradientPicture = sobel.processImage(pic);

    PicturePGM rang_pic{ pic.getHeight(), pic.getWidth(),
                    pic.getHeight() * pic.getWidth(), pic.getMaxValue(),
                    std::vector<std::vector<float>>(pic.getHeight(), std::vector<float>(pic.getWidth())) };

    const unsigned int surrounding_size = surrounding_ * surrounding_;
    std::vector<std::pair<float, std::uint8_t>> pixel_surrounding(surrounding_size);
    const std::uint32_t amountOfRows = GradientPicture.getHeight() - (surrounding_ - 1);
    const std::uint32_t amountOfCols = GradientPicture.getWidth() - (surrounding_ - 1);
    for (uint32_t row{0}; row < amountOfRows; ++row)
    {
        for (uint32_t col{0}; col < amountOfCols; ++col)
        {
            for(unsigned int i = 0; i<surrounding_; ++i)
                for(unsigned int j = 0; j<surrounding_; ++j)
                    pixel_surrounding[(i * surrounding_) + j] = { GradientPicture.get(row + i, col + j), (i * surrounding_) + j };

            auto rangePositions = calculate_rang_position(pixel_surrounding);

            for (unsigned int i = 0; i < surrounding_; ++i)
            {
                for (unsigned int j = 0; j < surrounding_; ++j)
                {
                    rang_pic.set(rangePositions[(i * surrounding_) + j], row + i, col + j);
                }
            }
        }
    }

    replace_by_threshold(rang_pic);
    rang_pic.removePadding();
    return rang_pic;
}