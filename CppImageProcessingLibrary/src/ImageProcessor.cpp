#include "ImageProcessor.h"
#include <cstring>
#include <fstream>

bool ImageProcessor::checkPaddingOption(uint8_t padding_size) noexcept
{
    if(padding_size < 2 || padding_size > 7)
    {
        std::cerr << "illegal padding size. Should be: 7 <= padding_size >= 2" << std::endl;
        return false;
    }
    return true;
}

PicturePGM ImageProcessor::readImage(const std::string& filename, uint8_t padding_size) noexcept
{
    if (!checkPaddingOption(padding_size)) return PicturePGM{};

    FILE *fhandle = fopen(filename.c_str(), "rb");
    if (!fhandle)
    {
        std::cerr << "File could not be opened." << std::endl;
        return PicturePGM{};
    }
    std::cout << "Opened file " << filename << " correctly!" << std::endl;

    char line[75];
    if (fgets(line, sizeof(line), fhandle) == NULL) return PicturePGM{};

    if(strncmp(line, "P5",2) != 0) return PicturePGM{};

    if(fgets(line, sizeof(line), fhandle) == NULL) return PicturePGM{};

    std::uint32_t width = strtoul(strtok(line, " "), NULL, 10) + padding_size;
    std::uint32_t height = strtoul(strtok(NULL, " "), NULL, 10) + padding_size;
    std::uint32_t size = height * width;

    if(fgets(line, sizeof(line), fhandle) == NULL) return PicturePGM{};
    std::uint8_t max_value = (uint8_t) strtoul(line, NULL, 10);

    std::vector<std::vector<float>> pictureMap(height, std::vector<float>(width, 0.0f));

    for (uint32_t i=1; i<height-1; ++i)
        for(uint32_t j=1; j<width-1; ++j)
            pictureMap[i][j] = static_cast<float> (fgetc(fhandle));

    fclose(fhandle);

    return PicturePGM (height, width, size, max_value, pictureMap);

}

PicturePGM ImageProcessor::processImage(PicturePGM& pic, Config& c) noexcept
{
    return algorithm_->processImage(pic,c);
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