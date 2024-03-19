#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

engine::GameObject::~GameObject() = default;

void engine::GameObject::Update(){}

void engine::GameObject::Render() const
{

}

//void engine::GameObject::SetTexture(const std::string& filename)
//{
//
//}
//
//void engine::GameObject::SetPosition(float x, float y)
//{
//
//}
