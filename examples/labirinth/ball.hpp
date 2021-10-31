#ifndef OBSTACLES_HPP_
#define OBSTACLES_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "player.hpp"

class OpenGLWindow;

class Balls {
    public:
        void initializeGL(GLuint program, int quantity);
        void paintGL();
        void terminateGL();
        
        void update(const Player &player, float deltaTime);

    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLint m_colorLoc{};
        GLint m_rotationLoc{};
        GLint m_translationLoc{};
        GLint m_scaleLoc{};

        struct Ball{
            GLuint m_vao{};
            GLuint m_vbo{};

            glm::vec4 m_color{1};
            float m_angularVelocity{};
            float m_polygonSides {900.0f};
            float m_scale{};
            float m_rotation{};
            glm::vec2 m_translation{glm::vec2(0)};
            glm::vec2 m_velocity{glm::vec2(0)};
            bool m_hit{false};
        };

        std::list<Ball> m_balls;

        std::default_random_engine m_randomEngine;
        std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

        Balls::Ball createBall(glm::vec2 translation, float scale = 0.025f);
};

#endif