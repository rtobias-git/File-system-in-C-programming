//Estrutura para armazenar uma entrada de diretorio
typedef struct
{
    char nome[15];
    int num_inode;
} tp_entrada_diretorio;

//Nó da arvore de diretórios
typedef struct no
{
    tp_entrada_diretorio info;
    struct no *esq;
    struct no *dir;
}
tp_no;

//Estrutura na qual armazenara os dados de um inode
typedef struct
{
    int num;
    int qtd_blocos;
    int tamanho;
    char tipo;
    int blocos[10];
    int prox_livre;
} tp_inode;
