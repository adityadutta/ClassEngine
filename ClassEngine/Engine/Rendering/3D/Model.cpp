#include "Model.h"



Model::Model(const std::string &objPath_, const std::string &matPath_, GLuint shaderProgram_) : subMeshes(std::vector<Mesh*>()), shaderProgram(0), modelInstances(std::vector<glm::mat4>())
{
	shaderProgram = shaderProgram_;

	obj = new LoadOBJModel();
	obj->LoadModel(objPath_, matPath_);
	this->LoadModel();
}

Model::Model(const std::string& objPath_, GLuint shaderProgram_) : subMeshes(std::vector<Mesh*>()), shaderProgram(0), modelInstances(std::vector<glm::mat4>())
{
	shaderProgram = shaderProgram_;

	obj = new LoadOBJModel();
	obj->LoadModel(objPath_);
	this->LoadModel();
}

Model::~Model()
{
	OnDestroy();
}

void Model::AddMesh(Mesh* mesh_)
{
	subMeshes.push_back(mesh_);
}

void Model::Render(Camera* camera_)
{
	if (!subMeshes.empty())
	{
		for (Mesh* mesh : subMeshes)
		{
			mesh->Render(camera_, modelInstances);
		}
	}
}

void Model::OnDestroy()
{
	if (!subMeshes.empty())
	{
		for (Mesh* mesh : subMeshes)
		{
			delete mesh;
			mesh = nullptr;
		}
	}

	subMeshes.clear();

	modelInstances.clear();
}

int Model::CreateInstance(glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_)
{
	modelInstances.push_back(GetTransform(position_, angle_, rotation_, scale_));
	return modelInstances.size() - 1;
}

void Model::UpdateInstance(int index_, glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_)
{
	modelInstances[index_] = GetTransform(position_, angle_, rotation_, scale_);
}

glm::mat4 Model::GetTransform(int index_) const
{
	return modelInstances[index_];
}

BoundingBox Model::GetBoundingBox()
{
	return boundingBox;
}

GLuint Model::GetShaderProgram() const
{
	return shaderProgram;
}

glm::mat4 Model::GetTransform(glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_) const
{
	glm::mat4 model;
	model = glm::translate(model, position_);
	model = glm::rotate(model, angle_, rotation_);
	model = glm::scale(model, scale_);

	return model;
}

void Model::LoadModel()
{
	for (int i = 0; i < obj->GetSubMeshes().size(); i++)
	{
		subMeshes.push_back(new Mesh(obj->GetSubMeshes()[i], shaderProgram));
	}

	boundingBox = obj->GetBoundingBox();

	delete obj;
	obj = nullptr;
}