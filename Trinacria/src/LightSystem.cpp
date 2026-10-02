#include "Trinacria/LightSystem.h"

#include <array>

#include "Trinacria/Renderer.h"
#include <cstring>


TRCN_CORE_NAMESPACE::Texture TRCN_CORE_NAMESPACE::LightSystem::_pointTexture;
TRCN_CORE_NAMESPACE::Texture TRCN_CORE_NAMESPACE::LightSystem::_spotTexture;
TRCN_CORE_NAMESPACE::Texture TRCN_CORE_NAMESPACE::LightSystem::_directionalTexture;

int TRCN_CORE_NAMESPACE::LightSystem::_pointLightIndex;
int TRCN_CORE_NAMESPACE::LightSystem::_spotLightIndex;
int TRCN_CORE_NAMESPACE::LightSystem::_dirLightIndex;

std::array<float, TRCN_CORE_NAMESPACE::LightSystem::MAX_POINT_LIGHTS * 8> TRCN_CORE_NAMESPACE::LightSystem::_pointLights;
std::array<float, TRCN_CORE_NAMESPACE::LightSystem::MAX_SPOT_LIGHTS * 12> TRCN_CORE_NAMESPACE::LightSystem::_spotLights;
std::array<float, TRCN_CORE_NAMESPACE::LightSystem::MAX_DIRECTIONAL_LIGHTS * 8> TRCN_CORE_NAMESPACE::LightSystem::_dirLights;

void Trinacria::DSL::LightSystem::InitFrame(float strength)
{
    Renderer::ShaderProgram.Bind();
    Renderer::ShaderProgram.SetUniformFloat("u_AmbientStrength", strength);

    memset(_pointLights.data(), 0, sizeof(_pointLights));

    memset(_spotLights.data(), 0, sizeof(_spotLights));

    memset(_dirLights.data(), 0, sizeof(_dirLights));

    _dirLightIndex = 0;
    _spotLightIndex = 0;
    _pointLightIndex = 0;
}

void Trinacria::DSL::LightSystem::Init()
{
    _pointTexture.GenerateTexture();

    _pointTexture.BoundTexImage(GL_RGBA32F, GL_RGBA, MAX_POINT_LIGHTS * 2, 1,
        GL_FLOAT, _pointLights.data(), GL_NEAREST);

    _spotTexture.GenerateTexture();

    _spotTexture.BoundTexImage(GL_RGBA32F, GL_RGBA, MAX_SPOT_LIGHTS * 3, 1,
        GL_FLOAT, _spotLights.data(), GL_NEAREST);

    _directionalTexture.GenerateTexture();

    _directionalTexture.BoundTexImage(GL_RGBA32F, GL_RGBA, MAX_DIRECTIONAL_LIGHTS * 2, 1,
                                GL_FLOAT, _dirLights.data(), GL_NEAREST);
}

void Trinacria::DSL::LightSystem::SetViewPos(glm::vec2 viewPos)
{
    Renderer::ShaderProgram.SetUniformVec2("u_ViewPos", viewPos);
}

void Trinacria::DSL::LightSystem::SetupLight(const PointLightData& lightData)
{
    if (_pointLightIndex + 8 > _pointLights.size()) return;

    _pointLights[_pointLightIndex] = lightData.LightColor.r;
    _pointLights[_pointLightIndex + 1] = lightData.LightColor.g;
    _pointLights[_pointLightIndex + 2] = lightData.LightColor.b;
    _pointLights[_pointLightIndex + 3] = lightData.LightPosition.x;
    _pointLights[_pointLightIndex + 4] = lightData.LightPosition.y;
    _pointLights[_pointLightIndex + 5] = lightData.LightHeight;
    _pointLights[_pointLightIndex + 6] = lightData.Attenuation;
    _pointLights[_pointLightIndex + 7] = 0.f;
    _pointLightIndex += 8;
}

void Trinacria::DSL::LightSystem::SetupLight(const SpotLightData& lightData)
{
    if (_spotLightIndex + 12 > _spotLights.size()) return;

    _spotLights[_spotLightIndex] = lightData.LightColor.r;
    _spotLights[_spotLightIndex + 1] = lightData.LightColor.g;
    _spotLights[_spotLightIndex + 2] = lightData.LightColor.b;
    _spotLights[_spotLightIndex + 3] = lightData.LightPosition.x;
    _spotLights[_spotLightIndex + 4] = lightData.LightPosition.y;
    _spotLights[_spotLightIndex + 5] = lightData.LightDirection.x;
    _spotLights[_spotLightIndex + 6] = lightData.LightDirection.y;
    _spotLights[_spotLightIndex + 7] = lightData.Attenuation;
    _spotLights[_spotLightIndex + 8] = glm::cos(glm::radians(lightData.InnerAngleInDegrees));
    _spotLights[_spotLightIndex + 9] = glm::cos(glm::radians(lightData.OuterAngleInDegrees));
    _spotLights[_spotLightIndex + 10] = lightData.LightHeight;
    _spotLights[_spotLightIndex + 11] = 0.f;

    _spotLightIndex += 12;
}

void Trinacria::DSL::LightSystem::SetupLight(const DirectionalLightData& lightData)
{
    if (_dirLightIndex + 8 > _dirLights.size()) return;

    _dirLights[_dirLightIndex] = lightData.LightColor.r;
    _dirLights[_dirLightIndex + 1] = lightData.LightColor.g;
    _dirLights[_dirLightIndex + 2] = lightData.LightColor.b;
    _dirLights[_dirLightIndex + 3] = lightData.LightDirection.x;
    _dirLights[_dirLightIndex + 4] = lightData.LightDirection.y;
    _dirLights[_dirLightIndex + 5] = 0.f;
    _dirLights[_dirLightIndex + 6] = 0.f;
    _dirLights[_dirLightIndex + 7] = 0.f;

    _dirLightIndex += 8;
}

void Trinacria::DSL::LightSystem::Done()
{
    _pointTexture.BoundSubImage(GL_RGBA, MAX_POINT_LIGHTS * 2, 1, GL_FLOAT, _pointLights.data());

    Renderer::ShaderProgram.SetUniformInt("u_PointLights",
        _pointTexture.GetTextureChosenSlot() - GL_TEXTURE0);

    memset(_pointLights.data(), 0, sizeof(_pointLights));

    _spotTexture.BoundSubImage(GL_RGBA, MAX_SPOT_LIGHTS * 3, 1, GL_FLOAT, _spotLights.data());

    Renderer::ShaderProgram.SetUniformInt("u_SpotLights",
        _spotTexture.GetTextureChosenSlot() - GL_TEXTURE0);

    memset(_spotLights.data(), 0, sizeof(_spotLights));

    _directionalTexture.BoundSubImage(GL_RGBA, MAX_DIRECTIONAL_LIGHTS * 2, 1, GL_FLOAT, _pointLights.data());

    Renderer::ShaderProgram.SetUniformInt("u_DirLights",
        _directionalTexture.GetTextureChosenSlot() - GL_TEXTURE0);

    memset(_dirLights.data(), 0, sizeof(_dirLights));

}

void Trinacria::DSL::LightSystem::Cleanup()
{
    {
        uint32_t id = _pointTexture.GetId();
        glDeleteTextures(1, &id);
    }

    {
        uint32_t id = _spotTexture.GetId();
        glDeleteTextures(1, &id);
    }

    {
        uint32_t id = _directionalTexture.GetId();
        glDeleteTextures(1, &id);
    }
}
