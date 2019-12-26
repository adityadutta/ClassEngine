#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "CollisionDetection.h"
#include "Ray.h"
#include "../Rendering/3D/GameObject.h"
#include "../Core/OctSpatialPartition.h"

class CollisionHandler
{
public:
	CollisionHandler(const CollisionHandler&) = delete; //Copy constructor
	CollisionHandler(CollisionHandler&&) = delete;//move constructor
	CollisionHandler operator=(const CollisionHandler&) = delete;
	CollisionHandler operator=(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();

	void OnCreate(float worldSize_);
	void AddObject(GameObject* gameObject_);
	void Update(glm::vec2 mousePosition_, int buttonType_);

	void OnDestroy();
private:
	CollisionHandler();
	~CollisionHandler();

	static std::unique_ptr<CollisionHandler> collisionHandlerInstance;
	friend std::default_delete<CollisionHandler>;

	static std::vector<GameObject*> prevCollisions;
	static OctSpatialPartition* scenePartition;
};

#endif //!COLLISIONHANDLER_H