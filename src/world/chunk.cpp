#include "world/chunk.hpp"

#include <iostream>

namespace World
{

  Chunk::Chunk(int x, int y)
      : chunk_x(x), chunk_y(y)
  {
    for (int row = 0; row < CHUNK_SIZE; row++)
    {
      for (int col = 0; col < CHUNK_SIZE; col++)
      {
        blocos[row][col] = 0;
      }
    }
  }

  void Chunk::definir_bloco(int linha, int coluna, int id_bloco)
  {
    if (linha >= 0 && linha < CHUNK_SIZE &&
        coluna >= 0 && coluna < CHUNK_SIZE)
    {
      blocos[linha][coluna] = id_bloco;
    }
  }

  void Chunk::imprimir_chunk() const
  {
    std::cout << "x: " << chunk_x << std::endl;
    std::cout << "y: " << chunk_y << std::endl;

    for (int linha = 0; linha < CHUNK_SIZE; linha++)
    {
      for (int coluna = 0; coluna < CHUNK_SIZE; coluna++)
      {
        std::cout << blocos[linha][coluna] << " ";
      }
      std::cout << std::endl;
    }
  }

}