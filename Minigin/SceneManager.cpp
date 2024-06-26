#include "SceneManager.h"
#include "Scene.h"

namespace engine {
	namespace sceneManager
	{
		std::vector<Scene*> currentScenes{};
		std::map<std::string, std::unique_ptr<Scene>> sceneMap;

		Scene& CreateScene(const std::string& name)
		{
			auto sceneptr = new Scene(name);
			std::unique_ptr<Scene> scene(sceneptr);
			sceneMap[name] = std::move(scene);
			auto ptr = sceneMap[name].get();
			return *ptr;
		}

		void UpdateScenes()
		{
			if (sceneMap.find("") != sceneMap.end()) sceneMap[""]->Update();
			for(auto scene : currentScenes) scene->Update();
		}

		void RenderScenes()
		{
			if (sceneMap.find("") != sceneMap.end()) sceneMap[""]->Render();
			for (auto scene : currentScenes) scene->Render();
		}

		void HandleDeletion()
		{
			if (sceneMap.find("") != sceneMap.end()) sceneMap[""]->HandleDeletion();
			for (auto scene : currentScenes) scene->HandleDeletion();
		}
	}
}
