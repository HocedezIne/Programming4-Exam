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
		Scene& CreateScene(const std::string& name);

		void Update();
		void Render();
		void HandleDeletion();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		~SceneManager();

		std::vector<Scene*> m_Scenes;
	};
}
