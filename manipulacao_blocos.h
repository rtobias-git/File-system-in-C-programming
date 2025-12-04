//Função para determinar quantos caracteres iram conter o vetor do mapa de bits
int calcular_quantidade_caracteres()
{
    int quantidade_blocos, quantidade_caracteres;

    quantidade_blocos = calcular_quantidade_blocos();
    quantidade_caracteres = quantidade_blocos/8;
    if(quantidade_blocos%8!=0)
        quantidade_caracteres++;
    return quantidade_caracteres;
}

// 1 - ocupado
// 0 - livre
//Função vai indicar todos os blocos como livres
void inicializar_mapa_bits()
{
    FILE *arq;
    int i,quantidade_caracteres;
    unsigned char zero = 0;

    quantidade_caracteres = calcular_quantidade_caracteres();
    arq = fopen("fs/freespace.dat", "wb");
    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO (FREESPACE.DAT) - FUNCAO INICIALIZAR MAPA BITS\n");
    }
    else
    {
        for(i=0; i<quantidade_caracteres; i++){
            fwrite(&zero, sizeof(unsigned char), 1, arq);
        }
        fclose(arq);
    }
}

//Função para obter o mapa de bits
void obter_mapa_bits(unsigned char mapa[])
{
    int tam,i=0;
    FILE *arq;
    tam = calcular_quantidade_blocos();
    arq = fopen("fs/freespace.dat", "rb");
    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO (freespace.dat) - funcao obter_mapa_bits\n");
    }
    else
    {
        while(fread(&mapa[i], sizeof(unsigned char), 1, arq) == 1)
        {
            i++;
        }
        fclose(arq);
    }
}

//Função para liberar um bloco
void libera_bloco_mapa(int num_bloco)
{
    FILE *arq;
    int quantidade_blocos, posicao_caracter,  posicao_bit;
    unsigned char desejado, validar;

    quantidade_blocos = calcular_quantidade_blocos();
    if(num_bloco< quantidade_blocos && num_bloco>-1)
    {
        arq = fopen("fs/freespace.dat", "rb+");
        if(arq == NULL)
        {
            printf("\nERRO AO ABRIR O ARQUIVO (FREESPACE.DAT) - FUNCAO LIBERA BLOCO MAPA\n");
        }
        else
        {
            posicao_caracter = num_bloco/8;
            posicao_bit = (((num_bloco%8)-8) * -1) - 1;

            fseek(arq,posicao_caracter,SEEK_SET);
            fread(&validar, sizeof(unsigned char), 1, arq);

            desejado = validar ^ (1 << posicao_bit);

            if(validar& (1 << posicao_bit))
            {
                fseek(arq,posicao_caracter,SEEK_SET);
                fwrite(&desejado, sizeof(unsigned char), 1, arq);
            }
            else
            {
                printf("\nO BLOCO DESEJADO JA ESATVA LIBERADO");
            }

            fclose(arq);
        }
    }
    else
    {
        printf("\nBLOCO INVALIDO, DEVE ESTAR NO INTERVALO DE 0 A %d\n", quantidade_blocos);
    }
}

//Função para gravar um bloco como ocupado
void grava_bloco_ocupado_mapa(int num_bloco)
{
    FILE *arq;
    int quantidade_blocos, posicao_caracter,  posicao_bit;
    unsigned char desejado, validar;

    quantidade_blocos = calcular_quantidade_blocos();
    if(num_bloco< quantidade_blocos && num_bloco>-1)
    {
        arq = fopen("fs/freespace.dat", "rb+");
        if(arq == NULL)
        {
            printf("\nERRO AO ABRIR O ARQUIVO (FREESPACE.DAT) - FUNCAO GRAVA BLOCO OCUPADO MAPA\n");
        }
        else
        {
            posicao_caracter = num_bloco/8;
            posicao_bit = (((num_bloco%8)-8) * -1) - 1;

            fseek(arq,posicao_caracter,SEEK_SET);
            fread(&validar, sizeof(unsigned char), 1, arq);
            if((validar & (1 << posicao_bit)) == 0)
            {
                desejado = validar | (1 << posicao_bit);
                fseek(arq,posicao_caracter,SEEK_SET);
                fwrite(&desejado, sizeof(unsigned char), 1, arq);
            }
            else
            {
                printf("\nO BLOCO DESEJADO JA ESTAVA OCUPADO");
            }

            fclose(arq);
        }
    }
    else
    {
        printf("\nBLOCO INVALIDO, DEVE ESTAR NO INTERVALO DE 0 A %d\n", quantidade_blocos);
    }
}

//Função para alocar um bloco no mapa
int ocupa_bloco_mapa()
{
    int num,i,tam,j;
    tam = calcular_quantidade_blocos();
    unsigned char mapa[tam];

    i=0;
    num = -1;
    obter_mapa_bits(mapa);
    while(i<tam && num==-1)
    {
        j = 7;
        while(j>0 && (mapa[i] & (1 << j)) != 0)
        {
            j--;
        }
        if(j>0 && (mapa[i] & (1 << j)) == 0)
        {
            num = i*8 + ((j-8) * -1) - 1;
            grava_bloco_ocupado_mapa(num);
        }
        i++;
    }
    return num;
}

// Função que retorna todos os blocos livres
int calcular_quant_blocos_livres()
{
    int qtd = 0, i, j, tam;
    unsigned char *mapa;

    tam = calcular_quantidade_caracteres();
    mapa = (unsigned char*) malloc(sizeof(unsigned char) * tam);
    obter_mapa_bits(mapa);

    for (i = 0; i < tam; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if ((mapa[i] & (1 << j)) == 0)
                qtd++;
        }
    }
    free(mapa);
    return qtd;
}

