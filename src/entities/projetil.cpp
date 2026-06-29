#include "entities/projetil.hpp"

namespace Entities
{
  Projetil::Projetil(
      float pos_x,
      float pos_y,
      float direcao_x,
      float direcao_y,
      float velocidade_bala)
  {
    x = pos_x;
    y = pos_y;

    tamanho = 4;
    ativo = true;

    vel_x = direcao_x * velocidade_bala;
    vel_y = direcao_y * velocidade_bala;
  }

  void Projetil::atualizar(float delta_time)
  {
    if (!ativo)
      return;

    x += vel_x * delta_time;
    y += vel_y * delta_time;
  }

  float Projetil::obter_x() const
  {
    return x;
  }

  float Projetil::obter_y() const
  {
    return y;
  }

  int Projetil::obter_tamanho() const
  {
    return tamanho;
  }

  bool Projetil::esta_ativo() const
  {
    return ativo;
  }

  void Projetil::desativar()
  {
    ativo = false;
  }

  bool Projetil::checar_colisao_AABB(
      float ix,
      float iy,
      int ilarg,
      int ialt) const
  {
    return (
        x < ix + ilarg &&
        x + tamanho > ix &&
        y < iy + ialt &&
        y + tamanho > iy);
  }
}