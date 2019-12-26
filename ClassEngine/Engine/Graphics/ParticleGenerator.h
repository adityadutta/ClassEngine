#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H
#include <vector>

#include <glew.h>
#include <glm/glm.hpp>

#include "../Camera/Camera.h"
#include "TextureHandler.h"


// Represents a single particle and its state
struct Particle {
	glm::vec3 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
	// Constructor
	ParticleGenerator(GLuint shaderProgram_, std::string textureName_, GLuint amount_, float lifeTime_);
	// Update all particles
	void Update(GLfloat deltaTime_, glm::vec3 position_, GLuint newParticles_, glm::vec3 velocity_);
	// Render all particles
	void Render(Camera* camera_);
private:
	// State
	std::vector<Particle> particles;
	GLuint amount;
	float lifeTime;
	// Stores the index of the last particle used (for quick access to next dead particle)
	GLuint lastUsedParticle = 0;

	// Render state
	GLuint shaderProgram;
	GLuint texture;
	GLuint VAO;
	GLuint textureLoc, projLoc, viewLoc, offsetLoc, colorLoc;
	// Initializes buffer and vertex attributes
	void init();
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle();
	// Respawns particle
	void respawnParticle(Particle &particle_, glm::vec3 position_, glm::vec3 velocity_);
};

#endif //!PARTICLEGENERATOR_H