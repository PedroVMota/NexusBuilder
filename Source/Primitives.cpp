#include "Primitives.h"
#include <cmath>

Mesh Primitives::createCube(float size) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	float half = size * 0.5f;
	
	// Define cube vertices (positions, normals, texture coordinates)
	vertices = {
		// Front face (Z+)
		{{-half, -half,  half}, {0, 0, 1}, {0, 0}, {1, 0, 0}},
		{{ half, -half,  half}, {0, 0, 1}, {1, 0}, {1, 0, 0}},
		{{ half,  half,  half}, {0, 0, 1}, {1, 1}, {1, 0, 0}},
		{{-half,  half,  half}, {0, 0, 1}, {0, 1}, {1, 0, 0}},
		
		// Back face (Z-)
		{{ half, -half, -half}, {0, 0, -1}, {0, 0}, {-1, 0, 0}},
		{{-half, -half, -half}, {0, 0, -1}, {1, 0}, {-1, 0, 0}},
		{{-half,  half, -half}, {0, 0, -1}, {1, 1}, {-1, 0, 0}},
		{{ half,  half, -half}, {0, 0, -1}, {0, 1}, {-1, 0, 0}},
		
		// Left face (X-)
		{{-half, -half, -half}, {-1, 0, 0}, {0, 0}, {0, 0, 1}},
		{{-half, -half,  half}, {-1, 0, 0}, {1, 0}, {0, 0, 1}},
		{{-half,  half,  half}, {-1, 0, 0}, {1, 1}, {0, 0, 1}},
		{{-half,  half, -half}, {-1, 0, 0}, {0, 1}, {0, 0, 1}},
		
		// Right face (X+)
		{{ half, -half,  half}, {1, 0, 0}, {0, 0}, {0, 0, -1}},
		{{ half, -half, -half}, {1, 0, 0}, {1, 0}, {0, 0, -1}},
		{{ half,  half, -half}, {1, 0, 0}, {1, 1}, {0, 0, -1}},
		{{ half,  half,  half}, {1, 0, 0}, {0, 1}, {0, 0, -1}},
		
		// Top face (Y+)
		{{-half,  half,  half}, {0, 1, 0}, {0, 0}, {1, 0, 0}},
		{{ half,  half,  half}, {0, 1, 0}, {1, 0}, {1, 0, 0}},
		{{ half,  half, -half}, {0, 1, 0}, {1, 1}, {1, 0, 0}},
		{{-half,  half, -half}, {0, 1, 0}, {0, 1}, {1, 0, 0}},
		
		// Bottom face (Y-)
		{{-half, -half, -half}, {0, -1, 0}, {0, 0}, {1, 0, 0}},
		{{ half, -half, -half}, {0, -1, 0}, {1, 0}, {1, 0, 0}},
		{{ half, -half,  half}, {0, -1, 0}, {1, 1}, {1, 0, 0}},
		{{-half, -half,  half}, {0, -1, 0}, {0, 1}, {1, 0, 0}}
	};
	
	// Define cube indices (two triangles per face)
	indices = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
	};
	
	return Mesh(vertices, indices, Material::CreateDefaultMaterial());
}

Mesh Primitives::createCylinder(float radius, float height, int segments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generateCylinderVertices(vertices, indices, radius, height, segments);
	
	return Mesh(vertices, indices, Material::CreateDefaultMaterial());
}

Mesh Primitives::createPlane(float width, float height, int widthSegments, int heightSegments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generatePlaneVertices(vertices, indices, width, height, widthSegments, heightSegments);
	
	return Mesh(vertices, indices, Material::CreateDefaultMaterial());
}

Mesh Primitives::createSphere(float radius, int latitudeSegments, int longitudeSegments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generateSphereVertices(vertices, indices, radius, latitudeSegments, longitudeSegments);
	
	return Mesh(vertices, indices, Material::CreateDefaultMaterial());
}

Mesh Primitives::createCone(float radius, float height, int segments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generateConeVertices(vertices, indices, radius, height, segments);
	
	return Mesh(vertices, indices, Material::CreateDefaultMaterial());
}

Mesh Primitives::createTorus(float majorRadius, float minorRadius, int majorSegments, int minorSegments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generateTorusVertices(vertices, indices, majorRadius, minorRadius, majorSegments, minorSegments);
	
	return Mesh(vertices, indices, Material::CreateDefaultMaterial());
}

