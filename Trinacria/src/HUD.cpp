#include "Trinacria/HUD.h"
#include <glad/glad.h>

#include "GLFW/glfw3.h"
#include "Trinacria/Assert.h"
#include "Trinacria/Macros.h"
#include "Trinacria/Renderer.h"

void TRCN_CORE_NAMESPACE::HUD::Init(const HUDShaderSet& shaderSet, const glm::vec2& windowDimensions)
{
    setupQuads();
    setupText();

    _shader.LoadCoreShader(shaderSet.HUDQuadVertPath, shaderSet.HUDQuadFragPath);
    _textShader.LoadCoreShader(shaderSet.TextVertPath, shaderSet.TextFragPath);

    onResize(windowDimensions);

    _vertices.reserve(MaxHUDVertices);
    _indices.reserve(MaxHUDIndices);

    _textVertices.reserve(MaxTextVertices);
    _textIndices.reserve(MaxTextIndices);
}

uint32_t Trinacria::DSL::HUD::setupAtlas(const std::string& path, const msdf_atlas::Charset &charset)
{
    uint32_t index = 0;

    if (_textAtlases.empty())
    {
        _textAtlases.emplace_back(path.c_str(), charset);
    }
    else if (!findAtlasIndex(path, index))
    {
        _textAtlases.emplace_back(path.c_str(), charset);
        index = _textAtlases.size() - 1;
    }

    return index;
}

bool Trinacria::DSL::HUD::findAtlasIndex(const std::string& path, uint32_t& outIndex)
{
    for (int i = 0; i < _textAtlases.size(); i++)
    {
        if (_textAtlases[i].GetName() == path)
        {
            outIndex = i;
            return true;
        }
    }

    outIndex = 0;

    return false;
}

void TRCN_CORE_NAMESPACE::HUD::Cleanup()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

void TRCN_CORE_NAMESPACE::HUD::EndHUD()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(HUDVertex) * _vertices.size(), _vertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * _indices.size(),
        _indices.data());

    glBindBuffer(GL_ARRAY_BUFFER, _textVbo);

    glBufferSubData(GL_ARRAY_BUFFER, 0,
        sizeof(TextVertex) * _textVertices.size(), _textVertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _textEbo);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
        sizeof(uint32_t) * _textIndices.size(), _textIndices.data());
}

void TRCN_CORE_NAMESPACE::HUD::CreateHUDQuad(const HUDQuadData& HUDQuad)
{
    TRCN_DEPEND_START("Create HUD Quad");

    TRCN_DEPEND_RETURN_ASSERT_VOID(_vertices.size() < MaxHUDVertices);
    TRCN_DEPEND_RETURN_ASSERT_VOID(_indices.size() < MaxHUDIndices);

    uint32_t index = setupTexture(HUDQuad.texture);

    createHUDQuad(-HUDQuad.transform.Pivot, HUDQuad.Color, index, glm::vec2(1),
                  HUDQuad.transform.GetMatrix(), HUDQuad.TexCoords, 0, -1.f, glm::vec4(0.f));
}

void Trinacria::DSL::HUD::AddOnClickFunction(const OnClickType& onClick, const Transform& transformOfTheQuad)
{
    _onClickAndTransform.emplace_back(onClick, transformOfTheQuad);
}

void TRCN_CORE_NAMESPACE::HUD::CreateProgressBar(const HUDQuadData& HUDQuad, Texture* fillTexture, float progress, const glm::vec4& fillColor)
{
    TRCN_DEPEND_START("Create Progress Bar");

    TRCN_DEPEND_RETURN_ASSERT_VOID(_vertices.size() < MaxHUDVertices);
    TRCN_DEPEND_RETURN_ASSERT_VOID(_indices.size() < MaxHUDIndices);

    uint32_t index = setupTexture(HUDQuad.texture);
    uint32_t fillTexIndex = setupTexture(fillTexture);

    createHUDQuad(-HUDQuad.transform.Pivot, HUDQuad.Color, index, glm::vec2(1),
                  HUDQuad.transform.GetMatrix(), HUDQuad.TexCoords, fillTexIndex, progress, fillColor);
}

