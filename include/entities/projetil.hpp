#ifndef PROJETIL_HPP
#define PROJETIL_HPP

namespace Entities
{
  class Projetil
  {
  private:
    float x;
    float y;

    float vel_x;
    float vel_y;

    int tamanho;

    bool ativo;

  public:
    Projetil(
        float pos_x,
        float pos_y,
        float direcao_x,
        float direcao_y,
        float velocidade_bala);

    void atualizar(float delta_time);

    float obter_x() const;
    float obter_y() const;
    int obter_tamanho() const;
    bool esta_ativo() const;

    void desativar();

    bool checar_colisao_AABB(
        float ix,
        float iy,
        int ilarg,
        int ialt) const;
  };
}

#endif