# 🎮 Roguelike (C++ Puro)

Este é um projeto de um jogo 2D desenvolvido em C++ com foco em alta performance, gerenciamento eficiente de memória e suporte a mapas gigantes utilizando o sistema de **Chunks**. (mesmo não precisando)

---

## 📂 Estrutura de Pastas

```text
roguelike/
│
├── bin/                    # Executáveis gerados (.exe)
├── assets/                 # Recursos (imagens, sprites, sons)
│
├── include/                # Arquivos de cabeçalho (.hpp)
│   ├── core/               # Motor básico (Loop do jogo, Janela, Input)
│   ├── graphics/           # Sistema visual (Renderer, Texturas)
│   └── world/              # Lógica do mundo (Tile, Chunk, WorldManager)
│
├── src/                    # Arquivos de código fonte (.cpp)
│   ├── core/
│   ├── graphics/
│   └── world/
│
└── main.cpp                # Ponto de entrada do programa
```

## 🧠 Como Funciona a Arquitetura (Fluxo Orgânico)

```text
[main.cpp] ➔ [Game Loop] ➔ [WorldManager] ➔ [Renderer]
```

1. `main.cpp`: Apenas liga o motor, instanciando a classe principal Game.
2. `core/Game`: Controla o tempo e o teclado. Ele sabe a posição de pixels do Jogador.
3. `world/WorldManager`: Recebe a posição do jogador. Se o jogador andar, ele calcula em qual quadrante (Chunk) o jogador está e carrega os blocos vizinhos na memória RAM, deletando os que ficaram para trás.
4. `graphics/Renderer`: Recebe as informações dos blocos ativos e desenha na tela apenas o que a câmera consegue enxergar.

## 🧱 O Sistema de Chunks ($16 \times 16$)

Para suportar mapas de proporções massivas (ex: $100.000 \times 100.000$ ) sem estourar a memória RAM, o mundo é fatiado em Chunks:

- Cada Chunk é uma matriz estática de $16 \times 16$ blocos.
- O WorldManager guarda os chunks ativos em um dicionário dinâmico (unordered_map).
- Resultado: O jogo consome apenas a memória dos 9 chunks ao redor do jogador, mantendo o desempenho leve e constante em qualquer computador.
