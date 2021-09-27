#include <fmt/core.h>
#include "openglwindow.hpp"
#include <imgui.h>
#include <array>

void OpenGLWindow::initializeGL(){
    auto windowSettings{getWindowSettings()};
    fmt::print("Initial window size: {}x{}\n", windowSettings.width, windowSettings.height);
}

void OpenGLWindow::paintGL(){
    abcg::glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    abcg:glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::paintUI(){

    abcg::OpenGLWindow::paintUI();

    {
        ImGui::Begin("Hello, first app!");

        auto windowSettings{getWindowSettings()};
        ImGui::Text("Current window size: %dx%d (in windowed mode)", windowSettings.width, windowSettings.height);

        static float f{};

        ImGui::ColorEdit3("clear color", m_clearColor.data());

        //framerate
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        //button 1
        if(ImGui::Button("Press me!", ImVec2(100, 50))){
            fmt::print("Button pressed. \n");
        }

        //button 2
        ImGui::Button("Press me!", ImVec2(-1, 50));
        if(ImGui::IsItemClicked()){
            fmt::print("Button presssed. \n");
        }

        //checkbox
        static bool enabled{true};
        ImGui::Checkbox("Some option", &enabled);
        fmt::print("The checkbox is {}\n", enabled ? "enabled" : "disabled");

        //combo box
        /*
        static std::size_t currentIndex{};
        std::vector<std::string> comboItems{"AAA", "BBB", "CCC"};

        if (ImGui::BeginCombo("Combo box", comboItems.at(currentIndex).c_str())) {
            for (auto index{0u}; index < comboItems.size(); ++index) {
                const bool isSelected{currentIndex == index};
                if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
                currentIndex = index;

                // Set the initial focus when opening the combo (scrolling + keyboard
                // navigation focus)
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        fmt::print("Selected combo box item: {}\n", comboItems.at(currentIndex));
        */

        ImGui::SetNextWindowSize(ImVec2(300, 100));
        auto flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize};
        ImGui::Begin("Window with menu", nullptr, flags);
        {
        bool save{};
        static bool showCompliment{};  // Hold state

        // Menu Bar
        if (ImGui::BeginMenuBar()) {
            // File menu
            if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Save", nullptr, &save);
            ImGui::EndMenu();
            }
            // View menu
            if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Show Compliment", nullptr, &showCompliment);
            ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (save) {
            // Save file...
        }

        if (showCompliment) {
            ImGui::Text("You're a beautiful person.");
        }
        }
        ImGui::End();

        ImGui::End();
    }
}