void TRCN_CORE_NAMESPACE::HUD::FlushBuffers()
{
    _vertices.clear();
    _indices.clear();
    _textVertices.clear();
    _textIndices.clear();
}

void TRCN_CORE_NAMESPACE::HUD::draw(const glm::vec2& windowDimensions, const glm::vec2& cameraPos, float zoom)
{
    _shader.Bind();

    for (auto& tex : _textures)
    {
        std::string format = std::format("u_Textures[{}]", tex.second);

        _shader.SetUniformInt(format.c_str(), tex.second - 1);

        tex.first->Bind(tex.second + GL_TEXTURE0 - 1);
    }

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);

    Texture::ClearTextureSlots();

    _textShader.Bind();

    for (int i = 0; i < _textAtlases.size(); i++)
    {
        std::string format = std::format("u_Textures[{}]", i);

        _textShader.SetUniformInt(format.c_str(), i);

        _textAtlases[i].GetTexture().Bind(i + GL_TEXTURE0);
    }

    glm::mat4 viewProjection = glm::scale(glm::mat4(1.f), glm::vec3(zoom, zoom, 1.f));

    viewProjection = glm::scale(viewProjection, glm::vec3(windowDimensions.y / windowDimensions.x, 1.f, 1.f));
    viewProjection = glm::translate(viewProjection, glm::vec3(-cameraPos.x, -cameraPos.y, 0.f));

    _textShader.SetUniformMat4("u_View", viewProjection);

    glBindVertexArray(_textVao);
    glDrawElements(GL_TRIANGLES, _textIndices.size(), GL_UNSIGNED_INT, nullptr);

    Texture::ClearTextureSlots();
}

void TRCN_CORE_NAMESPACE::HUD::createHUDQuad(const glm::vec2& position, const glm::vec4& color, uint32_t textureIndex,
                                             const glm::vec2& scale, glm::mat4 matrix, const QuadTexCoords& coord, uint32_t fillTextureIndex, float progress, const
                                             glm::vec4& fillColor)
{
    // to make this in NDC
    matrix[3][0] *= _aspectRatio;

    glm::vec2 p0 = glm::vec2(matrix * glm::vec4(position, 0.f, 1.f));
    glm::vec2 p1 = glm::vec2(matrix * glm::vec4(position + glm::vec2(scale.x, 0.f), 0.f, 1.f));
    glm::vec2 p2 = glm::vec2(matrix * glm::vec4(position + scale, 0.f, 1.f));
    glm::vec2 p3 = glm::vec2(matrix * glm::vec4(position + glm::vec2(0.f, scale.y), 0.f, 1.f));

    _vertices.emplace_back(p0, color, textureIndex, coord.Coord0, fillTextureIndex, progress, fillColor);
    _vertices.emplace_back(p1, color, textureIndex, coord.Coord1, fillTextureIndex, progress, fillColor);
    _vertices.emplace_back(p2, color, textureIndex, coord.Coord2, fillTextureIndex, progress, fillColor);
    _vertices.emplace_back(p3, color, textureIndex, coord.Coord3, fillTextureIndex, progress, fillColor);

    size_t offset = _vertices.size() - 4;

    _indices.push_back(offset);
    _indices.push_back(offset + 1);
    _indices.push_back(offset + 2);
    _indices.push_back(offset + 2);
    _indices.push_back(offset + 3);
    _indices.push_back(offset);
}

