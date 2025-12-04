// Funcao cria o diretorio fs e blocks no windows e linux
void criar_diretorio()
{
#ifdef __linux__
    mkdir("fs", 0755);
    mkdir("fs/blocks", 0755);
#elif _WIN32
    _mkdir("fs");
    _mkdir("fs/blocks");
#endif
}

//Fun��o para criar o diretorio raiz
void criar_diretorio_raiz()
{
    int valida;
    tp_inode inode;
    tp_entrada_diretorio entrada_diretorio;

    inode = retornar_inode_livre();
    inode.blocos[0] = ocupa_bloco_mapa();
    if(inode.tipo == '-')
    {
        printf("\nNAO FOI POSSIVEL ALOCAR UM INODE CONSEQUENTEMENTE NAO FOI POSSIVEL CRIAR UM DIRETORIO");
    }
    else if(inode.blocos[0] == -1)
    {
        printf("\nNAO FOI POSSIVEL ALOCAR UM BLOCO CONSEQUENTEMENTE NAO FOI POSSIVEL CRIAR UM DIRETORIO");
    }
    else
    {
        inode.tipo = 'd';
        inode.qtd_blocos = 1;
        inode.tamanho = retornar_blocksize();

        strcpy(entrada_diretorio.nome, ".");
        entrada_diretorio.num_inode = inode.num;
        gravar_entrada_diretorio(inode.blocos[0], entrada_diretorio);

        strcpy(entrada_diretorio.nome, "..");
        entrada_diretorio.num_inode = inode.num;
        gravar_entrada_diretorio(inode.blocos[0], entrada_diretorio);
        atualiza_inode(inode);
    }
}

//Fun��o para inicializar todas os arquivos necessarios
void inicializar_dados()
{
    criar_diretorio();
    gravar_superblock();
    inicializar_mapa_bits();
    inicializar_inodes();
    criar_blocos();
    criar_diretorio_raiz();
}

//Fun��o para fazer a leitura da shell
void realizar_leitura(char string[], int tam)
{
    int i=-1;
    char c;
    do
    {
        i++;
        scanf("%c", &c);
        string[i] = c;
    }
    while(i < tam && c != '\n');
    string[i]='\0';
}

//Fun��o para adicionar no pai uma entrada de diretorio do filho
int gravar_entrada_diretorio_pai(int num_inode, tp_entrada_diretorio entrada_filho)
{
    tp_inode inode_pai;
    int i, quantidade_entrada, max,verifica=1,valida=1;
    i=0;
    inode_pai = retornar_inode(num_inode);
    max = retornar_blocksize()/sizeof(tp_entrada_diretorio);
    quantidade_entrada = retornar_quantidade_entrada_diretorio(inode_pai.blocos[i]);
    while(i < inode_pai.qtd_blocos && quantidade_entrada >= max)
    {
        i++;
        if(i<inode_pai.qtd_blocos)
            quantidade_entrada = retornar_quantidade_entrada_diretorio(inode_pai.blocos[i]);
    }
    if(i==10)
    {
        printf("\nNAO FOI POSSIVEL ARMAZENAR ESSE DIRETORIO JA QUE SEU PAI ESTA CHEIO\n");
        verifica=-1;
    }
    else
    {
        if(i == inode_pai.qtd_blocos)
        {
            inode_pai.blocos[i]=ocupa_bloco_mapa();
            if(inode_pai.blocos[i] == -1)
            {
                printf("\nNAO FOI POSSIVEL ALOCAR UM BLOCO CONSEQUENTEMENTE NAO FOI POSSIVEL CRIAR UM DIRETORIO");
                verifica = -1;
            }
            reseta_bloco(inode_pai.blocos[i]);
            inode_pai.qtd_blocos++;
            inode_pai.tamanho += retornar_blocksize();

        }
        if(verifica == 1)
            gravar_entrada_diretorio(inode_pai.blocos[i], entrada_filho);
    }
    atualiza_inode(inode_pai);
    return verifica;
}

