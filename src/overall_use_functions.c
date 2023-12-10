#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>


#include "libdir.h"
#include "overall_use_functions.h"

#define BUFFER_SIZE 256

/*
 * Abre um arquivo para realizar a leitura de dados.
 */
FILE* create_reading_file(const char* filename){
    FILE* arq = NULL; /*Arquivo é criado como nulo antes de receber o argv da função main*/
    arq = fopen(filename, "r"); 
    if (!arq){ /*Verificação em caso de erro na abertura do arquivo*/
        printf("Erro ao criar o arquivo %s de leitura\n", filename); /*Mensagem de erro*/
        return NULL;
    }
    return arq;
}

/*
 * Abre um arquivo para realizar a escrita de dados.
 */
FILE* create_writing_file(const char* filename){
    FILE* arq = NULL; /*Arquivo é criado como nulo antes de receber o argv da função main*/
    arq = fopen(filename, "w"); 
    if (!arq){ /*Verificação em caso de erro na abertura do arquivo*/
        printf("Erro ao criar o arquivo %s de escrita\n", filename); /*Mensagem de erro*/
        return NULL;
    }
    return arq;
}

/*
 * Abre um arquivo ja existente.
 */
FILE* create_existing_file(const char* filename){
    FILE* arq = NULL; /*Arquivo é criado como nulo antes de receber o argv da função main*/
    arq = fopen(filename, "r+");

    if (!arq){ /*Verificação em caso de erro na abertura do arquivo*/
        printf("O arquivo %s não pode ser aberto\n", filename); /*Mensagem de erro*/
        return NULL;
    } 
   
    return arq;
}
