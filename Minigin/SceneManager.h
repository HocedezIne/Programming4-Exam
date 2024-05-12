#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <map>

namespace engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		~SceneManager();

		Scene& CreateScene(const std::string& name);

		void SetCurrentScene(const std::string& name);

		void Update();
		void Render();
		void HandleDeletion();

	private:
		Scene* currentScene{nullptr};
		std::map<std::string, Scene*> m_Scenes;
	};
}