//Fun��o que le as entradas de diretorio de um arquivo e coloca em uma arvore binaria
tp_no* ler_todas_entradas_inode(int num_inode, tp_no *raiz)
{
    tp_entrada_diretorio auxiliar;
    tp_inode inode;
    int bloco_desejado, i;
    char nomearq[15];
    char caminho[30];
    FILE *arq;

    inode = retornar_inode(num_inode);

    if (inode.qtd_blocos <= 0)
    {
        raiz = NULL;
    }
    else
    {
        for (i = 0; i < inode.qtd_blocos; i++)
        {
            bloco_desejado = inode.blocos[i];
            strcpy(caminho,"fs/blocks/");
            sprintf(nomearq, "%d", bloco_desejado);
            strcat(caminho, nomearq);
            strcat(caminho,".dat");
            arq = fopen(caminho, "rb");
            if (arq == NULL)
            {
                printf("\nERRO AO ABRIR ARQUIVO (%s) - funcao ler_todas_entradas_inode\n", nomearq);
            }
            else
            {
                while (fread(&auxiliar, sizeof(tp_entrada_diretorio), 1, arq) == 1)
                {
                    raiz = inserir_arvore(raiz,auxiliar);
                }
                fclose(arq);
            }
        }
    }
    return raiz;
}

//Fun��o que vai verificar se o nome esta ou nao no diretorio
int verificar_nome_diretorio(int num_dir, char *nome_arq)
{
    int valida;
    tp_no *raiz;

    valida = 1;
    inicializar_arvore(&raiz);
    raiz = ler_todas_entradas_inode(num_dir, raiz);
    if(pesquisa_entrada_diretorio(raiz,nome_arq) == NULL)
        valida = 0;
    return valida;
}

//Fun��o para criar um diretorio
void funcao_mkdir(int num_ant, char *nome_arq)
{
    int valida;
    tp_inode inode;
    tp_entrada_diretorio entrada_diretorio;
    if(strlen(nome_arq) < 15 && strlen(nome_arq)!=0)
    {
        valida = verificar_nome_diretorio(num_ant,nome_arq);
        if(valida == 1)
        {
            printf("\nJA EXISTE ARQUIVO COM ESSE NOME NO DIRETORIO, POR ISSO NAO FOI POSSSIVEL CRIAR ESSE DIRETORIO!!");
        }
        else
        {
            inode = retornar_inode_livre();

            inode.blocos[0] = ocupa_bloco_mapa();

            if(inode.tipo == '-')
            {
                printf("\nNAO FOI POSSIVEL ALOCAR UM INODE CONSEQUENTEMENTE NAO FOI POSSIVEL CRIAR UM DIRETORIO");
            }
            else if(inode.blocos[0] == -1)
            {
                printf("\nNAO FOI POSSIVEL ALOCAR UM BLOCO CONSEQUENTEMENTE NAO FOI POSSIVEL CRIAR UM DIRETORIO");
            }
            else
            {
                reseta_bloco(inode.blocos[0]);

                inode.tipo = 'd';
                inode.qtd_blocos = 1;

                inode.tamanho = retornar_blocksize();

                atualiza_inode(inode);

                strcpy(entrada_diretorio.nome, ".");
                entrada_diretorio.num_inode = inode.num;
                gravar_entrada_diretorio(inode.blocos[0], entrada_diretorio);

                strcpy(entrada_diretorio.nome, "..");
                entrada_diretorio.num_inode = num_ant;
                gravar_entrada_diretorio(inode.blocos[0], entrada_diretorio);

                strcpy(entrada_diretorio.nome, nome_arq);
                entrada_diretorio.num_inode = inode.num;
                valida = gravar_entrada_diretorio_pai(num_ant, entrada_diretorio);
                if(valida == -1)
                {
                    libera_bloco_mapa(inode.blocos[0]);
                    libera_inode(inode);
                }
            }
        }
    }
    else
        printf("O NOME DO ARQUIVO EXECEDE 14 CARACTERES OU ESTA VAZIO\n");
}

//Fun��o para organizar o texto digitado pelo usu�rio
void separar_prompt_usuario(char texto_digitado[], char comando[])
{
    int i = 0, j;
    comando[0]='\0';
    while( i < strlen(texto_digitado) && texto_digitado[i] != ' ' && texto_digitado[i] != '/')
    {
        comando[i] = texto_digitado[i];
        i++;
    }
    comando[i]='\0';
    if(texto_digitado[i] != '\0')
        i++;
    for(j=0; j<strlen(texto_digitado); j++)
    {
        texto_digitado[j] = texto_digitado[i];
        i++;
    }
    texto_digitado[j]='\0';
}

