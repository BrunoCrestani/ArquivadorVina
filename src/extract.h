/*
 * Copia o trecho de memória em que o membro em questão esta inserido em um
 * novo diretório fora do arquivador.
 */
void extract_member(FILE* file, diretorio_t* dir, const char* membername);

/*
 * Le os dados do arquivo que correspondem ao diretorio e os
 * insere no diretorio vazio.
 */
unsigned int diretory_extraction(FILE* file, diretorio_t* dir);

/*
 * Realiza a leitura dos dados de um membro presente no arquivo.
 */
void member_extraction(FILE* file, diretorio_t* dir);