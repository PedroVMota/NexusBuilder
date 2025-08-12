#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include <vector>
#include <cstdint>
#include <main.h>

using String = std::string;
using uint32 = std::uint32_t;

class Object 
{
private:
	static uint32 s_NextInstanceID;
	uint32 m_InstanceID;
	String m_Name;
	bool m_IsDestroyed;

public:
	Object();
	Object(const String& name);
	virtual ~Object();

	uint32 GetInstanceID() const { return m_InstanceID; }
	const String& GetName() const { return m_Name; }
	void SetName(const String& name) { m_Name = name; }
	
	bool IsDestroyed() const { return m_IsDestroyed; }
	void Destroy();

	virtual void OnDestroy() {}

	static void DestroyObject(Object* obj);
	static Object* FindObjectByName(const String& name);
	static std::vector<Object*> FindObjectsByName(const String& name);

	bool operator==(const Object& other) const;
	bool operator!=(const Object& other) const;
	
	explicit operator bool() const { return !m_IsDestroyed; }
};

#endif // OBJECT_H