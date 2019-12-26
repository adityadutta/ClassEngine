#include "LightSource.h"

LightSource::LightSource(glm::vec3 position_, float ambient_, float diffuse_, glm::vec3 color_)
{
	position = position_;
	ambient = ambient_;
	diffuse = diffuse_;
	color = color_;
}

LightSource::~LightSource()
{
}

glm::vec3 LightSource::GetLightPosition()
{
	return position;
}

float LightSource::GetAmbient()
{
	return ambient;
}

float LightSource::GetDiffuse()
{
	return diffuse;
}

glm::vec3 LightSource::GetLightColor()
{
	return color;
}

void LightSource::SetLightPosition(glm::vec3 position_)
{
	position = position_;
}

void LightSource::SetAmbient(float ambient_)
{
	ambient = ambient_;
}

void LightSource::SetDiffuse(float diffuse_)
{
	diffuse = diffuse_;
}

void LightSource::SetLightColor(glm::vec3 color_)
{
	color = color_;
}
