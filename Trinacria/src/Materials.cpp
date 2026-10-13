#include "Trinacria/Materials.h"
#include "Trinacria/Renderer.h"

void TRCN_CORE_NAMESPACE::Material::SetUniforms(int materialIndex) const
{
    Renderer::ShaderProgram.SetUniformVec3(std::format("u_Materials[{}].ambient",
        materialIndex).c_str(), Ambient);

    Renderer::ShaderProgram.SetUniformVec3(std::format("u_Materials[{}].diffuse",
        materialIndex).c_str(), Diffuse);

    Renderer::ShaderProgram.SetUniformVec3(std::format("u_Materials[{}].specular",
        materialIndex).c_str(), Specular);

    Renderer::ShaderProgram.SetUniformFloat(std::format("u_Materials[{}].shininess",
        materialIndex).c_str(), Shininess);
}