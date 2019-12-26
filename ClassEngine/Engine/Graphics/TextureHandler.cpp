#include "TextureHandler.h"

std::unique_ptr<TextureHandler> TextureHandler::textureInstance = nullptr;
std::map<std::string, Texture*>  TextureHandler::textures = std::map<std::string, Texture*>();

TextureHandler* TextureHandler::GetInstance()
{
	if (textureInstance.get() == nullptr)
	{
		textureInstance.reset(new TextureHandler);
	}

	return textureInstance.get();
}

void TextureHandler::OnDestroy()
{
	if (!textures.empty())
	{
		for (auto texture : textures)
		{
			glDeleteTextures(1, &texture.second->textureID);
			delete texture.second;
			texture.second = nullptr;
		}
		textures.clear();
	}
}

bool TextureHandler::CreateTexture(const std::string& textureName_, const std::string& textureFileName_)
{
	Texture* t = new Texture();
	SDL_Surface* surface = nullptr;
	try
	{
		surface = IMG_Load(textureFileName_.c_str());
	}
	catch (std::exception& e)
	{
		Debug::Error("Error loading texture: " + textureName_, __FILE__, __LINE__);
		return false;
	}

	t->width = static_cast<float>(surface->w);
	t->height = static_cast<float>(surface->h);

	glGenTextures(1, &t->textureID);
	glBindTexture(GL_TEXTURE_2D, t->textureID);
	int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	textures[textureName_] = t;
	SDL_FreeSurface(surface);
	surface = nullptr;

	return true;
}

const GLuint TextureHandler::GetTexture(const std::string& textureName_)
{
	if (textures.find(textureName_) != textures.end())
	{
		return textures[textureName_]->textureID;
	}

	return 0;
}

const Texture* TextureHandler::GetTextureData(const std::string& textureName_)
{
	if (textures.find(textureName_) != textures.end())
	{
		return textures[textureName_];
	}

	return nullptr;
}

TextureHandler::TextureHandler()
{
}


TextureHandler::~TextureHandler()
{
	OnDestroy();
}
