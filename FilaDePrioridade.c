/*********************************************************************/
/**   ACH2023 - Algoritmos e Estruturas de Dados I                  **/
/**   EACH-USP - Segundo Semestre de 2020                           **/
/**   <turma> - Prof. Luciano Antonio Digiampietri                  **/
/**                                                                 **/
/**   EP1 - Fila de Prioridade                                      **/
/**                                                                 **/
/**   <nome do(a) aluno(a)>                   <numero USP>          **/
/**                                                                 **/
/*********************************************************************/

#include "filaDePrioridade.h"

PFILA criarFila(int max){
    PFILA res = (PFILA) malloc(sizeof(FILADEPRIORIDADE));
    res->maxElementos = max;
    res->arranjo = (PONT*) malloc(sizeof(PONT)*max);
    int i;
    for (i=0;i<max;i++) res->arranjo[i] = NULL;
    PONT cabeca = (PONT) malloc(sizeof(ELEMENTO));
    res->fila = cabeca;
    cabeca->ant = cabeca;
    cabeca->prox = cabeca;
    cabeca->id = -1;
    cabeca->prioridade = 1000000;
    return res;
}

void exibirLog(PFILA f){
    printf("Log [elementos: %i (alem do no cabeca)]\n", tamanho(f));
    PONT atual = f->fila;
    printf("%p[%i;%f;%p]%p ", atual->ant, atual->id, atual->prioridade, atual, atual->prox);
    atual = atual->prox;
    while (atual != f->fila){
        printf("%p[%i;%f;%p]%p ", atual->ant, atual->id, atual->prioridade, atual, atual->prox);
        atual = atual->prox;
    }
    printf("\nElementos validos: ");
    atual = atual->prox;
    while (atual != f->fila){
        printf("[%i;%f;%p] ", atual->id, atual->prioridade, atual);
        atual = atual->prox;
    }

    printf("\nValores do arrajo:\n\[ ");
    int x;
    for (x=0;x<f->maxElementos;x++) printf("%p ",f->arranjo[x]);
    printf("]\n\n");
}

/*
 * Funcao que reposiciona o elemento de ID informado por parametro na fila
 * que tambem eh informada por parametro caso ele esteja em alguma posicao
 * incorreta de acordo com sua prioridade.
 */
bool reposiciona(PFILA f, int id){
    bool resposta = false;

    /*
     * Ponteiro auxiliar pra percorrer a fila ate encontrar uma prioridade menor que a do elemento no ID passado.
     */
    PONT aux = f->fila->prox;
    while (aux != f->fila){
        if (f->arranjo[id]->prioridade > aux->prioridade){
            break;
        }
        aux = aux->prox;
    }
    /*
     * Organizando as posicoes de acordo com as prioridades para encaixar o elemento em questao no seu local correto
     * na fila, mudando seus anteriores e proximos e os de aux tambem.
     */

    /*
     * 1. O proximo do anterior de aux sera o proprio elemento.
     * 2. O prox do elemento sera aux.
     * 3. O anterior do elemento sera o anterior de aux.
     * 4. O anterior de aux sera o proprio elemento.
     */
    aux->ant->prox = f->arranjo[id];
    f->arranjo[id]->prox = aux;
    f->arranjo[id]->ant = aux->ant;
    aux->ant = f->arranjo[id];
    resposta = true;
    return resposta;
}

/*
 * Funcao que retorna true se o elemento estiver na fila e false se nao tiver.
 */
bool estaNaFila(PFILA f, int id){
    bool resposta = false;
    if (f->arranjo[id] != NULL){
        resposta = true;
        return resposta;
    }
    return resposta;
}

int tamanho(PFILA f){
    int tam = 0;

    /*
     * Fila eh o no cabeca
     */
    for (int i = 0; i < f->maxElementos-1; ++i) {
        PONT aux = f->arranjo[i];
        if (aux != NULL) {
            tam++;
            aux = aux->prox;
        }
    }

    return tam;
}



bool inserirElemento(PFILA f, int id, float prioridade){
    bool resposta = false;

    /*
     * Ponteiro auxiliar que aponta pro primeiro elemento valido da fila.
     */
    if (id<0 || id>=f->maxElementos) return resposta;
    if (estaNaFila(f,id) == true) return resposta;

    /*
     * Alocacao de memoria para o novo item e insercao dele no arranjo assim como
     * o preenchimento da sua prioridade conforme passado por parametro.
     *
     * A partir disso eu chamo a funcao reposiciona que criei la em cima passando
     * como parametro o ponteiro PFila f passada na assinatura da funcao inserirElemento
     * e o id que tambem foi dado na assinatura para organizar a fila inserindo o novo
     * elemento na sua devida posicao.
     */
    PONT novo = (PONT) malloc(sizeof(ELEMENTO));
    novo->id = id;
    novo->prioridade = prioridade;

    f->arranjo[id] = novo;
     if (reposiciona(f,id) == true) {
        resposta = true;
    }
    return resposta;
}



bool aumentarPrioridade(PFILA f, int id, float novaPrioridade){
    bool resposta = false;

    if (id<0 || id>f->maxElementos) return resposta;
    if (estaNaFila(f,id) == false) return resposta;
    if (f->arranjo[id]->prioridade >= novaPrioridade) return resposta;

    /*
     * Rearranjo os ponteiros do elemento de indice ID de forma que ele "saia" da fila para poder realoca-lo com a sua
     * nova prioridade, usando a funcao reposiciona.
     */
    f->arranjo[id]->prioridade = novaPrioridade;
    f->arranjo[id]->prox->ant = f->arranjo[id]->ant;
    f->arranjo[id]->ant->prox = f->arranjo[id]->prox;
    reposiciona(f,id);
    resposta = true;
    return resposta;
}



bool reduzirPrioridade(PFILA f, int id, float novaPrioridade){
    bool resposta = false;

    if (id<0 || id >= f->maxElementos) return resposta;
    if (estaNaFila(f,id) == false) return resposta;
    if (f->arranjo[id]->prioridade <= novaPrioridade) return resposta;

    /*
     * Rearranjo os ponteiros do elemento de indice ID de forma que ele "saia" da fila para poder realoca-lo com a sua
     * nova prioridade, usando a funcao reposiciona.
     */
    f->arranjo[id]->prioridade = novaPrioridade;
    f->arranjo[id]->prox->ant = f->arranjo[id]->ant;
    f->arranjo[id]->ant->prox = f->arranjo[id]->prox;
    resposta = reposiciona(f,id);
    resposta = true;
    return resposta;
}



PONT removerElemento(PFILA f){

    PONT resposta = NULL;


    int tam = tamanho(f);
    if (tam == 0) return resposta;

    /*
     * Guardo o 1o elemento da fila na variavel resposta e arrumo os ponteiros para que o 2o elemento seja agora o 1o
     */
    resposta = f->fila->prox;
    f->fila->prox = resposta->prox;
    resposta->prox->ant = f->fila;
    f->arranjo[resposta->id] = NULL;


    return resposta;

}



bool consultarPrioridade(PFILA f, int id, float* resposta){
    bool resp = false;

    if (id < 0 || id >= f->maxElementos) return resp;
    if (estaNaFila(f,id) == false) return resp;

    *resposta = f->arranjo[id]->prioridade;
    resp = true;

    return resp;
}

