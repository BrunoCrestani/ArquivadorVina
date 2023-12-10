#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "libdir.h"
#include "overall_use_functions.h"
#include "insert.h"

#define BUFFER_SIZE 1024
#define NAME_SIZE 256
#define PATH_SIZE 4096

/*
 *  Realiza uma leitura bufferizada dos dados do membro
 * e os insere no arquivador, até que seja encontrado o 
 * final do membro.
 */

void data_insertion(FILE* file, member_t* member){

    char buffer[BUFFER_SIZE]; //Buffer utilizado para transferir os dados
    long rest;
    FILE * insertion_file; //Arquivo do membro.

    insertion_file = create_reading_file(member->path); //Abre o arquivo domembro para leitura de dados. 
    
    rest = member->size % BUFFER_SIZE;

    bytes_insertion(insertion_file, file, buffer, rest);

    fclose(insertion_file);
}

/*
 * Realiza o processo de leitura e escrita de bytes com o uso de um buffer.
 */
int bytes_insertion(FILE* insertion_file, FILE* file, char*buffer, long rest){

    while (fread(buffer, 1, BUFFER_SIZE, insertion_file) == BUFFER_SIZE){
        fwrite(buffer, 1, BUFFER_SIZE, file);
    }

    if (feof(insertion_file)){
        fwrite(buffer, 1, rest, file);
    } else {
        printf("\nErro ao ler o arquivo.\n");
        exit(1);
    }
    return 0;
}

/*
 * Insere o diretorio criado no arquivador.
 */
void diretory_insertion(FILE* file, diretorio_t* dir){
    
    unsigned int data_end = data_size(dir);
    member_t* aux;
    aux = dir->first;
    char vina_ticket[7] = "vina++";

    fseek(file, data_end, SEEK_SET);
    while (aux){
        aux->order = member_order(dir, aux->name);
        aux->location = location_member(dir, aux->name);
        fwrite(&aux->name, 1, sizeof(aux->name), file);
        fwrite(&aux->path, 1, sizeof(aux->path), file);
        fwrite(&aux->location, 1, sizeof(unsigned int), file);
        fwrite(&aux->size, 1, sizeof(size_t), file);
        fwrite(&aux->mode, 1, sizeof(mode_t), file);
        fwrite(&aux->uid, 1, sizeof(uid_t), file);
        fwrite(&aux->mtime, 1, sizeof(time_t), file);
        fwrite(&aux->order, 1, sizeof(unsigned int), file);
        aux = aux->next;
    }
        fwrite(&dir->dirSize, 1, sizeof(int), file);        
        fwrite(&data_end, 1, sizeof(unsigned int), file);
        fwrite(vina_ticket, 1, sizeof(vina_ticket), file);
}
