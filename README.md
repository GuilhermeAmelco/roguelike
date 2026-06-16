# 🎮 Roguelike (C++ Puro)

Este é um projeto de um jogo 2D desenvolvido em C++ com foco em alta performance, gerenciamento eficiente de memória e suporte a mapas gigantes utilizando o sistema de **Chunks**. (mesmo não precisando)

---

## ⚙️ Tecnologias Utilizadas

- C++23
- SDL3
- CMake
- MinGW-w64 (GCC)

---

## 📦 Pré-requisitos

Antes de compilar o projeto, instale:

### CMake

Verifique se está instalado:

```bash
cmake --version
```

Caso não esteja, baixe em:

https://cmake.org/download/

### GCC (MinGW-w64)

Verifique se está instalado:

```bash
g++ --version
```

Exemplo:

```text
g++.exe (MinGW-W64 x86_64-ucrt-posix-seh)
```

### Git

O CMake utiliza o Git para baixar automaticamente as dependências (SDL3).

Verifique:

```bash
git --version
```

---

## 🚀 Compilando o Projeto

Na raiz do projeto execute:

```bash
cmake -B build -G "MinGW Makefiles"
```

O comando acima:

- Cria a pasta `build/`
- Baixa automaticamente o SDL3
- Configura o projeto para o compilador MinGW

Depois compile:

```bash
cmake --build build
```

---

## ▶️ Executando

Após a compilação:

```bash
.\build\roguelike.exe
```

---

## 🔄 Recompilando

Após alterar qualquer arquivo `.cpp` ou `.hpp`, basta executar:

```bash
cmake --build build
```

O CMake recompilará apenas os arquivos modificados.

---

## 🧹 Limpar o Build

Caso ocorra algum problema de configuração:

### PowerShell

```powershell
Remove-Item build -Recurse -Force
```

### CMD

```cmd
rmdir /s /q build
```

Depois gere novamente:

```bash
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

---

## 📂 Estrutura de Pastas

```text
roguelike/
│
├── assets/                 # Recursos (imagens, sprites, sons)
│
├── include/                # Arquivos de cabeçalho (.hpp)
│   ├── core/
│   ├── graphics/
│   └── world/
│
├── src/                    # Arquivos de código fonte (.cpp)
│   ├── core/
│   ├── graphics/
│   └── world/
│
├── build/                  # Arquivos gerados pelo CMake
│
├── CMakeLists.txt          # Configuração do projeto
│
└── main.cpp                # Ponto de entrada do programa
```

## 🧠 Como Funciona a Arquitetura (Fluxo Orgânico)

```text
[main.cpp] ➔ [Game Loop] ➔ [WorldManager] ➔ [Renderer]
```

1. `main.cpp`: Apenas liga o motor, instanciando a classe principal `Game`.
2. `core/Game`: Controla o tempo e o teclado. Ele sabe a posição de pixels do jogador.
3. `world/WorldManager`: Recebe a posição do jogador. Se o jogador andar, ele calcula em qual Chunk o jogador está e carrega os blocos vizinhos na memória RAM, removendo os que ficaram para trás.
4. `graphics/Renderer`: Recebe as informações dos blocos ativos e desenha na tela apenas o que a câmera consegue enxergar.

## 🧱 O Sistema de Chunks (16 × 16)

Para suportar mapas de proporções massivas (ex: 100.000 × 100.000) sem estourar a memória RAM, o mundo é dividido em Chunks:

- Cada Chunk é uma matriz estática de 16 × 16 blocos.
- O WorldManager guarda os chunks ativos em um `unordered_map`.
- Apenas os chunks próximos ao jogador permanecem carregados.
- O consumo de memória permanece praticamente constante mesmo em mundos gigantes.
