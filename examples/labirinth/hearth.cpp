#include "hearth.hpp"
#include <imgui.h>
#include <fmt/core.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Hearths::initializeGL(GLuint program, int quantity){
    terminateGL();

    m_randomEngine.seed(
        std::chrono::steady_clock::now().time_since_epoch().count());
    
    m_program = program;
    m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
    m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
    m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

    m_hearths.clear();
    m_hearths.resize(quantity);

    for (auto &hearth: m_hearths){
        hearth = createhearth(glm::vec2(0.0f,0.0f),0.25f);
    }
}

void Hearths::paintGL(){
    abcg::glUseProgram(m_program);

    for (const auto &hearth : m_hearths) {
        abcg::glBindVertexArray(hearth.m_vao);

        abcg::glUniform4fv(m_colorLoc, 1, &hearth.m_color.r);
        abcg::glUniform1f(m_scaleLoc, hearth.m_scale);
        abcg::glUniform1f(m_rotationLoc, hearth.m_rotation);

        for (auto i : {-2, 0, 2}) {
            for (auto j : {-2, 0, 2}) {
                abcg::glUniform2f(m_translationLoc, hearth.m_translation.x + j,
                                    hearth.m_translation.y + i);

                abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, hearth.m_polygonSides);
            }
        }
    
        abcg::glBindVertexArray(0);
    }

    abcg::glUseProgram(0);
}

void Hearths::terminateGL(){
    for (auto hearth : m_hearths) {
        abcg::glDeleteBuffers(1, &hearth.m_vbo);
        abcg::glDeleteVertexArrays(1, &hearth.m_vao);
    }
}

void Hearths::update(const GameData &gameData, float deltaTime){
    for (auto &hearth : m_hearths){
        hearth.m_rotation = glm::wrapAngle(
            hearth.m_rotation + hearth.m_angularVelocity * deltaTime
        );
    }
}

Hearths::Hearth Hearths::createhearth(glm::vec2 translation, float scale){
    Hearth hearth;

    auto &re{m_randomEngine};
    std::uniform_int_distribution<int> randomSides(0,900);

    std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
    hearth.m_color = glm::vec4(0.5,0.7,0.12,0.3)*randomIntensity(re);
    hearth.m_color.a = 1.0f;
    hearth.m_rotation = 0.0f;
    hearth.m_scale = scale;
    hearth.m_translation = translation;

    hearth.m_angularVelocity = m_randomDist(re);

    glm::vec2 direction{m_randomDist(re), m_randomDist(re)};
    hearth.m_velocity = glm::normalize(direction) / 7.5f;

    std::vector<glm::vec2> positions(0);
    positions.emplace_back(0, 0);
    const auto step{M_PI * 2 / hearth.m_polygonSides};
    // hearth size randomly
    std::uniform_real_distribution<float> randomRadius(0.3f,0.4f);
    for (const auto angle : iter::range(0.0, M_PI * 2, step)){
        const auto radius{randomRadius(re)};
        positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
    }
    positions.push_back(positions.at(1));

    // Generate VBO
  abcg::glGenBuffers(1, &hearth.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, hearth.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &hearth.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(hearth.m_vao);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, hearth.m_vbo);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return hearth;
}