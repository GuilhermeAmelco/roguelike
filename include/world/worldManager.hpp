#ifndef WORLDMANAGER_HPP
#define WORLDMANAGER_HPP
using namespace std;

#include "chunk.hpp"
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

namespace World
{
  class WorldManager
  {
  private:
    // o array que guarda todos os chunks na memoria RAM.
    // Chave: "X,Y" (string) -> Valor: O Objeto chunk em si.
    unordered_map<string, Chunk> chunks_carregados; // declarando a variavel chunks_carregados

    string gerar_chave_chunk(int chunk_x, int chunk_y)
    {
      return to_string(chunk_x) + "," + to_string(chunk_y);
    }

  public:
    WorldManager()
    {
      // Constructor: Por enquanto tem nada
    }

    // 1. Método para garantir que um chunk exista na memória
    // se ele nao existir no unordered_map, nos o criamos
    void carregar_chunk(int chunk_x, int chunk_y)
    {
      string chave = gerar_chave_chunk(chunk_x, chunk_y);
      ifstream arquivo_mapa("assets/data/map.txt");

      if (!arquivo_mapa.is_open())
      {
        cerr << "Erro: não encontrei o arquivo de mapa!" << endl;
        return;
      }

      // Se não for encontrado no mapa de memória, cria um novo
      if (chunks_carregados.find(chave) == chunks_carregados.end())
      {
        Chunk novo_chunk(chunk_x, chunk_y);

        int inicio_global_x = chunk_x * CHUNK_SIZE;
        int inicio_global_y = chunk_y * CHUNK_SIZE;

        string linha;
        int linha_atual = 0;

        // Lê o arquivo do começo ao fim naturalmente
        while (getline(arquivo_mapa, linha))
        {
          // Verifica se a linha do arquivo pertence a este Chunk
          if (linha_atual >= inicio_global_y && linha_atual < inicio_global_y + CHUNK_SIZE)
          {
            int l = linha_atual - inicio_global_y;

            stringstream leitor(linha);
            int id_bloco;
            int coluna_atual_do_arquivo = 0;

            while (leitor >> id_bloco)
            {
              // Verifica se a coluna do arquivo pertence a este Chunk
              if (coluna_atual_do_arquivo >= inicio_global_x &&
                  coluna_atual_do_arquivo < inicio_global_x + CHUNK_SIZE)
              {
                // MATEMÁTICA CORRETA: Calcula a coluna interna (0 a 31) sem travar
                int c = coluna_atual_do_arquivo - inicio_global_x;

                // DEBUG CORRIGIDO: Se for a coluna interna 0 do Chunk(0,0), printe!
                if (chunk_x == 0 && chunk_y == 0 && c == 0)
                {
                  cout << "Chunk(0,0) - Linha Interna " << l
                       << " (Linha Global " << linha_atual
                       << ") recebeu o ID: " << id_bloco << endl;
                }

                // Injeta o bloco no Chunk!
                novo_chunk.definir_bloco(l, c, id_bloco);
              }
              coluna_atual_do_arquivo++;
            }
          }

          linha_atual++; // Avança sempre para a próxima linha do arquivo texturizado
        }

        arquivo_mapa.close();

        // Inserir no mapa que guarda os chunks ativos na RAM
        chunks_carregados.insert({chave, novo_chunk});
      }
    }
    // 2. Metodo para descarregar as chunks da memoria RAM.
    // Criamos o descarregar chunk que serve para tirar do array caso o usuário se afaste do chunk
    void descarregar_chunk(int chunk_x, int chunk_y)
    {
      string chave = gerar_chave_chunk(chunk_x, chunk_y);
      chunks_carregados.erase(chave);
    }

    // 3. Metodo para pegar a chunk
    Chunk *obter_chunk(int chunk_x, int chunk_y)
    {
      string chave = gerar_chave_chunk(chunk_x, chunk_y);
      auto it = chunks_carregados.find(chave);
      if (it != chunks_carregados.end())
      {
        return &(it->second); // Retorna o ponteiro para o chunk que está no mapa
      }
      return nullptr; // Retorna nulo se o chunk não estiver carregado
    }

    // Atualiza quais chunks devem estar na memória com base na posição do jogador (em pixels)
    void atualizar_mundo(float jogador_pixel_x, float jogador_pixel_y)
    {

      // 1. CONVERSÃO: Transforma os pixels do jogador para coordenadas de Bloco (Tile)
      int jogador_tile_x = static_cast<int>(jogador_pixel_x) / TILE_SIZE;
      int jogador_tile_y = static_cast<int>(jogador_pixel_y) / TILE_SIZE;

      // 2. CONVERSÃO: Transforma os blocos do jogador para a coordenada do Chunk atual
      int centro_chunk_x;
      int centro_chunk_y;

      // Caso a coordenada seja negativa
      if (jogador_tile_x < 0)
        centro_chunk_x = (jogador_tile_x - CHUNK_SIZE + 1) / CHUNK_SIZE;
      else
        centro_chunk_x = jogador_tile_x / CHUNK_SIZE;
      if (jogador_tile_y < 0)
        centro_chunk_y = (jogador_tile_y - CHUNK_SIZE + 1) / CHUNK_SIZE;
      else
        centro_chunk_y = jogador_tile_y / CHUNK_SIZE;

      // Definição do raio de visão: 1 significa um quadrado de 3x3 chunks ao redor do jogador
      const int RENDER_DISTANCE = 1;

      // Guardamos quais chaves de chunks DEVERIAM estar carregadas neste frame
      vector<string> chunks_necessarios;

      // 3. CARREGAMENTO: Loop para ativar os chunks na zona de visão
      for (int y = centro_chunk_y - RENDER_DISTANCE; y <= centro_chunk_y + RENDER_DISTANCE; ++y)
      {
        for (int x = centro_chunk_x - RENDER_DISTANCE; x <= centro_chunk_x + RENDER_DISTANCE; ++x)
        {

          // Ativa o chunk (se ele já existir, a função ignora; se não, cria do zero)
          carregar_chunk(x, y);

          // Guarda a chave dele como um chunk necessário
          chunks_necessarios.push_back(gerar_chave_chunk(x, y));
        }
      }

      // 4. LIMPEZA DE MEMÓRIA: Descarrega o que ficou longe demais
      // Vamos olhar cada chunk que está atualmente na memória RAM
      for (auto it = chunks_carregados.begin(); it != chunks_carregados.end();)
      {
        string chave_atual = it->first;

        // Verifica se o chunk atual da memória está na lista de chunks necessários
        bool precisa_estar_carregado = false;
        for (const auto &chave_nec : chunks_necessarios)
        {
          if (chave_atual == chave_nec)
          {
            precisa_estar_carregado = true;
            break;
          }
        }

        // Se ele não for necessário, removemos do unordered_map (libera RAM)
        if (!precisa_estar_carregado)
        {
          it = chunks_carregados.erase(it); // erase retorna o próximo iterador válido
        }
        else
        {
          ++it; // vai para o próximo chunk da memória
        }
      }
    }

    size_t quantidade_chunks_carregados() const
    {
      return chunks_carregados.size();
    }
  };
}

#endif