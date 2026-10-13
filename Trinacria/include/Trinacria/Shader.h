#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Trinacria/Macros.h"

namespace TRCN_CORE_NAMESPACE
{
    /**
     * @brief class dealing with shader pipeline, uniforms, linking, etc..
     */
    class Shader
    {
    public:
        Shader() = default;
        ~Shader() = default;

        /**
         * @brief load a shader from file path
         * @param vert vertex shader's path
         * @param frag fragment shader's path
         */

        void LoadShader(const std::string& vert, const std::string& frag);

        /**
         * @brief use it for loading the shader that handles textures
         * @param vert the vertex shader's path
         * @param frag the fragment shader's path
         */

        void LoadCoreShader(const std::string& vert, const std::string& frag);

        /**
         * @brief bind a shader
         * @note has to be done before setting uniforms
         */

        void Bind();

        /**
         * @brief get a shader program to use to your liking
         * @return returns the shader program's id
         */

        uint32_t GetShaderProgram() { return _shaderProgram; }

        /**
         * @brief sets a matrix uniform
         * @param name the name of uniform
         * @param mat the matrix you want to set
         */

        void SetUniformMat4(const char* name, const glm::mat4& mat);

        /**
         * @brief sets an integer/sampler2D uniform
         * @param name the name of uniform
         * @param value the integer/sampler2D you want to set
         */

        void SetUniformInt(const char* name, int value);

        /**
         * @brief sets a float uniform
         * @param name the name of uniform
         * @param value the float you want to set
         */

        void SetUniformFloat(const char* name, float value);

        /**
         * @brief sets a vec2 uniform
         * @param name the name of uniform
         * @param value the vec2 you want to set
         */

        void SetUniformVec2(const char* name, const glm::vec2& value);

        /**
         * @brief sets a vec3 uniform
         * @param name the name of uniform
         * @param value the vec3 you want to set
         */

        void SetUniformVec3(const char* name, const glm::vec3& value);

        /**
         * @brief sets a vec4 uniform
         * @param name the name of uniform
         * @param value the vec4 you want to set
         */

        void SetUniformVec4(const char* name, const glm::vec4& value);

    private:
        uint32_t _shaderProgram;

        static std::string getStringFromFile(const std::string& path);
        static uint32_t compileAndCreateShader(const char* source, uint32_t shaderType);
        static uint32_t linkAndCreateProgram(uint32_t fragID, uint32_t vertID);
        static uint32_t getSamplersInShader(const std::string& shader);

        inline static uint32_t textureUsedInNonArrayUniforms;

        inline static int maxTextures;

        static void loadMaxTextures();

        friend class Renderer;
    };
}