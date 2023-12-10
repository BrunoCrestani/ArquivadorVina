#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "archiver.h"
#include "libdir.h"
#include "overall_use_functions.h"
#include "insert.h"
#include "extract.h"

typedef enum {NOP, INSERT, ADJUSTED_INSERT, MOVE, EXTRACT, REMOVE, CONTENT, HELP} Mode_t;

#define BUFFER_SIZE 1024

int main(int argc, char** argv){


    int opt;
    Mode_t mode = NOP;
    while((opt = getopt(argc, argv, "iam:xrch")) != -1){
        switch(opt){
            case 'i':
                mode |= INSERT;
                if (argc < 4){
                    printf("esta opção necessita de ao menos um membro.\n");
                    printf("Forma de uso: ./vina++ -i arquivo [membros]\n");
                    return 1;
                }
                break;
            case 'a':
                mode |= ADJUSTED_INSERT;
                if (argc < 4 || argc > 4){
                    printf("esta opção necessita de apenas um membro.\n");
                    printf("Forma de uso: ./vina++ -a arquivo membro\n");
                    return 1;
                }
                break;
            case 'm':
                mode |= MOVE;
                if (argc != 5){
                    printf("esta opção necessita de exatamente um membro e um alvo.\n");
                    printf("Forma de uso: ./vina++ -m [alvo] arquivo [membro]\n");
                    return 1;
                }
                break;
            case 'x':
                mode |= EXTRACT;
                if (argc < 2)
                    return 1;
                break;
            case 'r':
                mode |= REMOVE;
                if (argc < 4){
                    printf("esta opção necessita de ao menos um membro.\n");
                    printf("Forma de uso: ./vina++ -r arquivo [membros]\n");
                    return 1;
                }
                break;
            case 'c':
                mode |= CONTENT;
                if (argc > 3){
                    printf("esta opção não trabalha com membros.\n");
                    printf("Forma de uso: ./vina++ -c arquivo\n");
                    return 3;
                }
                break;
            case 'h':
                mode |= HELP;
                printf("\nForma de uso: ./vina++ [-i | -a | -m target | -x | -r | -c | -h] arquivo [membros]\n");
                printf("O arquivador 'VINA++' salva diversos arquivos em um único e pode extrair ou remover, arquivos individualmente ou coletivamente do arquivo principal.\n\n");
                printf("Seguem as opções possíveis, e suas formas de uso: \n\n");
                printf(" ./vina++ -i arquivo [membros]\n");
                printf("    A opção principal de inserção do arquivador, insere um ou mais membros de uma\n vez,e se for inserido um arquivo com o nome já presente no diretório, este será substituido\n\n");
                printf(" ./vina++ -a arquivo membro\n");
                printf("    Realiza o mesmo papel da opção 'i' mas no caso da inserção de arquivos com um\n nome já presente, prevalecerá o com data de alteração mais recente\n\n");
                printf(" ./vina++ -m [alvo] arquivo [membro]\n");
                printf("    Move os dados binários, do [membro] para logo após o membro [alvo]\n\n");
                printf(" ./vina++ -x arquivo [membros]\n");
                printf("    Extrai os [membros] de dentro do arquivo arquivador indicado.\n\n");
                printf(" ./vina++ -r arquivo [membros]\n");
                printf("    remove os [membros] de dentro do arquivo arquivador indicado.\n\n");
                printf(" ./vina++ -c arquivo\n");
                printf("    Imprime informações de todos os membros presentes na ordem do diretório presente no arquivador\n");
                return 0;
                break;    
            default:
                if (argc < 2){
                    printf("Forma de uso: ./vina++ [-i | -a | -m target | -x | -r | -c | -h] arquivo [membros]\n");
                    return 1;
                }
        }
    }

    if (mode == NOP){
        printf("Forma de uso: ./vina++ [-i | -a | -m target | -x | -r | -c | -h] arquivo [membros]\n\n Para mais informações utilize: ./vina++ -h\n");
        return 1;
    }

    FILE* file;
    diretorio_t* dir = make_dir();
	unsigned int diretory_start, start, end;

    if (mode == INSERT || mode == ADJUSTED_INSERT){
    member_t *member, *aux, *oldMember;

	file = fopen(argv[2], "r+");
	if (!file){ 
		file = create_writing_file(argv[2]);

		for(int i = 3; i < argc; i++){
	    member = make_member(argv[i]);
        if(member == NULL){
            free(dir);
            fclose(file);
            remove(argv[2]);
            return 1;
        }
        member = get_content(member);
	    insert_in_dir(dir, member);
		}
		aux = dir->first;
		while(aux){
			data_insertion(file, aux);
			aux = aux->next;
		}
		diretory_insertion(file, dir);
	} else {
	    diretory_start = diretory_extraction(file, dir);

		fseek(file, diretory_start, SEEK_SET);

		for(int i = 3; i < argc; i++){
            member = make_member(argv[i]);
	        member = get_content(member);
	        member->location = location_member(dir, member->name);

            if ((present_member(dir, argv[i]) == 0)){
                oldMember = find_member(dir, argv[i]);
                if ((!(mtime_comparison(member->mtime, oldMember->mtime)) && (mode == ADJUSTED_INSERT))){
                    destroy_member(member);
                    destroy_dir(dir);
                    fclose(file);
                    return 0;
                }
                start = oldMember->location;
                end = start + oldMember->size - 1;
                remove_bytes(file, start, end);
                remove_from_dir(dir, oldMember);
            }
		    fseek(file, 0, SEEK_END);
	        data_insertion(file, member);
	        insert_in_dir(dir, member);
		}
		fseek(file, data_size(dir), SEEK_SET);
		diretory_insertion(file, dir);
	    }   
    }

    if (mode == MOVE){
        member_t *member, *target_m;
        unsigned int start, end, target;

        file = create_existing_file(argv[3]);
        if (!file){
            printf("A movimentação só ocorre dentro de um arquivador VINA");
            free(dir);
            return 1; 
        }

        diretory_start = diretory_extraction(file, dir);

        target_m = find_member(dir, argv[2]);
        if(target_m == NULL){
            diretory_insertion(file, dir);
            destroy_dir(dir);
            fclose(file);
            return 1;
        }
        member = find_member(dir ,argv[4]);
        if(member == NULL){
            diretory_insertion(file, dir);
            destroy_dir(dir); 
            fclose(file);
            return 1;
        }   
        
        if (target_m == member){
            diretory_insertion(file, dir);
            destroy_dir(dir);
            fclose(file);
            return 0;
        }
    
        if (member->order < target_m->order){
            target = (target_m->location + target_m->size - member->size);
        } else {
            target = (target_m->location + target_m->size);
        }

        start = member->location;
        end = member->location + member->size - 1;

        move_bytes(file, start, end, target);
        move_in_dir(dir, member, target_m);

        diretory_insertion(file, dir);
    }

    if (mode == EXTRACT){
        member_t *aux;
        file = create_reading_file(argv[2]);
        if (!file){
            printf("A extração só ocorre a partir de um arquivador VINA");
            free(dir);
            return 1; 
        }

        diretory_start = diretory_extraction(file, dir);

        aux = dir->first;
        if (argc < 4){
            while(aux){
                extract_member(file, dir, aux->name);
                aux = aux->next;
            }
        } else {
            for (int i = 3; i < argc; i++){
                extract_member(file, dir, argv[i]);
            }
        }
    }

    if (mode == REMOVE){
        member_t *member;

        file = create_existing_file(argv[2]);
        if (!file){
            printf("A remoção só ocorre dentro do arquivador VINA");
            free(dir);
            return 1; 
        }
        
        diretory_extraction(file, dir);

        for (int i = 3; i < argc; i++){
            member = find_member(dir, argv[i]);
            
            if(member == NULL){
                diretory_insertion(file, dir);
                destroy_dir(dir); 
                fclose(file);
            return 1;
        }   
            start = member->location;
            end = member->location + member->size - 1;

            remove_bytes(file, start, end);
            remove_from_dir(dir, member);
            if (dir->dirSize == 0){
                ftruncate(fileno(file), 0 );
            }
        }
        diretory_insertion(file, dir);
    }

    if (mode == CONTENT){
        file = create_reading_file(argv[2]);
        if (!file){
            printf("A impressão dos conteúdos só ocorre a partir de um arquivador VINA");
            free(dir);
            return 1; 
        }
        diretory_extraction(file, dir);
        print_dir(dir);
    }

    fclose(file);
    destroy_dir(dir);
    return 0;

}


