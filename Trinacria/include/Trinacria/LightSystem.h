#pragma once

#include "Macros.h"
#include <glm/glm.hpp>

#include "Texture.h"

namespace TRCN_CORE_NAMESPACE
{
    /**
     * @brief the struct having all a point light needs
     * @note a point light is a type of light that spreads in a circle
     */
    struct PointLightData
    {
        /**
         * @brief light's color
         */
        glm::vec3 LightColor;

        /**
         * @brief light's position
         */
        glm::vec2 LightPosition;

        /**
         * @brief attenuation
         * @note less than 1 the light is brighter, more than 1 the light darker
         */
        float Attenuation;

        /**
         * @brief it's the hidden z position of the light
         * @note higher it is more the x and y will be affected (not intended but inevitable)
         */
        float LightHeight = 2.f;
    }; // 7 float ---> 2 texel

    /**
     * @brief the struct having all a spotlight needs
     * @note a spotlight is a light that faces a direction illuminating the objects in that direction
     */
    struct SpotLightData
    {
        /**
         * @brief the light's color
         */
        glm::vec3 LightColor;
        /**
         * @brief the light's position
         */
        glm::vec2 LightPosition;
        /**
         * @brief the light's direction
         */
        glm::vec2 LightDirection;
        /**
         * @brief attenuation
         * @note less than 1 the light is brighter, more than 1 the light darker
         */
        float Attenuation;
        /**
         * @brief the angle where the light is at its max illumination
         */
        float InnerAngleInDegrees;
        /**
         * @brief the angle from where the light starts to fade
         */
        float OuterAngleInDegrees;

        /**
         * @brief it's the hidden z position of the light
         * @note higher it is more the x and y will be affected (not intended but inevitable)
         */
        float LightHeight = 1.f;
    }; // 11 float --> 3 texel

    /**
     * @brief struct representing the directional light
     * @note the directional light is a light applied globally (like the sun) that illuminates in a certain direction
     */
    struct DirectionalLightData
    {
        /**
         * @brief the light's color
         */
        glm::vec3 LightColor;
        /**
         * @brief the light's direction
         */
        glm::vec2 LightDirection;
    }; // 5 float ---> 2 texel

    /**
     * @brief static class dealing with light
     */
    class LightSystem
    {
    public:
        /**
         * @brief Prepares the light system for the new frame
         * @note: needs to be called every frame
         * @param strength ambient strength, use the default values
         */

        static void InitFrame(float strength);

        /**
         * @brief inits the light system
         * @note needs to be called once before using it
         */

        static void Init();

        /**
         * @brief Sets the view pos
         * @param viewPos the position from where you're viewing, like a camera position
         */

        static void SetViewPos(glm::vec2 viewPos);

        /**
         * @brief Adds a light to the scene
         * @param lightData the data of the light you wanna add
         */

        static void SetupLight(const PointLightData& lightData);
        /**
         * @brief Adds a light to the scene
         * @param lightData the data of the light you wanna add
         */

        static void SetupLight(const SpotLightData& lightData);
        /**
         * @brief Adds a light to the scene
         * @param lightData the data of the light you wanna add
         */

        static void SetupLight(const DirectionalLightData& lightData);

        /**
         * @brief use it when you finished adding lights, needs to be done at the end of every frame
         */

        static void Done();

        /**
         * @brief cleans up the light system
         * @note automatically called by Renderer::Cleanup()
         */
        static void Cleanup();

        // ambient strength provided values
        static constexpr float WORLD_NOT_AFFECTED_BY_LIGHT = 1.f;
        static constexpr float DEFAULT_USE_OF_LIGHT = 0.1f;
        static constexpr float BRIGHT = 0.2f;
        static constexpr float SUPER_BRIGHT = 0.4f;
        static constexpr float DARK = 0.06f;
        static constexpr float SUPER_DARK = 0.01f;

        static constexpr uint32_t MAX_POINT_LIGHTS = 64;
        static constexpr uint32_t MAX_SPOT_LIGHTS = 32;
        static constexpr uint32_t MAX_DIRECTIONAL_LIGHTS = 8;

    private:
        static Texture _pointTexture;
        static Texture _spotTexture;
        static Texture _directionalTexture;

        static std::array<float, MAX_POINT_LIGHTS * 8> _pointLights;
        static std::array<float, MAX_SPOT_LIGHTS * 12> _spotLights;
        static std::array<float, MAX_DIRECTIONAL_LIGHTS * 8> _dirLights;

        static int _pointLightIndex;
        static int _spotLightIndex;
        static int _dirLightIndex;
    };
}