void Trinacria::DSL::HUD::createText(const glm::vec3& position, const glm::vec4& color,
                                     uint32_t textureIndex, const glm::vec2& scale, glm::mat4 matrix,
                                     const QuadTexCoords& coord)
{
    matrix[3][0] *= _aspectRatio;

    glm::vec3 p0 = matrix * glm::vec4(position, 1.f);
    glm::vec3 p1 = matrix * glm::vec4(position + glm::vec3(scale.x, 0.f, 0.f), 1.f);
    glm::vec3 p2 = matrix * glm::vec4(position + glm::vec3(scale.x, scale.y, 0.f), 1.f);
    glm::vec3 p3 = matrix * glm::vec4(position + glm::vec3(0.f, scale.y, 0.f), 1.f);

    _textVertices.emplace_back(p0, color, coord.Coord0, textureIndex);
    _textVertices.emplace_back(p1, color, coord.Coord1, textureIndex);
    _textVertices.emplace_back(p2, color, coord.Coord2, textureIndex);
    _textVertices.emplace_back(p3, color, coord.Coord3, textureIndex);

    size_t offset = _textVertices.size() - 4;

    _textIndices.push_back(offset);
    _textIndices.push_back(offset + 1);
    _textIndices.push_back(offset + 2);
    _textIndices.push_back(offset + 2);
    _textIndices.push_back(offset + 3);
    _textIndices.push_back(offset);
}

bool TRCN_CORE_NAMESPACE::HUD::findTextureIndex(uint32_t& out, const Texture* textureToFind)
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

uint32_t TRCN_CORE_NAMESPACE::HUD::setupTexture(Texture* texture)
{
    uint32_t index = 0;

    if (_textures.empty())
    {
        index = 1;
        _textures.emplace_back(texture, index);
    }
    else if (!findTextureIndex(index, texture) && texture)
    {
        index = _textures[_textures.size() - 1].second + 1;
        _textures.emplace_back(texture, index);
    }

    return index;
}

void TRCN_CORE_NAMESPACE::HUD::UpdateEvents(GLFWwindow* window, const glm::vec2& windowDimensions)
{
    // cannot subscribe to input poller layer because it is a layer and it is logically incorrect

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _lastStateOfLeftMouseButton == GLFW_RELEASE)
    {
        // TODO: check if is in button range

        for (auto& el : _onClickAndTransform)
        {
            if (isInRange(el.second, window, windowDimensions)) el.first();
        }

        _lastStateOfLeftMouseButton = GLFW_PRESS;
    }
    else
    {
        _lastStateOfLeftMouseButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    }
}

bool Trinacria::DSL::HUD::isInRange(const Transform& transform, GLFWwindow* window, const glm::vec2& windowDimensions)
{
    double _xPos, _yPos;
    glfwGetCursorPos(window, &_xPos, &_yPos);

    auto xPos = ((float)_xPos / windowDimensions.x - 0.5f) * 2;
    auto yPos = -((float)_yPos / windowDimensions.y - 0.5f) * 2;

    // point to find
    glm::vec2 P(xPos, yPos);

    glm::mat4 matrix = transform.GetMatrix();

    // to make this in NDC
    matrix[3][0] *= _aspectRatio;

    glm::mat4 proj = glm::scale(glm::mat4(1.f), glm::vec3(windowDimensions.y / windowDimensions.x, 1.f, 1.f));

    glm::vec2 A = glm::vec2(proj * matrix * glm::vec4(-transform.Pivot, 0.f, 1.f));
    glm::vec2 B = glm::vec2(proj * matrix * glm::vec4(-transform.Pivot + glm::vec2(1.f, 0.f), 0.f, 1.f));
    glm::vec2 D = glm::vec2(proj * matrix * glm::vec4(-transform.Pivot + glm::vec2(0.f, 1.f), 0.f, 1.f));

    glm::vec2 AB = B - A; // one length
    glm::vec2 AD = D - A; // another one

    glm::vec2 AP = P - A;

    float lengthSquaredAB = glm::dot(AB, AB);
    float lengthSquaredAD = glm::dot(AD, AD);

    float dotABAP = glm::dot(AB, AP);
    float dotADAP = glm::dot(AD, AP);

    bool b = (dotABAP >= 0 && dotABAP <= lengthSquaredAB);
    bool b1 = (dotADAP >= 0 && dotADAP <= lengthSquaredAD);

    return b && b1;
}

