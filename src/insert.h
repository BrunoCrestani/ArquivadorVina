
/*
 *  Realiza uma leitura bufferizada dos dados do membro
 * e os insere no arquivador, até que seja encontrado o 
 * final do membro.
 */
void data_insertion( FILE* file, member_t* member);

/*
 * Realiza o processo de leitura e escrita de bytes com o uso de um buffer.
 */
int bytes_insertion(FILE* insertion_file, FILE* file, char*buffer, long rest);

/*
 * Insere o diretorio criado no arquivador.
 */
void diretory_insertion(FILE* file, diretorio_t* dir);