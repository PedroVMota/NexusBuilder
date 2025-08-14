#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform float gridSize;

out vec3 WorldPos;

void main() {
    // Snap grid to camera position in XZ plane only, keep Y at 0 (world origin height)
    vec3 snappedCameraPos = floor(cameraPos / gridSize) * gridSize;
    snappedCameraPos.y = 0.0; // Force grid to stay at world origin height
    
    vec3 worldPos = aPos + snappedCameraPos;
    worldPos.y = 0.0; // Ensure grid vertices stay at Y=0
    
    WorldPos = worldPos;
    gl_Position = projection * view * vec4(worldPos, 1.0);
}