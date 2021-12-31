#pragma once
#include <map>
#include <memory>
#include <string>
#include "ImageConfig.h"


class Config
{
	private:
		std::map<std::string, std::unique_ptr<ImageProperty>> imageConfig;
};

