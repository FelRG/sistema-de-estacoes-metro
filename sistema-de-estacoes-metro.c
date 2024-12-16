#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_LINHAS_ESTACAO 4
#define true 1
#define false 0
#define ABERTO 0
#define FECHADOTEMP 1

// Estrutura para representar uma esta��o
struct Estacao {
    char nome[50];
    int status;
    struct Estacao *prox;
    struct Linha *linhas[MAX_LINHAS_ESTACAO];
};

// Estrutura para representar uma linha do metr�
struct Linha {
    char nome[50];
    struct Estacao *estacoes;
    struct Linha *prox;
};

// Prototipa��o de todas as fun��es
struct Estacao *criarEstacao(char nome[]);
struct Linha *criarLinha(char nome[], struct Linha *l);
void adicionarEstacaoLinhas(struct Estacao *estacao, int numLinhas, struct Linha *linhaAdd[]);
void excluirPermanentementeEstacao(struct Linha *linhas, char nomeEstacao[]);
void gerarRelatorio(struct Linha *linhaAtual);
void InsercaoDadosMetro(struct Linha *linhaAzul, struct Linha *linhaRosa, struct Linha *linhaVerde, struct Linha *linhaAmarela);
struct Linha *encontrarLinha(struct Linha *linhas, char nomeLinha[]);
void alterarDadosEstacao(struct Estacao *estacao, char novoNome[]);
int ContagemTotalEstacoes(struct Linha *linhas);
struct Estacao *buscarEstacao(struct Linha *linhas, char nomeEstacao[]);
void buscarEstacoesComMaisConexoes(struct Linha *listaLinhas);
struct Linha *encontrarUltimaLinha(struct Linha *linhas);
void fecharEstacaoTemp(struct Estacao *estacao);
void abrirNovamenteEstacao(struct Estacao *estacao);
void mostrarEstacoesFechadasTemp(struct Linha *linhaAtual);
int verificaGrafoEuler(struct Estacao *estacoes[], int numEstacoes);
void identificarEstacoesComAcesso(struct Linha *listaLinhas, struct Estacao *estacaoOrigem, struct Linha *linhaDesejada);
//void dijkstra(struct Linha *linha, struct Estacao *origem, struct Estacao *destino) (nao implementado)

// Fun��o para criar uma nova esta��o
struct Estacao *criarEstacao(char nome[]) {
    struct Estacao *novaEstacao = malloc(sizeof(struct Estacao));
    strcpy(novaEstacao->nome, nome);
    novaEstacao->prox = NULL;

    int i;
    for (i = 0; i < MAX_LINHAS_ESTACAO; ++i) {
        novaEstacao->linhas[i] = NULL;
    }
    
    novaEstacao->status = ABERTO; //todas as estacoes vao estar abertas

    return novaEstacao;
}

// Fun��o para criar uma nova linha
struct Linha *criarLinha(char nome[], struct Linha *l) {
    struct Linha *novaLinha = malloc(sizeof(struct Linha));
    strcpy(novaLinha->nome, nome);
    novaLinha->estacoes = NULL;
    novaLinha->prox = NULL;
    
    if (l == NULL){ // se a lista estava vazia, "novo" ser� o primeiro elemento
        return novaLinha;
    }

    else{
        struct Linha *p = l; // ponteiro para varrer a lista at� encontrar o �ltimo elemento

        while(p->prox != NULL){
            p = p->prox;
        }

        p->prox = novaLinha;
        return l;     
    }
}

// Fun��o para adicionar uma esta��o a v�rias linhas
void adicionarEstacaoLinhas(struct Estacao *estacao, int numLinhas, struct Linha *linhaAdd[]) {

    int i;
    for (i = 0; i < numLinhas; ++i) {

        // Criar a nova esta��o
        struct Estacao *novaEstacao = estacao;
        
        // Adicionar a esta��o � linha
        novaEstacao->prox = linhaAdd[i]->estacoes;
        linhaAdd[i]->estacoes = novaEstacao;

        // Adicionar a linha � esta��o
        estacao->linhas[i] = linhaAdd[i];
    }

    return;
}

// Fun��o para adicionar uma esta��o a v�rias linhas
void adicionarEstacaoLinhas2(struct Estacao *estacao, int numLinhas, ...) {
    va_list args;
    va_start(args, numLinhas);

    int i;
    for (i = 0; i < numLinhas; ++i) {
        struct Linha *linha = va_arg(args, struct Linha *);

        // Criar a nova esta��o
        struct Estacao *novaEstacao = criarEstacao(estacao->nome);
        
        // Adicionar a esta��o � linha
        novaEstacao->prox = linha->estacoes;
        linha->estacoes = novaEstacao;

        // Adicionar a linha � esta��o
        estacao->linhas[i] = linha;
    }

    va_end(args);
}

