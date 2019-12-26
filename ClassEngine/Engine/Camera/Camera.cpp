#include "Camera.h"
#include "../Core/CoreEngine.h"


Camera::Camera() : position(glm::vec3()), perspective(glm::mat4()), orthographic(glm::mat4()), fieldOfView(0), pitch(0), yaw(0), nearPlane(0), farPlane(0), forwardVector(glm::vec3()), upVector(glm::vec3()), rightVector(glm::vec3()), worldUp(glm::vec3()), lights(std::vector<LightSource*>()), movementSpeed(0.05f)
{
	fieldOfView = 45.0f;
	forwardVector = glm::vec3(0.0f, 0.0f, -1.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	worldUp = upVector;
	nearPlane = 2.0f;
	farPlane = 50.0f;
	yaw = -90.0f;
	perspective = glm::perspective(fieldOfView, CoreEngine::GetInstance()->GetScreenSize().x / CoreEngine::GetInstance()->GetScreenSize().y, nearPlane, farPlane);
	orthographic = glm::ortho(0.0f, CoreEngine::GetInstance()->GetScreenSize().x, 0.0f, CoreEngine::GetInstance()->GetScreenSize().y, -1.0f, 1.0f);
	UpdateCameraVectors();
}


Camera::~Camera()
{
	OnDestroy();
}

void Camera::SetPosition(glm::vec3 position_)
{
	position = position_;
	UpdateCameraVectors();
}

void Camera::SetRotation(float yaw_, float pitch_)
{
	yaw = yaw_;
	pitch = pitch_;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetView() const
{
	return glm::lookAt(position, position + forwardVector, upVector);
}

const glm::mat4 Camera::GetPerspective()
{
	perspective = glm::perspective(fieldOfView, CoreEngine::GetInstance()->GetScreenSize().x / CoreEngine::GetInstance()->GetScreenSize().y, nearPlane, farPlane);

	return perspective;
}

const glm::mat4 Camera::GetOrthographic()
{
	orthographic = glm::ortho(0.0f, CoreEngine::GetInstance()->GetScreenSize().x, 0.0f, CoreEngine::GetInstance()->GetScreenSize().y, -1.0f, 1.0f);

	return orthographic;
}

const glm::vec3 Camera::GetPosition()
{
	return position;
}

void Camera::OnDestroy()
{
	for (auto light : lights)
	{
		delete light;
		light = nullptr;
	}
	lights.clear();
}

void Camera::AddLightSource(LightSource* light_)
{
	lights.push_back(light_);
}

std::vector<LightSource*>& Camera::GetLightSources()
{
	return lights;
}

void Camera::ProcessMouseMovement(float XOffset_, float YOffset_)
{
	XOffset_ *= movementSpeed;
	YOffset_ *= movementSpeed;

	yaw += XOffset_;
	pitch += YOffset_;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	if (yaw < 0.0f)
	{
		yaw += 360.0f;
	}
	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseZoom(int y_)
{
	if (y_ < 0 || y_ > 0)
	{
		position += static_cast<float>(y_) * (forwardVector * 2.0f);
	}
	UpdateCameraVectors();
}

glm::vec2 Camera::GetClippingPlanes() const
{
	return glm::vec2(nearPlane, farPlane);
}

bool Camera::CheckFrustumCulling(BoundingBox box_) const
{
	auto GetVisibility = [](const glm::vec4& clip, const BoundingBox& box)
	{
		double x0 = box.minVert.x * clip.x;
		double x1 = box.maxVert.x * clip.x;
		double y0 = box.minVert.y * clip.y;
		double y1 = box.maxVert.y * clip.y;
		double z0 = box.minVert.z * clip.z + clip.w;
		double z1 = box.maxVert.z * clip.z + clip.w;
		double p1 = x0 + y0 + z0;
		double p2 = x1 + y0 + z0;
		double p3 = x1 + y1 + z0;
		double p4 = x0 + y1 + z0;
		double p5 = x0 + y0 + z1;
		double p6 = x1 + y0 + z1;
		double p7 = x1 + y1 + z1;
		double p8 = x0 + y1 + z1;

		if (p1 <= 0 && p2 <= 0 && p3 <= 0 && p4 <= 0 && p5 <= 0 && p6 <= 0 && p7 <= 0 && p8 <= 0) {
			return false;
		}
		if (p1 > 0 && p2 > 0 && p3 > 0 && p4 > 0 && p5 > 0 && p6 > 0 && p7 > 0 && p8 > 0) {
			return true;
		}

		return true;
	};

	bool v0 = GetVisibility(GetPlane(Right), box_);
	if (v0 == false) {
		return false;
	}

	bool v1 = GetVisibility(GetPlane(Left), box_);
	if (v1 == false) {
		return false;
	}

	bool v2 = GetVisibility(GetPlane(Bottom), box_);
	if (v2 == false) {
		return false;
	}

	bool v3 = GetVisibility(GetPlane(Top), box_);
	if (v3 == false) {
		return false;
	}

	bool v4 = GetVisibility(GetPlane(Front), box_);
	if (v4 == false) {
		return false;
	}

	if (v0 == true && v1 == true &&
		v2 == true && v3 == true &&
		v4 == true)
	{
		return true;
	}

	return true;
}

bool Camera::CheckFrustumCulling(const glm::vec3& point_) const
{
	for (unsigned int i = 0; i < 6; i++)
	{
		if (plane_data[i][0] * point_.x + plane_data[i][1] * point_.y + plane_data[i][2] * point_.z + plane_data[i][3] <= 0)
		{
			return false;
		}
	}

	return true;
}

void Camera::UpdateCameraVectors()
{
	forwardVector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forwardVector.y = sin(glm::radians(pitch));
	forwardVector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forwardVector = glm::normalize(forwardVector);

	rightVector = glm::normalize(glm::cross(forwardVector, worldUp));
	upVector = glm::normalize(glm::cross(rightVector, forwardVector));

	UpdatePlanes(GetPerspective(), GetView());
}

void Camera::UpdatePlanes(const glm::mat4& proj_, const glm::mat4& view_)
{
	double clip[4][4];
	clip[0][0] = view_[0][0] * proj_[0][0] + view_[0][1] * proj_[1][0] + view_[0][2] * proj_[2][0] + view_[0][3] * proj_[3][0];
	clip[0][1] = view_[0][0] * proj_[0][1] + view_[0][1] * proj_[1][1] + view_[0][2] * proj_[2][1] + view_[0][3] * proj_[3][1];
	clip[0][2] = view_[0][0] * proj_[0][2] + view_[0][1] * proj_[1][2] + view_[0][2] * proj_[2][2] + view_[0][3] * proj_[3][2];
	clip[0][3] = view_[0][0] * proj_[0][3] + view_[0][1] * proj_[1][3] + view_[0][2] * proj_[2][3] + view_[0][3] * proj_[3][3];

	clip[1][0] = view_[1][0] * proj_[0][0] + view_[1][1] * proj_[1][0] + view_[1][2] * proj_[2][0] + view_[1][3] * proj_[3][0];
	clip[1][1] = view_[1][0] * proj_[0][1] + view_[1][1] * proj_[1][1] + view_[1][2] * proj_[2][1] + view_[1][3] * proj_[3][1];
	clip[1][2] = view_[1][0] * proj_[0][2] + view_[1][1] * proj_[1][2] + view_[1][2] * proj_[2][2] + view_[1][3] * proj_[3][2];
	clip[1][3] = view_[1][0] * proj_[0][3] + view_[1][1] * proj_[1][3] + view_[1][2] * proj_[2][3] + view_[1][3] * proj_[3][3];

	clip[2][0] = view_[2][0] * proj_[0][0] + view_[2][1] * proj_[1][0] + view_[2][2] * proj_[2][0] + view_[2][3] * proj_[3][0];
	clip[2][1] = view_[2][0] * proj_[0][1] + view_[2][1] * proj_[1][1] + view_[2][2] * proj_[2][1] + view_[2][3] * proj_[3][1];
	clip[2][2] = view_[2][0] * proj_[0][2] + view_[2][1] * proj_[1][2] + view_[2][2] * proj_[2][2] + view_[2][3] * proj_[3][2];
	clip[2][3] = view_[2][0] * proj_[0][3] + view_[2][1] * proj_[1][3] + view_[2][2] * proj_[2][3] + view_[2][3] * proj_[3][3];

	clip[3][0] = view_[3][0] * proj_[0][0] + view_[3][1] * proj_[1][0] + view_[3][2] * proj_[2][0] + view_[3][3] * proj_[3][0];
	clip[3][1] = view_[3][0] * proj_[0][1] + view_[3][1] * proj_[1][1] + view_[3][2] * proj_[2][1] + view_[3][3] * proj_[3][1];
	clip[3][2] = view_[3][0] * proj_[0][2] + view_[3][1] * proj_[1][2] + view_[3][2] * proj_[2][2] + view_[3][3] * proj_[3][2];
	clip[3][3] = view_[3][0] * proj_[0][3] + view_[3][1] * proj_[1][3] + view_[3][2] * proj_[2][3] + view_[3][3] * proj_[3][3];

	plane_data[Right][0] = clip[0][3] - clip[0][0];
	plane_data[Right][1] = clip[1][3] - clip[1][0];
	plane_data[Right][2] = clip[2][3] - clip[2][0];
	plane_data[Right][3] = clip[3][3] - clip[3][0];
	Normalize(Right);

	plane_data[Left][0] = clip[0][3] + clip[0][0];
	plane_data[Left][1] = clip[1][3] + clip[1][0];
	plane_data[Left][2] = clip[2][3] + clip[2][0];
	plane_data[Left][3] = clip[3][3] + clip[3][0];
	Normalize(Left);

	plane_data[Bottom][0] = clip[0][3] + clip[0][1];
	plane_data[Bottom][1] = clip[1][3] + clip[1][1];
	plane_data[Bottom][2] = clip[2][3] + clip[2][1];
	plane_data[Bottom][3] = clip[3][3] + clip[3][1];
	Normalize(Bottom);

	plane_data[Top][0] = clip[0][3] - clip[0][1];
	plane_data[Top][1] = clip[1][3] - clip[1][1];
	plane_data[Top][2] = clip[2][3] - clip[2][1];
	plane_data[Top][3] = clip[3][3] - clip[3][1];
	Normalize(Top);

	plane_data[Front][0] = clip[0][3] - clip[0][2];
	plane_data[Front][1] = clip[1][3] - clip[1][2];
	plane_data[Front][2] = clip[2][3] - clip[2][2];
	plane_data[Front][3] = clip[3][3] - clip[3][2];
	Normalize(Front);

	plane_data[Back][0] = clip[0][3] + clip[0][2];
	plane_data[Back][1] = clip[1][3] + clip[1][2];
	plane_data[Back][2] = clip[2][3] + clip[2][2];
	plane_data[Back][3] = clip[3][3] + clip[3][2];
	Normalize(Back);
}

void Camera::Normalize(Plane plane)
{
	double magnitude = glm::sqrt(plane_data[plane][0] * plane_data[plane][0] + plane_data[plane][1] * plane_data[plane][1] + plane_data[plane][2] * plane_data[plane][2]);

	plane_data[plane][0] /= magnitude;
	plane_data[plane][1] /= magnitude;
	plane_data[plane][2] /= magnitude;
	plane_data[plane][3] /= magnitude;
}
