#ifndef __ARCHIVER__
#define __ARCHIVER__

#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>

unsigned int get_size(FILE *arch);
int remove_bytes(FILE *arch, const unsigned int b_init, const unsigned int b_final);
int append_bytes(FILE *arch, char *b_buffer, const unsigned int b_total);
int extract_bytes(FILE *arch, char *b_buffer, const unsigned int b_init, const unsigned int b_final);
int move_bytes(FILE *arch, const unsigned int b_init, const unsigned int b_final, const unsigned int b_target);

#endif