#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace engine
{
	class Scene final
	{
		friend Scene& sceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::string name, std::unique_ptr<GameObject> object);
		void Remove(std::string& name);
		GameObject* GetObject(std::string name) const;
		void RemoveAll();

		void Update();
		void Render() const;
		void HandleDeletion();

		Scene() = default;
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector< std::pair<std::string, std::unique_ptr<GameObject>> > m_Objects{};

		static unsigned int m_IdCounter; 
	};
}
