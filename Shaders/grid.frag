#version 330 core
out vec4 FragColor;

in vec3 WorldPos;

uniform vec3 gridColor;
uniform float gridSize;
uniform vec3 cameraPos;

void main() {
    vec2 coord = WorldPos.xz / gridSize;
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    float line = min(grid.x, grid.y);
    
    // Fade based on horizontal distance from camera (ignore Y difference)
    vec2 horizontalDistance = WorldPos.xz - cameraPos.xz;
    float distance = length(horizontalDistance);
    float fadeStart = gridSize * 20.0;
    float fadeEnd = gridSize * 50.0;
    float alpha = 1.0 - smoothstep(fadeStart, fadeEnd, distance);
    
    // Make grid lines more visible
    float gridAlpha = (1.0 - min(line, 1.0)) * alpha * 0.5;
    
    FragColor = vec4(gridColor, gridAlpha);
}