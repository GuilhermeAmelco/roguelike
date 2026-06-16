#ifndef GAME_HPP
#define GAME_HPP

#include "graphics/Renderer.hpp"
#include "world/WorldManager.hpp"
#include "entities/Player.hpp"

namespace Core
{

  class Game
  {
  private:
    Graphics::Renderer *renderer;
    World::WorldManager *worldManager;
    Entities::Player *player;

    bool rodando;

    // Variáveis para controlar as teclas pressionadas
    bool tecla_esquerda = false;
    bool tecla_direita = false;
    bool tecla_cima = false;
    bool tecla_baixo = false;

    // Posição da Câmera no mundo (em pixels)
    float camera_x = 0.0f;
    float camera_y = 0.0f;

    void processar_eventos();
    void atualizar(float delta_time);
    void renderizar();

  public:
    Game();
    ~Game();

    void rodar();
  };

} // namespace Core

#endif // GAME_HPP