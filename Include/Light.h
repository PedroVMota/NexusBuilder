#pragma once

#include "GameObject.h"
#include <glm/glm.hpp>

class Light : public GameObject {
public:
	enum class LightType {
		DIRECTIONAL,
		POINT,
		SPOT
	};

private:
	LightType lightType;
	Vec3 color;
	float intensity;
	
	// Point and Spot light properties
	float constant;
	float linear;
	float quadratic;
	
	// Spot light properties
	Vec3 direction;
	float cutOff;
	float outerCutOff;
	
public:
	Light(LightType type = LightType::POINT);
	Light(const String& name, LightType type = LightType::POINT);
	Light(const String& name, const Vec3& position, LightType type = LightType::POINT);
	virtual ~Light();
	
	// Light properties
	void SetLightType(LightType type) { lightType = type; }
	LightType GetLightType() const { return lightType; }
	
	void SetColor(const Vec3& lightColor) { color = lightColor; }
	const Vec3& GetColor() const { return color; }
	
	void SetIntensity(float lightIntensity) { intensity = lightIntensity; }
	float GetIntensity() const { return intensity; }
	
	// Attenuation (for point and spot lights)
	void SetAttenuation(float constant, float linear, float quadratic);
	float GetConstantAttenuation() const { return constant; }
	float GetLinearAttenuation() const { return linear; }
	float GetQuadraticAttenuation() const { return quadratic; }
	
	// Direction (for directional and spot lights)
	void SetDirection(const Vec3& lightDirection) { direction = lightDirection; }
	const Vec3& GetDirection() const { return direction; }
	
	// Spot light cone (for spot lights)
	void SetSpotCone(float innerCutOff, float outerCutOff);
	float GetCutOff() const { return cutOff; }
	float GetOuterCutOff() const { return outerCutOff; }
	
	// Override from GameObject
	void Update(float deltaTime) override;
	float GetBoundingRadius() const;
	
	// Static factory methods
	static Light* CreateDirectionalLight(const String& name = "DirectionalLight");
	static Light* CreatePointLight(const String& name = "PointLight", const Vec3& position = Vec3(0.0f));
	static Light* CreateSpotLight(const String& name = "SpotLight", const Vec3& position = Vec3(0.0f), const Vec3& direction = Vec3(0.0f, -1.0f, 0.0f));
};
