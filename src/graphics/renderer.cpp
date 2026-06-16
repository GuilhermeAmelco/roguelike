#include "graphics/Renderer.hpp"
#include <iostream>

namespace Graphics
{

  Renderer::Renderer(const std::string &titulo, int largura, int altura)
      : largura_tela(largura), altura_tela(altura), janela(nullptr), renderizador_sdl(nullptr)
  {
    std::cout << "1\n";
    // 1. Inicializa o subsistema de vídeo do SDL3
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
      std::cerr << "Erro ao inicializar o SDL3: " << SDL_GetError() << std::endl;
      return;
    }

    // 2. Cria a janela usando a nova função do SDL3
    janela = SDL_CreateWindow(titulo.c_str(), largura, altura, 0);
    if (!janela)
    {
      std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return;
    }

    // 3. Cria o renderizador (o SDL3 escolhe a melhor GPU disponível por padrão)
    renderizador_sdl = SDL_CreateRenderer(janela, nullptr);
    if (!renderizador_sdl)
    {
      std::cerr << "Erro ao criar renderizador: " << SDL_GetError() << std::endl;
      SDL_DestroyWindow(janela);
      SDL_Quit();
      return;
    }
  }

  Renderer::~Renderer()
  {
    // Limpeza reversa obrigatória em C++ para evitar vazamento de memória
    if (renderizador_sdl)
      SDL_DestroyRenderer(renderizador_sdl);
    if (janela)
      SDL_DestroyWindow(janela);
    SDL_Quit();
  }

  void Renderer::limpar_tela(uint8_t r, uint8_t g, uint8_t b)
  {
    // Define a cor de fundo (R, G, B, Alpha=255)
    SDL_SetRenderDrawColor(renderizador_sdl, r, g, b, 255);
    // Limpa o buffer de tela com essa cor
    SDL_RenderClear(renderizador_sdl);
  }

  void Renderer::desenhar_retangulo(float x, float y, float largura, float altura, uint8_t r, uint8_t g, uint8_t b)
  {
    // Define a cor do desenho atual
    SDL_SetRenderDrawColor(renderizador_sdl, r, g, b, 255);

    // No SDL3 usamos SDL_FRect (F de Float) para precisão de pixels sub-pixel
    SDL_FRect retangulo = {x, y, largura, altura};

    // Desenha o retângulo preenchido na tela
    SDL_RenderFillRect(renderizador_sdl, &retangulo);
  }

  void Renderer::apresentar()
  {
    // Joga o desenho dos bastidores para a tela do usuário
    SDL_RenderPresent(renderizador_sdl);
  }

} // namespace Graphics