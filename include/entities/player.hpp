#ifndef PLAYER_HPP
#define PLAYER_HPP

namespace Entities
{
  class Player
  {
  private:
    // posicao do player
    float x;
    float y;

    // velocidade atual
    float velocidade_x;
    float velocidade_y;

    // velocidade limite
    float velocidade_movimento;

    // o tamanho do nosso quadrado do jogador na tela
    int tamanho;

  public:
    // Construtor inicializa o jogador em uma posição inicial do mundo
    Player(float posicao_inicial_x, float posicao_inicial_y)
    {
      x = posicao_inicial_x;
      y = posicao_inicial_y;
      velocidade_x = 0.0f;
      velocidade_y = 0.0f;
      velocidade_movimento = 200.0f; // 200 pixeis por segundo
      tamanho = 8;
    }

    // 1. Processar inpit altera a velocidade com base nas teclas pressionadas
    void processar_input(bool esquerda, bool direita, bool cima, bool baixo)
    {
      if (esquerda)
        velocidade_x = -velocidade_movimento;
      else if (direita)
        velocidade_x = velocidade_movimento;
      else
        velocidade_x = 0.0f;

      if (cima)
        velocidade_y = -velocidade_movimento;
      else if (baixo)
        velocidade_y = velocidade_y;
      else
        velocidade_y = 0;
    }

    // 2. Atualizar o jogador
    void atualizar(float delta_time)
    {
      // A posicao muda multiplicando a velocidade pelo tempo
      x += velocidade_x * delta_time;
      y += velocidade_y * delta_time;
    }

    // helpers para ajudar a debugar
    float obter_x() const { return x; }
    float obter_y() const { return y; }
    int obter_tamanho() const { return tamanho; }
  };
}

#endif