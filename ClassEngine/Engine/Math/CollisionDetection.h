#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "../Camera/Camera.h"

class Ray;
class BoundingBox;

class CollisionDetection
{
public:
	CollisionDetection(const CollisionDetection&) = delete; //Copy constructor
	CollisionDetection(CollisionDetection&&) = delete;//move constructor
	CollisionDetection operator=(const CollisionDetection&) = delete;
	CollisionDetection operator=(CollisionDetection&&) = delete;
	CollisionDetection() = delete;

	~CollisionDetection();

	static Ray ScreenToPosRay(glm::vec2 mouseCoords_, glm::vec2 screenSize_, Camera* camera_);
	static bool RayOBBIntersection(Ray* ray_, BoundingBox* box_);

};

#endif //!COLLISIONDETECTION_H