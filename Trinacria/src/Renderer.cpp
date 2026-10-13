#include "Trinacria/Renderer.h"
#include <glad/glad.h>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <format>

#include "Trinacria/Animation.h"
#include "Trinacria/Sprite.h"
#include "Trinacria/Log.h"

#include "Trinacria/ShapesData.h"

#include <GLFW/glfw3.h>

#include "Trinacria/HUD.h"
#include "Trinacria/LightSystem.h"

std::vector<TRCN_CORE_NAMESPACE::Vertex> TRCN_CORE_NAMESPACE::Renderer::_quadBuffer;
std::vector<uint32_t> TRCN_CORE_NAMESPACE::Renderer::_quadIndexBuffer;

uint32_t TRCN_CORE_NAMESPACE::Renderer::_vao;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_vbo;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_ebo;

TRCN_CORE_NAMESPACE::Shader TRCN_CORE_NAMESPACE::Renderer::ShaderProgram;

std::vector<TRCN_CORE_NAMESPACE::Vertex> TRCN_CORE_NAMESPACE::Renderer::_triangleBuffer;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_triangleVbo;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_triangleVao;

std::vector<std::pair<TRCN_CORE_NAMESPACE::Texture*, uint32_t>> TRCN_CORE_NAMESPACE::Renderer::_textures;

std::array<TRCN_CORE_NAMESPACE::Material, 32> TRCN_CORE_NAMESPACE::Renderer::_materials;
int TRCN_CORE_NAMESPACE::Renderer::_materialCount = 0;

TRCN_CORE_NAMESPACE::FrameBuffer TRCN_CORE_NAMESPACE::Renderer::_frameBuffer;
TRCN_CORE_NAMESPACE::Texture TRCN_CORE_NAMESPACE::Renderer::_colorAttachment;


void TRCN_CORE_NAMESPACE::Renderer::Init(const glm::vec2& windowDimensions, GLFWwindow* window)
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, MaxQuadVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxQuadIndices * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    attribPointer(0, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex),
        (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Position));
 
    glEnableVertexAttribArray(1);

    glVertexAttribIPointer(1, sizeof(int), GL_INT, sizeof(TRCN_CORE_NAMESPACE::Vertex),
        (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TextureIndex));

    attribPointer(2, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex),
        (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TexCoords));

    attribPointer(3, 4, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex),
        (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Color));

    glEnableVertexAttribArray(4);

    glVertexAttribIPointer(4, sizeof(int), GL_INT, sizeof(TRCN_CORE_NAMESPACE::Vertex),
        (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, MaterialIndex));


    // triangle setup

    glGenVertexArrays(1, &_triangleVao);
    glBindVertexArray(_triangleVao);

    glGenBuffers(1, &_triangleVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _triangleVbo);

    glBufferData(GL_ARRAY_BUFFER, MaxTrianglesVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    attribPointer(0, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, sizeof(int), GL_INT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TextureIndex));

    attribPointer(2, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TexCoords));
    attribPointer(3, 4, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Color));

    _quadBuffer.reserve(MaxQuadVertices);
    _quadIndexBuffer.reserve(MaxQuadIndices);

    _triangleBuffer.reserve(MaxTrianglesVertices);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initScreen();

    _colorAttachment.GenerateTexture();
    _colorAttachment.BoundTexImage(GL_RGBA, GL_RGBA, windowDimensions.x, windowDimensions.y,
        GL_UNSIGNED_BYTE, nullptr, GL_LINEAR);

    _frameBuffer.GenFrameBuffer();
    _frameBuffer.BindAttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttachment.GetId());

    Shader::loadMaxTextures();
}

