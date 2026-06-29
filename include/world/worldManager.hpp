#ifndef WORLDMANAGER_HPP
#define WORLDMANAGER_HPP

#include "chunk.hpp"
#include "entities/projetil.hpp"
#include "entities/inimigo.hpp"

#include <unordered_map>
#include <vector>
#include <string>

namespace World
{
  class WorldManager
  {
  private:
    std::unordered_map<std::string, Chunk> chunks_carregados;

    std::string gerar_chave_chunk(int chunk_x, int chunk_y);

    std::vector<Entities::Projetil> projeteis_ativos;
    std::vector<Entities::Inimigo *> inimigos_no_mundo;

  public:
    WorldManager();

    void carregar_chunk(int chunk_x, int chunk_y);
    void descarregar_chunk(int chunk_x, int chunk_y);
    Chunk *obter_chunk(int chunk_x, int chunk_y);

    void atualizar_mundo(float jogador_pixel_x, float jogador_pixel_y);

    bool existe_colisao_na_posicao(float jogador_pixel_x, float jogador_pixel_y, int tamanho_x, int tamanho_y);

    int obter_bloco_na_posicao(float pixel_x, float pixel_y);
    void definir_bloco_na_posicao(float pixel_x, float pixel_y, int novo_id);

    void gerar_item_no_mundo(float pixel_x, float pixel_y, int largura, int altura,
                             Entities::TipoItem tipo, std::string conteudo = "");

    void adicionar_projetil_mouse(float x, float y, float dir_x, float dir_y, float velocidade);
    void atualizar_projeteis(float delta_time);

    size_t quantidade_chunks_carregados() const;
    int obter_tile_size() const;

    std::vector<Chunk *> obter_chunks_ativos_ao_redor();
    const std::vector<Entities::Projetil> &obter_projeteis() const;
  };
}

#endif