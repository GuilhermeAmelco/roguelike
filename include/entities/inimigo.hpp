#ifndef INIMIGO_HPP
#define INIMIGO_HPP

#include "entities/item.hpp"
#include <string>

namespace World
{
  class WorldManager;
}

namespace Entities
{
  class Inimigo
  {
  private:
    float x, y, vida;

    bool possui_drop_obrigatorio;
    TipoItem item_obrigatorio;
    std::string conteudo_obrigatorio;

    void morrer(World::WorldManager *worldManager);

  public:
    Inimigo(float pos_x, float pos_y);

    void definir_drop_obrigatorio(TipoItem tipo, std::string conteudo = "");

    void receber_dano(float dano, World::WorldManager *worldManager);

    float obter_x();
    float obter_y();
    float obter_vida();
  };
}

#endif