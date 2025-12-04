//Essa função grava os dados no superblock
void gravar_superblock()
{
    FILE *arq;


    arq = fopen("fs/superblock.dat", "w");
    if (arq != NULL)
    {
        fputs("filesystem=togabifs\n"
              "blocksize=128\n"
              "partitionsize=10240\n"
              "next=00", arq);

        fclose(arq);
    }
    else
    {
        printf("ERRO AO TENTAR ABRIR O ARQUIVO(SUPERBLOCK.DAT) - FUNCAO GRAVAR SUPERBLOCK\n");
    }
}

void gravar_inode_livre_superblock(int num_inode)
{
    FILE *arq;
    char linha[256], conversor[5];
    int pos;
    sprintf(conversor, "%d", num_inode);
    if(strlen(conversor)==1){
        conversor[1]=conversor[0];
        conversor[0]='0';
        conversor[2]='\0';
    }

    arq = fopen("fs/superblock.dat", "r+"); // abre para leitura e escrita
    if (arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO (SUPERBLOCK.DAT)\n");
    }
    else
    {
        fseek(arq, 0, SEEK_END);
        pos = ftell(arq)-2;
        fseek(arq, pos , SEEK_SET);
        fputs(conversor,arq);
        fclose(arq);
    }
}

//Essa função retorna o numero de inode no next
int retornar_inode_livre_superblock()
{
    FILE *arq;
    char linha[100];
    int num_inode = -1;

    arq = fopen("fs/superblock.dat", "r");
    if (arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO(SUPERBLOCK.DAT) - FUNCAO RETORNAR BLOCKSIZE\n");
    }
    else
    {
        while (fgets(linha, sizeof(linha), arq) != NULL)
        {
            if (strncmp(linha, "next=", 5) == 0)
            {
                sscanf(linha + 5, "%d", &num_inode);
            }
        }
        fclose(arq);
    }
    return num_inode;
}

//Essa funcao retorna o tamanho do bloco
int retornar_blocksize()
{
    FILE *arq;
    char linha[100];
    int blocksize = -1;

    arq = fopen("fs/superblock.dat", "r");
    if (arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO(SUPERBLOCK.DAT) - FUNCAO RETORNAR BLOCKSIZE\n");
    }
    else
    {
        while (fgets(linha, sizeof(linha), arq) != NULL)
        {
            if (strncmp(linha, "blocksize=", 10) == 0)
            {
                sscanf(linha + 10, "%d", &blocksize);
            }
        }
        fclose(arq);
    }
    return blocksize;
}

//Essa função retorna a quantidade da partição
int retornar_partition()
{
    FILE *arq;
    char linha[100];
    int partition = -1;

    arq = fopen("fs/superblock.dat", "r");

    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO(SUPERBLOCK.DAT) - FUNCAO RETORNAR PARTITION\n");
    }
    else
    {
        while (fgets(linha, sizeof(linha), arq) != NULL)
        {
            if (strncmp(linha, "partitionsize=", 14) == 0)
            {
                sscanf(linha + 14, "%d", &partition);
            }
        }

        fclose(arq);
    }

    return partition;
}

//funcao calcula a quantidade de blocos e retorna
int calcular_quantidade_blocos()
{
    int qtd_blocos;

    qtd_blocos = retornar_partition() / retornar_blocksize();

    return qtd_blocos;
}

