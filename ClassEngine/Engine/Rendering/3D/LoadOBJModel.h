#ifndef LOADOBJMODEL_H
#define LOADOBJMODEL_H

#include <sstream>

#include "Mesh.h"
#include "../../Graphics/TextureHandler.h"
#include "../../Math/BoundingBox.h"
#include "../../FX/MaterialLoader.h"

class LoadOBJModel
{
public:
	LoadOBJModel();
	~LoadOBJModel();

	void LoadModel(const std::string &filename_);
	void LoadModel(const std::string &filename_, const std::string &matName_);
	std::vector<Vertex> GetVerts();
	std::vector<int> GetIndices();
	std::vector<SubMesh> GetSubMeshes();

	BoundingBox GetBoundingBox();

	void OnDestroy();

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<int> indices;
	std::vector<int> normalIndices;
	std::vector<int> textureIndices;
	std::vector<Vertex> meshVertices;
	std::vector<SubMesh> meshes;

	void PostProcessing();
	void LoadMaterial(const std::string &filename_);
	void LoadMaterialLibrary(const std::string& matName_);
	Material currentMaterial;

	BoundingBox boundingBox;

};

#endif //!LOADOBJMODEL_H
