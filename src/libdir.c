

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <libgen.h>

#include "libdir.h"
#include "overall_use_functions.h"

/*
 * Cria um diretorio vazio
 */
diretorio_t* make_dir(){
    diretorio_t *d;

    if (!(d = malloc(sizeof(diretorio_t))))
        return NULL;

    d->first = NULL;
    d->dirSize = 0;

    return d;
}

/*
 * Destroi o diretorio e retorna NULL.
 */
diretorio_t* destroy_dir(diretorio_t *d){ 
     member_t *aux;

    if (d->first == NULL)
        return NULL;

    while ((d->first) != NULL)
    { 
        aux = (d->first);
        d->first = (d->first)->next;
        destroy_member(aux);
        aux = NULL;
    }
    free(d);
    d = NULL;
    return d;
}

/*
 * Adiciona um membro na primeira posição do diretório.
 * Retorna 0 em caso de sucesso, 1 caso contrario.
 */
int insert_dir_start(diretorio_t *d, member_t* initialNode){
    initialNode->next = d->first;
    initialNode->previous = NULL;
    d->first = initialNode;

    d->dirSize++;
    return 0;
}
/*
 * Insere o membro em questão no diretório.
 * Retorna 0 em caso de sucesso, 1 caso contrario.
 */
int insert_in_dir(diretorio_t *d, member_t* current){
    member_t *aux;

    if (d->first == NULL)
    {
        insert_dir_start(d, current);
        return 0;
    }

    aux = d->first;
    while ((aux->next))
        aux = aux->next;

    current->next = NULL;
    current->previous = aux;
    aux->next = current;

    d->dirSize++;
    return 0;
}

/*
 * Remove o membro indicado do diretorio
 * Retorna 0 em caso de sucesso, 1 c.c.
 */
int remove_from_dir(diretorio_t* dir, member_t* nodeToRemove){
    if (dir->first == NULL)
        return 1;
    
    if (dir->first == nodeToRemove) {
        dir->first = nodeToRemove->next;
        if (dir->first != NULL)
            dir->first->previous = NULL;
        dir->dirSize--;
        free(nodeToRemove);
        return 0;
    }

    if (nodeToRemove->previous != NULL) nodeToRemove->previous->next = nodeToRemove->next;

    if (nodeToRemove->next != NULL) nodeToRemove->next->previous = nodeToRemove->previous;

    dir->dirSize--;
    free(nodeToRemove);
    return 0;
}

/*
 * Move o membro indicado para a posição seguinte do membro  target, dentro do
 * Diretório do arquivo.
 *  retorna 0 em caso de sucesso, 1 c.c.
 */
int move_in_dir(diretorio_t* dir, member_t* nodeToMove, member_t* destinationNode){
    if (nodeToMove == NULL || destinationNode == NULL)
        return 1;
    
    if (nodeToMove == destinationNode->next)
        return 1;
    
    if (nodeToMove->previous != NULL)
        nodeToMove->previous->next = nodeToMove->next;
    else
        dir->first = nodeToMove->next;
    
    if (nodeToMove->next != NULL)
        nodeToMove->next->previous = nodeToMove->previous;
    
    nodeToMove->previous = destinationNode;
    nodeToMove->next = destinationNode->next;
    
    destinationNode->next = nodeToMove;
    nodeToMove->location = location_member(dir, nodeToMove->name);

    return 0;
}

/*
 * Procura o membro recebido no parametro da funcao.
 * Caso o membro seja encontrado, retorna 0, e 1 c.c. 
 */
int present_member(diretorio_t *d, const char* membername){
    member_t* aux;

    aux = d->first;
    while (aux)
    {   
        if (strcmp(aux->name, membername) == 0)
            return 0;
        aux = aux->next;
    }
    return -1;
}

/*
 * Imprime o conteudo do diretorio.
 */
void print_dir(diretorio_t *d){
    member_t* aux;

    aux = d->first;

    while ((aux))
    {
        print_member_depuration(aux);
        printf("\n"); 
        aux = aux->next;
    }
}

member_t* make_member(const char* membername){
    FILE* arq;
    member_t* member;
    char* path;

    arq = create_reading_file(membername);
    if (!arq){
        printf("O arquivo não existe!");
        return NULL;
    }
    fclose(arq);

    if (!(member = malloc(sizeof(member_t))))
        return NULL;
    
    path = realpath(membername, NULL);

    if(strcmp(membername, path) == 0){
        strncpy(member->path, path, sizeof(member->path));
        strncpy(member->name, basename(member->path), sizeof(member->name));
    } else {
        strncpy(member->name, membername, sizeof(member->name));
        strncpy(member->path, path, sizeof(member->path));
    }
    
    free(path);
    return member;
}
/*
 *Destroi o membro e retorna NULL;
 */