void Trinacria::DSL::HUD::setupQuads()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(HUDVertex) * MaxHUDVertices, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MaxHUDIndices, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(HUDVertex),
        (void*)offsetof(HUDVertex, Position));

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(HUDVertex),
       (void*)offsetof(HUDVertex, Color));

    glEnableVertexAttribArray(1);

    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(HUDVertex),
        (void*)offsetof(HUDVertex, TextureIndex));

    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(HUDVertex), (void*)offsetof(HUDVertex, TexCoord));
    glEnableVertexAttribArray(3);

    glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, sizeof(HUDVertex), (void*)(offsetof(HUDVertex, FillTextureIndex)));
    glEnableVertexAttribArray(4);

    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(HUDVertex), (void*)offsetof(HUDVertex, Progress));
    glEnableVertexAttribArray(5);

    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(HUDVertex), (void*)offsetof(HUDVertex, FillColor));
    glEnableVertexAttribArray(6);
}

void Trinacria::DSL::HUD::setupText()
{
    glGenVertexArrays(1, &_textVao);
    glBindVertexArray(_textVao);

    glGenBuffers(1, &_textVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _textVbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(TextVertex) * MaxTextVertices,
        nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextVertex), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, Color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, TexCoord));
    glEnableVertexAttribArray(2);

    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(TextVertex), (void*)offsetof(TextVertex, TextureIndex));
    glEnableVertexAttribArray(3);

    glGenBuffers(1, &_textEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _textEbo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MaxTextIndices, nullptr, GL_DYNAMIC_DRAW);
}

void Trinacria::DSL::HUD::onResize(const glm::vec2& windowDimensions)
{
    glm::mat4 proj = glm::scale(glm::mat4(1.f), glm::vec3(windowDimensions.y / windowDimensions.x, 1.f, 1.f));

    _shader.Bind();
    _shader.SetUniformMat4("u_Transform", proj);

    _aspectRatio = windowDimensions.x / windowDimensions.y;
}

void TRCN_CORE_NAMESPACE::HUD::CreateButton(const HUDQuadData& HUDQuad, const glm::vec4& hoveredColor, const glm::vec4& pressedColor, GLFWwindow* window, const glm::vec2& windowDimensions)
{
    TRCN_DEPEND_START("Create Button");
    TRCN_DEPEND_ASSERT(_vertices.size() <= MaxHUDVertices);
    TRCN_DEPEND_ASSERT(_indices.size() <= MaxHUDIndices);

    uint32_t index = setupTexture(HUDQuad.texture);

    glm::vec4 color = HUDQuad.Color;

    if (isInRange(HUDQuad.transform, window, windowDimensions))
    {
        color = hoveredColor;

        // doing in this strange way because it is faster

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            color = pressedColor;
        }
    }

    createHUDQuad(-HUDQuad.transform.Pivot, color, index, glm::vec2(1.f), HUDQuad.transform.GetMatrix(), HUDQuad.TexCoords, 0, 0, glm::vec4(0.f));
}

void Trinacria::DSL::HUD::CreateText(const std::string& text, const Transform& transform, const glm::vec4& color, bool inWorld, const std::string &fontPath, const
                                     msdf_atlas::Charset& charset)
{
    TRCN_DEPEND_START("Create Text");

    uint32_t atlasIndex = setupAtlas(fontPath, charset);

    for (char c : text)
    {
        TRCN_DEPEND_RETURN_ASSERT_VOID(_textVertices.size() < MaxTextVertices);
        TRCN_DEPEND_RETURN_ASSERT_VOID(_textIndices.size() < MaxTextIndices);

        Glyph glyph(&_textAtlases[atlasIndex]);
        glyph.SetGlyph(c);

        QuadTexCoords texCoords {
            { glyph.GetBound0(), glyph.GetBound1() },
            { glyph.GetBound2(), glyph.GetBound1() },
            { glyph.GetBound2(), glyph.GetBound3() },
            {glyph.GetBound0(), glyph.GetBound3() },
        };

        createText({-transform.Pivot, inWorld}, color, atlasIndex, glm::vec2(1.f), transform.GetMatrix(), texCoords);
    }
}
