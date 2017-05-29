#pragma once
#include "GLTexture.h"
#include "IOManager.h"
#include <string>
#include <vector>

class ImageLoader {
public:
	static GLTexture loadPNG(std::string filePath);
};

