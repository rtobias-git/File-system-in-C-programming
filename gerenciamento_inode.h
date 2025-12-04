
//Função para inicializar os inodes
void inicializar_inodes()
{
    FILE *arq;
    tp_inode inode;
    int i,quantidade_inode;
    quantidade_inode = calcular_quantidade_blocos()/2;
    arq = fopen("fs/inodes.dat", "wb");

    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO (INODE.DAT) - FUNCAO INICIALIZAR INODES\n");
    }
    else
    {
        for(i=0; i<quantidade_inode; i++)
        {
            inode.num = i;
            inode.tipo = 'u';
            if(i == quantidade_inode-1)
                inode.prox_livre = -1;
            else
                inode.prox_livre = i+1;
            fwrite(&inode, sizeof(tp_inode), 1, arq);
        }
        fclose(arq);
    }
}

//Função trazer um inode livre
tp_inode retornar_inode_livre()
{
    FILE *arq;
    int quantidade_inode, num_inode;
    tp_inode inode;
    arq = fopen("fs/inodes.dat", "rb");
    quantidade_inode = calcular_quantidade_blocos()/2;
    inode.tipo='-';
    num_inode = retornar_inode_livre_superblock();
    if(num_inode != -1)
    {
        if(arq == NULL)
        {
            printf("\nERRO AO ABRIR O ARQUIVO (INODES.DAT) - FUNCAO RETORNAR INODE LIVRE\n");
        }
        else
        {
            fseek(arq,num_inode*sizeof(tp_inode),SEEK_SET);
            fread(&inode, sizeof(tp_inode), 1, arq);
            fclose(arq);
        }
        gravar_inode_livre_superblock(inode.prox_livre);
    }
    return inode;
}

//Função para retornar um inode a partir de seu num
tp_inode retornar_inode(int num)
{
    FILE *arq;
    tp_inode inode;
    arq = fopen("fs/inodes.dat", "rb");

    inode.tipo='-';
    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO (INODES.DAT) - RETORNAR INODE\n");
    }
    else
    {
        fseek(arq,num*sizeof(tp_inode),SEEK_SET);
        fread(&inode, sizeof(tp_inode), 1, arq);
        fclose(arq);
    }
    return inode;
}

//Função para gravar um inode no arquivo
void atualiza_inode(tp_inode inode)
{
    FILE *arq;
    arq = fopen("fs/inodes.dat", "rb+");

    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO (INODES.DAT) - FUNCAO INSERIR INODE\n");
    }
    else
    {
        fseek(arq,inode.num*sizeof(tp_inode),SEEK_SET);
        fwrite(&inode, sizeof(tp_inode), 1, arq);
        fclose(arq);
    }
}

//Função que libera um inode
void libera_inode(tp_inode inode)
{
    FILE *arq;
    arq = fopen("fs/inodes.dat", "rb+");
    inode.prox_livre = retornar_inode_livre_superblock();
    inode.qtd_blocos=0;
    inode.tipo = 'u';
    inode.tamanho = 0;
    gravar_inode_livre_superblock(inode.num);
    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO (INODES.DAT) - FUNCAO INSERIR INODE\n");
    }
    else
    {
        fseek(arq,inode.num*sizeof(tp_inode),SEEK_SET);
        fwrite(&inode, sizeof(tp_inode), 1, arq);
        fclose(arq);
    }
}
