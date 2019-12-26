#ifndef PARTICLESCENE_H
#define PARTICLESCENE_H

#include "../../Engine/Core/CoreEngine.h"
#include "../../Engine/Rendering/3D/GameObject.h"
#include "../../Engine/FX/LightSource.h"
#include "../../Engine/Graphics/ParticleGenerator.h"

class ParticleScene : public Scene
{
public:
	ParticleScene();
	virtual ~ParticleScene();

	bool OnCreate() override;
	void Update(const float deltaTime_) override;
	void Render() override;
	void OnDestroy() override;

private:
	//DEBUG
	bool isVisible = true;

	ParticleGenerator* particles;
	ParticleGenerator* smokeParticles;
};

#endif //!PARTICLESCENE_H