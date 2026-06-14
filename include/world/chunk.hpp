#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <vector>

// O chunk é um MOLDE, um bloco de memória limp de 16x16 que é preenchido por zeros. ele vai servir para guardar as informações de diferentes partes do mapa.
namespace World
{
  const int CHUNK_SIZE = 16; // o chunk será 16x16 blocos
  const int TILE_SIZE = 16;  // Cada bloco tem 16x16 pixeis

  struct Chunk
  {
    // 1. Posição do Chunk no "grid de chunks" do mundo.
    // Exemplo: O primeiro chunk é o (0,0), o seu vizinho da direita é (1,0)
    int chunk_x;
    int chunk_y;

    // 2. A matriz que gaurda os IDs dos blocos.
    // Usamos um array bidimensional com o tamanho CHUNK_SIZE (16x16)
    int blocos[CHUNK_SIZE][CHUNK_SIZE];

    // 3. Criamos o constructor, ele vai inicializar o chunk na posição que passarmos
    Chunk(int x, int y) : chunk_x(x), chunk_y(y)
    {
      // Por padrão preencheremos com blocos vazios que serão representados com o 0
      for (int row = 0; row < CHUNK_SIZE; row++)
      {
        for (int col = 0; col < CHUNK_SIZE; col++)
        {
          blocos[row][col] = 0;
        }
      }
    }
  };
}

#endif