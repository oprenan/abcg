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
    abcg::glClear(GL_COLOR_BUFFER_BIT);
}

int checkWinner(std::array<std::array<int,3>,3> board){
    int response = 0;
    int it_x, it_y;

    for(it_x = 0; it_x < 3; it_x++){
        for(it_y = 0; it_y < 3; it_y++){
            if (it_x == 0 && it_y == 0){
                if(board[it_x][it_y] == board[it_x + 1][it_y + 1] && board[it_x][it_y] == board[it_x + 2][it_y + 2]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x + 1][it_y] && board[it_x][it_y] == board[it_x + 2][it_y]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x][it_y + 1] && board[it_x][it_y] == board[it_x][it_y + 2]) return board[it_x][it_y];
            }
            else if (it_x == 0 && it_y == 1){
                if(board[it_x][it_y] == board[it_x -1][it_y] && board[it_x][it_y] == board[it_x + 1][it_y]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x][it_y-1] && board[it_x][it_y] == board[it_x][it_y + 1]) return board[it_x][it_y];
            }
            else if (it_x == 0 && it_y == 2){
                if(board[it_x][it_y] == board[it_x - 1][it_y - 1] && board[it_x][it_y] == board[it_x - 2][it_y - 2]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x - 1][it_y] && board[it_x][it_y] == board[it_x - 2][it_y]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x][it_y - 1] && board[it_x][it_y] == board[it_x][it_y - 2]) return board[it_x][it_y];
            }
            else if (it_x == 1 && it_y == 0){
                if(board[it_x][it_y] == board[it_x - 1][it_y] && board[it_x][it_y] == board[it_x + 1][it_y]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x][it_y + 1] && board[it_x][it_y] == board[it_x][it_y + 2]) return board[it_x][it_y];
            }
            else if (it_x == 1 && it_y == 1){
                if(board[it_x][it_y] == board[it_x - 1][it_y] && board[it_x][it_y] == board[it_x + 1][it_y]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x][it_y + 1] && board[it_x][it_y] == board[it_x][it_y + 2]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x - 1][it_y - 1] && board[it_x][it_y] == board[it_x +1][it_y + 1]) return board[it_x][it_y];
            }
            else if (it_x == 1 && it_y == 2){
                if(board[it_x][it_y] == board[it_x - 1][it_y] && board[it_x][it_y] == board[it_x + 1][it_y]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x][it_y + 1] && board[it_x][it_y] == board[it_x][it_y - 1]) return board[it_x][it_y];
            }
            else if (it_x == 2 && it_y == 0){
                if(board[it_x][it_y] == board[it_x - 1][it_y + 1] && board[it_x][it_y] == board[it_x - 2][it_y + 2]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x - 1][it_y] && board[it_x][it_y] == board[it_x - 2][it_y]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x][it_y + 1] && board[it_x][it_y] == board[it_x][it_y + 2]) return board[it_x][it_y];
            }
            else if (it_x == 2 && it_y == 1){
                if(board[it_x][it_y] == board[it_x - 1][it_y] && board[it_x][it_y] == board[it_x - 2][it_y]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x][it_y + 1] && board[it_x][it_y] == board[it_x][it_y -1]) return board[it_x][it_y];
            }
            else if (it_x == 2 && it_y == 2){
                if(board[it_x][it_y] == board[it_x - 1][it_y] && board[it_x][it_y] == board[it_x - 2][it_y]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x][it_y + 1] && board[it_x][it_y] == board[it_x][it_y -1]) return board[it_x][it_y];
                if(board[it_x][it_y] == board[it_x - 1][it_y - 1] && board[it_x][it_y] == board[it_x - 2][it_y - 2]) return board[it_x][it_y];
            }
        }
    } 

    return 0;
}

void OpenGLWindow::paintUI(){

    {
        /*
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
         */

        auto windowSettings{getWindowSettings()};
        ImGui::SetNextWindowSize(ImVec2(440, 690));
        ImGui::SetNextWindowPos(ImVec2(5, 5));
        auto flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize};

        ImGui::Begin("Tic-Tac-Toe", nullptr, flags);
        int it_x, it_y;
        static std::array<std::array<int,3>,3> board = {{{0,0,0},{0,0,0},{0,0,0}}};
        int aux_id = 0;
        static int round = 0;
        static bool restart = false;
        static bool winX = false;
        static bool winO = false;
        static bool draw = false;
        static std::string status = "";
        float font_size = 0;
        int winner = 0;
        if(!winX && !winO && !draw){
            if (round%2 == 0) status = "X turn";
            else status = "O turn";
            font_size = ImGui::GetFontSize() * status.size() / 2;
            ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
            ImGui::Text((status).c_str());
        }
        else{
            if(winX) status = "X is the winner!";
            else if(winO) status = "O is the winner!" ;
            else if(draw) status = "Draw";
            font_size = ImGui::GetFontSize() * status.size() / 2;
            ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
            ImGui::Text((status).c_str());
        }

        if (ImGui::BeginMenuBar()) {
            // File menu
            if (ImGui::BeginMenu("Game")) {
                ImGui::MenuItem("Restart", nullptr, &restart);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
            
        for (it_x = 0; it_x < 3; it_x++){
            for (it_y = 0; it_y < 3; it_y++){
                ImGui::PushID(aux_id);
                if (board[it_x][it_y]==1) ImGui::Button("X", ImVec2(135, 185));
                else if (board[it_x][it_y]==2) ImGui::Button("O", ImVec2(135, 185));
                else if (ImGui::Button("", ImVec2(135, 185)) && winX == false && winO == false && draw == false) {
                    if (round%2 == 0){
                        board[it_x][it_y] = 1;
                        round++;
                        
                    } else {
                        board[it_x][it_y] = 2;
                        round++;
                    }
                }
                aux_id++;
                ImGui::PopID();
                ImGui::SameLine();
            }
            if (it_x < 2) ImGui::Separator();
        }
        ImGui::Separator();
        if(ImGui::Button("Restart Game",ImVec2(425,40))){
            restart = true;
        }
        winner = checkWinner(board);
        if (winner == 1) winX = true;
        if (winner == 2) winO = true;
        if (round == 9 && winner == 0) draw = true;
        fmt::print("{} | {} | {}\n",winner,winO,status);
        if (restart){
            board = {{{0,0,0},{0,0,0},{0,0,0}}};
            round = 0;
            winO = false;
            winX = false;
            draw = false;
            status = "";
            restart = false;
        }
        
        ImGui::End();
        
    }

}

