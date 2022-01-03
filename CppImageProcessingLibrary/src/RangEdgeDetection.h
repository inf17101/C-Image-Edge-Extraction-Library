#pragma once

#include "ImageAlgorithmStrategy.h"
#include "Picture.h"

class RangEdgeDetection : public ImageAlgorithmStrategy
{
    public:
        RangEdgeDetection(const std::uint8_t surrounding, const float thresholdValue);
        PicturePGM processImage(PicturePGM& pic) noexcept override;
    private:
        std::uint8_t surrounding_;
        float thresholdValue_;

        std::vector<float> calculate_rang_position(std::vector<std::pair<float, std::uint8_t>>& pixelVector) noexcept;
        void replace_by_threshold(PicturePGM& pic) noexcept;
};
