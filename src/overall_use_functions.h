/*
 * Abre um arquivo para realizar a leitura de dados.
 */
FILE* create_reading_file(const char* filename);

/*
 * Abre um arquivo para realizar a escrita de dados.
 */
FILE* create_writing_file(const char* filename);

/*
 * Abre um arquivo ja existente.
 */
FILE* create_existing_file(const char* filename);