void TRCN_CORE_NAMESPACE::Renderer::createQuad(const glm::vec2& position, Texture* texture, const glm::vec2& size,
                                               const glm::vec4& color, int materialIndex, const glm::mat4& transform,
                                               const QuadTexCoords& texCoords)
{
    if (_quadBuffer.size() >= MaxQuadVertices)
    {
        TRCN_LOG("Vertex buffer overflowed please start a new batch!");
        return;
    }

    if (_quadIndexBuffer.size() >= MaxQuadIndices)
    {
        TRCN_LOG("Index buffer overflowed please start a new batch!");
        return;
    }

    uint32_t textureIndex;

    if (!findTextureIndex(textureIndex, texture) && texture != nullptr)
    {
        if (!_textures.empty())
        {
            // the index for the new texture is equal to the last index + 1

            textureIndex = _textures[_textures.size() - 1].second + 1;
        }
        else
        {
            textureIndex = 1;
        }

        _textures.emplace_back(texture, textureIndex);

    }
    else if (texture == nullptr)
    {
        textureIndex = 0;
    }

    glm::vec2 p0 = glm::vec2(transform * glm::vec4(position, 1.f, 1.f));
    glm::vec2 p1 = glm::vec2(transform * glm::vec4(position + glm::vec2(size.x, 0.f), 1.f, 1.f));
    glm::vec2 p2 = glm::vec2(transform * glm::vec4(position + size, 1.f, 1.f));
    glm::vec2 p3 = glm::vec2(transform * glm::vec4(position + glm::vec2(0.f, size.y), 1.f, 1.f));

    _quadBuffer.emplace_back(p0, textureIndex, texCoords.Coord0, color, materialIndex);
    _quadBuffer.emplace_back(p1, textureIndex, texCoords.Coord1, color, materialIndex);
    _quadBuffer.emplace_back(p2, textureIndex, texCoords.Coord2, color, materialIndex);
    _quadBuffer.emplace_back(p3, textureIndex, texCoords.Coord3, color, materialIndex);

    const size_t offset = _quadBuffer.size() - 4;

    _quadIndexBuffer.push_back(offset);
    _quadIndexBuffer.push_back(offset + 1);
    _quadIndexBuffer.push_back(offset + 2);

    _quadIndexBuffer.push_back(offset + 2);
    _quadIndexBuffer.push_back(offset + 3);
    _quadIndexBuffer.push_back(offset);
}

void TRCN_CORE_NAMESPACE::Renderer::createQuad(const glm::vec2& position, Sprite* sprite, const glm::vec2& size,
                                               const glm::vec4& color, int materialIndex, const glm::mat4& transform)
{
    QuadTexCoords texCoords = sprite->GetTexCoords();
    texCoords.Normalize(sprite->GetParent()->GetWidth(), sprite->GetParent()->GetHeight());

    createQuad(position, sprite->GetParent(), size, color, materialIndex, transform,texCoords);
}

void TRCN_CORE_NAMESPACE::Renderer::CreateTriangle(const TriangleData& triangleData)
{
    if (triangleData.texture != nullptr)
    {
        createTriangle(-triangleData.transform.Pivot, triangleData.texture, triangleData.Orientation,
                       glm::vec2(1.f), triangleData.Color, triangleData.MaterialIndex,
                       triangleData.transform.GetMatrix(), triangleData.TexCoords);
    }
    else if (triangleData.sprite != nullptr)
    {
        createTriangle(-triangleData.transform.Pivot, triangleData.sprite, triangleData.Orientation,
                       glm::vec2(1.f), triangleData.Color, triangleData.MaterialIndex, triangleData.transform.GetMatrix());
    }
    // using plain color
    else
    {
        createTriangle(-triangleData.transform.Pivot, Texture::NO_TEXTURE, triangleData.Orientation,
                       glm::vec2(1.f), triangleData.Color, triangleData.MaterialIndex,
                       triangleData.transform.GetMatrix());
    }
}

void TRCN_CORE_NAMESPACE::Renderer::createTriangle(const glm::vec2& position, Texture* texture, TriangleOrientation orientation, const glm::vec2& size,
                                                   const glm::vec4& color, int materialIndex, const glm::mat4& transform, const TriangleTexCoords& texCoords)
{
    if (_triangleBuffer.size() > MaxTrianglesVertices)
    {
        TRCN_LOG("Triangle vertex buffer overflowed please start a new batch!");
        return;
    }

    uint32_t textureIndex;

    if (!findTextureIndex(textureIndex, texture) && texture != nullptr)
    {
        if (!_textures.empty())
        {
            textureIndex = _textures[_textures.size() - 1].second + 1;
        }
        else
        {
            textureIndex = 1;
        }

        _textures.emplace_back(texture, textureIndex);

    }
    else if (texture == nullptr)
    {
        textureIndex = 0;
    }

    glm::vec2 p0 = transform * glm::vec4(position, 1.f, 1.f);
    glm::vec2 p3 = transform * glm::vec4(position.x, position.y + size.y, 1.f, 1.f);


    _triangleBuffer.emplace_back(p0, textureIndex, texCoords.Coord0, color, materialIndex);

    if (orientation == TriangleOrientation::Orientation_LEFT)
    {
        glm::vec2 p1 = transform * glm::vec4(position.x - size.x, position.y, 1.f, 1.f);
        _triangleBuffer.emplace_back(p1, textureIndex,
            texCoords.Coord1, color, materialIndex);
    }
    else if (orientation == TriangleOrientation::Orientation_RIGHT)
    {
        glm::vec2 p2 = transform * glm::vec4(position.x + size.x, position.y, 1.f, 1.f);
        _triangleBuffer.emplace_back(p2, textureIndex,
            texCoords.Coord1, color, materialIndex);
    }

    _triangleBuffer.emplace_back(p3, textureIndex,
        texCoords.Coord2, color, materialIndex);
}

