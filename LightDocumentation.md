# Light Class Implementation

## Overview

The `Light` class is a comprehensive lighting system for the Botapica engine that inherits from `GameObject`, providing full 3D transformation capabilities along with lighting-specific properties.

## Features

### Light Types
- **Directional Light**: Simulates distant light sources like the sun (parallel rays)
- **Point Light**: Omnidirectional light source with distance attenuation
- **Spot Light**: Cone-shaped light beam with direction and angle controls

### Properties
- **Color**: RGB color values (Vec3)
- **Intensity**: Light brightness multiplier
- **Attenuation**: Distance-based light falloff (constant, linear, quadratic)
- **Direction**: Light direction vector (for directional and spot lights)
- **Spot Cone**: Inner and outer cone angles for spot lights

## Class Hierarchy
```
Object
  └── GameObject (provides position, rotation, scale, transform matrix)
      └── Light (adds lighting-specific properties and behavior)
```

## Usage Examples

### Creating Lights

```cpp
// Directional Light (Sun)
Light* sunLight = Light::CreateDirectionalLight("Sun");
sunLight->SetColor(Vec3(1.0f, 0.95f, 0.8f));
sunLight->SetDirection(Vec3(-0.3f, -1.0f, -0.2f));

// Point Light (Lamp)
Light* lamp = Light::CreatePointLight("Lamp", Vec3(0.0f, 2.0f, 0.0f));
lamp->SetColor(Vec3(1.0f, 0.9f, 0.7f));
lamp->SetAttenuation(1.0f, 0.09f, 0.032f);

// Spot Light (Flashlight)
Light* flashlight = Light::CreateSpotLight("Flashlight", 
                                          Vec3(0.0f, 1.0f, 0.0f), 
                                          Vec3(0.0f, -1.0f, 0.0f));
flashlight->SetSpotCone(12.5f, 17.5f);
```

### Manual Construction

```cpp
// Manual construction with custom parameters
Light light("MyLight", Vec3(1.0f, 0.0f, 0.0f), Light::LightType::POINT);
light.SetColor(Vec3(0.8f, 0.2f, 0.2f)); // Red light
light.SetIntensity(1.5f);
```

### Accessing Properties

```cpp
// Get light information
Vec3 position = light.GetPosition();
Vec3 color = light.GetColor();
float intensity = light.GetIntensity();
Light::LightType type = light.GetLightType();

// For point/spot lights
float radius = light.GetBoundingRadius();
float constant = light.GetConstantAttenuation();

// For directional/spot lights
Vec3 direction = light.GetDirection();

// For spot lights only
float innerAngle = light.GetCutOff();
float outerAngle = light.GetOuterCutOff();
```

## Integration with Game Loop

```cpp
void GameLoop() {
    float deltaTime = GetDeltaTime();
    
    // Update all lights
    for (Light* light : lights) {
        if (light->IsActive()) {
            light->Update(deltaTime);
        }
    }
}
```

## Shader Integration

The Light class provides all necessary data for shader uniforms:

```glsl
// Example shader uniforms
struct Light {
    int type;           // 0=directional, 1=point, 2=spot
    vec3 position;      // World position
    vec3 direction;     // Light direction
    vec3 color;         // Light color
    float intensity;    // Light intensity
    
    // Attenuation
    float constant;
    float linear;
    float quadratic;
    
    // Spot light
    float cutOff;
    float outerCutOff;
};
```

## Memory Management

The Light class follows the same memory management patterns as other GameObjects:
- Use factory methods for convenient creation
- Call `Destroy()` to mark for deletion
- Use proper cleanup in destructors

## Performance Considerations

- **Bounding Radius**: Each light calculates its effective radius for culling
- **Active State**: Inactive lights can be skipped in rendering
- **Light Type**: Different light types have different computational costs
- **Attenuation**: Point and spot lights use quadratic attenuation for realistic falloff

## File Structure

- **Header**: `Include/Light.h` - Class declaration
- **Implementation**: `Source/Light.cpp` - Class implementation
- **Example**: `Source/LightExamples.cpp` - Usage examples