// Funcao touch cria um arquivo e grava o conteudo
void funcao_touch(int num_pai, char nome_arquivo[])
{
    tp_inode inode;
    tp_entrada_diretorio entrada_diretorio;
    int valida, verifica_pai = 1, i, j, blocksize, num_blocos_necessarios, inicio, fim, tamanho_parte, bloco,tam;
    int c, k, tamanho_total, c1, c2;
    blocksize = retornar_blocksize();
    tam = 10 * blocksize;
    char parte[blocksize + 1];
    char conteudo[tam];

    if (strlen(nome_arquivo) < 15 && strlen(nome_arquivo) != 0)
    {
        valida = verificar_nome_diretorio(num_pai, nome_arquivo);
        if (valida == 1)
        {
            printf("\nJA EXISTE ARQUIVO COM ESSE NOME NO DIRETORIO, POR ISSO NAO FOI POSSSIVEL CRIAR ESSE ARQUIVO!!");
        }
        else
        {
            inode = retornar_inode_livre();

            if (inode.tipo == '-')
            {
                printf("ERRO NAO HA INODE LIVRE DISPONIVEL.\n");
            }
            else
            {

                inode.tipo = 'f';
                inode.qtd_blocos = 0;
                inode.tamanho = 0;

                printf("DIGITE O CONTEUDO DO ARQUIVO: \n");
                i = 0;

                #ifdef __linux__
                    printf("USE CTRL+D NA PROXIMA LINHA PARA ENCERRAR \n");
                    while ((c = getchar()) != EOF && i < tam - 1)
                    {
                        if(c == 27){
                            if(i+3<tam-1){
                                conteudo[i++] = '^';
                                c = getchar();
                                conteudo[i++] = (char)c;
                                conteudo[i++] = (char)c;
                            }
                        }
                        else{
                            conteudo[i++] = (char)c;
                        }   
                    }
                    i--;
                    conteudo[i] = '\0';
                    clearerr(stdin);
                #elif _WIN32
                    printf("USE CTRL+D PARA ENCERRAR \n");
                    do
                    {
                        c = getchar();
                        conteudo[i] = c;
                        i++;
                    }
                    while (c != 4);
                    getchar();
                    i--;
                    conteudo[i] = '\0';
                #endif

                tamanho_total = strlen(conteudo);
                num_blocos_necessarios = (tamanho_total) / blocksize;
                if (tamanho_total % blocksize != 0)
                    num_blocos_necessarios++;

                if (num_blocos_necessarios > 10)
                {
                    printf("ERRO O ARQUIVO EXECEDE A QUANTIDADE DE 10 BLOCOS DO INODE\n");
                }
                else
                {
                    valida = 1;
                    i = 0;
                    while (i < num_blocos_necessarios && valida == 1)
                    {
                        inode.blocos[i] = ocupa_bloco_mapa();
                        if (inode.blocos[i] == -1)
                        {
                            printf("ERRO NAO A QUANTIDADE NECESSARIA DE BLOCOS DISPONIVEIS\n");
                            valida = -1;
                        }
                        i++;
                    }
                    if (valida == 1)
                    {
                        inode.qtd_blocos = num_blocos_necessarios;
                        for (i = 0; i < num_blocos_necessarios; i++)
                        {

                            inicio = i * blocksize;
                            fim = inicio + blocksize;
                            if (fim > tamanho_total)
                                fim = tamanho_total;

                            tamanho_parte = fim - inicio;

                            for (j = 0; j < tamanho_parte; j++)
                            {
                                parte[j] = conteudo[inicio + j];
                            }
                            parte[tamanho_parte] = '\0';

                            gravar_bloco(inode.blocos[i], parte);
                        }

                        inode.tamanho = tamanho_total;
                        atualiza_inode(inode);

                        strcpy(entrada_diretorio.nome, nome_arquivo);
                        entrada_diretorio.num_inode = inode.num;

                        verifica_pai = gravar_entrada_diretorio_pai(num_pai, entrada_diretorio);
                    }
                    if (valida == -1 || verifica_pai == -1)
                    {
                        j = 0;
                        while (j < i)
                        {
                            libera_bloco_mapa(inode.blocos[i]);
                        }
                        libera_inode(inode);
                        if (verifica_pai == -1)
                            printf("ERRO NO DIRETORIO PAI ESTA CHEIO \n");
                    }
                }
            }
        }
    }
    else
        printf("O NOME DO ARQUIVO EXECEDE 14 CARACTERES OU ESTA VAZIO\n");
}

