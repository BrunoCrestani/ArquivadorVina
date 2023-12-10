
/* O diretorio é uma lista que armazena informações dos arquivos membros do arquivador VINA. */

/*
 * membro_t é uma estrutura interna que representa o um membro do diretório.
 */
typedef struct member {
    char name[256]; //Nome do membro em questão presente no VINA 
    char path[4096];//datapath do membro em questão presente no VINA
    unsigned int location;//Byte de início do membro em questão presente no VINA
    size_t size; //Tamanho do membro em questão presente no VINA
    mode_t mode; //Permissões do membro em questão presente no VINA
    uid_t uid; //UID do membro em questão presente no VINA
    time_t mtime; //Data de modificação do membro em questão presente no VINA
    unsigned int order;//Ordem do membro no diretório do VINA
    struct member *next; //Nodo que aponta para o próximo membro presente no VINA
    struct member *previous; //Nodo que aponta para o próximo membro presente no VINA
} member_t;

/*
 * Representacao do diretorio
 */
typedef struct diretorio {
    int dirSize;
    member_t *first;
} diretorio_t;

/* 
 * Cria um diretorio vazio
 */
diretorio_t *make_dir();

/* 
 * Destroi o diretório e retorna NULL. 
 */ 
diretorio_t *destroy_dir(diretorio_t *d);

/*
 * Adiciona um membro na primeira posição do diretório.
 * Retorna 0 em caso de sucesso, 1 caso contrario.
 */
int insert_dir_start(diretorio_t *d, member_t* initialNode);

/*
 * Insere o membro em questão no diretório.
 * Retorna 0 em caso de sucesso, 1 caso contrario.
 */
int insert_in_dir(diretorio_t *d, member_t* current);

/*
 * Remove o membro indicado do diretorio
 * Retorna 0 em caso de sucesso, 1 c.c.
 */
int remove_from_dir(diretorio_t* dir, member_t* nodeToRemove);

/*
 * Move o membro indicado para a posição seguinte do membro  target, dentro do
 * Diretório do arquivo.
 *  retorna 0 em caso de sucesso, 1 c.c.
 */
int move_in_dir(diretorio_t* dir, member_t* nodeToMove, member_t* destinationNode);

/*
 * Procura o membro recebido no parametro da funcao.
 * Caso o membro seja encontrado, retorna 0.
 */
int present_member(diretorio_t *d, const char* name);

/*
 * Imprime o conteudo da lista de membros.
 */
void print_dir(diretorio_t *d);

/*
 * Inicializa um membro do diretorio.
 */
 member_t* make_member(const char* membername);

 /*
 *Destroi o membro e retorna NULL;
 */
member_t* destroy_member(member_t* member);
 
 /*
 * Recebe de um determinado membro todas as informações
 * a serem listadas pela opção -c do vina++ e que serão armazenadas
 * na parte "Diretório" do arquivo.
 * Retorna 1 em caso de erro e 0 em caso de acerto.
 */
member_t* get_content(member_t* membro);

 /*
 * Retorna o nodo de um membro específico presente no
 * arquivador.
 */
member_t* find_member(diretorio_t *d, const char* membername);

/*
 * Retorna o byte em que os dados do membro em questao iniciam.
 */
unsigned int location_member(diretorio_t* dir, const char* membername);

/*
 * Verifica se o tempo x é mais recente que o tempo y.
 * Retorna 1 caso sim, e 0 c.c.
 */
unsigned int mtime_comparison(const time_t x, const time_t y);

/*
 * Descobre a ordem na qual o membro está inserido no arquivador VINA.
 */
unsigned int member_order(diretorio_t *d, const char* memberename);

/*
 * Formata o tempo recebido pela struct stat em uma data especifica.
 */
char* time_format(const time_t timestamp, char* buffer);

/*
 * Descobre o fim dos dados binarios inseridos no arquivador VINA.
 */
unsigned int data_size(diretorio_t *d);

/*
 * Imprime todos os metadados do membro.
 */
void print_member(member_t* member);

/*
 * DEPURAÇÃO APENAS.
 */
void print_member_depuration(member_t* member);






