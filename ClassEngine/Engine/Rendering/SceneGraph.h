#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <memory>

#include "3D/GameObject.h"
#include "../Math/CollisionHandler.h"

class SceneGraph
{
public:
	SceneGraph(const SceneGraph&) = delete; //Copy constructor
	SceneGraph(SceneGraph&&) = delete;//move constructor
	SceneGraph operator=(const SceneGraph&) = delete;
	SceneGraph operator=(SceneGraph&&) = delete;

	static SceneGraph* GetInstance();

	void AddGameObject(GameObject* gameObject_, std::string name_ = "");
	void AddModel(Model* model_);
	GameObject* GetGameObject(std::string name_);

	void Update(const float deltaTime_);
	void Render(Camera* camera_);
	void OnDestroy();

private:
	SceneGraph();
	~SceneGraph();

	static std::unique_ptr<SceneGraph> sceneGraphInstance;
	friend std::default_delete<SceneGraph>;

	static std::map<std::string, GameObject*> sceneGameObjects;
	static std::map<GLuint, std::vector<Model*>> sceneModels;
};

#endif //!SCENEGRAPH_H