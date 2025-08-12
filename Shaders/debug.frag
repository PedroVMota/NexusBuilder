#version 330 core
in vec3 vertexColor; // Receive interpolated color from vertex shader
out vec4 FragColor;

void main() {
	FragColor = vec4(vertexColor, 1.0);
}