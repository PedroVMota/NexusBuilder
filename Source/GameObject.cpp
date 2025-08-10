#include "../Include/GameObject.h"

GameObject::GameObject() 
    : Object("GameObject"), m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f), m_Active(true)
{
}

GameObject::GameObject(const String& name) 
    : Object(name), m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f), m_Active(true)
{
}

GameObject::GameObject(const String& name, const Vec3& position) 
    : Object(name), m_Position(position), m_Rotation(0.0f), m_Scale(1.0f), m_Active(true)
{
}

GameObject::~GameObject()
{
}

Mat4 GameObject::GetTransformMatrix() const
{
    Mat4 translation = glm::translate(Mat4(1.0f), m_Position);
    Mat4 rotation = glm::rotate(Mat4(1.0f), glm::radians(m_Rotation.x), Vec3(1, 0, 0));
    rotation = glm::rotate(rotation, glm::radians(m_Rotation.y), Vec3(0, 1, 0));
    rotation = glm::rotate(rotation, glm::radians(m_Rotation.z), Vec3(0, 0, 1));
    Mat4 scale = glm::scale(Mat4(1.0f), m_Scale);
    
    return translation * rotation * scale;
}

void GameObject::Translate(const Vec3& translation)
{
    m_Position += translation;
}

void GameObject::Rotate(const Vec3& rotation)
{
    m_Rotation += rotation;
}

void GameObject::Scale(const Vec3& scale)
{
    m_Scale *= scale;
}

void GameObject::OnDestroy()
{
    BOTAPICA_LOG_TRACE("GameObject destroyed: " << GetName());
    Object::OnDestroy();
}

GameObject* GameObject::CreateGameObject(const String& name)
{
    return new GameObject(name);
}

GameObject* GameObject::CreateGameObject(const String& name, const Vec3& position)
{
    return new GameObject(name, position);
}