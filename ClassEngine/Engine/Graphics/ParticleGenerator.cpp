#include "ParticleGenerator.h"
#include "../Rendering/3D/GameObject.h"


ParticleGenerator::ParticleGenerator(GLuint shaderProgram_, std::string textureName_, GLuint amount_, float lifeTime_)
	: shaderProgram(shaderProgram_), texture(0), amount(amount_), lifeTime(lifeTime_)
{
	TextureHandler::GetInstance()->CreateTexture(textureName_, "Resources/Textures/" + textureName_);
	texture = TextureHandler::GetInstance()->GetTexture(textureName_);
	this->init();
}

void ParticleGenerator::Update(GLfloat deltaTime_, glm::vec3 position_, GLuint newParticles_, glm::vec3 velocity_)
{
	// Add new particles 
	for (GLuint i = 0; i < newParticles_; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], position_, velocity_);
	}
	// Update all particles
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.Life -= deltaTime_; // reduce life
		if (p.Life > 0.0f)
		{	// particle is alive, thus update
			p.Position -= p.Velocity * deltaTime_;
			p.Color.a -= deltaTime_ * 2.5;
		}
	}
}

// Render all particles
void ParticleGenerator::Render(Camera* camera_)
{
	//disable the depth mask in order to stop depth writes and thus blending artifacts while keeping the flame from drawing over other objects
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(shaderProgram);
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));

			glUniform2f(offsetLoc, particle.Position.x, particle.Position.y);
			glUniform4f(colorLoc, particle.Color.x, particle.Color.y, particle.Color.z, particle.Color.w);
			glUniform1i(textureLoc, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	// Don't forget to reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(true);

}

void ParticleGenerator::init()
{
	// Set up mesh and attribute properties
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; ++i)
	{
		this->particles.push_back(Particle());
	}

	textureLoc = glGetUniformLocation(shaderProgram, "sprite");
	projLoc = glGetUniformLocation(shaderProgram, "projection");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	offsetLoc = glGetUniformLocation(shaderProgram, "offset");
	colorLoc = glGetUniformLocation(shaderProgram, "color");
}

GLuint ParticleGenerator::firstUnusedParticle()
{
	// First search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, glm::vec3 position_, glm::vec3 velocity_)
{
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = position_;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = lifeTime;
	particle.Velocity = glm::vec3(velocity_.x, velocity_.y, 1.0f) * 0.1f;
}