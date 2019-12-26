#include "OctSpatialPartition.h"
#include <glm/gtx/string_cast.hpp>

int OctNode::childNumber = 0;

OctNode::OctNode(glm::vec3 position_, float size_, OctNode* parent_) : octBounds(nullptr), parent(nullptr), children(), objectList(std::vector<GameObject*>())
{
	octBounds = new BoundingBox();
	octBounds->minVert = position_;
	octBounds->maxVert = position_ + glm::vec3(size_);	

	size = size_;

	for (int i = 0; i < 8; i++)
	{
		children[i] = 0;
	}

	parent = parent_;
}

OctNode::~OctNode()
{
	OnDestroy();
}

void OctNode::OnDestroy()
{
	delete octBounds;
	octBounds = nullptr;

	if (!objectList.empty())
	{
		for (auto go : objectList)
		{
			go = nullptr;
		}
		objectList.clear();
	}

	for (int i = 0; i < 8; i++)
	{
		if (children[i] != nullptr)
		{
			delete children[i];
			children[i] = nullptr;
		}
	}
}

void OctNode::Octify(int depth_)
{
	if (depth_ > 0 && this)
	{
		float half = size / 2.0f;
		children[OCT_TLF] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);
		children[OCT_BLF] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z + half), half, this);
		children[OCT_BRF] = new OctNode(glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z + half), half, this);
		children[OCT_TRF] = new OctNode(glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);
		
		children[OCT_TLR] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z), half, this);
		children[OCT_BLR] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z), half, this);
		children[OCT_BRR] = new OctNode(glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z), half, this);
		children[OCT_TRR] = new OctNode(glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z), half, this);

		childNumber += 8;
	}
	if (depth_ > 0 && this)
	{
		for (int i = 0; i < 8; i++)
		{
			children[i]->Octify(depth_ - 1);
		}
	}
}

OctNode* OctNode::GetParent()
{
	return parent;
}

OctNode* OctNode::GetChild(OctChildren childPos_)
{
	return  children[childPos_];
}

void OctNode::AddCollisionObject(GameObject* obj_)
{
	objectList.push_back(obj_);
}

int OctNode::GetObjectCount() const
{
	return objectList.size();
}

bool OctNode::isLeaf() const
{
	if (children[0] == nullptr)
	{
		return true;
	}

	return false;
}

BoundingBox* OctNode::GetBoundingBox() const
{
	return octBounds;
}

int OctNode::GetChildCount() const
{
	return childNumber;
}

OctSpatialPartition::OctSpatialPartition(float worldSize_)
{
	root = new OctNode(glm::vec3(-(worldSize_ / 2.0f)), worldSize_, nullptr);
	root->Octify(3);
	std::cout << "There are " << root->GetChildCount() << " child nodes" << std::endl;
}


OctSpatialPartition::~OctSpatialPartition()
{
	OnDestroy();
}

void OctSpatialPartition::OnDestroy()
{
	delete root;
	root = nullptr;

	if (!rayIntersectionList.empty())
	{
		for (auto cell : rayIntersectionList)
		{
			cell = nullptr;
		}
		rayIntersectionList.clear();
	}
}

void OctSpatialPartition::AddObject(GameObject* obj_)
{
	AddObjectToCell(root, obj_);
}

GameObject* OctSpatialPartition::GetCollision(Ray ray_)
{
	rayIntersectionList.clear();
	PrepareCollisionQuery(root, ray_);

	GameObject* hitResult = nullptr;
	float shortestDist = FLT_MAX;

	for (auto node : rayIntersectionList)
	{
		for (auto go : node->objectList)
		{

			if (ray_.isColliding(&go->GetBoundingBox()))
			{
				if (ray_.intersectionDistance < shortestDist)
				{
					hitResult = go;
					shortestDist = ray_.intersectionDistance;
				}
			}
		}
	}

	return hitResult;
}

void OctSpatialPartition::AddObjectToCell(OctNode* cell_, GameObject* obj_)
{
	if (cell_)
	{
		BoundingBox nodeBox = *cell_->GetBoundingBox();
		if (nodeBox.Intersects(&obj_->GetBoundingBox()))
		{
			if (cell_->isLeaf())
			{
				cell_->AddCollisionObject(obj_);
				std::cout << "Added " << obj_->GetTag() << " to cell: " << glm::to_string(nodeBox.minVert) << std::endl;
			}
			else
			{
				for (int i = 0; i < 8; i++)
				{
					AddObjectToCell(cell_->GetChild(static_cast<OctNode::OctChildren>(i)), obj_);
				}
			}
		}
	}
}

void OctSpatialPartition::PrepareCollisionQuery(OctNode* cell_, Ray ray_)
{
	if (cell_)
	{
		if (ray_.isColliding(cell_->GetBoundingBox()))
		{
			if (cell_->isLeaf())
			{
				rayIntersectionList.push_back(cell_);
			}
			else
			{
				for (int i = 0; i < 8; i++)
				{
					PrepareCollisionQuery(cell_->GetChild(static_cast<OctNode::OctChildren>(i)), ray_);
				}
			}
		}
	}
}
