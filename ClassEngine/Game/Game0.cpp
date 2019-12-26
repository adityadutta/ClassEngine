#include "Game0.h"

Game0::Game0() : GameInterface(), currentScene(nullptr), sceneNum(0)
{
}


Game0::~Game0()
{
	OnDestroy();
}

bool Game0::OnCreate()
{
	if (currentScene)
		delete currentScene;

	if (CoreEngine::GetInstance()->GetCurrentScene() == 0)
	{
		currentScene = new StartScene();
		if (!currentScene->OnCreate())
		{
			return false;
		}
	}

	sceneNum = 0;

	return true;
}

void Game0::Update(const float deltaTime_)
{
	if (sceneNum != CoreEngine::GetInstance()->GetCurrentScene())
	{
		BuildScene();
	}

	currentScene->Update(deltaTime_);
}

void Game0::Render()
{
	currentScene->Render();
}

void Game0::OnDestroy()
{
	if (currentScene)
	{
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
}

void Game0::BuildScene()
{
	if (currentScene)
	{
		delete currentScene;
		currentScene = nullptr;
	}

	switch (CoreEngine::GetInstance()->GetCurrentScene())
	{
	case 1:
		currentScene = new GameScene();
		break;
	case 2:
		currentScene = new ParticleScene();
		break;
	default:
		currentScene = new StartScene();
		break;
	}

	if (!currentScene->OnCreate())
	{
		CoreEngine::GetInstance()->SetIsRunning(false);
	}

	sceneNum = CoreEngine::GetInstance()->GetCurrentScene();
}
