#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#ifdef __linux__
#include <sys/stat.h>
#elif _WIN32
#include <direct.h>
#endif
#include <locale.h>
#include "estruturas.h"
#include "gerir_superbloco.h"
#include "gerenciamento_inode.h"
#include "estrutura_dados_diretorios.h"
#include "manipulacao_blocos.h"
#include "funcoes.h"

void main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int num_inode;
    char texto[256],comando[256],caminho[256];
    inicializar_dados();

    strcpy(caminho,"/");
    num_inode = 0;
    comando[0]='\0';

    while(strcmp(comando,"exit")!=0)
    {
        printf("\n%s$ ", caminho);
        realizar_leitura(texto,256);
        separar_prompt_usuario(texto,comando);
        if(strcmp(comando,"mkdir")==0)
        {
            funcao_mkdir(num_inode, texto);
        }
        else if(strcmp(comando,"cd")==0)
        {
            num_inode = funcao_cd(num_inode, caminho, texto);
        }
        else if(strcmp(comando,"touch")==0)
        {
            funcao_touch(num_inode,texto);
        }
        else if(strcmp(comando,"cat")==0)
        {
            funcao_cat(num_inode,texto);
        }
        else if(strcmp(comando,"rm")==0)
        {
            funcao_rm(num_inode,texto);
        }
        else if(strlen(texto)==0)
        {
            if(strcmp(comando,"ls")==0)
            {
                ls(num_inode);
            }
            else if(strcmp(comando,"pwd")==0)
            {
                funcao_pwd(num_inode);
            }
            else if(strcmp(comando,"stat")==0)
            {
                funcao_stat();
            }
            else if(strcmp(comando,"exit")==0)
            {
                printf("\nATE BREVE!!");
            }
            else
            {
                printf("\nCOMANDO INVALIDO!!");
            }
        }
        else
        {
            printf("\nCOMANDO INVALIDO!!");
        }
    }
}