//Função cria os blocos com base na capacidade do superblock
void criar_blocos()
{
    FILE *arq;
    int i;
    char caminho[30];
    char nomearq[10];

    for(i=0; i<calcular_quantidade_blocos(); i++)
    {
        strcpy(caminho,"fs/blocks/");
        sprintf(nomearq, "%d", i);
        strcat(caminho,nomearq);
        strcat(caminho,".dat");
        arq = fopen(caminho, "w");
        if(arq == NULL)
        {
            printf("\nERRO AO ABRIR O ARQUIVO(BLOCK.DAT) - FUNCAO CRIAR_BLOCOS\n");
        }

        fclose(arq);
    }
}

//Função responsavel de limpar o arquivo para ser gravado em cima
void reseta_bloco(int num_bloco)
{
    FILE *origem, *destino;
    char nomearq[10];
    char caminho[30];

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    origem = fopen(caminho, "rb");
    destino = fopen("auxiliar.dat", "wb");

    if(origem == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO RESETA BLOCO\n");

    }
    else if(destino == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(AUXILIAR.DAT) - FUNCAO RESETA BLOCO\n" );
    }
    else
    {
        fclose(origem);
        fclose(destino);

        remove(caminho);
        rename("auxiliar.dat", caminho);
    }
}

//Função grava a entrada de diretorio com base em um numero
void gravar_entrada_diretorio(int num, tp_entrada_diretorio entrada_diretorio)
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "ab");

    if(arq == NULL)
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO GRAVAR ENTRADA DIRETORIO\n");
    else
    {
        fwrite(&entrada_diretorio,sizeof(tp_entrada_diretorio), 1,arq);
        fclose(arq);
    }
}

tp_entrada_diretorio retornar_entrada_diretorio(int num_bloco, int num_inode_procurado)
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];
    tp_entrada_diretorio aux;
    int quantidade_entradas;


    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "rb");
    aux.num_inode = -1;

    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO RETORNAR ENTRADA DIRETORIO\n");
    }
    else
    {
        fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        while(!feof(arq) && aux.num_inode != num_inode_procurado)
        {
            fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        }
        if(aux.num_inode != num_inode_procurado)
            aux.num_inode = -1;

        fclose(arq);
    }
    return aux;
}

//Retornar a ultima entrada de diretorio do bloco
tp_entrada_diretorio retornar_ultima_entrada_diretorio(int num_bloco)
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];
    tp_entrada_diretorio aux,ant;
    int quantidade_entradas;

    ant.num_inode = -1;

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "rb");

    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO RETORNAR ENTRADA DIRETORIO\n");
    }
    else
    {
        while(fread(&aux, sizeof(tp_entrada_diretorio), 1, arq))
        {
            ant = aux;

        }
        fclose(arq);
    }
    return ant;
}

//Função para retornar a quantidade de entradas validas no diretorio
int retornar_quantidade_entrada_diretorio(int num_bloco)
{
    tp_entrada_diretorio entrada_diretorio;
    char nomearq[10];
    char caminho[30];
    int cont = 0;
    FILE *arq;


    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "rb");

    if(arq == NULL)
    {
        printf("\n ERRO AO ABRIR O ARQUIVO(BLOCO.DAT) - FUNCAO RETORNAR QUANTIDADE ENTRADA DIRETORIO\n");
    }
    else
    {

        fread(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, arq);
        while(!feof(arq))
        {
            cont++;
            fread(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, arq);
        }

        fclose(arq);
    }

    return cont;
}

//Função para excluir um arquivo
int excluir_entrada_diretorio(int num_bloco, int num_inode_procurado)
{
    FILE *origem, *destino;
    char nomearq[10];
    char caminho[30];
    tp_entrada_diretorio entrada_diretorio;
    int valida = -1;

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    origem = fopen(caminho, "rb");
    destino = fopen("auxiliar.dat", "wb");

    if(origem == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO EXCLUIR ENTRADA DIRETORIO\n");

    }
    else if(destino == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(AUXILIAR.DAT) - FUNCAO EXCLUIR ENTRADA DIRETORIO\n" );
    }
    else
    {
        while(fread(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, origem))
        {
            if(entrada_diretorio.num_inode == num_inode_procurado)
            {
                valida = 1;
            }
            else{
                fwrite(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, destino);
            }
        }

        fclose(origem);
        fclose(destino);

        remove(caminho);
        rename("auxiliar.dat", caminho);
    }
    return valida;
}

void apresentar_entrada(int num_bloco)
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];
    tp_entrada_diretorio aux;

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "rb");

    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO APRESENTAR ENTRADA\n");

    }
    else
    {
        fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        while(!feof(arq) )
        {
            printf("\n\n%d", aux.num_inode);
            printf("\n%s", aux.nome);
            fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);

        }
    }
}


//Função grava texto em um determinado bloco
void gravar_bloco(int num, char string[])
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "w");

    if(arq == NULL)
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO GRAVAR BLOCO\n");
    else
    {
        fputs(string, arq);

        fclose(arq);
    }
}

//Função apresenta determinado bloco em base do numero
void apresentar_bloco(int num)
{
    FILE *arq;
    int tam = retornar_blocksize();
    char nomearq[10], caminho[30], linha[tam];


    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "r");

    if(arq == NULL)
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO APRESENTAR BLOCO\n");
    else
    {
        while(fgets(linha, sizeof(linha), arq) != NULL)
            printf("%s", linha);

        fclose(arq);
    }
}
