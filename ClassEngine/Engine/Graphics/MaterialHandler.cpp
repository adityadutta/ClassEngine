#include "MaterialHandler.h"
#include "../Core/Debug.h"

std::unique_ptr<MaterialHandler> MaterialHandler::materialInstance = nullptr;
std::map<std::string, Material>  MaterialHandler::materials = std::map<std::string, Material>();

MaterialHandler* MaterialHandler::GetInstance()
{
	if (materialInstance.get() == nullptr)
	{
		materialInstance.reset(new MaterialHandler);
	}

	return materialInstance.get();
}

void MaterialHandler::AddMaterial(const std::string& name_, Material mat_)
{
	materials[name_] = mat_;
}

const Material MaterialHandler::GetMaterial(const std::string& name_)
{
	if(materials.find(name_) != materials.end())
	{
		return materials[name_];
	}

	Debug::Warning("Trying to get material " + name_ + " does not exist", __FILE__, __LINE__);
	return Material();
}

void MaterialHandler::OnDestroy()
{
	if(!materials.empty())
	{
		materials.clear();
	}
}


MaterialHandler::MaterialHandler()
{
}


MaterialHandler::~MaterialHandler()
{
	OnDestroy();
}
