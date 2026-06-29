#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

namespace Entities
{
  enum TipoItem
  {
    CHAVE = 2,
    BAU = 3,
    ARMA = 4
  };

  struct Recompensa
  {
    TipoItem tipo;
    int larg;
    int alt;
    std::string conteudo;
  };

  // Declaração apenas
  Recompensa sortear_item_aleatorio();

  class Item
  {
  private:
    float x, y;
    int largura, altura;
    TipoItem tipo;

    bool interagido;
    std::string conteudo;

  public:
    Item(
        float pos_x,
        float pos_y,
        int larg,
        int alt,
        TipoItem tipo_item,
        std::string conteudo_bau = "");

    // Getters
    float obter_x() const;
    float obter_y() const;
    int obter_largura() const;
    int obter_altura() const;
    TipoItem obter_tipo() const;
    bool foi_interagido() const;
    std::string obter_conteudo() const;

    // Estado
    void interagir_com_item();

    // Colisão
    bool checar_colisao_AABB(float cx, float cy, int clarg, int calt) const;
  };
}

#endif