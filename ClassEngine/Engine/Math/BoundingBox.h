#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <glm/glm.hpp>

struct BoundingBox
{
	glm::vec3 minVert, maxVert;
	glm::mat4 transform;

	inline BoundingBox()
	{
		minVert = maxVert = glm::vec3();
		transform = glm::mat4();
	}

	inline BoundingBox(glm::vec3 minVert_, glm::vec3 maxVert_, glm::mat4 transform_)
	{
		minVert = minVert_;
		maxVert = maxVert_;
		transform = transform_;
	}

	inline bool Intersects(BoundingBox* box_)
	{
		glm::vec3 minCorner = GetTransformedPoint(minVert, transform);
		glm::vec3 maxCorner = GetTransformedPoint(maxVert, transform);

		glm::vec3 otherMinCorner = GetTransformedPoint(box_->minVert, box_->transform);
		glm::vec3 otherMaxCorner = GetTransformedPoint(box_->maxVert, box_->transform);


		if ((otherMinCorner.x <= maxCorner.x && otherMaxCorner.x >= minCorner.x) &&
			(otherMinCorner.y <= maxCorner.y && otherMaxCorner.y >= minCorner.y) &&
			(otherMinCorner.z <= maxCorner.z && otherMaxCorner.z >= minCorner.z))
		{
			return true;
		}

		return false;
	}

	inline glm::vec3 GetTransformedPoint(glm::vec3 point_, glm::mat4 transform_)
	{
		return (glm::vec3(transform_[3].x, transform_[3].y, transform_[3].z) + point_);
	}
};

#endif //!BOUNDINGBOX_H
