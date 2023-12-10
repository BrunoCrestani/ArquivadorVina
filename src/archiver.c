#include "archiver.h"

unsigned int get_size(FILE *arch){
	struct stat f_data;

	fstat(fileno(arch), &f_data);
	return (unsigned int) f_data.st_size;
}

int remove_bytes(FILE *arch, const unsigned int b_init, const unsigned int b_final){
	char *buffer[1024];
	unsigned int size = get_size(arch);
	unsigned int read = b_final;
	unsigned int write = b_init - 1;
	unsigned int rt;

	if (b_init < 1) return 1;
	if (b_final > size) return 2;
	if (b_init > b_final) return 3;

	if (read == size){
		ftruncate(fileno(arch), b_init-1);
		return 0;
	}

	while(read < size){
		fseek(arch, read, SEEK_SET);
		if (size - read > 1024) rt = fread(buffer, 1, 1024, arch);
		else rt = fread(buffer, 1, size-read, arch);
		fseek(arch, write, SEEK_SET);
		fwrite(buffer, 1, rt, arch);
		read += rt;
		write += rt;
	}
	rewind(arch);
	ftruncate(fileno(arch), size - (b_final-b_init+1));
	return 0;
}

int move_bytes(FILE *arch, const unsigned int b_init, const unsigned int b_final, const unsigned int b_target){
	unsigned char buffer[1024];
	unsigned int size = get_size(arch);
	unsigned int rt;	
	unsigned int block, read, write;

	if (b_init < 1) return 1;
	if (b_final > size) return 2;
	if (b_init > b_final) return 3;
	if ((size - b_target + 1) < (b_final - b_init + 1)) return 4;
	if (b_target == b_init) return 0;

	block = b_final - b_init + 1;//bloco é igual o tamanho
	read = b_init - 1;//define posicao onde vai comecar a ler 1 byte antes
	while (block > 0){//enquanto o bloco for maior que 0;
		fseek(arch, read, SEEK_SET);//vou ate o ponto de leitura
		if (block > 1024) rt = fread(buffer, 1, 1024, arch);//se o buffer estiver cheio, leia o tamanho do buffer
		else rt = fread(buffer, 1, block, arch);//se nao, leia o que sobrou do block
		fseek(arch, 0, SEEK_END);//va para o final do arquivo
		fwrite(buffer, 1, rt, arch);//escreva o buffer
		block -= rt;//desconte do bloco
		read += rt;//mude a posicao de leitura
	}
	
	if (b_target < b_init){//se o target vier antes 
		block = b_init - b_target;//bloco entre o target e o ponto inicial dos bytes
		read = b_init - 1;//comeca a ler no ponto inicial
		write = b_final;//comeca a escrever no ponto final
		while (block > 0){
			if (block > 1024){
				fseek(arch, read - 1024, SEEK_SET);
				rt = fread(buffer, 1, 1024, arch);
			} else{
				fseek(arch, read - block, SEEK_SET);
				rt = fread(buffer, 1, block, arch);
			}
			fseek(arch, write - rt, SEEK_SET);
			fwrite(buffer, 1, rt, arch);
			block -= rt;
			read -= rt;
			write -= rt;
		}
	}
	else{//caso o target venha depois do inicio,
		block = b_target - b_init;//bloco entre o byte inicial e o byte target
		read = b_final;//
		write = b_init - 1;

		while(block > 0){
			fseek(arch, read, SEEK_SET);
			if (block >= 1024){
				rt = fread(buffer, 1, 1024, arch);
			}
			else{
				rt = fread(buffer, 1, block, arch);
			}
			fseek(arch, write, SEEK_SET);
			fwrite(buffer, 1, rt, arch);
			read += rt;
			write += rt;
			block -= rt;
		}
	}

	block = b_final - b_init + 1;
	read = size;
	write = b_target - 1;
	while (block > 0){
		fseek(arch, read, SEEK_SET);
		if (block >= 1024){
			rt = fread(buffer, 1, 1024, arch);
		}
		else{
			rt = fread(buffer, 1, block, arch);
		}
		fseek(arch, write, SEEK_SET);
		fwrite(buffer, 1, rt, arch);
		read += rt;
		write += rt;
		block -= rt;
	}

	rewind(arch);
	ftruncate(fileno(arch), size);

	return 0;
}
