#pragma once

#include "main.h"
#include <glm/glm.hpp>
#include <string>

class Material {
private:
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	float shininess;

	// TODO = Texture properties
	String diffuseTexturePath;
	String normalTexturePath;
	String specularTexturePath;

public:
	Material();
	Material(const Vec3 &ambient, const Vec3 &diffuse, 
			const Vec3 &specular, float shininess);

	// Getters
	const Vec3 &GetAmbient() const { return ambient;}
	const Vec3 &GetDiffuse() const { return diffuse;}
	const Vec3 &GetSpecular() const { return specular;}
	float GetShininess() const { return shininess;}

	// Setters
	void SetAmbient(const glm::vec3& color) { ambient = color; }
	void SetDiffuse(const glm::vec3& color) { diffuse = color; }
	void SetSpecular(const glm::vec3& color) { specular = color; }
	void SetShininess(float shine) { shininess = shine; }

	// Texture paths (for later)
	const String &GetDiffuseTexturePath() const { return diffuseTexturePath; }
	const String &GetNormalTexturePath() const { return normalTexturePath; }
	const String &GetSpecularTexturePath() const { return specularTexturePath; }

	void SetDiffuseTexturePath(const String &path) { diffuseTexturePath = path; }
	void SetNormalTexturePath(const String &path) { normalTexturePath = path; }
	void SetSpecularTexturePath(const String &path) { specularTexturePath = path; }

	static Material CreateDefaultMaterial();
	static Material CreateMetalMaterial();
	static Material CreatePlasticMaterial();
	static Material CreateGlassMaterial();
};