//Fun��o para separar as instru��es do cd, se retornar -1 significa que tem alguem diretorio
int separar_comando_cd(char texto[], char matriz_dicionarios[][15])
{
    int tl, i, j, tamanho_nome;
    tl = 0;
    i = 0;
    if(texto[0] == '/')
        i++;
    while(i < strlen(texto) && tl>-1)
    {
        j=0;
        while(i<strlen(texto) && texto[i] != '/')
        {
            matriz_dicionarios[tl][j] = texto[i];
            j++;
            i++;
        }
        matriz_dicionarios[tl][j] = '\0';
        if(j<=15)
            tl++;
        else
            tl=-1;
        i++;
    }
    return tl;
}

//Fun��o cd
int funcao_cd(int num_inode, char caminho[], char comando[])
{
    char matriz_diretorios[15][15];
    int tl, i, verifica, auxiliar;
    tp_no *raiz;
    tp_inode inode;

    inicializar_arvore(&raiz);
    if(strcmp(comando, "..")==0)
    {
        raiz = ler_todas_entradas_inode(num_inode, raiz);
        raiz = pesquisa_entrada_diretorio(raiz,"..");
        num_inode = raiz->info.num_inode;
        if(strcmp(caminho, "/")!=0)
        {
            i = strlen(caminho);
            while(i>0 && caminho[i]!='/')
            {
                i--;
            }
            caminho[i] = '\0';
        }
        if(strcmp(caminho,"")==0)
            strcpy(caminho,"/");
    }
    else if(strcmp(comando, "/") == 0)
    {
        num_inode = 0;
        strcpy(caminho,"/");
    }
    else if(strcmp(comando, ".")!=0)
    {
        tl = separar_comando_cd(comando, matriz_diretorios);
        if(tl == -1)
        {
            printf("\nNAO FOI POSSIVEL REALIZAR O COMANDO CD JA QUE CONTEM UM NOME DE DIRETORIO QUE EXCEDE O TAMANHO\n");
        }
        else
        {

            if(comando[0] == '/')
            {
                num_inode = 0;
            }
            i=0;
            do
            {
                verifica = 0;
                raiz = ler_todas_entradas_inode(num_inode, raiz);
                raiz = pesquisa_entrada_diretorio(raiz,matriz_diretorios[i]);
                if(raiz == NULL)
                {
                    printf("NAO FOI POSSIVEL REALIZAR O COMANDO CD, JA QUE NAO FOI ENCONTRADO O DIRETORIO: %s\n", matriz_diretorios[i]);
                }
                else
                {
                    inode = retornar_inode(raiz->info.num_inode);
                    if(inode.tipo != 'd')
                    {
                        printf("NAO FOI POSSIVEL REALIZAR O COMANDO CD, JA QUE %s NAO E UM DIRETORIO\n", matriz_diretorios[i]);
                    }
                    else
                    {
                        num_inode = raiz->info.num_inode;
                        verifica = 1;
                        if(comando[0]=='/')
                        {
                            strcpy(caminho, comando);
                        }
                        else
                        {
                            if(caminho[strlen(caminho)-1]!='/')
                            {
                                strcat(caminho, "/");
                            }
                            strcat(caminho, comando);
                        }
                    }
                }
                i++;
            }
            while(verifica == 1 && i<tl);
        }
    }
    return num_inode;
}



//Listar todos os arquivos do diret�rio atual
void ls(int num_diretorio)
{

    tp_no *raiz;

    inicializar_arvore(&raiz);

    raiz = ler_todas_entradas_inode(num_diretorio, raiz);

    if (raiz == NULL)
    {
        printf("DIRETORIO VAZIO\n");
    }
    else
    {
        listar_em_ordem(raiz);
    }
}

