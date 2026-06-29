#include "entities/inimigo.hpp"

#include "world/worldManager.hpp"

#include <iostream>

namespace Entities
{
  Inimigo::Inimigo(float pos_x, float pos_y)
  {
    x = pos_x;
    y = pos_y;
    vida = 30.0f;

    possui_drop_obrigatorio = false;
    item_obrigatorio = CHAVE;
    conteudo_obrigatorio = "";
  }

  void Inimigo::definir_drop_obrigatorio(TipoItem tipo, std::string conteudo)
  {
    possui_drop_obrigatorio = true;
    item_obrigatorio = tipo;
    conteudo_obrigatorio = conteudo;
  }

  void Inimigo::receber_dano(float dano, World::WorldManager *worldManager)
  {
    vida -= dano;

    if (vida <= 0)
    {
      morrer(worldManager);
    }
  }

  float Inimigo::obter_x()
  {
    return x;
  }

  float Inimigo::obter_y()
  {
    return y;
  }

  float Inimigo::obter_vida()
  {
    return vida;
  }

  void Inimigo::morrer(World::WorldManager *worldManager)
  {
    std::cout << "[COMBATE] Inimigo derrotado!" << std::endl;

    if (possui_drop_obrigatorio)
    {
      int larg = (item_obrigatorio == CHAVE) ? 16 : 32;
      int alt = (item_obrigatorio == CHAVE) ? 16 : 32;

      worldManager->gerar_item_no_mundo(
          x,
          y,
          larg,
          alt,
          item_obrigatorio,
          conteudo_obrigatorio);

      std::cout << "[DROP] Drop OBRIGATORIO da sala liberado no chao!" << std::endl;
    }
    else
    {
      Recompensa drop = sortear_item_aleatorio();

      if (drop.tipo > 0)
      {
        worldManager->gerar_item_no_mundo(
            x,
            y,
            drop.larg,
            drop.alt,
            drop.tipo,
            drop.conteudo);

        std::cout << "[DROP] O inimigo deixou cair um item aleatorio!" << std::endl;
      }
    }
  }
}