// Overloaded methods with custom material
Mesh Primitives::createCube(const Material& material, float size) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	float half = size * 0.5f;
	
	// Define cube vertices (positions, normals, texture coordinates)
	vertices = {
		// Front face (Z+)
		{{-half, -half,  half}, {0, 0, 1}, {0, 0}, {1, 0, 0}},
		{{ half, -half,  half}, {0, 0, 1}, {1, 0}, {1, 0, 0}},
		{{ half,  half,  half}, {0, 0, 1}, {1, 1}, {1, 0, 0}},
		{{-half,  half,  half}, {0, 0, 1}, {0, 1}, {1, 0, 0}},
		
		// Back face (Z-)
		{{ half, -half, -half}, {0, 0, -1}, {0, 0}, {-1, 0, 0}},
		{{-half, -half, -half}, {0, 0, -1}, {1, 0}, {-1, 0, 0}},
		{{-half,  half, -half}, {0, 0, -1}, {1, 1}, {-1, 0, 0}},
		{{ half,  half, -half}, {0, 0, -1}, {0, 1}, {-1, 0, 0}},
		
		// Left face (X-)
		{{-half, -half, -half}, {-1, 0, 0}, {0, 0}, {0, 0, 1}},
		{{-half, -half,  half}, {-1, 0, 0}, {1, 0}, {0, 0, 1}},
		{{-half,  half,  half}, {-1, 0, 0}, {1, 1}, {0, 0, 1}},
		{{-half,  half, -half}, {-1, 0, 0}, {0, 1}, {0, 0, 1}},
		
		// Right face (X+)
		{{ half, -half,  half}, {1, 0, 0}, {0, 0}, {0, 0, -1}},
		{{ half, -half, -half}, {1, 0, 0}, {1, 0}, {0, 0, -1}},
		{{ half,  half, -half}, {1, 0, 0}, {1, 1}, {0, 0, -1}},
		{{ half,  half,  half}, {1, 0, 0}, {0, 1}, {0, 0, -1}},
		
		// Top face (Y+)
		{{-half,  half,  half}, {0, 1, 0}, {0, 0}, {1, 0, 0}},
		{{ half,  half,  half}, {0, 1, 0}, {1, 0}, {1, 0, 0}},
		{{ half,  half, -half}, {0, 1, 0}, {1, 1}, {1, 0, 0}},
		{{-half,  half, -half}, {0, 1, 0}, {0, 1}, {1, 0, 0}},
		
		// Bottom face (Y-)
		{{-half, -half, -half}, {0, -1, 0}, {0, 0}, {1, 0, 0}},
		{{ half, -half, -half}, {0, -1, 0}, {1, 0}, {1, 0, 0}},
		{{ half, -half,  half}, {0, -1, 0}, {1, 1}, {1, 0, 0}},
		{{-half, -half,  half}, {0, -1, 0}, {0, 1}, {1, 0, 0}}
	};
	
	// Define cube indices (two triangles per face)
	indices = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
	};
	
	return Mesh(vertices, indices, material);
}

Mesh Primitives::createCylinder(const Material& material, float radius, float height, int segments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generateCylinderVertices(vertices, indices, radius, height, segments);
	
	return Mesh(vertices, indices, material);
}

Mesh Primitives::createPlane(const Material& material, float width, float height, int widthSegments, int heightSegments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generatePlaneVertices(vertices, indices, width, height, widthSegments, heightSegments);
	
	return Mesh(vertices, indices, material);
}

Mesh Primitives::createSphere(const Material& material, float radius, int latitudeSegments, int longitudeSegments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generateSphereVertices(vertices, indices, radius, latitudeSegments, longitudeSegments);
	
	return Mesh(vertices, indices, material);
}

Mesh Primitives::createCone(const Material& material, float radius, float height, int segments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generateConeVertices(vertices, indices, radius, height, segments);
	
	return Mesh(vertices, indices, material);
}

Mesh Primitives::createTorus(const Material& material, float majorRadius, float minorRadius, int majorSegments, int minorSegments) {
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	
	generateTorusVertices(vertices, indices, majorRadius, minorRadius, majorSegments, minorSegments);
	
	return Mesh(vertices, indices, material);
}

void Primitives::generatePlaneVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
									  float width, float height, int widthSegments, int heightSegments) {
	vertices.clear();
	indices.clear();
	
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	
	for (int y = 0; y <= heightSegments; ++y) {
		for (int x = 0; x <= widthSegments; ++x) {
			float u = static_cast<float>(x) / widthSegments;
			float v = static_cast<float>(y) / heightSegments;
			
			Vertex vertex;
			vertex.position = Vec3(
				-halfWidth + u * width,
				0.0f,
				-halfHeight + v * height
			);
			vertex.normal = Vec3(0, 1, 0);
			vertex.texCoords = Vec2(u, v);
			vertex.normalTangent = Vec3(1, 0, 0);
			
			vertices.push_back(vertex);
		}
	}
	
	// Generate indices
	for (int y = 0; y < heightSegments; ++y) {
		for (int x = 0; x < widthSegments; ++x) {
			uint32 a = y * (widthSegments + 1) + x;
			uint32 b = a + 1;
			uint32 c = (y + 1) * (widthSegments + 1) + x;
			uint32 d = c + 1;
			
			addQuadIndices(indices, a, b, d, c);
		}
	}
}

