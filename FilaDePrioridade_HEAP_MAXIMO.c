/*********************************************************************/
/**   ACH2023 - Algoritmos e Estruturas de Dados I                  **/
/**   EACH-USP - Segundo Semestre de 2020                           **/
/**   <turma> - Prof. Luciano Antonio Digiampietri                  **/
/**                                                                 **/
/**   EP3 - Fila de Prioridade (utilizando heap)                    **/
/**                                                                 **/
/**   <Gustavo Ryan Nascimento Silva>         <11796785>            **/
/**                                                                 **/
/*********************************************************************/

#include "filaDePrioridade2.h"

PFILA criarFila(int max){
  PFILA res = (PFILA) malloc(sizeof(FILADEPRIORIDADE));
  res->maxElementos = max;
  res->arranjo = (PONT*) malloc(sizeof(PONT)*max);
  res->heap = (PONT*) malloc(sizeof(PONT)*max);
  int i;
  for (i=0;i<max;i++) {
    res->arranjo[i] = NULL;
    res->heap[i] = NULL;
  }
  res->elementosNoHeap = 0;
  return res;
}

void exibirLog(PFILA f){
  printf("Log [elementos: %i]\n", f->elementosNoHeap);
  PONT atual;
  int i;
  for (i=0;i<f->elementosNoHeap;i++){
    atual = f->heap[i];
    printf("[%i;%f;%i] ", atual->id, atual->prioridade, atual->posicao);
  }
  printf("\n\n");
}

/*
 * Funcao para reorganizar o no "novo" na fila "f" comparando ele com os seus pais e alterando ate que
 * o heap esteja correto.
 * Basicamente esse reorganiza faz uma comparacao de baixo pra cima.
 */
bool reorganiza(PFILA f, PONT novo){
    bool res = false;
    /*
     * "Continha" para pegar o pai do no atual.
     * No while eu estou comparado se a prioridade do pai eh maior que a do no atual, enquanto isso for verdade
     * o codigo dentro do while acerta os ponteiros necessarios e os campos posicao do pai e do filho.
     */
    int pai = (novo->posicao -1)/2;
    while (f->heap[pai]->prioridade < novo->prioridade) {
        PONT auxx = f->heap[pai];
        f->heap[pai] = novo;
        f->heap[novo->posicao] = auxx;
        auxx->posicao = novo->posicao;
        novo->posicao = pai;
        f->heap[pai]->posicao = novo->posicao;
        pai = (novo->posicao - 1)/2;
        res = true;
    }
    return res;
}

/*
 * Funcao para reorganizar o no "novo" na fila "f" comparando ele com os seus filhos e alterando ate que
 * o heap esteja correto.
 * Basicamente esse reorganizaInverso faz uma comparacao de cima pra baixo.
 */
bool reorganizaInverso(PFILA f,PONT noAtual){
    /*
     * Conferindo se o noAtual a ser reorganizado eh valido.
     * Depois guardo os filhos esquerdo e direito e o "novoPai"(o filho maior entre o direito e o esquerdo).
     */
    if (f->heap[noAtual->posicao] == NULL) return false;
    int filhoEsq = (2*noAtual->posicao)+1;
    int filhoDir = (2*noAtual->posicao)+2;
    int novoPai;

    /*
     * Aqui confiro se o filho esquerdo existe, se nao existir retorno false pois ele sempre eh o primeiro a ser
     * preenchido, ou seja, se ele nao existe, nao existe o direito tambem.
     *
     * No 2o if confiro se o direito eh nulo, e se for, entao o esquerdo eh o maior.
     */
    if (f->heap[filhoEsq] == NULL) return false;
    if (f->heap[filhoDir] == NULL) novoPai = filhoEsq;

    /*
     * No 1o if confiro se a posicao eh valida(menor que a qtde de elementos no heap e se existe algo la)
     *
     * No 2o if eu comparo pra descobrir qual o maior e coloco ele dentro dno "novoPai"
     */
    if (filhoDir < f->elementosNoHeap && f->heap[filhoDir] != NULL) {
        if (f->heap[filhoDir]->prioridade > f->heap[filhoEsq]->prioridade) {
            novoPai = filhoDir;
        } else {
            novoPai = filhoEsq;
        }
    }

    /*
     * No 1o if confiro se a prioridade do no atual eh maior ou igual a do filho maior(novoPai), se for, retorna false.
     * Depois, guardo em 'aux' a posicao do novoPai no heap pra poder realocar os ponteiros sem perder ele "de vista"
     * Apos isso so acerto os ponteiros necessarios pra trocar o novoPai(filho maior) com o noAtual(pai atual)
     */
    if(noAtual->prioridade >= f->heap[novoPai]->prioridade) return false;
    int aux = f->heap[novoPai]->posicao;
    f->heap[noAtual->posicao] = f->heap[novoPai];
    f->heap[novoPai] = noAtual;
    f->heap[noAtual->posicao]->posicao = noAtual->posicao;
    f->heap[novoPai]->posicao = novoPai;

    /*
     * Realizo uma nova chamada da propria funcao(recursao) pra que a mudanca seja feita ate que o heap esteja balanceado,
     * quando o heap estiver balanceado, a funcao retornara false no if anterior a esse(que compara a prioridade do noAtual
     * com a do pai atual) e a recursao acabara.
     */
    reorganizaInverso(f,noAtual);

    return true;
}

