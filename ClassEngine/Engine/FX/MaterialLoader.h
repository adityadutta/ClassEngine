#ifndef MATERIALLOADER_H
#define MATERIALLOADER_H

#include <sstream>

#include "../Graphics/MaterialHandler.h"
#include "../Graphics/TextureHandler.h"

class MaterialLoader
{
public:
	MaterialLoader() = delete;
	MaterialLoader(const MaterialLoader&) = delete; //Copy constructor
	MaterialLoader(MaterialLoader&&) = delete;//move constructor
	MaterialLoader operator=(const MaterialLoader&) = delete;
	MaterialLoader operator=(MaterialLoader&&) = delete;
	~MaterialLoader();

	static void LoadMaterial(std::string file_);
	static GLuint LoadTexture(std::string filename_);
};

#endif //!MATERIALLOADER_H