void TRCN_CORE_NAMESPACE::Renderer::createTriangle(const glm::vec2& position, Sprite* sprite,
                                              TriangleOrientation orientation, const glm::vec2& size, const glm::vec4& color, int materialIndex, const glm::mat4& transform)
{
    TriangleTexCoords texCoords = sprite->GetTriangleTexCoords();
    texCoords.Normalize(sprite->GetParent()->GetWidth(), sprite->GetParent()->GetHeight());

    createTriangle(position, sprite->GetParent(), orientation, size, color, 0, transform, texCoords);
}

void TRCN_CORE_NAMESPACE::Renderer::drawInScreen(Shader& screenShader)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);

    ClearColorBuffer(glm::vec3(1.f));

    screenShader.Bind();

    glBindVertexArray(_vaoScreen);

    _colorAttachment.Bind(GL_TEXTURE0);

    screenShader.SetUniformInt("u_ColorAttachment", 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    _frameBuffer.Bind();
}

void TRCN_CORE_NAMESPACE::Renderer::initScreen()
{
    constexpr float screenQuadVertices[] = {
        -1.f, -1.f,
        1.f, -1.f,
        -1.f, 1.f,
        1.f, -1.f,
        1.f, 1.f,
        -1.f, 1.f
    };

    glGenVertexArrays(1, &_vaoScreen);
    glBindVertexArray(_vaoScreen);

    glGenBuffers(1, &_vboScreen);
    glBindBuffer(GL_ARRAY_BUFFER, _vboScreen);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), screenQuadVertices, GL_STATIC_DRAW);

    attribPointer(0, 2, GL_FLOAT, 2 * sizeof(float), nullptr);
}

void TRCN_CORE_NAMESPACE::Renderer::EndScene()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _quadBuffer.size() * sizeof(Vertex), _quadBuffer.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _quadIndexBuffer.size() * sizeof(uint32_t), _quadIndexBuffer.data());

    glBindBuffer(GL_ARRAY_BUFFER, _triangleVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _triangleBuffer.size() * sizeof(Vertex), _triangleBuffer.data());
}

