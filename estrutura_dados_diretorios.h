//Função para reservar espaco em memoria
tp_no* getnode()
{
    return (tp_no*) malloc(sizeof(tp_no));
}

//Função para inicializar a arvore
void inicializar_arvore(tp_no **raiz)
{
    *raiz = NULL;
}

// Função recursiva para contar os nós da árvore
int contar_nos(tp_no *raiz)
{
    if (raiz == NULL)
        return 0;
    else
        return 1 + contar_nos(raiz->esq) + contar_nos(raiz->dir);
}

//Função para inserir um elemento na arvore
tp_no* inserir_arvore(tp_no *raiz, tp_entrada_diretorio elemento)
{
    if(raiz == NULL)
    {
        tp_no *q;
        q=getnode();
        strcpy(q->info.nome, elemento.nome);
        q->info.num_inode = elemento.num_inode;
        q->esq = NULL;
        q->dir = NULL;
        return q;
    }
    else if(strcmp(raiz->info.nome, elemento.nome)>=0)
    {
        raiz->esq = inserir_arvore(raiz->esq,elemento);
    }
    else
    {
        raiz->dir = inserir_arvore(raiz->dir,elemento);
    }
    return raiz;
}


//Função para encontrar um elemento na arvore
tp_no* pesquisa_entrada_diretorio(tp_no *raiz, char nome_arquivo[])
{
    if(raiz==NULL)
    {
        return NULL;
    }
    else if(strcmp(raiz->info.nome,nome_arquivo)<0)
    {
        return pesquisa_entrada_diretorio(raiz->dir, nome_arquivo);
    }
    else if(strcmp(raiz->info.nome,nome_arquivo)>0)
    {
        return pesquisa_entrada_diretorio(raiz->esq, nome_arquivo);
    }
    else
    {
        return raiz;
    }
}

// Função que imprime uma árvore de entradas em ordem
void listar_em_ordem(tp_no *raiz)
{
    tp_inode inode;

    if (raiz != NULL)
    {
        listar_em_ordem(raiz->esq);

        inode = retornar_inode(raiz->info.num_inode);

        printf("%c - %-5d %-20s %5d\n", inode.tipo, raiz->info.num_inode, raiz->info.nome, inode.tamanho);

        listar_em_ordem(raiz->dir);
    }
}

//Função que converte a arvore para vetor
void arvore_para_vetor(tp_no *raiz, tp_entrada_diretorio vetor[], int *indice)
{
    if (raiz != NULL)
    {
        arvore_para_vetor(raiz->esq, vetor, indice);
        vetor[*indice] = raiz->info;
        (*indice)++;
        arvore_para_vetor(raiz->dir, vetor, indice);
    }
}
