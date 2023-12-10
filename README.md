# Arquivador-Vina-

	AUTORIA:
    Bruno Corrado Crestani
    GRR: 20221240

	SRC:
	Contém os arquivos .c e .h utilizados no programa, sendo eles:
	
	OVERALL_USE_FUNCTIONS:
	Códigos de uso geral do programa, que realizam processos como abrir arquivos, descobrir o tamanho de um arquivo, e criação de diretórios.
	
	LIBDIR:
	Principalmente as funções que realizam operações com a struct diretório e seus membros.

	INSERT:
	Utilizado principalmente para qualquer tipo de inserções no próprio arquivador.
	
	EXTRACT:
	Utilizado principalmente para qualquer tipo de extração de dados do arquivador.

	VINA:
	O main do projeto, realiza o trabalho com as opções e sua funcionalidades, utilizando as funções dos diversos arquivos para que a execução ocorra da maneira esperada.
	
	OBJ:
	Armazena os objetos gerados pelos arquivos de SRC.
	
	MAKEFILE
	Operação MAKE.
	    
	BIBLIOTECA ARCHIVER:
    A bibilioteca implementada em sala foi utilizada neste projeto, e para que ele funcione corretamente, os arquivos devem ser inseridos no diretorio ./src

	FUNCIONAMENTO
	A única maneira de inicializar o arquivador é utilizando a opção -i com um arquivo ou path de arquivo existente.


	DIRETÓRIO DO ARQUIVADOR
    O Diretório se trata de uma lista encadeada, em que os nodos são membros presentes no arquivador, ordenados por ordem de inserção (A não ser que seja utiliada a opção m).
Os dados do diretório são sempre inseridos logo após o último byte de dados binários do último arquivo a ser inserido. Logo após estes dados do diretório, sempre se encontram 15 bytes (Os 15 últimos do arquivador) gravados, sendo os 4 primeiros para gravar o byte de início dos dados do diretório, os 4 após para gravar o número de membros presente no diretório e consequentemente no arquivador, e os últimos 7 uma string de conteúdo "vina++" que serve como uma etiqueta para arquivos que possuem a função arquivadora vina++.
 todos os metadados dos membros incluindo nome e path do arquivo utilizam um tamanho estático, e da mesma maneira, são escritos no arquivador, logo,  conjunto dos metadados dos membros possui um tamanho específico de bytes, e cada um ocupa um mesmo espaço quando inserido no arquivador.

	STRUCT MEMBRO
	Struct da qual é feita os nodos da lista encadeada. Possui os campos: NAME; PATH; LOCATION; SIZE; MODE; UID; MTIME; ORDER. Além disso, abriga os ponteiros de membro NEXT e PREVIOUS.
Seus campos são preenchidos pelas informações de um determinado arquivo que será/é um membro do arquivador.

	BUGS CONHECIDOS
	Os bugs que foram tratados em suma eram em relação a inserção equivocada de um arquivo na linha de comando. Dessa maneira, tentei blindar o máximo possível o arquivo .vpp de mudanças indevidas, ou interupções de execução por erros que acabam corrompendo o arquivo, dessa maneira, tentando sempre evitar o segmentation fault.
O projeto não conta com a opção de extrair um arquivo diretamente para um diretório.
