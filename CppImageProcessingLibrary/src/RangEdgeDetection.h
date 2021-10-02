#pragma once

#include "ImageAlgorithmStrategy.h"
#include "Picture.h"
#include <memory>

class RangEdgeDetection : public ImageAlgorithmStrategy
{
    void calculate_rang_position(float array[], const std::size_t len);
    void replace_by_threshold(PicturePGM* pic, float threshold);
    bool isConfigValid(Config& c, float& threshold_value, unsigned int& surrounding);
    public:
        RangEdgeDetection() = default;
        ~RangEdgeDetection() = default;
        virtual std::unique_ptr<PicturePGM> processImage(PicturePGM* pic, Config& c) override;
};
