#ifndef MESH_H
#define MESH_H

#include <main.h>
#include <vector>
#include "Material.h"


struct Vertex {
	Vec3 position;      // Vec3
	Vec3 normal;        // Vec3
	Vec2 texCoords;     // Vec2
	Vec3 normalTangent;  // Vec3 (assuming this is a typo for "normalTangent")
};

class Mesh {
	private:
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;
		uint32 VAO, VBO, EBO;
		Material material;

		bool setup();
		void cleanup();
		GLuint createDefaultShader() const;

	public:
		Mesh();
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const Material& material);
		~Mesh();
		
		// Copy/move constructors and operators
		Mesh(const Mesh& other);
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(const Mesh& other);
		Mesh& operator=(Mesh&& other) noexcept;
		
		// Data access
		const std::vector<Vertex>& getVertices() const;
		const std::vector<uint32>& getIndices() const;
		
		// Material access
		const Material& getMaterial() const;
		void setMaterial(const Material& mat);
		
		// OpenGL operations
		void bind() const;
		void unbind() const;
		void draw() const;
		
		// Utility
		size_t getVertexCount() const;
		size_t getIndexCount() const;
		bool isValid() const;
		
		// Optional: mesh manipulation
		void updateVertices(const std::vector<Vertex>& newVertices);
		void updateIndices(const std::vector<uint32>& newIndices);
		
		// Static factory methods for primitive meshes
		static Mesh CreateTriangle();
};

#endif // MESH_H