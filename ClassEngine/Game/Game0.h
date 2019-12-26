#ifndef GAME0_H
#define GAME0_H

#include "../Engine/Core/CoreEngine.h"
#include "Scenes/StartScene.h"
#include "Scenes/GameScene.h"
#include "Scenes/ParticleScene.h"

class Game0 : public GameInterface
{
public:
	Game0();
	virtual ~Game0();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void OnDestroy();

private:
	int sceneNum;
	Scene* currentScene;

	void BuildScene();
};

#endif //!GAME0_H