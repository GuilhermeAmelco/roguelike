#include "entities/player.hpp"

#include "world/worldManager.hpp"

#include <iostream>

namespace Entities
{
  Player::Player(
      float posicao_inicial_x,
      float posicao_inicial_y,
      World::WorldManager *gerenciador_do_mundo)
  {
    x = posicao_inicial_x;
    y = posicao_inicial_y;

    velocidade_x = 0.0f;
    velocidade_y = 0.0f;
    velocidade_movimento = 200.0f;

    tamanho = 16;

    direcao_olhar = 1;

    quantidade_chaves = 0;
    possui_arma = false;

    worldManager = gerenciador_do_mundo;
  }

  void Player::processar_input(bool esquerda, bool direita, bool cima, bool baixo)
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
      velocidade_y = velocidade_movimento;
    else
      velocidade_y = 0.0f;
  }

  void Player::interagir()
  {
    if (!worldManager)
      return;

    int t_size = worldManager->obter_tile_size();

    float alc_x = x;
    float alc_y = y;
    int alc_l = tamanho;
    int alc_a = tamanho;

    int alcance_frontal = static_cast<int>(t_size * 1.2f);
    int expansao_lateral = t_size;

    switch (direcao_olhar)
    {
    case 0:
      alc_y -= alcance_frontal;
      alc_a = alcance_frontal;
      alc_x -= expansao_lateral / 2;
      alc_l += expansao_lateral;
      break;

    case 1:
      alc_y += tamanho;
      alc_a = alcance_frontal;
      alc_x -= expansao_lateral / 2;
      alc_l += expansao_lateral;
      break;

    case 2:
      alc_x -= alcance_frontal;
      alc_l = alcance_frontal;
      alc_y -= expansao_lateral / 2;
      alc_a += expansao_lateral;
      break;

    case 3:
      alc_x += tamanho;
      alc_l = alcance_frontal;
      alc_y -= expansao_lateral / 2;
      alc_a += expansao_lateral;
      break;
    }

    std::vector<World::Chunk *> chunks_para_testar =
        worldManager->obter_chunks_ativos_ao_redor();

    for (World::Chunk *chunk : chunks_para_testar)
    {
      for (auto it = chunk->itens.begin(); it != chunk->itens.end(); ++it)
      {
        if (it->checar_colisao_AABB(alc_x, alc_y, alc_l, alc_a))
        {
          if (it->obter_tipo() == CHAVE)
          {
            quantidade_chaves++;

            std::cout << "[INVENTÁRIO] Chave coletada! Total: "
                      << quantidade_chaves << std::endl;

            chunk->itens.erase(it);
            return;
          }

          if (it->obter_tipo() == BAU &&
              !it->foi_interagido())
          {
            it->interagir_com_item();

            possui_arma = true;

            std::cout
                << "[BAÚ] Você abriu o baú e pegou uma: "
                << it->obter_conteudo()
                << "!"
                << std::endl;

            return;
          }
        }
      }
    }

    int coluna_inicial = static_cast<int>(alc_x) / t_size;
    int coluna_final = static_cast<int>(alc_x + alc_l - 1) / t_size;

    int linha_inicial = static_cast<int>(alc_y) / t_size;
    int linha_final = static_cast<int>(alc_y + alc_a - 1) / t_size;

    for (int l_global = linha_inicial; l_global <= linha_final; l_global++)
    {
      for (int c_global = coluna_inicial; c_global <= coluna_final; c_global++)
      {
        float pixel_teste_x = (c_global * t_size) + (t_size / 2.0f);
        float pixel_teste_y = (l_global * t_size) + (t_size / 2.0f);

        int bloco_id =
            worldManager->obter_bloco_na_posicao(pixel_teste_x, pixel_teste_y);

        if (bloco_id == 5)
        {
          if (quantidade_chaves > 0)
          {
            quantidade_chaves--;

            worldManager->definir_bloco_na_posicao(
                pixel_teste_x,
                pixel_teste_y,
                0);

            std::cout
                << "[PORTA] Porta destrancada com sucesso!"
                << std::endl;
          }
          else
          {
            std::cout
                << "[PORTA] A dungeon bloqueia você. Encontre uma chave!"
                << std::endl;
          }

          return;
        }
      }
    }
  }

  void Player::atualizar(float delta_time)
  {
    if (!worldManager)
      return;

    float proximo_x = x + velocidade_x * delta_time;

    if (!worldManager->existe_colisao_na_posicao(
            proximo_x,
            y,
            tamanho,
            tamanho))
    {
      x = proximo_x;
    }

    float proximo_y = y + velocidade_y * delta_time;

    if (!worldManager->existe_colisao_na_posicao(
            x,
            proximo_y,
            tamanho,
            tamanho))
    {
      y = proximo_y;
    }
  }

  void Player::atirar(float mouse_x, float mouse_y, float camera_x, float camera_y)
  {
    if (!worldManager)
      return;

    // centro do player no mundo
    float spawn_x = x + tamanho / 2.0f;
    float spawn_y = y + tamanho / 2.0f;

    // 🔥 converte mouse da tela → mundo
    float world_mouse_x = mouse_x + camera_x;
    float world_mouse_y = mouse_y + camera_y;

    float delta_x = world_mouse_x - spawn_x;
    float delta_y = world_mouse_y - spawn_y;

    float distancia = std::sqrt(delta_x * delta_x + delta_y * delta_y);

    if (distancia == 0.0f)
      return;

    float dir_x = delta_x / distancia;
    float dir_y = delta_y / distancia;

    worldManager->adicionar_projetil_mouse(
        spawn_x,
        spawn_y,
        dir_x,
        dir_y,
        400.0f);
  }
  float Player::obter_x() const
  {
    return x;
  }

  float Player::obter_y() const
  {
    return y;
  }

  int Player::obter_tamanho() const
  {
    return tamanho;
  }
}