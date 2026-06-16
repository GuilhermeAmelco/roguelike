#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL3/SDL.h>
#include <string>

namespace Graphics
{

  class Renderer
  {
  private:
    SDL_Window *janela;
    SDL_Renderer *renderizador_sdl;
    int largura_tela;
    int altura_tela;

  public:
    Renderer(const std::string &titulo, int largura, int altura);
    ~Renderer(); // Destrutor para limpar a memória do SDL3 automaticamente

    // Limpa a tela com uma cor de fundo (R, G, B) antes de desenhar o novo frame
    void limpar_tela(uint8_t r, uint8_t g, uint8_t b);

    // Apresenta tudo o que foi desenhado na tela (Double Buffering)
    void apresentar();

    // Função para desenhar um quadrado preenchido (usaremos para o Jogador e Tiles temporários)
    void desenhar_retangulo(float x, float y, float largura, float altura, uint8_t r, uint8_t g, uint8_t b);

    // Getters úteis
    int obter_largura() const { return largura_tela; }
    int obter_altura() const { return altura_tela; }
    SDL_Renderer *obter_sdl_renderer() { return renderizador_sdl; }
  };

} // namespace Graphics

#endif // RENDERER_HPP