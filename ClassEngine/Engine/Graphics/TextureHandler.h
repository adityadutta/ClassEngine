#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <memory>
#include <string>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>

#include "../Core/Debug.h"

struct Texture
{
	GLuint textureID = 0;
	float width = 0.0f;
	float height = 0.0f;
};

class TextureHandler
{
public:
	TextureHandler(const TextureHandler&) = delete; //Copy constructor
	TextureHandler(TextureHandler&&) = delete;//move constructor
	TextureHandler operator=(const TextureHandler&) = delete;
	TextureHandler operator=(TextureHandler&&) = delete;

	static TextureHandler* GetInstance();
	void OnDestroy();
	bool CreateTexture(const std::string& textureName_, const std::string& textureFileName_);
	static const GLuint GetTexture(const std::string& textureName_);
	static const Texture* GetTextureData(const std::string& textureName_);

private:
	TextureHandler();
	~TextureHandler();

	static std::unique_ptr<TextureHandler> textureInstance;
	friend std::default_delete<TextureHandler>;

	static std::map<std::string, Texture*> textures;
};

#endif //!TEXTUREHANDLER_H