void excluirPermanentementeEstacao(struct Linha *linhas, char nomeEstacao[]) {
    struct Linha *linhaAtual = linhas;

    while (linhaAtual != NULL) {
        struct Estacao *estacaoAtual = linhaAtual->estacoes;
        struct Estacao *estacaoAnterior = NULL;

        while (estacaoAtual != NULL) {
            if (strcmp(estacaoAtual->nome, nomeEstacao) == 0) {
                // Remover a esta��o da lista de linhas da esta��o
                int j;
                for (j = 0; j < MAX_LINHAS_ESTACAO; ++j) {
                    if (estacaoAtual->linhas[j] == linhaAtual) {
                        estacaoAtual->linhas[j] = NULL;
                        break;
                    }
                }

                // Atualizar os ponteiros da lista
                if (estacaoAnterior == NULL) {
                    linhaAtual->estacoes = estacaoAtual->prox;
                } else {
                    estacaoAnterior->prox = estacaoAtual->prox;
                }

                // Liberar mem�ria da esta��o
                free(estacaoAtual);

                // Se a lista de esta��es da linha ficar vazia, ajustar ponteiros
                if (linhaAtual->estacoes == NULL) {
                    struct Linha *linhaRemover = linhaAtual;
                    linhaAtual = linhaAtual->prox;

                    // Atualizar o ponteiro anterior (pode ser nulo)
                    if (linhaRemover == linhas) {
                        linhas = linhaAtual;
                    } else {
                        // Encontrar o n� anterior
                        struct Linha *linhaAnterior = linhas;
                        while (linhaAnterior->prox != linhaRemover) {
                            linhaAnterior = linhaAnterior->prox;
                        }

                        // Atualizar o ponteiro anterior para pular a linha removida
                        linhaAnterior->prox = linhaAtual;
                    }

                    // Liberar mem�ria da linha
                    free(linhaRemover);
                } else {
                    // Avan�ar para a pr�xima esta��o
                    estacaoAnterior = estacaoAtual;
                    estacaoAtual = estacaoAtual->prox;
                }

                return;  // Esta��o encontrada e exclu�da permanentemente
            }

            // Avan�ar para a pr�xima esta��o
            estacaoAnterior = estacaoAtual;
            estacaoAtual = estacaoAtual->prox;
        }

        // Avan�ar para a pr�xima linha
        linhaAtual = linhaAtual->prox;
    }

    // Esta��o n�o encontrada
}

// Fun��o para gerar relat�rio
void gerarRelatorio(struct Linha *linhaAtual) {
    if (linhaAtual == NULL) {
        // Caso base: atingiu o final da lista de linhas
        return;
    }

    // Imprime o nome da linha
    printf("Linha: %s\n", linhaAtual->nome);

    // Imprime as esta��es da linha
    struct Estacao *estacaoAtual = linhaAtual->estacoes;
    struct Estacao *estacaoAnterior = NULL;
    while (estacaoAtual != NULL) {
    	if(estacaoAtual->status == FECHADOTEMP){
			estacaoAtual = estacaoAtual->prox;
		}
		else{
			printf("  Estacao: %s\n", estacaoAtual->nome); //mostra somente as estacoes abertas
			if((estacaoAtual->prox != NULL) || estacaoAnterior != NULL){
				printf("  Vizinhas: ");
			}
			if((estacaoAtual->nome != estacaoAnterior->nome) && estacaoAnterior != NULL){
				printf("%s", estacaoAnterior->nome);
			}
			if(estacaoAtual->prox != NULL){
				printf(", %s", estacaoAtual->prox->nome);
			}
			
			printf("\n\n");
			estacaoAnterior = estacaoAtual;
        	estacaoAtual = estacaoAtual->prox;
		}
    }

    // Chama recursivamente para a pr�xima linha
    gerarRelatorio(linhaAtual->prox);
} 

