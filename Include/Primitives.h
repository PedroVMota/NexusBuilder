#pragma once

#include "main.h"
#include "Mesh.h"

class Primitives {
public:
	// Static methods to create primitive meshes with default material
	static Mesh createCube(float size = 1.0f);
	static Mesh createCylinder(float radius = 0.5f, float height = 1.0f, int segments = 32);
	static Mesh createPlane(float width = 1.0f, float height = 1.0f, int widthSegments = 1, int heightSegments = 1);
	static Mesh createSphere(float radius = 0.5f, int latitudeSegments = 32, int longitudeSegments = 32);
	static Mesh createCone(float radius = 0.5f, float height = 1.0f, int segments = 32);
	static Mesh createTorus(float majorRadius = 0.75f, float minorRadius = 0.25f, int majorSegments = 32, int minorSegments = 16);
	
	// Static methods to create primitive meshes with custom material
	static Mesh createCube(const Material& material, float size = 1.0f);
	static Mesh createCylinder(const Material& material, float radius = 0.5f, float height = 1.0f, int segments = 32);
	static Mesh createPlane(const Material& material, float width = 1.0f, float height = 1.0f, int widthSegments = 1, int heightSegments = 1);
	static Mesh createSphere(const Material& material, float radius = 0.5f, int latitudeSegments = 32, int longitudeSegments = 32);
	static Mesh createCone(const Material& material, float radius = 0.5f, float height = 1.0f, int segments = 32);
	static Mesh createTorus(const Material& material, float majorRadius = 0.75f, float minorRadius = 0.25f, int majorSegments = 32, int minorSegments = 16);

private:
	// Helper methods for generating vertices and indices
	static void generatePlaneVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
									 float width, float height, int widthSegments, int heightSegments);
	static void generateSphereVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
									  float radius, int latitudeSegments, int longitudeSegments);
	static void generateCylinderVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
										float radius, float height, int segments);
	static void generateConeVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
									float radius, float height, int segments);
	static void generateTorusVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
									 float majorRadius, float minorRadius, int majorSegments, int minorSegments);
	
	// Utility methods
	static Vec3 calculateNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3);
	static void addQuadIndices(std::vector<uint32>& indices, uint32 a, uint32 b, uint32 c, uint32 d);
};