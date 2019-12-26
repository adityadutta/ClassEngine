#include "CoreEngine.h"

std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

CoreEngine::CoreEngine() : window(nullptr), isRunning(false), fps(120), gameInterface(nullptr), currentScene(0)
{

}


CoreEngine::~CoreEngine()
{
	OnDestroy();
}

bool CoreEngine::OnCreate(std::string name_, int width_, int height_)
{
	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	window = new Window();

	//Create window
	if (!window->OnCreate(name_, width_, height_))
	{
		Debug::FatalError("Window failed to create!", __FILE__, __LINE__);
		OnDestroy();

		return isRunning = false;
	}

	MouseEventListener::RegisterEngineObject(GetInstance());
	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2);

	ShaderHandler::GetInstance()->CreateProgram("colorShader", "Engine/Shaders/ColorVert.glsl", "Engine/Shaders/ColorFrag.glsl");
	ShaderHandler::GetInstance()->CreateProgram("basicShader", "Engine/Shaders/textureVert.glsl", "Engine/Shaders/textureFrag.glsl");
	ShaderHandler::GetInstance()->CreateProgram("particleShader", "Engine/Shaders/particleVert.glsl", "Engine/Shaders/particleFrag.glsl");

	if (gameInterface)
	{
		if (!gameInterface->OnCreate())
		{
			Debug::FatalError("Failed to create GameInterface", __FILE__, __LINE__);
			OnDestroy();

			return isRunning = false;
		}
	}

	timer.Start();

	Debug::Info("Engine created", __FILE__, __LINE__);

	return isRunning = true;
}

void CoreEngine::Run()
{
	while (isRunning)
	{
		EventListener::Update();
		timer.UpdateFrameTicks();
		Update(timer.GetDeltaTime());
		Render();
		SDL_Delay(timer.GetSleepTime(fps));
	}

	if (!isRunning)
	{
		OnDestroy();
	}
}

bool CoreEngine::GetIsRunning()
{
	return isRunning;
}

void CoreEngine::SetIsRunning(bool isRunning_)
{
	isRunning = isRunning_;
}

void CoreEngine::SetGameInterface(GameInterface* gameInterface_)
{
	gameInterface = gameInterface_;
}

void CoreEngine::SetCurrentScene(int currentScene_)
{
	currentScene = currentScene_;
}

int CoreEngine::GetCurrentScene() const
{
	return currentScene;
}

glm::vec2 CoreEngine::GetScreenSize() const
{
	return glm::vec2(window->GetWidth(), window->GetHeight());
}

void CoreEngine::NotifyOfMousePressed(int x_, int y_)
{

}

void CoreEngine::NotifyOfMouseRelease(int x_, int y_, int buttonType_)
{
	CollisionHandler::GetInstance()->Update(glm::vec2(x_, y_), buttonType_);
}

void CoreEngine::NotifyOfMouseMove(int x_, int y_)
{
	if(camera)
	{
		camera->ProcessMouseMovement(MouseEventListener::GetMouseOffset().x, MouseEventListener::GetMouseOffset().y);
	}
}

void CoreEngine::NotifyOfMouseScroll(int y_)
{
	if(camera)
	{
		camera->ProcessMouseZoom(y_);
	}
}

void CoreEngine::SetCamera(Camera* camera_)
{
	camera = camera_;
}

Camera* CoreEngine::GetCamera()
{
	return camera;
}

void CoreEngine::ExitGame()
{
	isRunning = false;
}

CoreEngine* CoreEngine::GetInstance()
{
	if (engineInstance.get() == nullptr)
	{
		engineInstance.reset(new CoreEngine);
	}

	return engineInstance.get();
}

void CoreEngine::Update(const float deltaTime_)
{
	if (gameInterface)
	{
		gameInterface->Update(deltaTime_);
	}
}

void CoreEngine::Render()
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//CALL GAME RENDER
	if (gameInterface)
	{
		gameInterface->Render();
	}

	SDL_GL_SwapWindow(window->GetWindow());
}

void CoreEngine::OnDestroy()
{

	if (gameInterface)
	{
		gameInterface->OnDestroy();
		delete gameInterface;
		gameInterface = nullptr;
	}

	if(camera)
	{
		camera->OnDestroy();
		delete camera;
		camera = nullptr;
	}

	if (window)
	{
		delete window;
		window = nullptr;
	}

	CollisionHandler::GetInstance()->OnDestroy();
	ShaderHandler::GetInstance()->OnDestroy();
	MaterialHandler::GetInstance()->OnDestroy();
	TextureHandler::GetInstance()->OnDestroy();

	SDL_Quit();
	exit(0);
}
