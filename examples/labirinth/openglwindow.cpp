#include "openglwindow.hpp"

#include <imgui.h>
#include <fmt/core.h>
#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }
}

void OpenGLWindow::initializeGL(){
    // Render background - background.frag/vert in assets
    m_backgroundProgram = createProgramFromFile(getAssetsPath()+"background.vert",
                                                getAssetsPath()+"background.frag");
    
    // Render objects - objects.frag/vert in assets
    m_objectsProgram = createProgramFromFile(getAssetsPath()+"objects.vert",
                                             getAssetsPath()+"objects.frag");
    // Others/ Pseudo-random generator 
    abcg::glClearColor(0, 0, 0, 1);
    #if !defined(__EMSCRIPTEN__)
        abcg::glEnable(GL_PROGRAM_POINT_SIZE);
    #endif
    m_randomEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    restart(); 
}

void OpenGLWindow::restart() {
    m_gameData.m_state = State::Playing;

    m_hearths.initializeGL(m_objectsProgram, 1);
    if (m_winCondition.elapsed() > 20) m_balls.initializeGL(m_backgroundProgram, 2);
    else m_balls.initializeGL(m_backgroundProgram, 1);
    m_player.initalizeGL(m_objectsProgram);
}

void OpenGLWindow::update(){
    const float deltaTime{static_cast<float>(getDeltaTime())};

    // Wait 5 seconds before restarting
    if (m_gameData.m_state != State::Playing && m_restartWaitTimer.elapsed() > 5) {
        restart();
        return;
    }

    m_player.update(m_gameData, deltaTime);
    m_hearths.update(m_gameData, deltaTime);
    m_balls.update(m_player, deltaTime);
    
    if (m_gameData.m_state == State::Playing) {
        checkCollisions();
        checkWinCondition();
    }
}

void OpenGLWindow::paintGL(){
    update();

    abcg::glClear(GL_COLOR_BUFFER_BIT);
    abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

    m_player.paintGL(m_gameData);
    m_hearths.paintGL();
    m_balls.paintGL();

}

void OpenGLWindow::paintUI(){
    abcg::OpenGLWindow::paintUI();

    {
        const auto size{ImVec2(150,300)};
        const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
        ImGui::SetNextWindowPos(position);
        ImGui::SetNextWindowSize(size);
        ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
        ImGui::Begin(" ", nullptr, flags);
        if (m_gameData.m_state == State::GameOver) {
            ImGui::Text("Game Over!");
        } else if (m_gameData.m_state == State::Win) {
            ImGui::Text("*You Win!*");
        }

        ImGui::End();
    }
}

void OpenGLWindow::resizeGL(int width, int height){
    m_viewportWidth = width;
    m_viewportHeight = height;

    abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL(){
    abcg::glDeleteProgram(m_backgroundProgram);
    abcg::glDeleteProgram(m_objectsProgram);

    m_balls.terminateGL();
    m_hearths.terminateGL();
    m_player.terminateGL();
}

void OpenGLWindow::checkCollisions(){
  // colision with the hearth
  for (auto &ball : m_balls.m_balls) {
    for (auto &hearth : m_hearths.m_hearths) {
      const auto ballTranslation{ball.m_translation};
      const auto distance{glm::distance(hearth.m_translation, ballTranslation)};
      if (distance < hearth.m_scale * 0.5f + ball.m_scale * 0.5f) {
        m_gameData.m_state = State::GameOver;
        m_restartWaitTimer.restart();
        m_winCondition.restart();
      }
    }
  }

  for (auto &ball : m_balls.m_balls) {
    const auto ballTranslation{ball.m_translation};
    const auto distance{glm::distance(m_player.m_translation, ballTranslation)};
    if (distance < m_player.m_scale * 1.0f + ball.m_scale * 0.5f) {
        auto velocity = ball.m_velocity;
        velocity = -1.0f*velocity;
        ball.m_velocity = velocity;
        
    }
  }
}

void OpenGLWindow::checkWinCondition(){
  if (m_winCondition.elapsed() > 45) {
    m_gameData.m_state = State::Win;
    m_winCondition.restart();
  }
}

