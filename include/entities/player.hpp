#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cmath>

#include "entities/item.hpp"
#include "world/chunk.hpp"

namespace World
{
  class WorldManager;
}

namespace Entities
{
  class Player
  {
  private:
    float x;
    float y;

    float velocidade_x;
    float velocidade_y;

    float velocidade_movimento;

    int tamanho;

    int direcao_olhar;

    int quantidade_chaves;
    bool possui_arma;

    World::WorldManager *worldManager;

  public:
    Player(float posicao_inicial_x,
           float posicao_inicial_y,
           World::WorldManager *gerenciador_do_mundo);

    void processar_input(bool esquerda, bool direita, bool cima, bool baixo);

    void interagir();

    void atualizar(float delta_time);

    void atirar(float mouse_x, float mouse_y, float camera_x, float camera_y);

    float obter_x() const;
    float obter_y() const;
    int obter_tamanho() const;
  };
}

#endif