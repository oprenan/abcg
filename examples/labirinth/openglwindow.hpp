#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "player.hpp"
#include "ball.hpp"
#include "hearth.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
    protected:
        void handleEvent(SDL_Event& event) override;
        void initializeGL() override;
        void paintGL() override;
        void paintUI() override;
        void resizeGL(int width, int height) override;
        void terminateGL() override;
    private:
        GLuint m_backgroundProgram{};
        GLuint m_objectsProgram {};
        
        int m_viewportWidth{};
        int m_viewportHeight{};

        GameData m_gameData;
        
        Player m_player;
        Hearths m_hearths;
        Balls m_balls;

        abcg::ElapsedTimer m_restartWaitTimer;
        abcg::ElapsedTimer m_winCondition;

        std::default_random_engine m_randomEngine;

        void checkCollisions();
        void checkWinCondition();

        void restart();
        void update();
};

#endif