// Inser��o dos dados iniciais do programa
void InsercaoDadosMetro(struct Linha *linhaAzul, struct Linha *linhaRosa, struct Linha *linhaVerde, struct Linha *linhaAmarela){
	
	/* linha rosa */
	struct Estacao *aeroporto;
	aeroporto = criarEstacao("Aeroporto");
	adicionarEstacaoLinhas2(aeroporto, 1, linhaRosa);
	
	struct Estacao *encarnacao;
	encarnacao = criarEstacao("Encarnacao");
	adicionarEstacaoLinhas2(encarnacao, 1, linhaRosa);
	
	struct Estacao *oriente;
	oriente = criarEstacao("Oriente");
	adicionarEstacaoLinhas2(oriente, 1, linhaRosa);
	
	struct Estacao *olivais;
	olivais = criarEstacao("Olivais");
	adicionarEstacaoLinhas2(olivais, 1, linhaRosa);
	
	struct Estacao *olaias;
	olaias = criarEstacao("Olaias");
	adicionarEstacaoLinhas2(olaias, 1, linhaRosa);
	
	struct Estacao *alameda;
	alameda = criarEstacao("Alameda");
	adicionarEstacaoLinhas2(alameda, 2, linhaRosa, linhaVerde);
	
	struct Estacao *saldanha;
	saldanha = criarEstacao("Saldanha");
	adicionarEstacaoLinhas2(saldanha, 2, linhaRosa, linhaVerde);
	
	struct Estacao *sebastiao;
	sebastiao = criarEstacao("S.Sebastiao");
	adicionarEstacaoLinhas2(sebastiao, 2, linhaRosa, linhaAzul);
	
	/* linha amarela */
	struct Estacao *odivelas;
	odivelas = criarEstacao("Odivelas");
	adicionarEstacaoLinhas2(odivelas, 1, linhaAmarela);
	
	struct Estacao *senhorRoubado;
	senhorRoubado = criarEstacao("Senhor Roubado");
	adicionarEstacaoLinhas2(senhorRoubado, 1, linhaAmarela);
	
	struct Estacao *ameixoeira;
	ameixoeira = criarEstacao("Ameixoeira");
	adicionarEstacaoLinhas2(ameixoeira, 1, linhaAmarela);
	
	struct Estacao *campoGrande;
	campoGrande = criarEstacao("Campo Grande");
	adicionarEstacaoLinhas2(campoGrande, 2, linhaAmarela, linhaVerde);
	
	struct Estacao *telheiras;
	telheiras = criarEstacao("Telheiras");
	adicionarEstacaoLinhas2(telheiras, 1, linhaAmarela);
	
	/*linha verde*/
	struct Estacao *estrela;
	estrela = criarEstacao("Estrela");
	adicionarEstacaoLinhas2(estrela, 1, linhaVerde);
	
	struct Estacao *marques;
	marques = criarEstacao("Marques de Pombal");
	adicionarEstacaoLinhas2(marques, 2, linhaVerde, linhaAzul);
	
	struct Estacao *campoPequeno;
	campoPequeno = criarEstacao("Campo Pequeno");
	adicionarEstacaoLinhas2(campoPequeno, 1, linhaVerde);
	
	struct Estacao *entreCampos;
	entreCampos = criarEstacao("Entre Campos");
	adicionarEstacaoLinhas2(entreCampos, 1, linhaVerde);
	
	struct Estacao *cidadeUniversitaria;
	cidadeUniversitaria = criarEstacao("Cidade Universitaria");
	adicionarEstacaoLinhas2(cidadeUniversitaria, 1, linhaVerde);
	
	struct Estacao *roma;
	roma = criarEstacao("Roma");
	adicionarEstacaoLinhas2(roma, 1, linhaVerde);
	
	struct Estacao *intendente;
	intendente = criarEstacao("Intendente");
	adicionarEstacaoLinhas2(intendente, 1, linhaVerde);
	
	struct Estacao *rossio;
	rossio = criarEstacao("Rossio");
	adicionarEstacaoLinhas2(rossio, 1, linhaVerde);
	
	struct Estacao *baixaChiado;
	baixaChiado = criarEstacao("Baixa-Chiado");
	adicionarEstacaoLinhas2(baixaChiado, 2, linhaVerde, linhaAzul);
	
	struct Estacao *caisSodre;
	caisSodre = criarEstacao("Cais de Sodre");
	adicionarEstacaoLinhas2(caisSodre, 1, linhaVerde);
	
	/*linha azul*/
	struct Estacao *reboleira;
	reboleira = criarEstacao("Reboleira");
	adicionarEstacaoLinhas2(reboleira, 1, linhaAzul);
	
	struct Estacao *colegioMilitar;
	colegioMilitar = criarEstacao("Colegio Militar/Luz");
	adicionarEstacaoLinhas2(colegioMilitar, 1, linhaAzul);
	
	struct Estacao *jardimZoologico;
	jardimZoologico = criarEstacao("Jardim Zoologico");
	adicionarEstacaoLinhas2(jardimZoologico, 1, linhaAzul);
	
	struct Estacao *pracaEspanha;
	pracaEspanha = criarEstacao("Praca Espanha");
	adicionarEstacaoLinhas2(pracaEspanha, 1, linhaAzul);
	
	struct Estacao *restaurantes;
	restaurantes = criarEstacao("Restaurantes");
	adicionarEstacaoLinhas2(restaurantes, 1, linhaAzul);
	
	struct Estacao *terreiroPaco;
	terreiroPaco = criarEstacao("Terreiro do Paco");
	adicionarEstacaoLinhas2(terreiroPaco, 1, linhaAzul);
	
	struct Estacao *santaApolonia;
	santaApolonia = criarEstacao("Santa Apolonia");
	adicionarEstacaoLinhas2(santaApolonia, 1, linhaAzul);
}

