#include "../Include/ObjectManager.h"
#include "../Include/Object.h"

std::vector<Object*> ObjectManager::s_AllObjects;
std::vector<Object*> ObjectManager::s_ObjectsToDestroy;
std::unordered_map<String, std::vector<Object*>> ObjectManager::s_ObjectsByName;

void ObjectManager::RegisterObject(Object* obj)
{
	if (obj)
	{
		s_AllObjects.push_back(obj);
		AddToNameMap(obj);
		
		BOTAPICA_LOG_TRACE("Registered object: " << obj->GetName() << " (ID: " << obj->GetInstanceID() << ")");
	}
}

void ObjectManager::UnregisterObject(Object* obj)
{
	if (obj)
	{
		auto it = std::find(s_AllObjects.begin(), s_AllObjects.end(), obj);
		if (it != s_AllObjects.end())
		{
			s_AllObjects.erase(it);
		}
		
		RemoveFromNameMap(obj);
		
		auto destroyIt = std::find(s_ObjectsToDestroy.begin(), s_ObjectsToDestroy.end(), obj);
		if (destroyIt != s_ObjectsToDestroy.end())
		{
			s_ObjectsToDestroy.erase(destroyIt);
		}
		
		BOTAPICA_LOG_TRACE("Unregistered object: " << obj->GetName() << " (ID: " << obj->GetInstanceID() << ")");
	}
}

void ObjectManager::MarkForDestruction(Object* obj)
{
	if (obj && std::find(s_ObjectsToDestroy.begin(), s_ObjectsToDestroy.end(), obj) == s_ObjectsToDestroy.end())
	{
		s_ObjectsToDestroy.push_back(obj);
		BOTAPICA_LOG_TRACE("Marked for destruction: " << obj->GetName() << " (ID: " << obj->GetInstanceID() << ")");
	}
}

void ObjectManager::ProcessDestroyQueue()
{
	for (Object* obj : s_ObjectsToDestroy)
	{
		UnregisterObject(obj);
		delete obj;
	}
	s_ObjectsToDestroy.clear();
}

void ObjectManager::DestroyAllObjects()
{
	BOTAPICA_LOG_INFO("Destroying all objects...");
	
	for (Object* obj : s_AllObjects)
	{
		if (obj && !obj->IsDestroyed())
		{
			obj->OnDestroy();
		}
		delete obj;
	}
	
	s_AllObjects.clear();
	s_ObjectsToDestroy.clear();
	s_ObjectsByName.clear();
	
	BOTAPICA_LOG_INFO("All objects destroyed.");
}

Object* ObjectManager::FindObjectByName(const String& name)
{
	auto it = s_ObjectsByName.find(name);
	if (it != s_ObjectsByName.end() && !it->second.empty())
	{
		for (Object* obj : it->second)
		{
			if (obj && !obj->IsDestroyed())
			{
				return obj;
			}
		}
	}
	return nullptr;
}

std::vector<Object*> ObjectManager::FindObjectsByName(const String& name)
{
	std::vector<Object*> result;
	auto it = s_ObjectsByName.find(name);
	if (it != s_ObjectsByName.end())
	{
		for (Object* obj : it->second)
		{
			if (obj && !obj->IsDestroyed())
			{
				result.push_back(obj);
			}
		}
	}
	return result;
}

void ObjectManager::LogObjectStats()
{
	BOTAPICA_LOG_INFO("Object Statistics:");
	BOTAPICA_LOG_INFO("  Total objects: " << s_AllObjects.size());
	BOTAPICA_LOG_INFO("  Objects pending destruction: " << s_ObjectsToDestroy.size());
	BOTAPICA_LOG_INFO("  Named object groups: " << s_ObjectsByName.size());
}

void ObjectManager::RemoveFromNameMap(Object* obj)
{
	if (!obj) return;
	
	auto it = s_ObjectsByName.find(obj->GetName());
	if (it != s_ObjectsByName.end())
	{
		auto& objects = it->second;
		auto objIt = std::find(objects.begin(), objects.end(), obj);
		if (objIt != objects.end())
		{
			objects.erase(objIt);
		}
		
		if (objects.empty())
		{
			s_ObjectsByName.erase(it);
		}
	}
}

void ObjectManager::AddToNameMap(Object* obj)
{
	if (obj)
	{
		s_ObjectsByName[obj->GetName()].push_back(obj);
	}
}