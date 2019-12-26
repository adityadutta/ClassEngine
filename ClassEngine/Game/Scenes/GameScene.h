#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../Engine/Core/CoreEngine.h"
#include "../../Engine/Rendering/3D/GameObject.h"
#include "../../Engine/FX/LightSource.h"

class GameScene: public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	bool OnCreate() override;
	void Update(const float deltaTime_) override;
	void Render() override;
	void OnDestroy() override;

private:
	//DEBUG
	bool isVisible = true;
};

#endif //!GAMESCENE_H