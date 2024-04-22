#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		~SceneManager();

		//SceneManager(const SceneManager& other) = delete;
		//SceneManager(SceneManager&& other) = delete;
		//SceneManager& operator=(const SceneManager& other) = delete;
		//SceneManager& operator=(SceneManager&& other) = delete;

		Scene& CreateScene(const std::string& name);

		void Update();
		void Render();
		void HandleDeletion();

	private:
		//SceneManager() = default;

		std::vector<Scene*> m_Scenes;
	};
}
