#ifndef PLAYER_HPP
#define PLAYER_HPP

namespace World
{
  class WorldManager;
}
namespace Entities
{
  class Player
  {
  private:
    // posicao do player
    float x;
    float y;

    // velocidade atual
    float velocidade_x;
    float velocidade_y;

    // velocidade limite
    float velocidade_movimento;

    // o tamanho do nosso quadrado do jogador na tela
    int tamanho;

    // Rastreamento de direção (0: Cima, 1: Baixo, 2: Esquerda, 3: Direita)
    int direcao_olhar;

    // Inventário do jogador
    int quantidade_chaves;
    bool possui_arma;

    World::WorldManager *worldManager;

  public:
    // Construtor inicializa o jogador em uma posição inicial do mundo
    Player(float posicao_inicial_x, float posicao_inicial_y, World::WorldManager *gerenciador_do_mundo)
    {
      x = posicao_inicial_x;
      y = posicao_inicial_y;
      velocidade_x = 0.0f;
      velocidade_y = 0.0f;
      velocidade_movimento = 200.0f; // 200 pixeis por segundo
      tamanho = 16;
      direcao_olhar = 1; // Começa olhando para baixo
      quantidade_chaves = 0;
      possui_arma = false;
      worldManager = gerenciador_do_mundo;
    }

    // 1. Processar inpit altera a velocidade com base nas teclas pressionadas
    void processar_input(bool esquerda, bool direita, bool cima, bool baixo)
    {
      if (esquerda)
        velocidade_x = -velocidade_movimento;
      else if (direita)
        velocidade_x = velocidade_movimento;
      else
        velocidade_x = 0.0f;

      if (cima)
        velocidade_y = -velocidade_movimento;
      else if (baixo)
        velocidade_y = velocidade_movimento;
      else
        velocidade_y = 0;
    }

    void interagir()
    {
      if (!worldManager)
        return;

      int t_size = worldManager->obter_tile_size();

      // 1. Definimos a caixa de alcance inicial idêntica ao corpo do jogador
      float alc_x = x;
      float alc_y = y;
      int alc_l = tamanho;
      int alc_a = tamanho;

      // Ajustamos o "braço" de alcance para se estender pela distância de mais de 1 bloco (ex: 1.5x TILE_SIZE)
      // E expandimos a largura perpendicular para garantir que cubra pelo menos 2 blocos vizinhos
      int alcance_frontal = static_cast<int>(t_size * 1.2f);
      int expansao_lateral = t_size; // Alarga a caixa para os lados para pegar os vizinhos

      switch (direcao_olhar)
      {
      case 0: // CIMA: Estica para cima e expande para os lados (Esquerda e Direita)
        alc_y -= alcance_frontal;
        alc_a = alcance_frontal;
        alc_x -= expansao_lateral / 2;
        alc_l += expansao_lateral;
        break;

      case 1: // BAIXO: Estica para baixo e expande para os lados (Esquerda e Direita)
        alc_y += tamanho;
        alc_a = alcance_frontal;
        alc_x -= expansao_lateral / 2;
        alc_l += expansao_lateral;
        break;

      case 2: // ESQUERDA: Estica para a esquerda e expande verticalmente (Cima e Baixo)
        alc_x -= alcance_frontal;
        alc_l = alcance_frontal;
        alc_y -= expansao_lateral / 2;
        alc_a += expansao_lateral;
        break;

      case 3: // DIREITA: Estica para a direita e expande verticalmente (Cima e Baixo)
        alc_x += tamanho;
        alc_l = alcance_frontal;
        alc_y -= expansao_lateral / 2;
        alc_a += expansao_lateral;
        break;
      }

      // 2. Transforma essa nova área robusta em coordenadas do Grid (Tiles)
      int coluna_inicial = static_cast<int>(alc_x) / t_size;
      int coluna_final = static_cast<int>(alc_x + alc_l - 1) / t_size;
      int linha_inicial = static_cast<int>(alc_y) / t_size;
      int linha_final = static_cast<int>(alc_y + alc_a - 1) / t_size;

      // 3. Varre todos os blocos dentro desse Range de 2+ blocos
      for (int l_global = linha_inicial; l_global <= linha_final; l_global++)
      {
        for (int c_global = coluna_inicial; c_global <= coluna_final; c_global++)
        {
          // Garantimos o teste exatamente no CENTRO de cada bloco daquela área
          float pixel_teste_x = (c_global * t_size) + (t_size / 2.0f);
          float pixel_teste_y = (l_global * t_size) + (t_size / 2.0f);

          int bloco_id = worldManager->obter_bloco_na_posicao(pixel_teste_x, pixel_teste_y);

          // Se encontrar o alvo interativo primeiro, ativa e encerra o loop
          if (bloco_id == 2 || bloco_id == 3 || bloco_id == 5)
          {
            switch (bloco_id)
            {
            case 2: // CHAVE
              quantidade_chaves++;
              worldManager->definir_bloco_na_posicao(pixel_teste_x, pixel_teste_y, 0);
              std::cout << "[INFO] Voce coletou uma chave! Total: " << quantidade_chaves << std::endl;
              return;

            case 3: // BAÚ
              worldManager->definir_bloco_na_posicao(pixel_teste_x, pixel_teste_y, 4);
              possui_arma = true;
              std::cout << "[INFO] Voce abriu o bau e encontrou uma Espada!" << std::endl;
              return;

            case 5: // PORTA
              if (quantidade_chaves > 0)
              {
                quantidade_chaves--;
                worldManager->definir_bloco_na_posicao(pixel_teste_x, pixel_teste_y, 0);
                std::cout << "[INFO] Porta aberta! Chaves restantes: " << quantidade_chaves << std::endl;
              }
              else
              {
                std::cout << "[AVISO] Esta porta esta trancada! Voce precisa de uma chave." << std::endl;
              }
              return;
            }
          }
        }
      }
    } // 2. Atualizar o jogador

    void atualizar(float delta_time)
    {
      if (!worldManager)
        return;
      // A posicao muda multiplicando a velocidade pelo tempo
      float proximo_x = x + velocidade_x * delta_time;
      if (!worldManager->existe_colisao_na_posicao(proximo_x, y, tamanho, tamanho))
      {
        x = proximo_x;
      }

      float proximo_y = y + velocidade_y * delta_time;
      if (!worldManager->existe_colisao_na_posicao(x, proximo_y, tamanho, tamanho))
      {
        y = proximo_y;
      }
    }

    // helpers para ajudar a debugar
    float obter_x() const { return x; }
    float obter_y() const { return y; }
    int obter_tamanho() const { return tamanho; }
  };
}

#endif