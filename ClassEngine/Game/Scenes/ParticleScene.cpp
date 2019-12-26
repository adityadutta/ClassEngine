#include "ParticleScene.h"



ParticleScene::ParticleScene() : Scene()
{
}


ParticleScene::~ParticleScene()
{
	OnDestroy();
}

bool ParticleScene::OnCreate()
{
	std::cout << "Creating Particle scene! \n";

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	CoreEngine::GetInstance()->GetCamera()->AddLightSource(new LightSource(glm::vec3(0.0f, 0.0f, 2.0f), 0.1f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f)));

	CollisionHandler::GetInstance()->OnCreate(100.0f);

	Model* appleModel = new Model("Resources/Models/Apple.obj", "Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	Model* diceModel = new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));

	if (!appleModel || !diceModel)
	{
		return false;
	}
	SceneGraph::GetInstance()->AddModel(appleModel);
	SceneGraph::GetInstance()->AddGameObject(new GameObject(appleModel), "Apple");
	SceneGraph::GetInstance()->AddModel(diceModel);
	SceneGraph::GetInstance()->AddGameObject(new GameObject(diceModel), "Dice");

	SceneGraph::GetInstance()->GetGameObject("Dice")->SetPosition(glm::vec3(4.0f, 0.0f, -2.0f));
	SceneGraph::GetInstance()->GetGameObject("Apple")->SetPosition(glm::vec3(-3.0f, -1.0f, -6.0f));

	particles = new ParticleGenerator(ShaderHandler::GetInstance()->GetShader("particleShader"), "fire2.png", 500, 5.0f);
	smokeParticles = new ParticleGenerator(ShaderHandler::GetInstance()->GetShader("particleShader"), "smoke.png", 500, 3.0f);

	return true;
}

void ParticleScene::Update(const float deltaTime_)
{
	SceneGraph::GetInstance()->Update(deltaTime_);
	// Update particles
	GLfloat randomVelX = ((rand() % 100) - 100);
	particles->Update(deltaTime_, glm::vec3(-2.0f, -2.0f, -5.0f), 2, glm::vec3(randomVelX, -100.0f,1.0f));
	randomVelX = ((rand() % 100) - 50);
	GLfloat randomVelY = ((rand() % 100) - 50);
	smokeParticles->Update(deltaTime_, glm::vec3(2.0f, -2.0f, -5.0f), 3, glm::vec3(randomVelX, randomVelY, 1.0f));
}

void ParticleScene::Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
	particles->Render((CoreEngine::GetInstance()->GetCamera()));
	smokeParticles->Render((CoreEngine::GetInstance()->GetCamera()));
}

void ParticleScene::OnDestroy()
{
	delete particles;
	particles = nullptr;
	
	delete smokeParticles;
	smokeParticles = nullptr;
}