void Primitives::generateSphereVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
									   float radius, int latitudeSegments, int longitudeSegments) {
	vertices.clear();
	indices.clear();
	
	for (int lat = 0; lat <= latitudeSegments; ++lat) {
		float theta = static_cast<float>(lat) * BOTAPICA_PI / latitudeSegments;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);
		
		for (int lon = 0; lon <= longitudeSegments; ++lon) {
			float phi = static_cast<float>(lon) * BOTAPICA_TWO_PI / longitudeSegments;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);
			
			Vec3 position = Vec3(
				radius * sinTheta * cosPhi,
				radius * cosTheta,
				radius * sinTheta * sinPhi
			);
			
			Vec3 normal = glm::normalize(position);
			
			Vertex vertex;
			vertex.position = position;
			vertex.normal = normal;
			vertex.texCoords = Vec2(
				static_cast<float>(lon) / longitudeSegments,
				static_cast<float>(lat) / latitudeSegments
			);
			vertex.normalTangent = Vec3(-sinPhi, 0, cosPhi);
			
			vertices.push_back(vertex);
		}
	}
	
	// Generate indices
	for (int lat = 0; lat < latitudeSegments; ++lat) {
		for (int lon = 0; lon < longitudeSegments; ++lon) {
			uint32 a = lat * (longitudeSegments + 1) + lon;
			uint32 b = a + 1;
			uint32 c = (lat + 1) * (longitudeSegments + 1) + lon;
			uint32 d = c + 1;
			
			addQuadIndices(indices, a, b, d, c);
		}
	}
}

void Primitives::generateCylinderVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
										 float radius, float height, int segments) {
	vertices.clear();
	indices.clear();
	
	float halfHeight = height * 0.5f;
	
	// Generate side vertices
	for (int i = 0; i <= segments; ++i) {
		float angle = static_cast<float>(i) * BOTAPICA_TWO_PI / segments;
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);
		
		Vec3 normal = Vec3(cosAngle, 0, sinAngle);
		Vec3 tangent = Vec3(-sinAngle, 0, cosAngle);
		
		// Bottom vertex
		Vertex bottomVertex;
		bottomVertex.position = Vec3(radius * cosAngle, -halfHeight, radius * sinAngle);
		bottomVertex.normal = normal;
		bottomVertex.texCoords = Vec2(static_cast<float>(i) / segments, 0);
		bottomVertex.normalTangent = tangent;
		vertices.push_back(bottomVertex);
		
		// Top vertex
		Vertex topVertex;
		topVertex.position = Vec3(radius * cosAngle, halfHeight, radius * sinAngle);
		topVertex.normal = normal;
		topVertex.texCoords = Vec2(static_cast<float>(i) / segments, 1);
		topVertex.normalTangent = tangent;
		vertices.push_back(topVertex);
	}
	
	// Generate side indices
	for (int i = 0; i < segments; ++i) {
		uint32 a = i * 2;
		uint32 b = a + 1;
		uint32 c = (i + 1) * 2;
		uint32 d = c + 1;
		
		addQuadIndices(indices, a, c, d, b);
	}
	
	// Generate top and bottom caps
	uint32 centerBottomIndex = vertices.size();
	uint32 centerTopIndex = centerBottomIndex + 1;
	
	// Center vertices for caps
	Vertex centerBottom;
	centerBottom.position = Vec3(0, -halfHeight, 0);
	centerBottom.normal = Vec3(0, -1, 0);
	centerBottom.texCoords = Vec2(0.5f, 0.5f);
	centerBottom.normalTangent = Vec3(1, 0, 0);
	vertices.push_back(centerBottom);
	
	Vertex centerTop;
	centerTop.position = Vec3(0, halfHeight, 0);
	centerTop.normal = Vec3(0, 1, 0);
	centerTop.texCoords = Vec2(0.5f, 0.5f);
	centerTop.normalTangent = Vec3(1, 0, 0);
	vertices.push_back(centerTop);
	
	// Bottom cap indices
	for (int i = 0; i < segments; ++i) {
		uint32 a = centerBottomIndex;
		uint32 b = i * 2;
		uint32 c = ((i + 1) % segments) * 2;
		
		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}
	
	// Top cap indices
	for (int i = 0; i < segments; ++i) {
		uint32 a = centerTopIndex;
		uint32 b = ((i + 1) % segments) * 2 + 1;
		uint32 c = i * 2 + 1;
		
		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}
}

