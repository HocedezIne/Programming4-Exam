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

		Scene& CreateScene(const std::string& name);

		void Update();
		void LateUpdate();
		void Render();
		void HandleDeletion();

	private:
		std::vector<Scene*> m_Scenes;
	};
}
