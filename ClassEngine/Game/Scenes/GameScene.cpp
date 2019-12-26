#include "GameScene.h"



GameScene::GameScene() : Scene()
{
}


GameScene::~GameScene()
{
	OnDestroy();
}

bool GameScene::OnCreate()
{
	std::cout << "Creating game scene! \n";

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	CoreEngine::GetInstance()->GetCamera()->AddLightSource(new LightSource(glm::vec3(0.0f, 0.0f, 2.0f), 0.1f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f)));

	CollisionHandler::GetInstance()->OnCreate(100.0f);

	Model* appleModel = new Model("Resources/Models/Apple.obj", "Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	Model* diceModel = new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));

	if(!appleModel || !diceModel)
	{
		return false;
	}
	SceneGraph::GetInstance()->AddModel(appleModel);
	SceneGraph::GetInstance()->AddGameObject(new GameObject(appleModel), "Apple");
	SceneGraph::GetInstance()->AddModel(diceModel);
	SceneGraph::GetInstance()->AddGameObject(new GameObject(diceModel), "Dice");

	SceneGraph::GetInstance()->GetGameObject("Dice")->SetPosition(glm::vec3(4.0f, 0.0f, -2.0f));
	SceneGraph::GetInstance()->GetGameObject("Apple")->SetPosition(glm::vec3(-3.0f, -1.0f, -6.0f));

	SceneGraph::GetInstance()->AddGameObject(new GameObject(appleModel), "Apple2");
	SceneGraph::GetInstance()->GetGameObject("Apple2")->SetPosition(glm::vec3(-10.0f, -1.0f, -6.0f));
	SceneGraph::GetInstance()->AddGameObject(new GameObject(diceModel), "Dice2");
	SceneGraph::GetInstance()->GetGameObject("Dice2")->SetPosition(glm::vec3(10.0f, -1.0f, -6.0f));

	return true;
}

void GameScene::Update(const float deltaTime_)
{
	SceneGraph::GetInstance()->Update(deltaTime_);
}

void GameScene::Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//TODO: DEBUG REMOVE
	/*BoundingBox box = SceneGraph::GetInstance()->GetGameObject("Apple")->GetBoundingBox();
	isVisible = (CoreEngine::GetInstance()->GetCamera()->CheckFrustumCulling(box) != isVisible) ? CoreEngine::GetInstance()->GetCamera()->CheckFrustumCulling(box) : isVisible;
	std::string str = isVisible ? "VISIBLE\n" : "NOT VISIBLE\n";
	std::cout << "The Apple is " << str;

	std::string str2 = CoreEngine::GetInstance()->GetCamera()->CheckFrustumCulling(SceneGraph::GetInstance()->GetGameObject("Dice")->GetPosition()) == true ? "VISIBLE\n" : "NOT VISIBLE\n";
	std::cout << "The Dice is " << str2;*/

	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}

void GameScene::OnDestroy()
{

}
