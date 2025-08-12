#include "../Include/Mesh.h"

Mesh::Mesh() : VAO(0), VBO(0), EBO(0), material(Material::CreateDefaultMaterial()) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) 
	: vertices(vertices), indices(indices), VAO(0), VBO(0), EBO(0), material(Material::CreateDefaultMaterial()) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	setup();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const Material& material) 
	: vertices(vertices), indices(indices), VAO(0), VBO(0), EBO(0), material(material) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	setup();
}

Mesh::~Mesh() {
	cleanup();
}

Mesh::Mesh(const Mesh& other) : vertices(other.vertices), indices(other.indices), VAO(0), VBO(0), EBO(0), material(other.material) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	if (!other.vertices.empty()) {
		setup();
	}
}

Mesh::Mesh(Mesh&& other) noexcept 
	: vertices(std::move(other.vertices)), indices(std::move(other.indices)),
	  VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), material(std::move(other.material)) {
	other.VAO = other.VBO = other.EBO = 0;
}

Mesh& Mesh::operator=(const Mesh& other) {
	if (this != &other) {
		cleanup();
		vertices = other.vertices;
		indices = other.indices;
		material = other.material;
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		if (!vertices.empty()) {
			setup();
		}
	}
	return *this;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
	if (this != &other) {
		cleanup();
		
		vertices = std::move(other.vertices);
		indices = std::move(other.indices);
		material = std::move(other.material);
		VAO = other.VAO;
		VBO = other.VBO;
		EBO = other.EBO;
		
		other.VAO = other.VBO = other.EBO = 0;
	}
	return *this;
}

const std::vector<Vertex>& Mesh::getVertices() const {
	return vertices;
}

const std::vector<uint32>& Mesh::getIndices() const {
	return indices;
}

const Material& Mesh::getMaterial() const {
	return material;
}

void Mesh::setMaterial(const Material& mat) {
	material = mat;
}

void Mesh::bind() const {
	glBindVertexArray(VAO);
}

void Mesh::unbind() const {
	glBindVertexArray(0);
}

void Mesh::draw() const {
	if (isValid()) {
		// Check if a shader program is currently bound
		GLint currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		
		if (currentProgram == 0) {
			// No shader program bound, create minimal shader for Core Profile
			static GLuint defaultShader = 0;
			if (defaultShader == 0) {
				defaultShader = createDefaultShader();
			}
			
			if (defaultShader != 0) {
				glUseProgram(defaultShader);
				
				bind();
				if (!indices.empty()) {
					glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
				} else {
					glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
				}
				unbind();
				
				glUseProgram(0);
			} else {
				BOTAPICA_LOG_ERROR("Failed to create default shader for mesh rendering");
			}
		} else {
			// Shader program is bound, use modern OpenGL
			bind();
			if (!indices.empty()) {
				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
			} else {
				glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
			}
			unbind();
		}
	}
}

size_t Mesh::getVertexCount() const {
	return vertices.size();
}

size_t Mesh::getIndexCount() const {
	return indices.size();
}

bool Mesh::isValid() const {
	return VAO != 0 && VBO != 0 && !vertices.empty();
}

void Mesh::updateVertices(const std::vector<Vertex>& newVertices) {
	vertices = newVertices;
	setup();
}

void Mesh::updateIndices(const std::vector<uint32>& newIndices) {
	indices = newIndices;
	setup();
}

bool Mesh::setup() {
	if (vertices.empty()) {
		BOTAPICA_LOG_WARN("Attempting to setup mesh with no vertices");
		return false;
	}

	bind();

	// Upload vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Upload index data if available
	if (!indices.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), indices.data(), GL_STATIC_DRAW);
	}

	// Set vertex attributes
	// Position attribute (location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// Normal attribute (location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// Texture coordinate attribute (location 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	// Normal tangent attribute (location 3)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normalTangent));
	glEnableVertexAttribArray(3);

	unbind();

	BOTAPICA_LOG_TRACE("Mesh setup complete - Vertices: " << vertices.size() << ", Indices: " << indices.size());
	return true;
}

void Mesh::cleanup() {
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (EBO != 0) {
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
}

Mesh Mesh::CreateTriangle() {
	std::vector<Vertex> vertices = {
		// Triangle vertices with positions, normals, texture coordinates, and tangents
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Bottom-left
		{{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Bottom-right
		{{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}}  // Top-center
	};
	
	std::vector<uint32> indices = {
		0, 1, 2  // Single triangle
	};
	
	BOTAPICA_LOG_TRACE("Created default triangle mesh");
	return Mesh(vertices, indices);
}

GLuint Mesh::createDefaultShader() const {
	const char* vertexShaderSource = R"(
		#version 410 core
		layout (location = 0) in vec3 aPos;
		
		void main() {
			gl_Position = vec4(aPos, 1.0);
		}
	)";
	
	const char* fragmentShaderSource = R"(
		#version 410 core
		out vec4 FragColor;
		
		void main() {
			FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	)";
	
	// Compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	GLint success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		BOTAPICA_LOG_ERROR("Vertex shader compilation failed: " << infoLog);
		return 0;
	}
	
	// Compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		BOTAPICA_LOG_ERROR("Fragment shader compilation failed: " << infoLog);
		glDeleteShader(vertexShader);
		return 0;
	}
	
	// Link shaders into program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		BOTAPICA_LOG_ERROR("Shader program linking failed: " << infoLog);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return 0;
	}
	
	// Clean up individual shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	BOTAPICA_LOG_TRACE("Created default white shader program");
	return shaderProgram;
}