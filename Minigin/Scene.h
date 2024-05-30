#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include "ColliderComponent.h"

namespace engine
{
	class Scene final
	{
		friend Scene& sceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::unique_ptr<GameObject> object);
		void AddCollidableObject(GameObject* object);
		void Remove(std::unique_ptr<GameObject> object);
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
		std::vector < std::unique_ptr<GameObject>> m_Objects{};

		std::vector <ColliderComponent*> m_CollidableObjects{};

		static unsigned int m_IdCounter; 
	};
}
