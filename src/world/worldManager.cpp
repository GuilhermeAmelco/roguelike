#include "world/worldManager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

namespace World
{

  WorldManager::WorldManager() {}

  std::string WorldManager::gerar_chave_chunk(int chunk_x, int chunk_y)
  {
    return std::to_string(chunk_x) + "," + std::to_string(chunk_y);
  }

  void WorldManager::carregar_chunk(int chunk_x, int chunk_y)
  {
    std::string chave = gerar_chave_chunk(chunk_x, chunk_y);
    std::ifstream arquivo_mapa("assets/data/map.txt");

    if (!arquivo_mapa.is_open())
    {
      std::cerr << "Erro: não encontrei o arquivo de mapa!" << std::endl;
      return;
    }

    if (chunks_carregados.find(chave) != chunks_carregados.end())
      return;

    Chunk novo_chunk(chunk_x, chunk_y);

    int inicio_global_x = chunk_x * CHUNK_SIZE;
    int inicio_global_y = chunk_y * CHUNK_SIZE;

    std::string linha;
    int linha_atual = 0;

    while (std::getline(arquivo_mapa, linha))
    {
      if (linha_atual >= inicio_global_y &&
          linha_atual < inicio_global_y + CHUNK_SIZE)
      {
        int l = linha_atual - inicio_global_y;

        std::stringstream leitor(linha);
        int id_bloco;
        int coluna_atual = 0;

        while (leitor >> id_bloco)
        {
          if (coluna_atual >= inicio_global_x &&
              coluna_atual < inicio_global_x + CHUNK_SIZE)
          {
            int c = coluna_atual - inicio_global_x;

            float px = coluna_atual * TILE_SIZE;
            float py = linha_atual * TILE_SIZE;

            if (id_bloco == 2)
            {
              novo_chunk.blocos[l][c] = 0;
              novo_chunk.itens.push_back(
                  Entities::Item(px + 8, py + 8, 16, 16, Entities::CHAVE));
            }
            else if (id_bloco == 3)
            {
              novo_chunk.blocos[l][c] = 1;
              novo_chunk.itens.push_back(
                  Entities::Item(px, py, 32, 32, Entities::BAU, "Espada de Ferro"));
            }
            else
            {
              novo_chunk.definir_bloco(l, c, id_bloco);
            }
          }
          coluna_atual++;
        }
      }
      linha_atual++;
    }

    arquivo_mapa.close();
    chunks_carregados.insert({chave, novo_chunk});
  }

  void WorldManager::descarregar_chunk(int chunk_x, int chunk_y)
  {
    chunks_carregados.erase(gerar_chave_chunk(chunk_x, chunk_y));
  }

  Chunk *WorldManager::obter_chunk(int chunk_x, int chunk_y)
  {
    auto it = chunks_carregados.find(gerar_chave_chunk(chunk_x, chunk_y));
    if (it != chunks_carregados.end())
      return &it->second;

    return nullptr;
  }

  void WorldManager::atualizar_mundo(float jogador_pixel_x, float jogador_pixel_y)
  {
    int jogador_tile_x = (int)jogador_pixel_x / TILE_SIZE;
    int jogador_tile_y = (int)jogador_pixel_y / TILE_SIZE;

    int centro_chunk_x = (jogador_tile_x < 0)
                             ? (jogador_tile_x - CHUNK_SIZE + 1) / CHUNK_SIZE
                             : jogador_tile_x / CHUNK_SIZE;

    int centro_chunk_y = (jogador_tile_y < 0)
                             ? (jogador_tile_y - CHUNK_SIZE + 1) / CHUNK_SIZE
                             : jogador_tile_y / CHUNK_SIZE;

    const int RENDER_DISTANCE = 1;
    std::vector<std::string> necessarios;

    for (int y = centro_chunk_y - RENDER_DISTANCE;
         y <= centro_chunk_y + RENDER_DISTANCE; y++)
    {
      for (int x = centro_chunk_x - RENDER_DISTANCE;
           x <= centro_chunk_x + RENDER_DISTANCE; x++)
      {
        carregar_chunk(x, y);
        necessarios.push_back(gerar_chave_chunk(x, y));
      }
    }

    for (auto it = chunks_carregados.begin(); it != chunks_carregados.end();)
    {
      bool ok = false;

      for (auto &k : necessarios)
        if (k == it->first)
          ok = true;

      if (!ok)
        it = chunks_carregados.erase(it);
      else
        ++it;
    }
  }