member_t* destroy_member(member_t* member){
    free(member);
    return NULL;
}

 /*
 * Recebe de um determinado membro todas as informações
 * a serem listadas pela opção -c do vina++ e que serão armazenadas
 * na parte "Diretório" do arquivo.
 * Retorna 1 em caso de erro e 0 em caso de acerto.
 */
member_t* get_content(member_t* member){
    struct stat fileStat;
    
    if(stat(member->name, &fileStat) == -1){
        perror("Erro ao obter informacoes do arquivo");
        return NULL;
    }

    member->size = fileStat.st_size;
    member->mode = fileStat.st_mode & 0777;
    member->uid = fileStat.st_uid;
    member->mtime = fileStat.st_mtime;
    
    return member;
}

/*
 * Retorna o nodo de um membro específico presente no
 * arquivador.
 */
member_t* find_member(diretorio_t *d, const char* membername){
    member_t* aux;

    aux = d->first;

    while (aux)
    {   
        if (strcmp(aux->name, membername) == 0)
            return aux;
        aux = aux->next;
    }
    printf("Não existe um membro com o nome desejado, portanto não foi possível retorna-lo");
    return NULL;
}

/*
 * Retorna o byte em que os dados do membro em questao iniciam.
 */
unsigned int location_member(diretorio_t* dir, const char* membername){
    member_t *aux;
    unsigned int position = 1;

    aux = dir->first;
    while (aux && (aux->name != membername)){
        position = position + aux->size;
        aux = aux->next;
    }
    return position;
}

/*
 * Verifica se o tempo x é mais recente que o tempo y.
 * Retorna 0 caso sim, e 1 c.c.
 */
unsigned int mtime_comparison(const time_t x,  const time_t y){
    if (x < y)
        return 0;
    return 1;
}

/*
 * Descobre a ordem na qual o membro está inserido no arquivador VINA.
 */
unsigned int member_order(diretorio_t *d, const char* membername){
    unsigned int index = 0;
    member_t* aux;

    aux = d->first;
    while (aux)
    {   
        if (aux->name == membername)
            return index;
        aux = aux->next;
        index++;
    }
    return index;
}

/*
 * Formata o tempo recebido pela struct stat em uma data especifica.
 */
char* time_format(const time_t timestamp, char* buffer){
    struct tm *tm_info;

    tm_info = localtime(&timestamp);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    
    return buffer;
}

/*
 * Descobre o fim dos dados binarios inseridos no arquivador VINA.
 */
unsigned int data_size(diretorio_t *d){
    unsigned int d_size = 0;
    member_t* aux;
    aux = d->first;
    while (aux)
    {   
        d_size = d_size + aux->size;
        aux = aux->next;
    }
    return d_size;
}

/*
 * Imprime as permissões de um membro de maneira
 * própriamente formatada.
 */
void print_permissions(mode_t permissions){
    printf((S_ISDIR(permissions)) ? "d" : "-");
    printf((permissions & S_IRUSR) ? "r" : "-");
    printf((permissions & S_IWUSR) ? "w" : "-");
    printf((permissions & S_IXUSR) ? "x" : "-");
    printf((permissions & S_IRGRP) ? "r" : "-");
    printf((permissions & S_IWGRP) ? "w" : "-");
    printf((permissions & S_IXGRP) ? "x" : "-");
    printf((permissions & S_IROTH) ? "r" : "-");
    printf((permissions & S_IWOTH) ? "w" : "-");
    printf((permissions & S_IXOTH) ? "x" : "-");    
}

/*
 * Imprime todos os metadados do membro.
 */
void print_member(member_t* member){
    char buffer[26];
    char* uuid;

    struct passwd *pw = getpwuid(member->uid);
    if (pw != NULL){
        uuid = pw->pw_name;
    }
    time_format(member->mtime, buffer);

    print_permissions(member->mode);
    printf(" %s", uuid);
    printf(" %8u", (unsigned int) member->size);
    printf(" %s", buffer);
    printf(" %s ", member->path);    
}

/*
 * DEPURAÇÃO APENAS.
 */
void print_member_depuration(member_t* member){
    char buffer[26];

    strncpy(buffer, time_format(member->mtime, buffer), 26);

    printf("ORDER:%d ", member->order);
    printf("Nome:%s ", member->name);
    printf("\n");
    printf("Path:%s ", member->path);        
    printf("\n");
    printf("Byte de inicio dos dados:%u ", member->location);
    printf("\n");
    printf("Tamanho:%u ", (unsigned int) member->size);
    printf("\n");
    printf("Permissões:%o ", member->mode);
    printf("\n");
    printf("UID:%d ", member->uid);
    printf("\n");
    printf("Data de Última modificação:%s ", buffer);
    printf("\n");
}