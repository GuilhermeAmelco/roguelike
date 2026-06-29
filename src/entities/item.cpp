#include "entities/item.hpp"

#include <cstdlib>

namespace Entities
{
  Recompensa sortear_item_aleatorio()
  {
    int chance = rand() % 100;

    if (chance < 40)
    {
      return {static_cast<TipoItem>(0), 0, 0, ""};
    }
    else if (chance < 70)
    {
      return {ARMA, 24, 24, "Pocaozinha de Vida"};
    }
    else if (chance < 90)
    {
      return {CHAVE, 16, 16, ""};
    }
    else
    {
      return {BAU, 32, 32, "Espada de Fogo Rara"};
    }
  }

  Item::Item(
      float pos_x,
      float pos_y,
      int larg,
      int alt,
      TipoItem tipo_item,
      std::string conteudo_bau)
      : x(pos_x),
        y(pos_y),
        largura(larg),
        altura(alt),
        tipo(tipo_item),
        interagido(false),
        conteudo(conteudo_bau)
  {
  }

  float Item::obter_x() const
  {
    return x;
  }

  float Item::obter_y() const
  {
    return y;
  }

  int Item::obter_largura() const
  {
    return largura;
  }

  int Item::obter_altura() const
  {
    return altura;
  }

  TipoItem Item::obter_tipo() const
  {
    return tipo;
  }

  bool Item::foi_interagido() const
  {
    return interagido;
  }

  std::string Item::obter_conteudo() const
  {
    return conteudo;
  }

  void Item::interagir_com_item()
  {
    interagido = true;
  }

  bool Item::checar_colisao_AABB(float cx, float cy, int clarg, int calt) const
  {
    return (x < cx + clarg &&
            x + largura > cx &&
            y < cy + calt &&
            y + altura > cy);
  }
}