// Busca em �rvore bin�ria pelo nome do diret�rio com o inode
int buscar_nome_por_inode(tp_no *raiz, int num_inode, char *nome_encontrado)
{
    int valida=0;
    if (raiz != NULL)
    {
        if (raiz->info.num_inode == num_inode && strcmp(raiz->info.nome, ".") != 0 && strcmp(raiz->info.nome, "..") != 0)
        {
            strcpy(nome_encontrado, raiz->info.nome);
            valida = 1;
        }

        if(valida ==0)
        {
            if (buscar_nome_por_inode(raiz->esq, num_inode, nome_encontrado))
                valida = 1;
            if (buscar_nome_por_inode(raiz->dir, num_inode, nome_encontrado))
                valida = 1;
        }
    }
    return valida;
}

//Fun�ao pwd mostra em que diretorio esta
void funcao_pwd(int num_inode_atual)
{
    tp_inode inode_atual, inode_pai;
    tp_no *raiz, *raiz_pai, *atual, *pai;
    char caminho[200], aux[200];
    char nome[50];
    int num_pai, achou;

    inicializar_arvore(&raiz);

    caminho[0] = '\0';
    inode_atual = retornar_inode(num_inode_atual);
    raiz = ler_todas_entradas_inode(inode_atual.num, NULL);
    atual = pesquisa_entrada_diretorio(raiz, ".");
    pai   = pesquisa_entrada_diretorio(raiz, "..");

    while (atual != NULL && pai != NULL && atual->info.num_inode != pai->info.num_inode)
    {
        num_pai = pai->info.num_inode;
        inode_pai = retornar_inode(num_pai);
        raiz_pai = ler_todas_entradas_inode(num_pai, NULL);

        nome[0] = '\0';
        achou = buscar_nome_por_inode(raiz_pai, num_inode_atual, nome);

        aux[0] = '\0';
        if (achou && strlen(nome) > 0)
        {
            strcat(aux, "/");
            strcat(aux, nome);
        }
        strcat(aux, caminho);
        strcpy(caminho, aux);
        num_inode_atual = num_pai;
        inode_atual = inode_pai;
        raiz = raiz_pai;
        atual = pesquisa_entrada_diretorio(raiz, ".");
        pai   = pesquisa_entrada_diretorio(raiz, "..");
    }

    if (strlen(caminho) == 0)
        strcpy(caminho, "/");

    printf("%s\n", caminho);
}

//Apresentar o conte�do de um arquivo (no diret�rio atual)
void funcao_cat(int num_diretorio_atual, char nome_arquivo[])
{
    tp_no *raiz;
    tp_no *no_arquivo;
    tp_inode inode;
    int i, bloco;
    char linha[128];
    char nomearq[15];

    inicializar_arvore(&raiz);

    raiz = ler_todas_entradas_inode(num_diretorio_atual, raiz);

    no_arquivo = pesquisa_entrada_diretorio(raiz, nome_arquivo);

    if(no_arquivo == NULL)
    {
        printf("ARQUIVO NAO ENCONTRADO NO DIRETORIO ATUAL\n");
    }
    else
    {

        inode = retornar_inode(no_arquivo->info.num_inode);

        if(inode.tipo != 'f')
        {
            printf("ESSE NOME NAO E DE UM ARQUIVO\n");
        }
        else
        {

            for(i = 0; i < inode.qtd_blocos; i++)
            {
                apresentar_bloco(inode.blocos[i]);
            }
        }
    }

    printf("\n");
}



//Fun��o para apagar um arquivo
void apagar_arquivo(int num_inode)
{
    int i;
    tp_inode inode;

    inode = retornar_inode(num_inode);
    for(i = 0; i<inode.qtd_blocos; i++)
    {
        libera_bloco_mapa(inode.blocos[i]);
    }
    libera_inode(inode);
}