/*
 * Funcao tamanho, basicamente confere se ha elementos no heap e se sim, responde quantos.
 */
int tamanho(PFILA f){
  int tam = 0;
  int i = 0;
  if (f->elementosNoHeap > 0) tam = f->elementosNoHeap;

  return tam;
}

bool inserirElemento(PFILA f, int id, float prioridade){
  bool res = false;
  /*
   * Verifico se o id eh valido e se a posicao desse id no arranjo nao eh nula.
   */
    if (id < 0 || id > f->maxElementos) return res;
    if (f->arranjo[id] != NULL) return res;

    /*
     * Aloco a memoria para o novo elemento e ajusto os campos necessarios.
     */
    PONT novo = malloc(sizeof(ELEMENTO));
    novo->id = id;
    novo->prioridade = prioridade;
    f->arranjo[id] = novo;
    int tam = tamanho(f);
    int i = 0;
    /*
     * Aqui confiro se o tamanho eh 0, se for, o elemento a ser inserido sera o 1o do heap
     * se nao for, adiciona o novo elemento na posicao "tam" que corresponde ao tamanho do heap
     * e acerta os campos necessarios.
     */
    if (tam == 0){
        f->heap[0] = novo;
        novo->posicao = 0;
        f->elementosNoHeap++;
        res = true;
    } else {
        f->heap[tam] = novo;
        novo->posicao = tam;
        f->elementosNoHeap++;
        res = true;
    }

    /*
     * Chamada da funcao reorganiza para, se necessario, balancear o heap.
     */
    reorganiza(f,novo);
  return res;
}

bool aumentarPrioridade(PFILA f, int id, float novaPrioridade){
  bool res = false;
  /*
   * Nos 3 IF abaixo verifico se o id eh valido, se a posicao no arranjo nao eh nula e se a prioridade nova eh maior que a atual.
   */
    if (id<0 || id > f->maxElementos) return res;
    if (f->arranjo[id] == NULL) return res;
    if (f->arranjo[id]->prioridade >= novaPrioridade) return res;

    /*
     * Passada todas as condicoes, ajusto as prioridades e chamo a funcao reorganiza pra balancear o heap, ou seja,
     * encaixar o novo elemento na sua nova posicao(se existir), conforme sua nova prioridade.
     */
    f->arranjo[id]->prioridade = novaPrioridade;
    f->heap[f->arranjo[id]->posicao]->prioridade = novaPrioridade;
    reorganiza(f,f->heap[f->arranjo[id]->posicao]);
    res = true;


  return res;
}

bool reduzirPrioridade(PFILA f, int id, float novaPrioridade){
  bool res = false;
  /*
   * Nos 3 IF abaixo verifico se o id eh valido, se a posicao no arranjo nao eh nula e se a prioridade nova eh menor ou igual que a atual.
   */
    if (id < 0 || id > f->maxElementos) return res;
    if (f->arranjo[id] == NULL) return res;
    if (f->arranjo[id]->prioridade <= novaPrioridade) return res;

    /*
     * Ajusto as prioridades e chamo a funcao reorganizaInverso para balancear o heap, ou seja,
     * encaixar o novo elemento na sua nova posicao(se existir), conforme sua nova prioridade.
     */
    f->arranjo[id]->prioridade = novaPrioridade;
    f->heap[f->arranjo[id]->posicao]->prioridade = novaPrioridade;
    reorganizaInverso(f,f->heap[f->arranjo[id]->posicao]);
    res = true;
  return res;
}

PONT removerElemento(PFILA f){
  PONT res = NULL;
  /*
   * 1o confiro se a fila nao esta vazia.
   *
   * Depois verifico se existe apenas 1 elemento no heap, se sim, removo somente ele e acerto o necessario.
   *
   * Havendo mais de 1 elemento no heap, removo o de prioridade maxima e arrumo os ponteiros, depois chamo
   * a funcao reorganizaInverso para que ela balanceie o heap novamente.
   */
    if (tamanho(f) == 0) return res;
    if (f->elementosNoHeap == 1){
        res = f->heap[0];
        f->arranjo[f->heap[0]->id] = NULL;
        f->heap[0] = NULL;
        f->elementosNoHeap--;
    } else {
        res = f->heap[0];
        f->arranjo[f->heap[0]->id] = NULL;
        f->heap[0] = f->heap[f->elementosNoHeap - 1];
        f->heap[0]->posicao = 0;
        f->heap[f->elementosNoHeap - 1] = NULL;
        reorganizaInverso(f, f->heap[0]);
        f->elementosNoHeap--;
    }


  return res;
}

bool consultarPrioridade(PFILA f, int id, float* resposta){
  bool res = false;
  /*
   * Verifico se o id eh valido e se a posicao no arranjo eh valida.
   *
   * Depois, guardo a prioridade de id solicitado na variavel resposta e retorno true.
   */
    if (id < 0 || id >= f->maxElementos) return res;
    if (f->arranjo[id] == NULL) return res;
    *resposta = f->arranjo[id]->prioridade;
    res = true;

  return res;
}