// Fun��o para encontrar a linha correspondente
struct Linha *encontrarLinha(struct Linha *linhas, char nomeLinha[]) {
    struct Linha *linhaAtual = linhas;
    while (linhaAtual != NULL) {
        if (strcmp(linhaAtual->nome, nomeLinha) == 0) {
            return linhaAtual;
        }
        linhaAtual = linhaAtual->prox;
    }
    return NULL; // Linha n�o encontrada
}

// Fun��o para alterar dados de uma esta��o
void alterarDadosEstacao(struct Estacao *estacao, char novoNome[]) {
    strcpy(estacao->nome, novoNome);
}

// Fun��o para contar o n�mero total de esta��es
int ContagemTotalEstacoes(struct Linha *linhas) {
    int totalEstacoes = 0;

    // Percorre todas as linhas
    struct Linha *linhaAtual = linhas;
    while (linhaAtual != NULL) {
        // Percorre todas as esta��es da linha
        struct Estacao *estacaoAtual = linhaAtual->estacoes;
        while (estacaoAtual != NULL) {
            totalEstacoes++;
            estacaoAtual = estacaoAtual->prox;
        }
        linhaAtual = linhaAtual->prox;
    }

    return totalEstacoes;
}

// Fun��o para buscar uma esta��o pelo nome
struct Estacao *buscarEstacao(struct Linha *linhas, char nomeEstacao[]) {
    struct Linha *linhaAtual = linhas;
    while (linhaAtual != NULL) {
        struct Estacao *estacaoAtual = linhaAtual->estacoes;
        while (estacaoAtual != NULL) {
            if (strcmp(estacaoAtual->nome, nomeEstacao) == 0) {
                return estacaoAtual;
            }
            estacaoAtual = estacaoAtual->prox;
        }
        linhaAtual = linhaAtual->prox;
    }
    return NULL; // Esta��o n�o encontrada
}

void buscarEstacoesComMaisConexoes(struct Linha *listaLinhas) {
    int i;
    int j;
    int maxConexoes = 0;
    struct Estacao *estacoesMaxConexoes[50]; // Assumindo que pode haver at� 50 esta��es
    int numEstacoesMaxConexoes = 0;

    struct Linha *linhaAtual = listaLinhas;
    while (linhaAtual != NULL) {
        struct Estacao *estacaoAtual = linhaAtual->estacoes;
        while (estacaoAtual != NULL) {
            // Contar o n�mero de conex�es da esta��o
            int numConexoes = 0;
            for (i = 0; i < MAX_LINHAS_ESTACAO; i++) {
                if (estacaoAtual->linhas[i] != NULL) {
                    numConexoes++;
                }
            }

            // Se o n�mero de conex�es for maior ou igual ao m�ximo encontrado at� agora
            if (numConexoes >= maxConexoes) {
                // Se for estritamente maior, limpar a lista e adicionar esta esta��o
                if (numConexoes > maxConexoes) {
                    numEstacoesMaxConexoes = 0;
                    maxConexoes = numConexoes;
                }

                // Verificar se o nome da esta��o j� foi impresso antes
                int nomeRepetido = false;
                for (j = 0; j < numEstacoesMaxConexoes; j++) {
                    if (estacoesMaxConexoes[j] == estacaoAtual) {
                        nomeRepetido = true;
                        break;
                    }
                }

                // Adicionar esta esta��o � lista se o nome n�o foi impresso antes
                if (!nomeRepetido) {
                    estacoesMaxConexoes[numEstacoesMaxConexoes++] = estacaoAtual;
                }
            }

            estacaoAtual = estacaoAtual->prox;
        }
        linhaAtual = linhaAtual->prox;
    }

    // Mostrar as esta��es com mais conex�es
    if (numEstacoesMaxConexoes > 0) {
        printf("\nEstacoes com mais conexoes (%d conexoes):\n", maxConexoes);
        for (i = 0; i < numEstacoesMaxConexoes; i++) {
            printf("- %s\n", estacoesMaxConexoes[i]->nome);
        }
    } else {
        printf("Nenhuma estacao encontrada.\n");
    }
}

struct Linha *encontrarUltimaLinha(struct Linha *linhas) {
    if (linhas == NULL) {
        return NULL; // Lista vazia, n�o h� �ltima linha
    }

    struct Linha *linhaAtual = linhas;
    while (linhaAtual->prox != NULL) {
        linhaAtual = linhaAtual->prox;
    }

    return linhaAtual;
}

void fecharEstacaoTemp(struct Estacao *estacao) {
    // Marcar a esta��o como fechada temporariamente
    estacao->status = FECHADOTEMP;
}