//Fun��o para apagar um diret�rio
void apagar_diretorio(int num_inode)
{
    tp_no *raiz;
    tp_inode inode;
    tp_entrada_diretorio *vetor_entradas;
    int i=0, tl;

    inicializar_arvore(&raiz);

    raiz = ler_todas_entradas_inode(num_inode,raiz);
    tl = contar_nos(raiz);
    vetor_entradas = (tp_entrada_diretorio*) malloc(sizeof(tp_entrada_diretorio)*tl);
    arvore_para_vetor(raiz,vetor_entradas,&i);
    for(i=0; i<tl; i++)
    {

        if(strcmp(vetor_entradas[i].nome,".")!=0 && strcmp(vetor_entradas[i].nome,"..")!=0)
        {
            inode = retornar_inode(vetor_entradas[i].num_inode);
            if(inode.tipo == 'd')
            {
                apagar_diretorio(inode.num);
            }
            else
            {
                apagar_arquivo(inode.num);
            }
        }
    }
    free(vetor_entradas);
    inode = retornar_inode(num_inode);
    for(i=0; i<inode.qtd_blocos; i++)
    {
        libera_bloco_mapa(inode.blocos[i]);
    }
    libera_inode(inode);
}

//Fun��o para remover corretamente a entrada de diretorio do arquivo
void remover_entrada_do_diretorio(int num_inode_atual, int num_inode_apagar)
{
    tp_no *raiz;
    tp_inode inode;
    tp_entrada_diretorio aux;
    int i,valida=-1;

    inode = retornar_inode(num_inode_atual);

    i=0;
    while(i<inode.qtd_blocos && valida == -1)
    {
        valida = excluir_entrada_diretorio(inode.blocos[i], num_inode_apagar);
        if(valida==-1)
            i++;
    }
    if(i != inode.qtd_blocos-1)
    {
        if(retornar_quantidade_entrada_diretorio(inode.blocos[inode.qtd_blocos-1])>0)
        {
            aux = retornar_ultima_entrada_diretorio(inode.blocos[inode.qtd_blocos-1]);
            gravar_entrada_diretorio(inode.blocos[i],aux);
            excluir_entrada_diretorio(inode.blocos[inode.qtd_blocos-1], aux.num_inode);
        }

    }
    if(retornar_quantidade_entrada_diretorio(inode.blocos[inode.qtd_blocos-1])==0)
    {
        libera_bloco_mapa(inode.blocos[inode.qtd_blocos-1]);
        inode.qtd_blocos-=1;
        inode.tamanho -= retornar_blocksize();
        atualiza_inode(inode);
    }
}

//Fun��o para excluir um arquivo ou diret�rio do atual
void funcao_rm(int inode_pai, char nome_arq[])
{
    tp_no *raiz;
    tp_no *desejado;
    tp_inode inode;

    inicializar_arvore(&raiz);
    raiz = ler_todas_entradas_inode(inode_pai,raiz);
    desejado = pesquisa_entrada_diretorio(raiz,nome_arq);
    if(desejado != NULL)
    {
        if(strcmp(nome_arq,".")!=0 && strcmp(nome_arq,"..")!=0)
        {
            inode = retornar_inode(desejado->info.num_inode);
            if(inode.tipo == 'f')
            {
                apagar_arquivo(inode.num);
            }
            else
            {
                apagar_diretorio(inode.num);
            }
            remover_entrada_do_diretorio(inode_pai, desejado->info.num_inode);
        }
        else
            printf("\nNAO SE PODE EXCLUIR AS ENTRADAS '.' E '..'\n");
    }
    else
    {
        printf("\nESSE ARQUIVO OU DIRETORIO NAO ESTA PRESENTE NO DIRETORIO ATUAL\n");
    }
}

//Fun��o stat que vai apresentar a situa��o do sistema de arquivo
void funcao_stat()
{
    int qtd_livres_bytes,qtd_blocos_livres,tam_bloco;

    qtd_blocos_livres = calcular_quant_blocos_livres();
    tam_bloco = retornar_blocksize();

    qtd_livres_bytes = qtd_blocos_livres * tam_bloco;

    printf("\nESPACO LIVRE: %d BYTES", qtd_livres_bytes);
    printf("\nBLOCOS LIVRES: %d BLOCOS", qtd_blocos_livres);
    printf("\nTAMANHO DO BLOCO: %d BYTES", tam_bloco);
}

