#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <strings.h>
#include <string.h>

#include "libdir.h"
#include "archiver.h"
#include "overall_use_functions.h"
#include "extract.h"

#define BUFFER_SIZE 1024
#define NAME_SIZE 256
#define PATH_SIZE 4096

/*
 * Copia o trecho de memória em que o membro em questão esta inserido em um
 * novo diretório fora do arquivador.
 */
void extract_member(FILE* file, diretorio_t* dir, const char* membername){
    member_t* member = find_member(dir, membername);
    if (member == NULL){
        destroy_dir(dir);
        fclose(file);
        printf("\nNão é possível extrair um membro que não esteja presente no arquivador VINA");
        exit(1);
    }
    FILE* extraction_file = create_writing_file(membername);
    char buffer[BUFFER_SIZE];

    unsigned int start = member->location; //1
    unsigned int end = member->location + member->size - 1; //9
    unsigned int block, rt, read, write;

    block = end - start; //TAMANHO A SER LIDO
    read = start - 1; //ONDE COMECAR A LEITURA
    write = 0; //ONDE COMECAR A ESCRITA

    while(block > 0){
		fseek(file, read, SEEK_SET);
        if (block > 1024){
            rt = fread(buffer, 1, BUFFER_SIZE, file);
        } else {
            rt = fread(buffer, 1, block, file);
        }
        fseek(extraction_file, write, SEEK_SET);
        fwrite(buffer, 1, rt, extraction_file);
        read += rt;
        write += rt;
        block -= rt;
    }
}

/*
 * Le os dados do arquivo que correspondem ao diretorio e os
 * insere no diretorio vazio.
 */
unsigned int diretory_extraction(FILE* file, diretorio_t* dir){
    unsigned int diretory_start;
    int member_quantity;
    int index = 0;
    char vina_ticket[7];

    fseek(file, -7, SEEK_END);
    fread(vina_ticket, 7, 1, file);
    if (strcmp(vina_ticket, "vina++")){
        printf("O arquivo passado não é um arquivador vina++!\n");
        fclose(file);
        return 1;
    }

    fseek(file, -11, SEEK_END);
    fread(&diretory_start, sizeof(unsigned int), 1, file);
    fseek(file, -15, SEEK_END);
    fread(&member_quantity, sizeof(int), 1, file);

    fseek(file, diretory_start, SEEK_SET);
    
    while(index < member_quantity){
        member_extraction(file, dir);
        index++;
    }
    ftruncate(fileno(file), diretory_start);
    return diretory_start;
}

void member_extraction(FILE* file, diretorio_t* dir){
    char buffer[BUFFER_SIZE];
    unsigned int total_bytes_read = 0;
    unsigned int rt;
     member_t* member = malloc(sizeof(member_t));
     memset(member->path, 0, sizeof(member->path));

    fread(buffer, NAME_SIZE, 1, file);
    strncpy(member->name, buffer, sizeof(member->name));

    for (int i = 1; i <= 4; i++){
        rt = fread(buffer, 1, BUFFER_SIZE, file);
        memcpy(member->path + total_bytes_read, buffer, rt);
        total_bytes_read = total_bytes_read + rt;
    }

    fread(&member->location, 1, sizeof(unsigned int), file);
    fread(&member->size, 1, sizeof(size_t), file);
    fread(&member->mode, 1, sizeof(mode_t), file);
    fread(&member->uid, 1, sizeof(uid_t), file);
    fread(&member->mtime, 1, sizeof(time_t), file);
    fread(&member->order, 1, sizeof(unsigned int), file);

    insert_in_dir(dir, member);

    return;
}
