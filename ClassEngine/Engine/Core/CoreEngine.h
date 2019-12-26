#ifndef COREENGINE_H
#define COREENGINE_H


#include "Window.h"
#include "Timer.h"
#include "GameInterface.h"
#include "Scene.h"
#include "../Graphics/ShaderHandler.h"
#include "../Graphics/TextureHandler.h"
#include "../Camera/Camera.h"
#include "../Events/EventListener.h"
#include "../Rendering/SceneGraph.h"

#include<memory>
#include<time.h>
#include<stdlib.h>
#include <glm/detail/type_vec2.hpp>

class CoreEngine
{
public:
	CoreEngine(const CoreEngine&) = delete; //Copy constructor
	CoreEngine(CoreEngine&&) = delete;//move constructor
	CoreEngine operator=(const CoreEngine&) = delete;
	CoreEngine operator=(CoreEngine&&) = delete;

	bool OnCreate(std::string name_, int width_, int height_);
	void Run();
	bool GetIsRunning();
	void SetIsRunning(bool isRunning_);
	void SetGameInterface(GameInterface* gameInterface_);

	void SetCurrentScene(int currentScene_);
	int GetCurrentScene() const;
	glm::vec2 GetScreenSize() const;

	void NotifyOfMousePressed(int x_, int y_);
	void NotifyOfMouseRelease(int x_, int y_, int buttonType_);
	void NotifyOfMouseMove(int x_, int y_);
	void NotifyOfMouseScroll(int y_);

	void SetCamera(Camera* camera_);
	Camera* GetCamera();

	void ExitGame();

	//Get Engine instance
	static CoreEngine* GetInstance();
private:
	CoreEngine();
	~CoreEngine();

	void Update(const float deltaTime_);
	void Render();
	void OnDestroy();

	Window* window;
	Timer timer;
	bool isRunning;
	unsigned int fps;

	Camera* camera;

	GameInterface* gameInterface;
	int currentScene;

	static std::unique_ptr<CoreEngine> engineInstance;
	friend std::default_delete<CoreEngine>;

};

#endif //!COREENGINE_H