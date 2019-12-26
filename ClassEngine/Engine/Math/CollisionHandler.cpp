#include "CollisionHandler.h"
#include "../Core/CoreEngine.h"

std::unique_ptr<CollisionHandler> CollisionHandler::collisionHandlerInstance = nullptr;
std::vector<GameObject*> CollisionHandler::prevCollisions = std::vector<GameObject*>();
OctSpatialPartition* CollisionHandler::scenePartition = nullptr;

CollisionHandler* CollisionHandler::GetInstance()
{
	if (collisionHandlerInstance.get() == nullptr)
	{
		collisionHandlerInstance.reset(new CollisionHandler);
	}

	return collisionHandlerInstance.get();
}

void CollisionHandler::OnCreate(float worldSize_)
{
	prevCollisions.clear();
	scenePartition = new OctSpatialPartition(worldSize_);
}

void CollisionHandler::AddObject(GameObject* gameObject_)
{
	scenePartition->AddObject(gameObject_);
}

void CollisionHandler::Update(glm::vec2 mousePosition_, int buttonType_)
{
	Ray mouseRay = CollisionDetection::ScreenToPosRay(mousePosition_, CoreEngine::GetInstance()->GetScreenSize(), CoreEngine::GetInstance()->GetCamera());

	if(scenePartition != nullptr)
	{
		GameObject* hitResult = scenePartition->GetCollision(mouseRay);
		if(hitResult)
		{
			hitResult->SetHit(true, buttonType_);
		}
		for (auto prev : prevCollisions)
		{
			if (hitResult != prev && prev != nullptr)
			{
				prev->SetHit(false, buttonType_);
				prev = nullptr;
			}
		}
		prevCollisions.clear();

		if (hitResult)
		{
			prevCollisions.push_back(hitResult);
		}
		
	}
}

void CollisionHandler::OnDestroy()
{
	delete scenePartition;
	scenePartition = nullptr;
	
	if(!prevCollisions.empty())
	{
		for(auto go : prevCollisions)
		{
			go = nullptr;
		}
		prevCollisions.clear();
	}
}

CollisionHandler::CollisionHandler()
{
}


CollisionHandler::~CollisionHandler()
{
	OnDestroy();
}
