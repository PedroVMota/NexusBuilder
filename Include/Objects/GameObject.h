#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Object.h"

class GameObject : public Object
{
private:
	Vec3 m_Position;
	Vec3 m_Rotation;
	Vec3 m_Scale;
	bool m_Active;

public:
	GameObject();
	GameObject(const String& name);
	GameObject(const String& name, const Vec3& position);
	virtual ~GameObject();

	const Vec3& GetPosition() const { return m_Position; }
	void SetPosition(const Vec3& position) { m_Position = position; }
	
	const Vec3& GetRotation() const { return m_Rotation; }
	void SetRotation(const Vec3& rotation) { m_Rotation = rotation; }
	
	const Vec3& GetScale() const { return m_Scale; }
	void SetScale(const Vec3& scale) { m_Scale = scale; }
	
	bool IsActive() const { return m_Active && !IsDestroyed(); }
	void SetActive(bool active) { m_Active = active; }

	Mat4 GetTransformMatrix() const;
	
	void Translate(const Vec3& translation);
	void Rotate(const Vec3& rotation);
	void Scale(const Vec3& scale);

	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void Render() {}
	virtual void OnDestroy() override;

	static GameObject* CreateGameObject(const String& name = "GameObject");
	static GameObject* CreateGameObject(const String& name, const Vec3& position);
};

#endif // GAMEOBJECT_H