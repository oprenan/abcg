#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class Path;
class Balls;
class OpenGLWindow;

class Player{
    public:
        void initalizeGL(GLuint program);
        void paintGL(const GameData &gameData);
        void terminateGL();

        void update(const GameData &gameData, float deltaTime);
    
    private:
        friend Path;
        friend Balls;
        friend OpenGLWindow;

        GLuint m_program{};
        GLint m_colorLoc{};
        GLint m_scaleLoc{};
        GLint m_translationLoc{};
        GLint m_rotationLoc{};

        GLuint m_vao{};
        GLuint m_vbo{};
        GLuint m_ebo{};

        glm::vec4 m_color{0.70, 1.0, .45, 1.0};
        float m_rotation{};
        float m_scale{0.125f};
        glm::vec2 m_translation{glm::vec2(0)};
        glm::vec2 m_velocity{glm::vec2(0)};
        
};


#endif