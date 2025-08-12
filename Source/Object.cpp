#include "../Include/Object.h"
#include "../Include/ObjectManager.h"

uint32 Object::s_NextInstanceID = 1;

Object::Object() : m_InstanceID(s_NextInstanceID++), m_Name("Object"), m_IsDestroyed(false)
{
    ObjectManager::RegisterObject(this);
}

Object::Object(const String& name) : m_InstanceID(s_NextInstanceID++), m_Name(name), m_IsDestroyed(false)
{
    ObjectManager::RegisterObject(this);
}

Object::~Object()
{
    if (!m_IsDestroyed)
    {
        ObjectManager::UnregisterObject(this);
    }
}

void Object::Destroy()
{
    if (!m_IsDestroyed)
    {
        m_IsDestroyed = true;
        OnDestroy();
        ObjectManager::MarkForDestruction(this);
    }
}

void Object::DestroyObject(Object* obj)
{
    if (obj && !obj->m_IsDestroyed)
    {
        obj->Destroy();
    }
}

Object* Object::FindObjectByName(const String& name)
{
    return ObjectManager::FindObjectByName(name);
}

std::vector<Object*> Object::FindObjectsByName(const String& name)
{
    return ObjectManager::FindObjectsByName(name);
}

bool Object::operator==(const Object& other) const
{
    return m_InstanceID == other.m_InstanceID && !m_IsDestroyed && !other.m_IsDestroyed;
}

bool Object::operator!=(const Object& other) const
{
    return !(*this == other);
}