  bool WorldManager::existe_colisao_na_posicao(float x, float y, int sx, int sy)
  {
    int c0 = (int)x / TILE_SIZE;
    int c1 = (int)(x + sx - 1) / TILE_SIZE;
    int l0 = (int)y / TILE_SIZE;
    int l1 = (int)(y + sy - 1) / TILE_SIZE;

    for (int l = l0; l <= l1; l++)
      for (int c = c0; c <= c1; c++)
      {
        int cx = (c < 0) ? (c - CHUNK_SIZE + 1) / CHUNK_SIZE : c / CHUNK_SIZE;
        int cy = (l < 0) ? (l - CHUNK_SIZE + 1) / CHUNK_SIZE : l / CHUNK_SIZE;

        Chunk *chunk = obter_chunk(cx, cy);
        if (!chunk)
          return true;

        int li = (l % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;
        int ci = (c % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;

        int id = chunk->blocos[li][ci];
        if (id == 1 || id == 5)
          return true;
      }

    return false;
  }

  int WorldManager::obter_bloco_na_posicao(float x, float y)
  {
    int bx = (int)x / TILE_SIZE;
    int by = (int)y / TILE_SIZE;

    int cx = (bx < 0) ? (bx - CHUNK_SIZE + 1) / CHUNK_SIZE : bx / CHUNK_SIZE;
    int cy = (by < 0) ? (by - CHUNK_SIZE + 1) / CHUNK_SIZE : by / CHUNK_SIZE;

    Chunk *chunk = obter_chunk(cx, cy);
    if (!chunk)
      return 1;

    int li = (by % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;
    int ci = (bx % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;

    return chunk->blocos[li][ci];
  }

  void WorldManager::definir_bloco_na_posicao(float x, float y, int id)
  {
    int bx = (int)x / TILE_SIZE;
    int by = (int)y / TILE_SIZE;

    int cx = (bx < 0) ? (bx - CHUNK_SIZE + 1) / CHUNK_SIZE : bx / CHUNK_SIZE;
    int cy = (by < 0) ? (by - CHUNK_SIZE + 1) / CHUNK_SIZE : by / CHUNK_SIZE;

    Chunk *chunk = obter_chunk(cx, cy);
    if (!chunk)
      return;

    int li = (by % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;
    int ci = (bx % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE;

    chunk->blocos[li][ci] = id;
  }

  void WorldManager::gerar_item_no_mundo(float x, float y, int w, int h,
                                         Entities::TipoItem tipo, std::string conteudo)
  {
    int bx = (int)x / TILE_SIZE;
    int by = (int)y / TILE_SIZE;

    int cx = (bx < 0) ? (bx - CHUNK_SIZE + 1) / CHUNK_SIZE : bx / CHUNK_SIZE;
    int cy = (by < 0) ? (by - CHUNK_SIZE + 1) / CHUNK_SIZE : by / CHUNK_SIZE;

    Chunk *chunk = obter_chunk(cx, cy);
    if (!chunk)
      return;

    chunk->itens.push_back(Entities::Item(x, y, w, h, tipo, conteudo));
  }

  void WorldManager::adicionar_projetil_mouse(float x, float y,
                                              float dx, float dy, float v)
  {
    projeteis_ativos.push_back(
        Entities::Projetil(x, y, dx, dy, v));
  }

  void WorldManager::atualizar_projeteis(float dt)
  {
    for (int i = projeteis_ativos.size() - 1; i >= 0; i--)
    {
      projeteis_ativos[i].atualizar(dt);

      float x = projeteis_ativos[i].obter_x();
      float y = projeteis_ativos[i].obter_y();
      int t = projeteis_ativos[i].obter_tamanho();

      if (existe_colisao_na_posicao(x, y, t, t))
      {
        projeteis_ativos.erase(projeteis_ativos.begin() + i);
      }
    }
  }

  size_t WorldManager::quantidade_chunks_carregados() const
  {
    return chunks_carregados.size();
  }

  int WorldManager::obter_tile_size() const
  {
    return TILE_SIZE;
  }

  std::vector<Chunk *> WorldManager::obter_chunks_ativos_ao_redor()
  {
    std::vector<Chunk *> lista;
    for (auto &p : chunks_carregados)
      lista.push_back(&p.second);

    return lista;
  }

  const std::vector<Entities::Projetil> &
  WorldManager::obter_projeteis() const
  {
    return projeteis_ativos;
  }

}