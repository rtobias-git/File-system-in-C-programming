# Sistema de Arquivos togabifs em C

![C](https://img.shields.io/badge/Linguagem-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Sistemas Operacionais](https://img.shields.io/badge/Disciplina-Sistemas%20Operacionais%202-blueviolet?style=for-the-badge)
![Multiplataforma](https://img.shields.io/badge/SO-Linux%20%7C%20Windows-lightgrey?style=for-the-badge)

Implementação de um simulador de sistema de arquivos com persistência em disco e um shell interativo em linguagem C para a disciplina de Sistemas Operacionais 2 no IFSP (Campus Presidente Epitácio).

A explicação detalhada das estruturas de dados adotadas está disponível no documento [Explicação da estrutura.pdf](Explicação%20da%20estrutura.pdf).

---

## Execução no terminal

Demonstração dos comandos no terminal:

![Execução do Sistema de Arquivos togabifs no Terminal](images/execucao-terminal.png)

---

## Estruturas de dados e funcionamento

Os dados do sistema de arquivos ficam salvos no diretório `fs/` nos arquivos `superblock.dat`, `inodes.dat`, `freespace.dat` e nos blocos em `fs/blocks/*.dat`.

- **Superbloco (`gerir_superbloco.h` e `fs/superblock.dat`):** guarda as informações gerais do sistema, como nome (`togabifs`), tamanho dos blocos (128 bytes), tamanho da partição (80 blocos) e o próximo inode livre.
- **Diretórios em árvore binária (`estrutura_dados_diretorios.h`):** as entradas de diretório são organizadas em uma árvore binária de busca em ordem alfabética para agilizar a localização de arquivos e pastas.
- **Gerenciamento de inodes (`gerenciamento_inode.h`):** mantém uma lista encadeada dos inodes disponíveis para alocação rápida. Cada inode tem capacidade para 10 blocos diretos.
- **Mapa de bits de blocos livres (`manipulacao_blocos.h` e `fs/freespace.dat`):** bitmap que marca com 0 os blocos livres e com 1 os blocos ocupados.

---

## Comandos do shell

| Comando | Descrição | Exemplo |
| :--- | :--- | :--- |
| `mkdir <nome>` | Cria um novo diretório no diretório atual | `mkdir projetos` |
| `cd <caminho>` | Altera o diretório atual (suporta `/`, `..` e caminhos) | `cd projetos` |
| `pwd` | Exibe o caminho do diretório atual | `pwd` |
| `touch <arquivo>` | Cria um arquivo e lê seu conteúdo até `CTRL+D` | `touch readme.txt` |
| `cat <arquivo>` | Exibe o conteúdo armazenado em um arquivo | `cat readme.txt` |
| `ls` | Lista entradas do diretório com tipo (`d`/`f`), inode e tamanho | `ls` |
| `rm <nome>` | Remove um arquivo ou diretório do diretório atual | `rm readme.txt` |
| `stat` | Exibe espaço livre em bytes, blocos livres e tamanho do bloco | `stat` |
| `exit` | Encerra o interpretador de comandos | `exit` |

---

## Estrutura de arquivos

```text
File-system-in-C-programming/
├── principal.c                     # Loop principal do shell e despacho de comandos
├── estruturas.h                    # Definições de tp_entrada_diretorio, tp_no e tp_inode
├── gerir_superbloco.h              # Leitura e atualização de fs/superblock.dat
├── gerenciamento_inode.h           # Alocação, liberação e persistência de inodes
├── estrutura_dados_diretorios.h    # Árvore binária de entradas de diretório
├── manipulacao_blocos.h            # Mapa de bits e leitura/escrita em fs/blocks/
├── funcoes.h                       # Implementação dos comandos do sistema de arquivos
├── Explicação da estrutura.pdf     # Documento técnico detalhando as estruturas adotadas
└── images/
    └── execucao-terminal.png       # Captura da execução no terminal
```

---

## Como compilar e executar

O código roda tanto em Linux quanto em Windows (`#ifdef __linux__` / `#elif _WIN32`).

```bash
# Clonar o repositório
git clone https://github.com/rtobias-git/File-system-in-C-programming.git
cd File-system-in-C-programming

# Compilar com GCC
gcc principal.c -o togabifs

# Executar (no Linux use ./togabifs; no Windows use togabifs.exe)
./togabifs
```

---

## Autores

- Tobias Rafael da Silva Rosa ([@rtobias-git](https://github.com/rtobias-git))
- Gabriel Pontel de Mori