void abrirNovamenteEstacao(struct Estacao *estacao) {
    // Marcar a esta��o como aberta novamente
    estacao->status = ABERTO;
}


void mostrarEstacoesFechadasTemp(struct Linha *linhaAtual) {
    if (linhaAtual == NULL) {
        // Caso base: atingiu o final da lista de linhas
        return;
    }

    // Imprime o nome da linha
    printf("Linha: %s\n", linhaAtual->nome);

    // Imprime as esta��es da linha
    struct Estacao *estacaoAtual = linhaAtual->estacoes;
    while (estacaoAtual != NULL) {
    	if(estacaoAtual->status == FECHADOTEMP){
    		printf("  Estacao: %s\n", estacaoAtual->nome);
			estacaoAtual = estacaoAtual->prox; //Mostra as estacoes fechadas temporariamente
		}
		else{
        	estacaoAtual = estacaoAtual->prox;
		}
    }

    // Chama recursivamente para a pr�xima linha
    mostrarEstacoesFechadasTemp(linhaAtual->prox);
}

// Fun��o para verificar se um conjunto de esta��es forma um grafo de Euler
int verificaGrafoEuler(struct Estacao *estacoes[], int numEstacoes) {
	int i;
	int j;
    // Contagem de graus de cada esta��o
    int graus[MAX_LINHAS_ESTACAO] = {0};

    // Calculando os graus de cada esta��o
    for (i = 0; i < numEstacoes; i++) {
        struct Estacao *estacaoAtual = estacoes[i];
        while (estacaoAtual != NULL) {
            for (j = 0; j < MAX_LINHAS_ESTACAO; j++) {
                if (estacaoAtual->linhas[j] != NULL) {
                    graus[i]++;
                }
            }
            estacaoAtual = estacaoAtual->prox;
        }
    }

    // Verificando se todos os graus s�o pares
    for (i = 0; i < numEstacoes; i++) {
        if (graus[i] % 2 != 0) {
            return false; // N�o � um grafo de Euler
        }
    }

    return true; // � um grafo de Euler
}

void identificarEstacoesComAcesso(struct Linha *listaLinhas, struct Estacao *estacaoOrigem, struct Linha *linhaDesejada) {
    struct Estacao *estacaoAtual = estacaoOrigem;

    printf("\nRota:\n\n ");
    printf("%s", estacaoOrigem->nome);

    while (estacaoAtual != NULL) {
        // Verifica se a esta��o seguinte pertence � linha desejada
        if (estacaoAtual->prox != NULL) {
            int i;
            for (i = 0; i < MAX_LINHAS_ESTACAO; i++) {
                if (estacaoAtual->prox->linhas[i] == linhaDesejada) {
                    printf("->%s\n\nEssa e a rota entre a estacao %s para chegar a linha desejada %s.\n", estacaoAtual->prox->nome, estacaoOrigem->nome, linhaDesejada->nome);
                    return;
                }
            }
        }

        // Verifica se h� vizinhos na mesma linha
        int i;
        for (i = 0; i < MAX_LINHAS_ESTACAO; i++) {
            if (estacaoAtual->linhas[i] != NULL && estacaoAtual->linhas[i] != linhaDesejada) {
                struct Estacao *vizinho = estacaoAtual->linhas[i]->estacoes;
                while (vizinho != NULL) {
                    // Verifica se o vizinho pertence � linha desejada
                    if (vizinho->linhas[0] == linhaDesejada) {
                        printf("->%s\n\nEssa e a rota entre a estacao %s para chegar a linha desejada %s.\n", vizinho->nome, estacaoOrigem->nome, linhaDesejada->nome);
                        return;
                    }
                    vizinho = vizinho->prox;
                }
            }
        }

        // Se n�o encontrou a linha desejada, continua navegando
        estacaoAtual = estacaoAtual->prox;
        if (estacaoAtual != NULL) {
            printf("->%s", estacaoAtual->nome);
        }
    }
	printf("\n");
    printf("\nNenhuma estacao encontrada com acesso a linha desejada %s.\n", linhaDesejada->nome);
}