void Primitives::generateConeVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
									 float radius, float height, int segments) {
	vertices.clear();
	indices.clear();
	
	float halfHeight = height * 0.5f;
	
	// Generate base vertices
	for (int i = 0; i <= segments; ++i) {
		float angle = static_cast<float>(i) * BOTAPICA_TWO_PI / segments;
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);
		
		Vec3 basePos = Vec3(radius * cosAngle, -halfHeight, radius * sinAngle);
		Vec3 apexPos = Vec3(0, halfHeight, 0);
		
		// Calculate normal for side face
		Vec3 sideDir = apexPos - basePos;
		Vec3 radialDir = Vec3(cosAngle, 0, sinAngle);
		Vec3 sideNormal = glm::normalize(glm::cross(sideDir, Vec3(-sinAngle, 0, cosAngle)));
		
		// Base vertex for side
		Vertex sideVertex;
		sideVertex.position = basePos;
		sideVertex.normal = sideNormal;
		sideVertex.texCoords = Vec2(static_cast<float>(i) / segments, 0);
		sideVertex.normalTangent = Vec3(-sinAngle, 0, cosAngle);
		vertices.push_back(sideVertex);
		
		// Apex vertex for side
		Vertex apexVertex;
		apexVertex.position = apexPos;
		apexVertex.normal = sideNormal;
		apexVertex.texCoords = Vec2(static_cast<float>(i) / segments, 1);
		apexVertex.normalTangent = Vec3(-sinAngle, 0, cosAngle);
		vertices.push_back(apexVertex);
	}
	
	// Generate side indices
	for (int i = 0; i < segments; ++i) {
		uint32 a = i * 2;
		uint32 b = a + 1;
		uint32 c = ((i + 1) % segments) * 2;
		uint32 d = c + 1;
		
		// Triangle for side face
		indices.push_back(a);
		indices.push_back(c);
		indices.push_back(b);
	}
	
	// Generate base cap
	uint32 centerIndex = vertices.size();
	
	Vertex centerVertex;
	centerVertex.position = Vec3(0, -halfHeight, 0);
	centerVertex.normal = Vec3(0, -1, 0);
	centerVertex.texCoords = Vec2(0.5f, 0.5f);
	centerVertex.normalTangent = Vec3(1, 0, 0);
	vertices.push_back(centerVertex);
	
	// Base cap indices
	for (int i = 0; i < segments; ++i) {
		uint32 a = centerIndex;
		uint32 b = i * 2;
		uint32 c = ((i + 1) % segments) * 2;
		
		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}
}

void Primitives::generateTorusVertices(std::vector<Vertex>& vertices, std::vector<uint32>& indices,
									  float majorRadius, float minorRadius, int majorSegments, int minorSegments) {
	vertices.clear();
	indices.clear();
	
	for (int i = 0; i <= majorSegments; ++i) {
		float majorAngle = static_cast<float>(i) * BOTAPICA_TWO_PI / majorSegments;
		float cosMajor = cos(majorAngle);
		float sinMajor = sin(majorAngle);
		
		for (int j = 0; j <= minorSegments; ++j) {
			float minorAngle = static_cast<float>(j) * BOTAPICA_TWO_PI / minorSegments;
			float cosMinor = cos(minorAngle);
			float sinMinor = sin(minorAngle);
			
			Vec3 position = Vec3(
				(majorRadius + minorRadius * cosMinor) * cosMajor,
				minorRadius * sinMinor,
				(majorRadius + minorRadius * cosMinor) * sinMajor
			);
			
			Vec3 normal = Vec3(
				cosMinor * cosMajor,
				sinMinor,
				cosMinor * sinMajor
			);
			
			Vertex vertex;
			vertex.position = position;
			vertex.normal = normal;
			vertex.texCoords = Vec2(
				static_cast<float>(i) / majorSegments,
				static_cast<float>(j) / minorSegments
			);
			vertex.normalTangent = Vec3(-sinMajor, 0, cosMajor);
			
			vertices.push_back(vertex);
		}
	}
	
	// Generate indices
	for (int i = 0; i < majorSegments; ++i) {
		for (int j = 0; j < minorSegments; ++j) {
			uint32 a = i * (minorSegments + 1) + j;
			uint32 b = a + 1;
			uint32 c = (i + 1) * (minorSegments + 1) + j;
			uint32 d = c + 1;
			
			addQuadIndices(indices, a, b, d, c);
		}
	}
}

Vec3 Primitives::calculateNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3) {
	Vec3 edge1 = p2 - p1;
	Vec3 edge2 = p3 - p1;
	return glm::normalize(glm::cross(edge1, edge2));
}

void Primitives::addQuadIndices(std::vector<uint32>& indices, uint32 a, uint32 b, uint32 c, uint32 d) {
	// First triangle
	indices.push_back(a);
	indices.push_back(b);
	indices.push_back(c);
	
	// Second triangle
	indices.push_back(c);
	indices.push_back(d);
	indices.push_back(a);
}
