 • Um breve relatório explicando as técnicas utilizadas na atividade, em formato PDF ou Markdown (ex: arquivo README.md do repositório);
 Foi usado como base o código fonte de asteroides.
 
 Jogo:
 - Para vencer, basta manter a bola vermelha longe do circulo central por 45 segundos.
 - Após 20 segundos uma segunda bola é gerada.
 
 Shaders:
 - Shader adicional para funcionamento da bola.
 - Shader para outros objetos.
 
 Player:
 Forma de barra.
 Usa como base da translação o seno e cosseno da rotação, que representa o circulo que é desenhado pela movimentação.
 
 Hearth:
 Funciona como a vida do jogo, tem muitos vértices aleatórios e gira para ter o formato de sol.
 Em colisão com a bola encerra o jogo.
 
 Ball:
 "Inimigo" do jogo, tem estrutura para ser mais de uma e muda em quantidade conforme o tempo passado em jogo.
 Caso bata na barra tem a velocidade invertida.
 Direção de origem aleatória.
 
 • O link da página web (GitHub Pages ou similar) com a aplicação rodando em WebAssembly. Excepcionalmente, se não for possível gerar o binário WebAssembly, poderá ser entregue um vídeo no lugar na página.
 
 Arquivo com video da execução local disponível nesse repositório com nome de screen_recordig.mp4
