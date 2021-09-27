#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <array>

#include "abcg.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;

 private:
  std::array<float, 4> m_clearColor{0.906f, 0.910f, 0.918f, 1.0f};
};

#endif

/*
initializeGL é onde colocaremos os comandos de inicialização do estado da janela e do OpenGL. Internamente a ABCg chama essa função apenas uma vez no início do programa, após ter inicializado os subsistemas da SDL e o OpenGL.

paintGL é onde colocaremos todas as funções de desenho do OpenGL. Internamente a ABCg chama essa função continuamente no laço principal da aplicação, uma vez para cada quadro (frame) de exibição. Por exemplo, na imagem acima, paintGL estava sendo chamada a uma média de 3988.7 vezes por segundo;

paintUI é onde colocaremos todas as funções de desenho de widgets da ImGui (botões, menus, caixas de seleção, etc). Internamente, paintUI é chamado sempre que paintGL é chamado;

m_clearColor é um arranjo de quatro valores float entre 0 e 1. Esses valores definem a cor de fundo da janela (neste caso, um cinza claro).
*/