// Fun��o principal de Dijkstra para encontrar a rota mais curta (nao implementado)
/*void dijkstra(struct Linha *linha, struct Estacao *origem, struct Estacao *destino) {
    int numEstacoes = linha->numConexoes;
    int distancias[numEstacoes];
    int visitado[numEstacoes];

    // Inicializa��o
    for (int i = 0; i < numEstacoes; i++) {
        distancias[i] = INF;
        visitado[i] = 0;
    }

    int origemIndex = -1, destinoIndex = -1;
    for (int i = 0; i < numEstacoes; i++) {
        if (linha->conexoes[i].estacao == origem) {
            origemIndex = i;
        }
        if (linha->conexoes[i].estacao == destino) {
            destinoIndex = i;
        }
    }

    if (origemIndex == -1 || destinoIndex == -1) {
        fprintf(stderr, "Erro: Esta��o de origem ou destino n�o encontrada na linha\n");
        return;
    }

    distancias[origemIndex] = 0;

    // Loop principal do algoritmo de Dijkstra
    for (int count = 0; count < numEstacoes - 1; count++) {
        int u = minEstacao(distancias, visitado, numEstacoes);
        visitado[u] = 1;

        // Atualiza as dist�ncias dos vizinhos de u
        for (int v = 0; v < numEstacoes; v++) {
            int tempo = linha->conexoes[u].tempo;
            if (!visitado[v] && tempo != INF && distancias[u] + tempo < distancias[v]) {
                distancias[v] = distancias[u] + tempo;
            }
        }
    }

    printf("Tempo m�nimo entre %s e %s: %d\n", origem->nome, destino->nome, distancias[destinoIndex]);
}*/

