#include "../Include/Light.h"
#include <algorithm>
#include <limits>
#include <cmath>

Light::Light(LightType type) 
	: GameObject("Light"), lightType(type), color(1.0f, 1.0f, 1.0f), intensity(1.0f),
	  constant(1.0f), linear(0.09f), quadratic(0.032f),
	  direction(0.0f, -1.0f, 0.0f), cutOff(12.5f), outerCutOff(15.0f)
{
}

Light::Light(const String& name, LightType type) 
	: GameObject(name), lightType(type), color(1.0f, 1.0f, 1.0f), intensity(1.0f),
	  constant(1.0f), linear(0.09f), quadratic(0.032f),
	  direction(0.0f, -1.0f, 0.0f), cutOff(12.5f), outerCutOff(15.0f)
{
}

Light::Light(const String& name, const Vec3& position, LightType type) 
	: GameObject(name, position), lightType(type), color(1.0f, 1.0f, 1.0f), intensity(1.0f),
	  constant(1.0f), linear(0.09f), quadratic(0.032f),
	  direction(0.0f, -1.0f, 0.0f), cutOff(12.5f), outerCutOff(15.0f)
{
}

Light::~Light()
{
}

void Light::SetAttenuation(float constant, float linear, float quadratic)
{
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void Light::SetSpotCone(float innerCutOff, float outerCutOff)
{
	this->cutOff = innerCutOff;
	this->outerCutOff = outerCutOff;
}

void Light::Update(float deltaTime)
{
	// Call parent Update
	GameObject::Update(deltaTime);
	
	// Light-specific update logic can be added here
	// For example, animated lights, flickering effects, etc.
}

float Light::GetBoundingRadius() const
{
	switch (lightType)
	{
		case LightType::DIRECTIONAL:
			// Directional lights have infinite range
			return std::numeric_limits<float>::max();
			
		case LightType::POINT:
		{
			// Calculate effective radius based on attenuation
			// Using the formula where light contribution becomes negligible (e.g., 1/256 of original intensity)
			float threshold = 1.0f / 256.0f;
			float maxRadius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - intensity / threshold))) / (2 * quadratic);
			return std::max(0.0f, maxRadius);
		}
		
		case LightType::SPOT:
		{
			// For spot lights, use the same calculation as point lights
			// The cone angle affects the coverage area but not the maximum distance
			float threshold = 1.0f / 256.0f;
			float maxRadius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - intensity / threshold))) / (2 * quadratic);
			return std::max(0.0f, maxRadius);
		}
		
		default:
			return 10.0f; // Default radius
	}
}

// Static factory methods
Light* Light::CreateDirectionalLight(const String& name)
{
	Light* light = new Light(name, LightType::DIRECTIONAL);
	light->SetDirection(Vec3(0.0f, -1.0f, 0.0f)); // Default downward direction
	light->SetColor(Vec3(1.0f, 1.0f, 0.9f)); // Slightly warm white
	light->SetIntensity(1.0f);
	return light;
}

Light* Light::CreatePointLight(const String& name, const Vec3& position)
{
	Light* light = new Light(name, position, LightType::POINT);
	light->SetColor(Vec3(1.0f, 1.0f, 1.0f)); // Pure white
	light->SetIntensity(1.0f);
	light->SetAttenuation(1.0f, 0.09f, 0.032f); // Standard attenuation values
	return light;
}

Light* Light::CreateSpotLight(const String& name, const Vec3& position, const Vec3& direction)
{
	Light* light = new Light(name, position, LightType::SPOT);
	light->SetColor(Vec3(1.0f, 1.0f, 1.0f)); // Pure white
	light->SetIntensity(1.0f);
	light->SetDirection(glm::normalize(direction));
	light->SetAttenuation(1.0f, 0.09f, 0.032f); // Standard attenuation values
	light->SetSpotCone(12.5f, 15.0f); // Inner and outer cone angles in degrees
	return light;
}
