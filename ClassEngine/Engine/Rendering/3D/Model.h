#ifndef MODEL_H
#define MODEL_H

#include<glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "LoadOBJModel.h"

class Model
{
public:
	Model(const std::string &objPath_, const std::string &matPath_, GLuint shaderProgram_);
	Model(const std::string &objPath_, GLuint shaderProgram_);
	~Model();

	void AddMesh(Mesh* mesh_);
	void Render(Camera* camera_);
	void OnDestroy();

	int CreateInstance(glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_);
	void UpdateInstance(int index_, glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_);
	glm::mat4 GetTransform(int index_) const;

	BoundingBox GetBoundingBox();

	GLuint GetShaderProgram() const;

private:
	std::vector<Mesh*> subMeshes;
	GLuint shaderProgram;

	glm::mat4 GetTransform(glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_) const;
	void LoadModel();
	std::vector<glm::mat4> modelInstances;
	LoadOBJModel *obj;

	BoundingBox boundingBox;
};


#endif // !MODEL_H