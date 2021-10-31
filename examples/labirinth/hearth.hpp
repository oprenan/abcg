#ifndef PATH_HPP_
#define PATH_HPP_

#include <random>
#include <list>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Hearths {
    public:
        void initializeGL(GLuint program, int quantity);
        void paintGL();
        void terminateGL();

        void update(const GameData &gameData, float deltaTime);

    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLint m_colorLoc{};
        GLint m_rotationLoc{};
        GLint m_translationLoc{};
        GLint m_scaleLoc{};

        struct Hearth {
            GLuint m_vao{};
            GLuint m_vbo{};

            glm::vec4 m_color{1};
            float m_angularVelocity{};
            float m_polygonSides {9000.0f};
            float m_scale;
            float m_rotation{};
            glm::vec2 m_translation{glm::vec2(0)};
            glm::vec2 m_velocity{glm::vec2(0)};
            bool m_hit{false};
        };

        std::list<Hearth> m_hearths;
        std::default_random_engine m_randomEngine;
        std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

        Hearths::Hearth createhearth(glm::vec2 rotation = glm::vec2(0), float scale = 0.1f);

};

#endif