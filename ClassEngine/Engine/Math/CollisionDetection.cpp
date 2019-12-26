#include "CollisionDetection.h"
#include "Ray.h"
#include "../Core/CoreEngine.h"

CollisionDetection::~CollisionDetection()
{
}

Ray CollisionDetection::ScreenToPosRay(glm::vec2 mouseCoords_, glm::vec2 screenSize_, Camera* camera_)
{
	glm::vec4 rayStart_NDC((mouseCoords_.x / screenSize_.x - 0.5f) * 2.0f, (mouseCoords_.y / screenSize_.y - 0.5f) * 2.0f, -1.0f, 1.0f);
	glm::vec4 rayEnd_NDC((mouseCoords_.x / screenSize_.x - 0.5f) * 2.0f, (mouseCoords_.y / screenSize_.y - 0.5f) * 2.0f, 0.0f, 1.0f);

	glm::mat4 inverse = glm::inverse(camera_->GetPerspective() * camera_->GetView());

	glm::vec4 rayStart_WS = inverse * rayStart_NDC;
	rayStart_WS /= rayStart_WS.w;
	glm::vec4 rayEnd_WS = inverse * rayEnd_NDC;
	rayEnd_WS /= rayEnd_WS.w;

	glm::vec3 rayDir_WS(rayEnd_WS - rayStart_WS);
	rayDir_WS = glm::normalize(rayDir_WS);

	return Ray(glm::vec3(rayStart_WS), rayDir_WS);

}

bool CollisionDetection::RayOBBIntersection(Ray* ray_, BoundingBox* box_)
{
	glm::mat4 modelMatrix = box_->transform;
	glm::vec3 rayOrigin = ray_->origin;
	glm::vec3 rayDir = ray_->direction;
	glm::vec3 aabbMin = box_->minVert;
	glm::vec3 aabbMax = box_->maxVert;

	float tMin = CoreEngine::GetInstance()->GetCamera()->GetClippingPlanes().x;
	float tMax = CoreEngine::GetInstance()->GetCamera()->GetClippingPlanes().y;

	//translation from model
	glm::vec3 OBBposition_WS(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

	glm::vec3 delta = OBBposition_WS - rayOrigin;

	// X Axis
	glm::vec3 xAxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
	float e = glm::dot(xAxis, delta);
	float f = glm::dot(rayDir, xAxis);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + aabbMin.x) / f;
		float t2 = (e + aabbMax.x) / f;

		if (t1 > t2)
		{
			float tmp = t1;
			t1 = t2;
			t2 = tmp;
		}
		if (t2 < tMax)
		{
			tMax = t2;
		}
		if (t1 > tMin)
		{
			tMin = t1;
		}
		if (tMax < tMin)
		{
			return  false;
		}
	}
	else
	{
		if (-e + aabbMin.x > 0.0f || -e + aabbMax.x < 0.0f)
		{
			return false;
		}
	}
	// Y Axis
	glm::vec3 yAxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
	e = glm::dot(yAxis, delta);
	f = glm::dot(rayDir, yAxis);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + aabbMin.y) / f;
		float t2 = (e + aabbMax.y) / f;

		if (t1 > t2)
		{
			float tmp = t1;
			t1 = t2;
			t2 = tmp;
		}
		if (t2 < tMax)
		{
			tMax = t2;
		}
		if (t1 > tMin)
		{
			tMin = t1;
		}
		if (tMax < tMin)
		{
			return  false;
		}
	}
	else
	{
		if (-e + aabbMin.y > 0.0f || -e + aabbMax.y < 0.0f)
		{
			return false;
		}
	}

	// Z Axis
	glm::vec3 zAxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
	e = glm::dot(zAxis, delta);
	f = glm::dot(rayDir, zAxis);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + aabbMin.z) / f;
		float t2 = (e + aabbMax.z) / f;

		if (t1 > t2)
		{
			float tmp = t1;
			t1 = t2;
			t2 = tmp;
		}
		if (t2 < tMax)
		{
			tMax = t2;
		}
		if (t1 > tMin)
		{
			tMin = t1;
		}
		if (tMax < tMin)
		{
			return  false;
		}
	}
	else
	{
		if (-e + aabbMin.z > 0.0f || -e + aabbMax.z < 0.0f)
		{
			return false;
		}
	}

	ray_->intersectionDistance = tMin;
	return true;
}
