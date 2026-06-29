#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <vector>

#include "entities/item.hpp"

namespace World
{
  constexpr int CHUNK_SIZE = 32;
  constexpr int TILE_SIZE = 32;

  struct Chunk
  {
    int chunk_x;
    int chunk_y;

    std::vector<Entities::Item> itens;

    int blocos[CHUNK_SIZE][CHUNK_SIZE];

    Chunk(int x, int y);

    void definir_bloco(int linha, int coluna, int id_bloco);
    void imprimir_chunk() const;
  };
}

#endif