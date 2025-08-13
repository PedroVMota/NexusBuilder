#ifndef MATERIALSHADERINTERFACE_H
#define MATERIALSHADERINTERFACE_H
class Material;

class MaterialShaderInterface {
public:
	virtual ~MaterialShaderInterface() = default;
    // Pure virtual functions = interface contract
    virtual void BindMaterialUniforms(const Material& material) = 0;
    virtual bool IsCompatibleWith(const Material& material) = 0;
    virtual void Use() = 0;  // calls glUseProgram
};


#endif
