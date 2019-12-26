#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"

class GameObject
{
public:
	GameObject(Model* model_);
	GameObject(Model* model_, glm::vec3 position_);
	~GameObject();

	void Update(const float deltaTime_);
	void Render(Camera* camera_);

	void OnDestroy();

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;
	float GetAngle() const;

	void SetPosition(glm::vec3 position_);
	void SetRotation(glm::vec3 rotation_);
	void SetScale(glm::vec3 scale_);
	void SetAngle(float angle_);

	std::string GetTag() const;
	void SetTag(std::string tag_);

	BoundingBox GetBoundingBox();

	bool GetHit() const;
	void SetHit(bool hit_, int buttonType_);

	bool GetVisibility() const;
	void SeVisibility(bool visibility_);

private:
	Model* model;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	float angle;
	int modelInstance;

	BoundingBox boundingBox;

	std::string tag;

	bool hit;
	bool visible;
};


#endif // !GAMEOBJECT_H
