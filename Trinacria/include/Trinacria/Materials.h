#pragma once

#include "Macros.h"
#include <glm/glm.hpp>

namespace TRCN_CORE_NAMESPACE
{
    /**
     * @brief class representing materials
     */

    class Material
    {
    public:
        /**
         * @brief the color the object has <<by default>> when there's no light
         */

        glm::vec3 Ambient;

        /**
         * @brief the color the object has when light is hitting it
         */

        glm::vec3 Diffuse;

        /**
         * @brief the color the object reflects
         */

        glm::vec3 Specular;

        /**
         * @brief the higher this number the sharper the reflection
         */

        float Shininess;

        /**
         * @brief it implements the material in the shader's array
         * @param materialIndex the index where to put the material in the array
         */

        void SetUniforms(int materialIndex) const;

        bool operator==(const Material& material) const {return this->Ambient == material.Ambient &&
            this->Diffuse == material.Diffuse && this->Specular == material.Specular;}
    };
    
    inline constexpr Material SOLID_OPAQUE {glm::vec3(1.f), glm::vec3(1.f),
        glm::vec3(1.f), 32.f};

    inline constexpr Material METAL {glm::vec3(0.01f), glm::vec3(0.7f),
        glm::vec3(1.f), 256.f};

}
