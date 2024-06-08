#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <map>


namespace engine
{
	class Scene;
	namespace sceneManager
	{
		extern std::vector<Scene*> currentScenes;
		extern std::map<std::string, std::unique_ptr<Scene>> sceneMap;

		Scene& CreateScene(const std::string& name);
		void UpdateScenes();
		void RenderScenes();
		void HandleDeletion();
	}

}
