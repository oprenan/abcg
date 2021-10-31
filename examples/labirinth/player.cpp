#include "player.hpp"
#include <imgui.h>
#include <fmt/core.h>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Player::initalizeGL(GLuint program){
    terminateGL();

    m_program = program;
    m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
    m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
    m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
    
    m_rotation = 0.0f;
    m_velocity = glm::vec2(0);
    m_translation = glm::vec2(0.0f,0.25f);

    std::array<glm::vec2, 4> positions{
        // square player
        glm::vec2{-10.0f, +14.0f}, glm::vec2{+10.0f, +14.0f},
        glm::vec2{-10.0f, +11.0f}, glm::vec2{+10.0f, +11.0f}
    };

    for (auto &position: positions){
        position /=glm::vec2{12.0f, 8.5f};
    }

    const std::array indices {0, 1, 3,
                              0, 2, 3};

    // Generate VBO
    abcg::glGenBuffers(1, &m_vbo);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                        GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Generate EBO
    abcg::glGenBuffers(1, &m_ebo);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                        GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Get location of attributes in the program
    GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

    // Create VAO
    abcg::glGenVertexArrays(1, &m_vao);

    // Bind vertex attributes to current VAO
    abcg::glBindVertexArray(m_vao);

    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                                nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    // End of binding to current VAO
    abcg::glBindVertexArray(0);
}

void Player::paintGL(const GameData &gameData){
    if (gameData.m_state != State::Playing) return;

    abcg::glUseProgram(m_program);

    abcg::glBindVertexArray(m_vao);

    abcg::glUniform1f(m_scaleLoc, m_scale);
    abcg::glUniform1f(m_rotationLoc, m_rotation);
    abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

    // aqui cabe colocar alguma "animação" de pulo e alteração de velocidade
    
    abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
    abcg::glDrawElements(GL_TRIANGLES, 10, GL_UNSIGNED_INT, nullptr);
    abcg::glBindVertexArray(0);
    abcg::glUseProgram(0);
}

void Player::terminateGL(){
    abcg::glDeleteBuffers(1, &m_vbo);
    abcg::glDeleteBuffers(1, &m_ebo);
    abcg::glDeleteVertexArrays(1, &m_vao);
}

void Player::update(const GameData &gameData, float deltaTime){
    if (gameData.m_input[static_cast<size_t>(Input::Left)]){
        m_rotation = glm::wrapAngle(m_rotation + 5.0f * deltaTime);
        m_translation.x = cos(m_rotation)/5.0f;
        m_translation.y = sin(m_rotation)/5.0f;
    }
    if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
        m_rotation = glm::wrapAngle(m_rotation - 5.0f * deltaTime);
        m_translation.x = cos(m_rotation)/5.0f;
        m_translation.y = sin(m_rotation)/5.0f;
    }
        
    
    // Apply thrust
    if (gameData.m_input[static_cast<size_t>(Input::Up)] && gameData.m_state == State::Playing) {
    // Thrust in the forward vector
        glm::vec2 forward = glm::rotate(glm::vec2{0.0f, 1.0f}, m_rotation);
        m_velocity += forward * deltaTime;
        m_translation += m_velocity;
    }
}