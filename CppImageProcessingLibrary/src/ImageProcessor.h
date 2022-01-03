#pragma once

#include "ImageAlgorithmStrategy.h"
#include <memory>

class ImageProcessor
{
    public:
        explicit ImageProcessor(std::unique_ptr<ImageAlgorithmStrategy> a) : algorithm_(std::move(a)) {}
        PicturePGM readImage(const std::string& filename, std::uint8_t padding_size = 2) noexcept;
        PicturePGM processImage(PicturePGM& pic) noexcept;
        bool writeImageAsPGM(PicturePGM& pic, const std::string& FILE_PATH) noexcept;
        ImageProcessor(const ImageProcessor& ) = delete;
        ImageProcessor(const ImageProcessor&&) = delete;
        ImageProcessor& operator=(ImageProcessor& ) = delete;
        ImageProcessor& operator=(ImageProcessor&&) = delete;
        ~ImageProcessor() = default;

    private:
        std::unique_ptr<ImageAlgorithmStrategy> algorithm_;

        bool checkPaddingOption(std::uint8_t padding_size) const noexcept;
        std::tuple<std::string, std::uint32_t, std::uint32_t, std::uint16_t> readHeader(std::ifstream& inputFile) noexcept;
        bool isValidPGMHeader(const std::string& pgmVersion, const std::uint32_t height, const std::uint32_t width, const std::uint16_t maxValue) const noexcept;
};
