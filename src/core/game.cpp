#include "core/Game.hpp"
#include <iostream>
#include <thread>

namespace Core
{

  Game::Game() : rodando(true)
  {
    // Inicializa nossos módulos (Janela 800x600)
    renderer = new Graphics::Renderer("Meu Jogo 2D - Teste de Chunks", 1200, 800);
    worldManager = new World::WorldManager();

    // Coloca o jogador nascido no meio do pixel zero
    player = new Entities::Player(1350.0f, 512.0f, worldManager);
  }

  Game::~Game()
  {
    delete player;
    delete worldManager;
    delete renderer;
  }

  void Game::processar_eventos()
  {
    SDL_Event evento;
    // O SDL3 coleta todas as ações do usuário (teclado, mouse, fechar janela)
    while (SDL_PollEvent(&evento))
    {
      if (evento.type == SDL_EVENT_QUIT)
      {
        rodando = false;
      }
      // Verifica se uma tecla foi pressionada (Down) ou solta (Up)
      else if (evento.type == SDL_EVENT_KEY_DOWN || evento.type == SDL_EVENT_KEY_UP)
      {
        bool status = (evento.type == SDL_EVENT_KEY_DOWN);

        switch (evento.key.key)
        {
        case SDLK_A:
        case SDLK_LEFT:
          tecla_esquerda = status;
          break;
        case SDLK_D:
        case SDLK_RIGHT:
          tecla_direita = status;
          break;
        case SDLK_W:
        case SDLK_UP:
          tecla_cima = status;
          break;
        case SDLK_S:
        case SDLK_DOWN:
          tecla_baixo = status;
          break;
        case SDLK_E:
          player->interagir();
          break;
        case SDLK_ESCAPE:
          rodando = false;
          break;
        }
      }
      else if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
      {
        if (evento.button.button == SDL_BUTTON_LEFT)
        {
          // No SDL3, evento.button.x e y são FLOAT
          float mouseX = evento.button.x;
          float mouseY = evento.button.y;

          float camera_x = player->obter_x() - (renderer->obter_largura() / 2.0f);
          float camera_y = player->obter_y() - (renderer->obter_altura() / 2.0f);
          // Passa as coordenadas obtidas para a função do seu jogador
          player->atirar(mouseX, mouseY, camera_x, camera_y);
        }
      }
    }
  }

  void Game::atualizar(float delta_time)
  {
    // 1. Passa os inputs para o jogador e atualiza a posição dele
    player->processar_input(tecla_esquerda, tecla_direita, tecla_cima, tecla_baixo);
    player->atualizar(delta_time); // Nota: ajuste para casar com o método do Player

    // 2. Faz o sistema de Chunks atualizar baseado na nova posição do jogador
    worldManager->atualizar_mundo(player->obter_x(), player->obter_y());
    worldManager->atualizar_projeteis(delta_time);
    // 3. Centraliza a Câmera no jogador
    // (Posição do jogador menos a metade da tela para ele ficar bem no centro)
    camera_x = player->obter_x() - (renderer->obter_largura() / 2.0f);
    camera_y = player->obter_y() - (renderer->obter_altura() / 2.0f);
  }

