#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include "main.hpp"

class Object;



class ObjectManager
{
private:
    static std::vector<Object*> s_AllObjects;
    static std::vector<Object*> s_ObjectsToDestroy;
    static std::unordered_map<String, std::vector<Object*>> s_ObjectsByName;

    ObjectManager() = delete;
    ~ObjectManager() = delete;

public:
    static void RegisterObject(Object* obj);
    static void UnregisterObject(Object* obj);
    static void MarkForDestruction(Object* obj);
    
    static void ProcessDestroyQueue();
    static void DestroyAllObjects();
    
    static Object* FindObjectByName(const String& name);
    static std::vector<Object*> FindObjectsByName(const String& name);
    static const std::vector<Object*>& GetAllObjects() { return s_AllObjects; }
    
    static size_t GetObjectCount() { return s_AllObjects.size(); }
    static void LogObjectStats();

private:
    static void RemoveFromNameMap(Object* obj);
    static void AddToNameMap(Object* obj);
};

#endif // OBJECTMANAGER_H