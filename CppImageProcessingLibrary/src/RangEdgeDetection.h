#pragma once

#include "ImageAlgorithmStrategy.h"
#include "Picture.h"

class RangEdgeDetection : public ImageAlgorithmStrategy
{
    public:
        PicturePGM processImage(PicturePGM& pic, Config& c) noexcept override;
    private:
        void calculate_rang_position(float array[], const std::size_t len) noexcept;
        void replace_by_threshold(PicturePGM& pic, float threshold) noexcept;
        bool isConfigValid(Config& c, float& threshold_value, unsigned int& surrounding) noexcept;
};
