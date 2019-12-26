#include "LoadOBJModel.h"
#include <glm/gtx/string_cast.hpp>


LoadOBJModel::LoadOBJModel() : vertices(std::vector<glm::vec3>()), normals(std::vector<glm::vec3>()), textureCoords(std::vector<glm::vec2>()), indices(std::vector<int>()), normalIndices(std::vector<int>()), textureIndices(std::vector<int>()), meshVertices(std::vector<Vertex>()), meshes(std::vector<SubMesh>()), currentMaterial(Material())
{
}


LoadOBJModel::~LoadOBJModel()
{
}

void LoadOBJModel::LoadModel(const std::string& filename_)
{
	float minX = std::numeric_limits<float>::max(), minY = std::numeric_limits<float>::max(), minZ = std::numeric_limits<float>::max(), maxX = std::numeric_limits<float>::min(), maxY = std::numeric_limits<float>::min(), maxZ = std::numeric_limits<float>::min();

	std::ifstream in(filename_.c_str(), std::ios::in);
	if(!in)
	{
		Debug::Error("Cannot open OBJ file: " + filename_, __FILE__, __LINE__);
		return;
	}
	std::string line;
	while(std::getline(in, line))
	{
		//Vertex data
		if(line.substr(0, 2) == "v ")
		{
			std::istringstream v(line.substr(2));
			glm::vec3 vert;
			double x, y, z;
			v >> x >> y >> z;

			//min
			if(x < minX)
			{
				minX = x;
			}
			if (y < minY)
			{
				minY = y;
			}
			if (z < minZ)
			{
				minZ = z;
			}
			//max
			if (x > maxX)
			{
				maxX = x;
			}
			if (y > maxY)
			{
				maxY = y;
			}
			if (z > maxZ)
			{
				maxZ = z;
			}

			vert = glm::vec3(x, y, z);
			vertices.push_back(vert);
		}
		//Normal data
		if (line.substr(0, 3) == "vn ")
		{
			std::istringstream v(line.substr(3));
			glm::vec3 normal;
			double x, y, z;
			v >> x >> y >> z;
			normal = glm::vec3(x, y, z);
			normals.push_back(normal);
		}
		//Texture Coordinate data
		if (line.substr(0, 3) == "vt ")
		{
			std::istringstream v(line.substr(3));
			glm::vec2 textureCoord;
			double x, y;
			v >> x >> y;
			textureCoord = glm::vec2(x, y);
			textureCoords.push_back(textureCoord);
		}
		//Face data
		//vertex indices/texture indices/normal indices
		if (line.substr(0, 2) == "f ")
		{
			unsigned int vertexIndex[3], textureIndex[3], normalIndex[3];
			char c;
			std::istringstream v(line.substr(2));
			v >> vertexIndex[0] >> c >> textureIndex[0] >> c >> normalIndex[0] >> vertexIndex[1] >> c >> textureIndex[1] >> c >> normalIndex[1] >> vertexIndex[2] >> c >> textureIndex[2] >> c >> normalIndex[2];
			indices.push_back(vertexIndex[0] - 1);
			indices.push_back(vertexIndex[1] - 1);
			indices.push_back(vertexIndex[2] - 1);
			textureIndices.push_back(textureIndex[0] - 1);
			textureIndices.push_back(textureIndex[1] - 1);
			textureIndices.push_back(textureIndex[2] - 1);
			normalIndices.push_back(normalIndex[0] - 1);
			normalIndices.push_back(normalIndex[1] - 1);
			normalIndices.push_back(normalIndex[2] - 1);
		}

		//new material
		else if(line.substr(0, 7) == "usemtl ")
		{
			if(indices.size() > 0)
			{
				PostProcessing();
			}
			LoadMaterial(line.substr(7));
		}

	}

	boundingBox.minVert = glm::vec3(minX, minY, minZ);
	boundingBox.maxVert = glm::vec3(maxX, maxY, maxZ);

	PostProcessing();
}

void LoadOBJModel::LoadModel(const std::string& filename_, const std::string& matName_)
{
	LoadMaterialLibrary(matName_);
	LoadModel(filename_);
}

std::vector<Vertex> LoadOBJModel::GetVerts()
{
	return meshVertices;
}

std::vector<int> LoadOBJModel::GetIndices()
{
	return indices;
}

std::vector<SubMesh> LoadOBJModel::GetSubMeshes()
{
	return meshes;
}

BoundingBox LoadOBJModel::GetBoundingBox()
{
	return boundingBox;
}

void LoadOBJModel::OnDestroy()
{
	vertices.clear();
	normals.clear();
	textureCoords.clear();
	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();
	meshes.clear();
}

void LoadOBJModel::PostProcessing()
{
	for(int i = 0; i < indices.size(); i++)
	{
		Vertex vert;
		vert.position = vertices[indices[i]];
		vert.normal = normals[normalIndices[i]];
		vert.texCoords = textureCoords[textureIndices[i]];
		meshVertices.push_back(vert);
	}
	SubMesh mesh;
	mesh.vertexList = meshVertices;
	mesh.meshIndices = indices;
	mesh.material = currentMaterial;
	meshes.push_back(mesh);
	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();
	currentMaterial = Material();
}

void LoadOBJModel::LoadMaterial(const std::string& filename_)
{
	currentMaterial = MaterialHandler::GetInstance()->GetMaterial(filename_);
}

void LoadOBJModel::LoadMaterialLibrary(const std::string& matName_)
{
	MaterialLoader::LoadMaterial(matName_);
}
