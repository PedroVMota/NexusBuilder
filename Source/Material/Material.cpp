#include "Material.h"

// Default constructor
Material::Material() 
	: ambient(0.2f, 0.2f, 0.2f),
	  diffuse(0.8f, 0.8f, 0.8f),
	  specular(1.0f, 1.0f, 1.0f),
	  shininess(32.0f),
	  diffuseTexturePath(""),
	  normalTexturePath(""),
	  specularTexturePath("")
{
}

// Parameterized constructor
Material::Material(const Vec3 &ambient, const Vec3 &diffuse, 
				   const Vec3 &specular, float shininess)
	: ambient(ambient),
	  diffuse(diffuse),
	  specular(specular),
	  shininess(shininess),
	  diffuseTexturePath(""),
	  normalTexturePath(""),
	  specularTexturePath("")
{
}

//* THESE ARE TEMPORARY, FUTURE IMPLEMENTATION SHOULD ALLOW USER TO CHANGE MATERIAL WITHIN EDITOR ITSELF. -- NEED TO CREATE MATERIAL EDITING 

// Static factory methods for common materials
Material Material::CreateDefaultMaterial() {
	return Material(
		Vec3(0.2f, 0.2f, 0.2f),	// ambient
		Vec3(0.8f, 0.8f, 0.8f),	// diffuse
		Vec3(1.0f, 1.0f, 1.0f),	// specular
		32.0f					// shininess
	);
}

Material Material::CreateMetalMaterial() {
	return Material(
		Vec3(0.25f, 0.25f, 0.25f),				// ambient - darker for metal
		Vec3(0.4f, 0.4f, 0.4f),					// diffuse - moderate reflection
		Vec3(0.774597f, 0.774597f, 0.774597f),	// specular - high reflectivity
		76.8f									// shininess - very shiny
	);
}

Material Material::CreatePlasticMaterial() {
	return Material(
		Vec3(0.0f, 0.1f, 0.06f),						// ambient - slight green tint
		Vec3(0.0f, 0.50980392f, 0.50980392f),			// diffuse - cyan plastic
		Vec3(0.50196078f, 0.50196078f, 0.50196078f),	// specular - moderate shine
		32.0f											// shininess - moderate
	);
}

Material Material::CreateGlassMaterial() {
	return Material(
		Vec3(0.0f, 0.0f, 0.0f),			// ambient - no ambient for glass
		Vec3(0.588f, 0.670f, 0.729f),	// diffuse - light blue glass
		Vec3(0.9f, 0.9f, 0.9f),			// specular - high reflectivity
		96.0f							// shininess - very shiny
	);
}