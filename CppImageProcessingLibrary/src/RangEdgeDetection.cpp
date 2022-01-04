#include "RangEdgeDetection.h"
#include "Sobel.h"
#include <algorithm>

RangEdgeDetection::RangEdgeDetection(const std::uint8_t surrounding, const float thresholdValue) 
    : surrounding_(surrounding), thresholdValue_(thresholdValue)
{
}

auto RangEdgeDetection::calculate_rang_position(std::vector<float>& pixelVector)
{
    std::vector<float> copyPixelVector(pixelVector);
    std::sort(copyPixelVector.begin(), copyPixelVector.end());
    for (auto& element : pixelVector)
    {
        auto itFirst = std::find(copyPixelVector.begin(), copyPixelVector.end(), element);
        auto itLast = std::find(copyPixelVector.rbegin(), copyPixelVector.rend(), element);
        if (itFirst == copyPixelVector.end() || itLast == copyPixelVector.rend())
            throw std::logic_error("vector with range positions might be corrupt.");

        std::uint64_t posFirst = std::distance(copyPixelVector.begin(), itFirst) + 1;
        std::uint64_t posLast = std::distance(itLast, copyPixelVector.rend());
        std::uint64_t rang = posFirst == posLast ? posFirst : ((posFirst + posLast) / 2);
        element = static_cast<float>(rang);
    }
    return pixelVector;
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

    const std::uint32_t surrounding_size = surrounding_ * surrounding_;
    std::vector<float> pixel_surrounding(surrounding_size);
    const std::uint32_t amountOfRows = GradientPicture.getHeight() - (surrounding_ - 1);
    const std::uint32_t amountOfCols = GradientPicture.getWidth() - (surrounding_ - 1);
    for (uint32_t row{0}; row < amountOfRows; ++row)
    {
        for (uint32_t col{0}; col < amountOfCols; ++col)
        {
            for(unsigned int i = 0; i<surrounding_; ++i)
                for(unsigned int j = 0; j<surrounding_; ++j)
                    pixel_surrounding[(i * surrounding_) + j] = GradientPicture.get(row + i, col + j);

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