int main() {
	int opcao;
    char estacao1[50];
    char estacao2[50];
    char estacao[50];
    char linha[50];
    int numEst;
    char nomeEst[50];
    char nomeEst2[50];
    char nomeEst3[50];
    int opcaoFecharAbrir;
	int opcaoFechar;
	char nomeEstFechada[50];
    char novoNomeEst[50];
    char nomeDaLinhaCriada[50];
    char nomeDaEstacaoCriada[50];
    int qntEstacoes;
    int qntJaExistente;
    int i;
    int j;
    int qntLinhas;
	char nomeDaLinha[50];
	
	// Mapendo as linhas e estacoes
	
	struct Linha *l = NULL;
	
	// Criando algumas linhas
	l = criarLinha("Azul", l);
	l = criarLinha("Rosa", l);
	l = criarLinha("Verde", l);
	l = criarLinha("Amarela", l);
	    
	InsercaoDadosMetro(l, l->prox, l->prox->prox, l->prox->prox->prox);
	
    
    printf("Bem vindo ao sistema de metro da cidade de Lisboa!\n");
    
    do {
    printf("\n");
    printf("O que gostaria de fazer?\n\n");
    
    printf("1. Gerar relatorio\n");
    printf("2. Criar uma nova linha\n");
    printf("3. Criar uma nova estacao\n");
    printf("4. Alterar dados de uma estacao\n");
    printf("5. Fechar/Abrir uma estacao\n");
    printf("6. Mostrar rotas entre 2 estacoes\n"); //pelo menos 2 e mostrar tambem os tempos de viagem 
    printf("7. Mostrar rota que da acesso a uma linha a partir de uma estacao de origem\n");
    printf("8. Imprimir estacoes com mais conexoes\n"); //com mais graus
    printf("9. Verificar se eh um grafo de Euler\n");
    printf("10. Sair do programa\n");
    
    printf("\nDigite uma opcao: ");
    if (scanf("%d", &opcao) == 1) {
    
    switch(opcao){
    	case 1:
    		
    		//Gerando relat�rio
    		gerarRelatorio(l);
    		break;
    	
    	case 2:

		    // Criando uma nova linha
		    /*
			Vari�veis utilizadas do case 2:
			- char nomeDaLinhaCriada[50];
    		- int qntEstacoes;
    		- int qntJaExistente;
			- char nomeEst[50];
			*/
		
		    printf("\nDigite um nome para uma nova linha: ");
			fflush(stdin);
		    gets(nomeDaLinhaCriada);
		
		    printf("\nDigite quantas estacoes estarao vinculadas a essa linha: ");
		    scanf("%d", &qntEstacoes);
		    
		    if(qntEstacoes <= 0){
		    	printf("\nQuantidade invalida\n");
		    	break;
			}
		
		    qntJaExistente = ContagemTotalEstacoes(l);
		    //levar por par�metro o n� cabe�a de linhas para fazer a varredura que vai ser o (l)
		
		    if (qntEstacoes > qntJaExistente){
		        printf("Nao eh possivel adicionar uma linha a uma quantidade maior de estacoes ja existentes\n");
		        break; 
		    }
		    else{
		        l = criarLinha(nomeDaLinhaCriada, l);
		        struct Linha *novaLinha = encontrarUltimaLinha(l);
		        struct Linha *conjTemp1[4] = {novaLinha};
		        int i;
		        for (i = 0; i < qntEstacoes; i++){
			            printf("\nDigite o nome de uma estacao %d ja existente (exemplo: Alameda): ", qntEstacoes);
			            fflush(stdin);
		    			gets(nomeEst);
			            
			            struct Estacao *estacaoExistente = buscarEstacao(l, nomeEst);
			            
			            if (estacaoExistente != NULL){
			                adicionarEstacaoLinhas(estacaoExistente, 1, conjTemp1);
			                printf("\nOperacao concluida!\n");

			            }
			            else{
			                printf("Estacao nao encontrada. Certifique-se de que a estacao existe.\n");
			                i--; // Para repetir a leitura da mesma esta��o
			            }
		        }
		    }
		    break;
    		
    	case 3: 
			//Criando uma nova esta��o
			
			/*
			Vari�veis utilizadas do case 3:
			- char nomeDaEstacaoCriada[50];
			- int qntLinhas;
			- char nomeDaLinha[50];
			- int j;
			*/
			

		    // Criando uma nova esta��o
		    //char nomeDaEstacaoCriada[50];
		    
		    printf("\nDigite um nome para uma nova estacao: ");	    
		    fflush(stdin);
		    gets(nomeDaEstacaoCriada);
		
		    printf("\nDigite quantas linhas estarao vinculadas a essa estacao (max 4 linhas): ");
		    scanf("%d", &qntLinhas);
		    
		    if (qntLinhas > 4 || qntLinhas <= 0) {
		        printf("\nQuantidade invalida\n");
		        break;
		    }
		    
		    struct Linha *linhasAlvo[4];

			struct Estacao *novaEstacao = criarEstacao(nomeDaEstacaoCriada);
			
			for (i = 0; i < qntLinhas; i++) {
	        printf("\nDigite o nome da linha %d que ja existe (exemplo: Azul): ", i + 1);
	        fflush(stdin);
	        gets(nomeDaLinha);
	
	        linhasAlvo[i] = encontrarLinha(l, nomeDaLinha);
	
		        if (linhasAlvo[i] == NULL) {
		            printf("Linha nao encontrada. Certifique-se de que a linha existe.\n");
		            i--;
		        }
		    }
		    
	
		    // Adiciona a esta��o � lista de esta��es das linhas
		    adicionarEstacaoLinhas(novaEstacao, qntLinhas, linhasAlvo);
		    printf("\nEstacao adicionada as %d linhas!\n", qntLinhas);
		    break;
		    
		   	

			
		case 4:
			//Altera��o dos dados de uma esta��o
			
			/*
			Vari�veis utilizadas do case 4:
			- char nomeEst2[50];
			- char novoNomeEst[50];
			*/
			
		    printf("\nDigite o nome da estacao para alterar os seus dados (exemplo: S.Sebastiao): ");
		    fflush(stdin);
		    gets(nomeEst2);
		
		    struct Estacao *estacaoParaAlterar = buscarEstacao(l, nomeEst2);
		
		    if (estacaoParaAlterar == NULL) {
		        printf("\nEstacao nao encontrada.\n");
		        break;
		    } else {
		        printf("\nEstacao encontrada.\n");
		        printf("\nDigite um novo nome para essa estacao: ");
		        fflush(stdin);
		    	gets(novoNomeEst);
		        alterarDadosEstacao(estacaoParaAlterar, novoNomeEst);
		        printf("\nDados da estacao alterados com sucesso!\n");
		    }
		
		    break;
			
		case 5:
			/*Vari�veis utilizadas do case 5 atual:
			int opcaoFecharAbrir;
			char nomeEst3[50];
			int opcaoFechar;
			char nomeEstFechada[50];			
			*/
		
		    // Fechar ou abrir uma esta��o
		    printf("\nDeseja fechar ou abrir uma estacao?\n");
		    printf("1. Fechar estacao\n");
		    printf("2. Abrir estacao\n");
		    printf("\nDigite uma opcao: ");
		    scanf("%d", &opcaoFecharAbrir);
		
		    if (opcaoFecharAbrir == 1) {
		        // Fechar esta��o
		
		        printf("\nDigite o nome da estacao para fechar (exemplo: Saldanha): ");		        
		        fflush(stdin);
		    	gets(nomeEst3);
		
		        struct Estacao *estacaoParaFechar = buscarEstacao(l, nomeEst3);
		
		        if (estacaoParaFechar == NULL) {
		            printf("\nEstacao nao encontrada.\n");
		            break;
		        } else {
		            printf("\nEstacao encontrada.");
		            printf("\n\nComo voce gostaria de fechar a estacao?\n\n");
		            printf("1. Fechar temporariamente\n");
		            printf("2. Fechar permanentemente");
		
		            printf("\n\nDigite uma opcao: ");
		            scanf("%d", &opcaoFechar);
		
		            if (opcaoFechar == 1) {
		                fecharEstacaoTemp(estacaoParaFechar);
		                printf("\nEstacao fechada temporariamente.\n");
		                break;
		            } else if (opcaoFechar == 2) {
		                excluirPermanentementeEstacao(l, nomeEst3);
		                printf("\nEstacao fechada permanentemente.\n");
		            } else {
		                printf("Opcao invalida.\n");
		            }
		        }
		    } else if (opcaoFecharAbrir == 2) {
		        // Abrir esta��o
		        
		        printf("Estacoes fechadas temporariamente:\n\n\n");
		        // Fun��o para mostrar a lista de esta��es fechadas temporariamente
		        mostrarEstacoesFechadasTemp(l);
		        printf("\n\n\n");
		
		        // L�gica para perguntar qual esta��o abrir novamente
		        printf("Digite o nome da estacao que deseja abrir novamente: ");		        
		        fflush(stdin);
		    	gets(nomeEstFechada);
		
		        struct Estacao *estacaoParaAbrir = buscarEstacao(l, nomeEstFechada);
		
		        if (estacaoParaAbrir == NULL) {
		            printf("\nEstacao nao encontrada.\n");
		            break;
		        } else {
		            // Abrir esta��o novamente
		            abrirNovamenteEstacao(estacaoParaAbrir);
		            printf("\nEstacao aberta novamente.\n");
		        }
		    } else {
		        printf("Opcao invalida.\n");
		    }
		
		    break;
			
		case 6: 
			//Mostrar rotas entre 2 estacoes
			
			/*
			Vari�veis utilizadas do case 6:
			- char estacao1[50];
			- char estacao2[50];*/
			
			printf("\n\nDigite a primeira estacao: ");
			fflush(stdin);
		    gets(estacao1);
			
			printf("\n\nDigite a segunda estacao: ");
			fflush(stdin);
		    gets(estacao2);
		    
		    // Buscar ponteiros das esta��es
		    struct Estacao *estacaoprimeira = buscarEstacao(l, estacao1);
		    struct Estacao *estacaosegunda = buscarEstacao(l, estacao2);
		
		    // Verificar se as esta��es foram encontradas
		    if (estacao1 != NULL && estacao2 != NULL) {
		        // Mostrar rotas entre as esta��es
		        //dijkstra(l, estacaoprimeira, estacaosegunda); (nao implementado)
		    } else {
		        printf("Uma ou ambas as estacoes nao foram encontradas.\n");
		    }
			
			break;
			
		case 7:
			/*
			Vari�veis utilizadas do case 7:
			- char estacao[50];
			- char linha[50];
			*/
			
			printf("\n\nDigite a estacao de origem (exemplo: Santa Apolonia): ");
			fflush(stdin);
		    gets(estacao);
			
			printf("\n\nDigite a linha onde quer chegar (exemplo: Verde): ");
			fflush(stdin);
		    gets(linha);
		    
		    struct Estacao *estacaoEnc = buscarEstacao(l, estacao);
		    struct Linha *LinhaEnc = encontrarLinha(l, linha);
		    
		    if((LinhaEnc == NULL) || estacaoEnc == NULL){
		    	printf("\nEstacao ou linha nao encontrada.\n");
		    	break;
			}
						
			identificarEstacoesComAcesso(l, estacaoEnc, LinhaEnc);
			
			break;
			
		case 8:
			buscarEstacoesComMaisConexoes(l);
			break;
			
		case 9:
			/*
			Vari�veis utilizadas do case 9:
			- int numEst;
			*/
			
			printf("\nDigite quantas estacoes voce quer analisar: ");
		    scanf("%d", &numEst);
		    
		    struct Estacao *conjuntoEst[50];
		
		    for (i = 0; i < numEst; i++) {
			    do {
			        printf("\nDigite o nome da estacao %d que voce quer analisar (exemplo: Restaurantes): ", i + 1);
			        fflush(stdin);
			        gets(nomeDaLinha);
			
			        struct Estacao *estacaoEncontrada = buscarEstacao(l, nomeDaLinha);
			
			        if (estacaoEncontrada == NULL) {
			            printf("Estacao nao encontrada. Tente novamente.\n");
			        } else {
			            conjuntoEst[i] = estacaoEncontrada;
			        }
			    } while (conjuntoEst[i] == NULL);
			}
		
		    printf("\n\nVerificando se eh um grafo de euler...");
		
		    if (verificaGrafoEuler(conjuntoEst, numEst)) {
		        printf("\nO conjunto de estacoes forma um grafo de Euler.\n");
		    } else {
		        printf("\nO conjunto de estacoes nao forma um grafo de Euler.\n");
		    }
		    break;

			
		case 10:
			printf("\nPrograma encerrando...\n");
			break;
			
		default:
			printf("\nOpcao invalida. Escolha um numero de 1 a 10.\n");
			break;						
		}
	}else {
        printf("\nOpcao invalida. Escolha um numero de 1 a 10.\n");
        scanf("%*s"); // Limpa o buffer de entrada para evitar loops infinitos
    }
    } while (opcao != 10);
    

    return 0;
}

