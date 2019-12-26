#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../FX/LightSource.h"

class BoundingBox;

class Camera
{
public:
	Camera();
	~Camera();
	void SetPosition(glm::vec3 position_);
	void SetRotation(float yaw_, float pitch_);
	glm::mat4 GetView() const;
	const glm::mat4 GetPerspective();
	const glm::mat4 GetOrthographic();
	const glm::vec3 GetPosition();
	void OnDestroy();

	void AddLightSource(LightSource* light_);
	std::vector<LightSource*> &GetLightSources();

	void ProcessMouseMovement(float XOffset_, float YOffset_);
	void ProcessMouseZoom(int y_);

	glm::vec2 GetClippingPlanes() const;

	//frustum culling
	bool CheckFrustumCulling(BoundingBox box_) const;
	bool CheckFrustumCulling(const glm::vec3& point_) const;
private:
	enum Plane { Right, Left, Bottom, Top, Front, Back };

	void UpdateCameraVectors();
	void UpdatePlanes(const glm::mat4& proj_, const glm::mat4& view_);
	void Normalize(Plane plane);

	glm::vec3 position;
	glm::mat4 perspective;
	glm::mat4 orthographic;
	float fieldOfView;
	float pitch;
	float yaw;
	float nearPlane, farPlane;
	glm::vec3 forwardVector, upVector, rightVector, worldUp;

	std::vector<LightSource*> lights;

	float movementSpeed;

	double plane_data[6][4];
	inline glm::vec4 GetPlane(Plane plane) const {
		return glm::vec4(plane_data[plane][0], plane_data[plane][1], plane_data[plane][2], plane_data[plane][3]);
	}
};

#endif //!CAMERA_H