void TRCN_CORE_NAMESPACE::Renderer::Draw(Shader& screenShader, const glm::vec2& windowDimensions, const glm::vec2& cameraPos, float zoom)
{
    _frameBuffer.Bind();

    glUseProgram(ShaderProgram.GetShaderProgram());
 
    for (auto& element : _textures)
    {
        std::string uniformName = std::string("u_Textures[") + std::to_string(element.second) + std::string("]");

        ShaderProgram.SetUniformInt(uniformName.c_str(), element.second - 1);

        element.first->Bind(element.second + GL_TEXTURE0 - 1);
    }

    glm::mat4 viewProjection = glm::scale(glm::mat4(1.f), glm::vec3(zoom, zoom, 1.f));

    viewProjection = glm::scale(viewProjection, glm::vec3(windowDimensions.y / windowDimensions.x, 1.f, 1.f));
    viewProjection = glm::translate(viewProjection, glm::vec3(-cameraPos.x, -cameraPos.y, 0.f));

    ShaderProgram.Bind();
    ShaderProgram.SetUniformMat4("u_View", viewProjection);

    glBindVertexArray(_vao);

    glDrawElements(GL_TRIANGLES, _quadIndexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(_triangleVao);
    glDrawArrays(GL_TRIANGLES, 0, _triangleBuffer.size());

    Texture::ClearTextureSlots();

    HUD::draw(windowDimensions, cameraPos, zoom);

    drawInScreen(screenShader);
}

void TRCN_CORE_NAMESPACE::Renderer::FlushBuffers()
{
    _quadBuffer.clear();
    _quadIndexBuffer.clear();

    _triangleBuffer.clear();
}

void TRCN_CORE_NAMESPACE::Renderer::AddMaterial(const Material& material)
{
    if (_materialCount >= 32) return;

    _materials[_materialCount] = material;
    material.SetUniforms(_materialCount++);
}

int TRCN_CORE_NAMESPACE::Renderer::SearchMaterial(const Material& material)
{
    for (int i = 0; i < _materialCount; i++)
    {
        if (_materials[i] == material)
        {
            return i;
        }
    }

    return 0;
}

void TRCN_CORE_NAMESPACE::Renderer::OnResize(const glm::vec2& windowDimensions)
{
    _colorAttachment.BoundTexImage(GL_RGB, GL_RGB, windowDimensions.x, windowDimensions.y,
    GL_UNSIGNED_BYTE, nullptr, GL_LINEAR);

    glViewport(0, 0, windowDimensions.x, windowDimensions.y);

    HUD::onResize(windowDimensions);
}

void TRCN_CORE_NAMESPACE::Renderer::Cleanup()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);

    glDeleteBuffers(1, &_vboScreen);
    glDeleteVertexArrays(1, &_vaoScreen);

    glDeleteVertexArrays(1, &_triangleVao);

    glDeleteProgram(ShaderProgram.GetShaderProgram());

    glDeleteBuffers(1, &_triangleVbo);

    _frameBuffer.Cleanup();
    LightSystem::Cleanup();
}

void TRCN_CORE_NAMESPACE::Renderer::attribPointer(uint32_t location, uint32_t nParameters, uint32_t parameterType,
    size_t stride, void* offsetFromOrigin)
{
    glVertexAttribPointer(location, nParameters, parameterType, GL_FALSE, stride, offsetFromOrigin);
    glEnableVertexAttribArray(location);
}

bool TRCN_CORE_NAMESPACE::Renderer::findTextureIndex(uint32_t& out, const Texture* textureToFind)
{
    for (auto& tex : _textures)
    {
        if (tex.first == textureToFind)
        {
            out = tex.second;
            return true;
        }
    }
    
    out = 0;

    return false;
}

void TRCN_CORE_NAMESPACE::Renderer::ClearColorBuffer(const glm::vec3& color)
{
    glClearColor(color.r, color.g, color.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void TRCN_CORE_NAMESPACE::Renderer::CreateQuad(const QuadData& quadData)
{
    if (quadData.texture != nullptr)
    {
        createQuad(-quadData.transform.Pivot, quadData.texture, glm::vec2(1.f),
                   quadData.Color, quadData.MaterialIndex, quadData.transform.GetMatrix(), quadData.TexCoords);
    }
    else if (quadData.sprite != nullptr)
    {
        createQuad(-quadData.transform.Pivot, quadData.sprite, glm::vec2(1.f),
                   quadData.Color, quadData.MaterialIndex, quadData.transform.GetMatrix());
    }
    // using plain color
    else
    {
        createQuad(-quadData.transform.Pivot, Texture::NO_TEXTURE, glm::vec2(1.f),
                   quadData.Color, quadData.MaterialIndex, quadData.transform.GetMatrix());
    }
}

void TRCN_CORE_NAMESPACE::QuadTexCoords::Normalize(uint32_t texWidth, uint32_t texHeight)
{
    Coord0.x = Coord0.x / texWidth;
    Coord0.y = Coord0.y / texHeight;

    Coord1.x = Coord1.x / texWidth;
    Coord1.y = Coord1.y / texHeight;

    Coord2.x = Coord2.x / texWidth;
    Coord2.y = Coord2.y / texHeight;

    Coord3.x = Coord3.x / texWidth;
    Coord3.y = Coord3.y / texHeight;
}

void TRCN_CORE_NAMESPACE::TriangleTexCoords::Normalize(uint32_t texWidth, uint32_t texHeight)
{
    Coord0.x = Coord0.x / texWidth;
    Coord0.y = Coord0.y / texHeight;

    Coord1.x = Coord1.x / texWidth;
    Coord1.y = Coord1.y / texHeight;

    Coord2.x = Coord2.x / texWidth;
    Coord2.y = Coord2.y / texHeight;
}