  void Game::renderizar()
  {
    // Limpa a tela com uma cor cinza escuro de fundo
    renderer->limpar_tela(30, 30, 30);

    // 1. Calcula o tamanho total de um chunk em pixels (32 * 32 = 1024)
    int chunk_pixel_size = World::CHUNK_SIZE * World::TILE_SIZE;

    // 2. CONVERSÃO CORRETA: Protegendo contra arredondamentos errados em coordenadas negativas
    int jogador_pixel_x = static_cast<int>(player->obter_x());
    int jogador_pixel_y = static_cast<int>(player->obter_y());

    int jogador_chunk_x;
    int jogador_chunk_y;

    if (jogador_pixel_x < 0)
      jogador_chunk_x = (jogador_pixel_x - chunk_pixel_size + 1) / chunk_pixel_size;
    else
      jogador_chunk_x = jogador_pixel_x / chunk_pixel_size;

    if (jogador_pixel_y < 0)
      jogador_chunk_y = (jogador_pixel_y - chunk_pixel_size + 1) / chunk_pixel_size;
    else
      jogador_chunk_y = jogador_pixel_y / chunk_pixel_size;

    // Desenha o chunk atual e os vizinhos mais próximos
    for (int cx = jogador_chunk_x - 1; cx <= jogador_chunk_x + 1; ++cx)
    {
      for (int cy = jogador_chunk_y - 1; cy <= jogador_chunk_y + 1; ++cy)
      {

        World::Chunk *chunk = worldManager->obter_chunk(cx, cy);

        if (chunk != nullptr)
        {
          // Descobre a posição inicial desse chunk no mundo em pixels
          float chunk_mundo_x = cx * World::CHUNK_SIZE * World::TILE_SIZE;
          float chunk_mundo_y = cy * World::CHUNK_SIZE * World::TILE_SIZE;

          // Passa por cada bloco interno do chunk
          for (int r = 0; r < World::CHUNK_SIZE; ++r)
          {
            for (int c = 0; c < World::CHUNK_SIZE; ++c)
            {
              float bloco_mundo_x = chunk_mundo_x + (c * World::TILE_SIZE);
              float bloco_mundo_y = chunk_mundo_y + (r * World::TILE_SIZE);

              float tela_x = bloco_mundo_x - camera_x;
              float tela_y = bloco_mundo_y - camera_y;

              // Desenha apenas se for parede (ID 1), que são as nossas bordinhas de teste
              switch (chunk->blocos[r][c])
              {
              case 0:
                renderer->desenhar_retangulo(tela_x, tela_y, World::TILE_SIZE, World::TILE_SIZE, 0, 0, 0);
                break;
              case 1:
                renderer->desenhar_retangulo(tela_x, tela_y, World::TILE_SIZE, World::TILE_SIZE, 34, 139, 34);
                break;
              case 5:
                renderer->desenhar_retangulo(tela_x, tela_y, World::TILE_SIZE, World::TILE_SIZE, 128, 128, 128);
                break;
              }
            }
          }
        }
      }
    }

    std::vector<World::Chunk *> chunks_visiveis = worldManager->obter_chunks_ativos_ao_redor();
    for (World::Chunk *chunk : chunks_visiveis)
    {
      for (const auto &item : chunk->itens)
      {
        if (item.obter_tipo() == Entities::CHAVE)
        {
          renderer->desenhar_retangulo(item.obter_x(), item.obter_y(), item.obter_altura(), item.obter_largura(), 255, 215, 0);
        }
        else if (item.obter_tipo() == Entities::BAU)
        {
          if (item.foi_interagido())
          {
            renderer->desenhar_retangulo(item.obter_x(), item.obter_y(), item.obter_altura(), item.obter_largura(), 205, 133, 63);
          }
          else
          {
            renderer->desenhar_retangulo(item.obter_x(), item.obter_y(), item.obter_altura(), item.obter_largura(), 139, 69, 19);
          }
        }
      }
    }

    // --- DESENHAR PROJÉTEIS ---
    const auto &projeteis = worldManager->obter_projeteis();

    for (const auto &p : projeteis)
    {
      float tela_x = p.obter_x() - camera_x;
      float tela_y = p.obter_y() - camera_y;

      int tamanho = p.obter_tamanho();

      // projétil como um quadrado amarelo simples
      renderer->desenhar_retangulo(
          tela_x,
          tela_y,
          tamanho,
          tamanho,
          255, 255, 0);
    }
    // --- DESENHAR O JOGADOR ---
    float player_tela_x = player->obter_x() - camera_x;
    float player_tela_y = player->obter_y() - camera_y;
    // Desenha o jogador como um quadrado vermelho bem no centro da tela
    renderer->desenhar_retangulo(player_tela_x, player_tela_y, player->obter_tamanho(), player->obter_tamanho(), 220, 20, 60);

    // Apresenta o resultado final
    renderer->apresentar();
  }

  void Game::rodar()
  {
    // Controle de tempo simples para debug (Fixo em aproximadamente 60 FPS)
    const float DELTA_TIME = 0.016f; // 16 milissegundos por frame

    // Imprime no console quantos chunks começam carregados (deve ser 0)
    std::cout << "Chunks carregados ao iniciar: " << worldManager->quantidade_chunks_carregados() << std::endl;

    while (rodando)
    {
      processar_eventos();
      atualizar(DELTA_TIME);
      renderizar();

      // Uma pequena trava de segurança para a CPU não fritar no teste básico
      SDL_Delay(16);
    }

    std::cout << "Jogo finalizado com sucesso!" << std::endl;
  }

} // namespace Core