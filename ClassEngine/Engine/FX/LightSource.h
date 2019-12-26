#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <glm/glm.hpp>

class LightSource
{
public:
	LightSource(glm::vec3 position_, float ambient_, float diffuse_, glm::vec3 color_);
	~LightSource();

	glm::vec3 GetLightPosition();
	float GetAmbient();
	float GetDiffuse();
	glm::vec3 GetLightColor();

	void SetLightPosition(glm::vec3 position_);
	void SetAmbient(float ambient_);
	void SetDiffuse(float diffuse_);
	void SetLightColor(glm::vec3 color_);
private:
	glm::vec3 position;
	float ambient;
	float diffuse;
	glm::vec3 color;
};

#endif //!LIGHTSOURCE_H