#include "MaterialLoader.h"



MaterialLoader::~MaterialLoader()
{
}

void MaterialLoader::LoadMaterial(std::string file_)
{
	std::ifstream in(file_.c_str(), std::ios::in);
	if (!in)
	{
		Debug::Error("Cannot open MTL file: " + file_, __FILE__, __LINE__);
		return;
	}
	Material m = Material();
	std::string matName = "";


	std::string line;
	while (std::getline(in, line))
	{
		if (line.substr(0, 7) == "newmtl ")
		{
			if (m.diffuseMap != 0)
			{
				MaterialHandler::GetInstance()->AddMaterial(matName, m);
				m = Material();
			}
			matName = line.substr(7);
			m.diffuseMap = LoadTexture(matName);
		}
		//TODO: Assign values to Material
		//shininess
		if (line.substr(0, 3) == "\tNs")
		{
			std::istringstream v(line.substr(3));
			float Ns;
			v >> Ns;
			m.shininess = Ns;
		}
		//transparency
		if (line.substr(0, 2) == "\td")
		{
			std::istringstream v(line.substr(2));
			float d;
			v >> d;
			m.transparency = d;
		}
		//diffuse
		if (line.substr(0, 3) == "\tKd")
		{
			std::istringstream v(line.substr(3));
			glm::vec3 kd;
			v >> kd.x >> kd.y >> kd.z;
			m.diffuse = kd;
		}
		//ambient
		if (line.substr(0, 3) == "\tKa")
		{
			std::istringstream v(line.substr(3));
			glm::vec3 ka;
			v >> ka.x >> ka.y >> ka.z;
			m.ambient = ka;
		}
		//specular
		if (line.substr(0, 3) == "\tKs")
		{
			std::istringstream v(line.substr(3));
			glm::vec3 ks;
			v >> ks.x >> ks.y >> ks.z;
			m.specular = ks;
		}
	}
	if (m.diffuseMap != 0)
	{
		MaterialHandler::GetInstance()->AddMaterial(matName, m);
	}
}

GLuint MaterialLoader::LoadTexture(std::string filename_)
{
	GLuint currentTexture = TextureHandler::GetInstance()->GetTexture(filename_);
	if (currentTexture == 0)
	{
		TextureHandler::GetInstance()->CreateTexture(filename_, "Resources/Textures/" + filename_ + ".jpg");
		currentTexture = TextureHandler::GetInstance()->GetTexture(filename_);
	}

	return currentTexture;
}
