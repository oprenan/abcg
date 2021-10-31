#include "ball.hpp"
#include <imgui.h>
#include <fmt/core.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Balls::initializeGL(GLuint program, int quantity){
    terminateGL();

    m_randomEngine.seed(
        std::chrono::steady_clock::now().time_since_epoch().count());
    
    m_program = program;
    m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
    m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
    m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

    m_balls.clear();
    m_balls.resize(quantity);

    for (auto &ball: m_balls){
        ball = createBall(glm::vec2(0.75f, 10.5f),0.015f);

        do {
            ball.m_translation = {
                10.0f,
                0.5f
            };
        } while (glm::length(ball.m_translation) < 1.5f);
    }
}

void Balls::paintGL(){
    abcg::glUseProgram(m_program);

    for (const auto &ball : m_balls) {
        abcg::glBindVertexArray(ball.m_vao);

        abcg::glUniform4fv(m_colorLoc, 1, &ball.m_color.r);
        abcg::glUniform1f(m_scaleLoc, ball.m_scale);
        abcg::glUniform1f(m_rotationLoc, ball.m_rotation);

        for (auto i : {-2, 0, 2}) {
            for (auto j : {-2, 0, 2}) {
                abcg::glUniform2f(m_translationLoc, ball.m_translation.x + j,
                                    ball.m_translation.y + i);

                abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, ball.m_polygonSides);
            }
        }
    
        abcg::glBindVertexArray(0);
    }

    abcg::glUseProgram(0);
}

void Balls::terminateGL(){
    for (auto ball : m_balls) {
        abcg::glDeleteBuffers(1, &ball.m_vbo);
        abcg::glDeleteVertexArrays(1, &ball.m_vao);
    }
}

void Balls::update(const Player &player, float deltaTime){
    for (auto &ball : m_balls){
        //ball.m_translation -= ball.m_velocity * deltaTime;
        ball.m_rotation = glm::wrapAngle(
            ball.m_rotation + ball.m_angularVelocity 
        );
        ball.m_translation += ball.m_velocity * deltaTime;
        
        if (ball.m_translation.x < -1.0f) ball.m_translation.x += 2.0f;
        if (ball.m_translation.x > +1.0f) ball.m_translation.x -= 2.0f;
        if (ball.m_translation.y < -1.0f) ball.m_translation.y += 2.0f;
        if (ball.m_translation.y > +1.0f) ball.m_translation.y -= 2.0f;
        
    }
}

Balls::Ball Balls::createBall(glm::vec2 translation, float scale){
    Ball ball;

    auto &re{m_randomEngine};

    ball.m_color = glm::vec4(1.0f,0.0f,0.0f,0.0f);
    ball.m_color.a = 1.0f;
    ball.m_rotation = 0.0f;
    ball.m_scale = scale;
    ball.m_translation = translation;

    ball.m_angularVelocity = m_randomDist(re);

    // random direction
    glm::vec2 direction{m_randomDist(re), m_randomDist(re)};
    ball.m_velocity = glm::normalize(direction);
    
    // geometry
    std::vector<glm::vec2> positions(0);
    positions.emplace_back(0, 0);
    const auto step{M_PI * 2 / ball.m_polygonSides};

    // ball size randomly
    std::uniform_real_distribution<float> randomRadius(0.02f,0.02f);
    for (const auto angle : iter::range(0.0, M_PI * 2, step)){
        const auto radius{randomRadius(re)};
        positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
    }
    positions.push_back(positions.at(1));

    // Generate VBO
  abcg::glGenBuffers(1, &ball.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, ball.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &ball.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(ball.m_vao);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, ball.m_vbo);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return ball;
}