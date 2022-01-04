#include "ImageProcessor.h"
#include <cstring>
#include <fstream>
#include <sstream>

bool ImageProcessor::checkPaddingOption(std::uint8_t padding_size) const noexcept
{
    if(padding_size < 2 || padding_size > 7)
    {
        std::cerr << "illegal padding size. Should be: 7 <= padding_size >= 2" << std::endl;
        return false;
    }
    return true;
}

std::tuple<std::string, std::uint32_t, std::uint32_t, std::uint16_t> ImageProcessor::readHeader(std::ifstream& inputFile) noexcept
{
    std::string header{ "" };
    std::uint32_t width{ 0 };
    std::uint32_t height{ 0 };
    std::uint16_t maxValue{ 0 };
    std::string line;
    std::string buffer{ "" };
    for (int i = 0; i < 3; ++i)
    {
        std::getline(inputFile, line);
        buffer += (line + "\n");
    }
    std::istringstream iss{ buffer };
    iss >> header >> width >> height >> maxValue;
    return { header, height, width, maxValue };
}

bool ImageProcessor::isValidPGMHeader(const std::string& pgmVersion, const std::uint32_t height,
    const std::uint32_t width, const std::uint16_t maxValue) const noexcept
{
    return pgmVersion.compare("P5\n") && height > 0 && width > 0 && maxValue > 0 && maxValue <= 255;
}

PicturePGM ImageProcessor::readImage(const std::string& filename, std::uint8_t padding_size) noexcept
{
    std::ifstream inputFile{ filename, std::ios::in | std::ios_base::binary };
    if (!inputFile.is_open()) { return PicturePGM{}; }

    const auto [pgmVersion, height, width, maxValue] = readHeader(inputFile);
    if (!isValidPGMHeader(pgmVersion, height, width, maxValue)) { return PicturePGM{}; }

    const std::uint64_t size = height * width;
    std::vector<std::vector<float>> pictureMap(height, std::vector<float>(width, 0.0f));
    char pixelValue;
    for (std::uint32_t row{ 0 }; row < height; ++row)
    {
        for (std::uint32_t col{ 0 }; col < width && inputFile.get(pixelValue); ++col)
        {
            const auto tmp = static_cast<unsigned char>(pixelValue);
            pictureMap[row][col] = static_cast<float>(tmp);
        }
    }

    return PicturePGM{ height, width, size, static_cast<std::uint8_t>(maxValue), pictureMap };
}

PicturePGM ImageProcessor::processImage(PicturePGM& pic) noexcept
{
    return algorithm_->processImage(pic);
}

bool ImageProcessor::writeImageAsPGM(PicturePGM& pic, const std::string& FILE_PATH) noexcept
{
    std::ofstream of(FILE_PATH, std::ios::out
                                | std::ios_base::binary
                                | std::ios_base::trunc);
    if(!of)
        return false;
    
    of << "P5\n";
    of << pic.getWidth() << " " << pic.getHeight() << "\n";

    /*
        must be converted to unsigned int otherwise
        image software cannot open the result image properly
    */
    of << static_cast<unsigned int>(pic.getMaxValue()) << "\n";

    if(of.bad())
        return false;

    for(uint32_t i=0; i<pic.getHeight(); ++i)
        for(uint32_t j=0; j<pic.getWidth(); ++j)
            of.put(static_cast<char>(pic.get(i,j)));
    
    if(of.bad())
        return false;
    of.close();

    return true;
}