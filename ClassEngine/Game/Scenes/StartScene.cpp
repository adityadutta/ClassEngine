#include "StartScene.h"



StartScene::StartScene(): Scene()
{
}


StartScene::~StartScene()
{
	OnDestroy();
}

bool StartScene::OnCreate()
{
	std::cout << "Creating start scene! \n";
	CoreEngine::GetInstance()->SetCurrentScene(2);
	return true;
}

void StartScene::Update(const float deltaTime_)
{

}

void StartScene::Render()
{

}

void StartScene::OnDestroy()
{

}
