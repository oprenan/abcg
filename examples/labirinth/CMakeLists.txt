project(labirinth)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp
                               player.cpp ball.cpp hearth.cpp)

enable_abcg(${PROJECT_NAME})