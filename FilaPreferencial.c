/*********************************************************************/
/**   ACH2023 - Algoritmos e Estruturas de Dados I                  **/
/**   EACH-USP - Segundo Semestre de 2020                           **/
/**   <T04> - Prof. Luciano Antonio Digiampietri                    **/
/**                                                                 **/
/**   EP2 - Fila Preferencial                                       **/
/**                                                                 **/
/**   <Gustavo Ryan Nascimento Silva>         <11796785>            **/
/**                                                                 **/
/*********************************************************************/

#include "filapreferencial.h"

PFILA criarFila(){
  PFILA res = (PFILA) malloc(sizeof(FILAPREFERENCIAL));
  res->inicioPref = NULL;
  res->fimPref = NULL;
  res->inicioGeral = NULL;
  res->fimGeral = NULL;
  return res;
}

int tamanho(PFILA f){
  PONT atual = f->inicioGeral;
  int tam = 0;
  while (atual) {
    atual = atual->prox;
    tam++;
  }
  return tam;
}

int tamanhoFilaPreferencial(PFILA f){
  PONT atual = f->inicioPref;
  int tam = 0;
  while (atual) {
    atual = atual->prox;
    tam++;
  }
  return tam;
}

PONT buscarID(PFILA f, int id){
  PONT atual = f->inicioGeral;
   while (atual) {
    if (atual->id == id) return atual;
    atual = atual->prox;
  }
  return NULL;
}

void exibirLog(PFILA f){
  int numElementos = tamanho(f);
  printf("\nLog fila geral [elementos: %i] - Inicio:", numElementos);
  PONT atual = f->inicioGeral;
  while (atual){
    printf(" [%i;%i]", atual->id, atual->ehPreferencial);
    atual = atual->prox;
  }
  printf("\n");
  numElementos = tamanhoFilaPreferencial(f);
  printf("\nLog fila preferencial [elementos: %i] - Inicio:", numElementos);
  atual = f->inicioPref;
  while (atual){
    printf(" [%i;%i]", atual->id, atual->ehPreferencial);
    atual = atual->prox;
  }
  printf("\n\n");
}


bool inserirPessoaNaFila(PFILA f, int id, bool ehPreferencial) {
    bool resposta = false;

    if (id < 0 || buscarID(f, id) != NULL) return resposta;

    /*
     * Aqui estou alocando memoria e preenchendo os campos do novo elemento da fila.
     */
    PONT novo = (PONT) malloc(sizeof(ELEMENTO));
    novo->id = id;
    novo->ehPreferencial = ehPreferencial;
    novo->prox = NULL;

    /*
     * checando se a fila esta vazia, e no caso de estar colocando o novo elemento como o 1o e unico elemento da fila.
     * caso não esteja eu apenas coloco esse novo elemento no fim da fila normalmente.
     */
    if (f->inicioGeral == NULL){
        f->inicioGeral = novo;
        f->fimGeral = novo;

    } else {
        f->fimGeral->prox = novo;
        f->fimGeral = novo;
    }

    /*
     * Checando se ele eh preferencial, e caso seja, coloco ele na fila preferencial tambem seguindo a mesma logica
     * da insercao anterior, se ele estiver entrando em uma fila vazia ele eh o 1o e unico, se tiver mais gente na fila
     * ele entra no fim dela.
     */
    if (ehPreferencial == true) {
        PONT novoP = malloc(sizeof(ELEMENTO));
        novoP->id = id;
        novoP->ehPreferencial = ehPreferencial;
        novoP->prox = NULL;

        if (f->inicioPref == NULL) {
            f->inicioPref = novoP;
            f->inicioPref->prox = NULL;
            f->fimPref = novoP;
        } else {
                f->fimPref->prox = novoP;
                f->fimPref = novoP;
        }
    }

    resposta = true;
    return resposta;
}


bool atenderPrimeiraDaFilaPreferencial(PFILA f, int* id) {
    bool resposta = false;

    if (f->inicioGeral == NULL) return resposta;

    /*
     * Checando se o 1o da fila preferencial nao eh nulo, e nao sendo eu "atendo" esse 1o da fila, removendo ele de la
     * e acertando os ponteiros pra liberar a memoria dele e pro antigo 2o lugar ser agora o 1o.
     */

    if (f->inicioPref != NULL) {

        PONT excluir = f->inicioPref;
        *id = f->inicioPref->id;
        f->inicioPref = f->inicioPref->prox;
        free(excluir);
        resposta = true;

        /*
         * Aqui eu fiz um IF que verifica se a "pessoa" a ser atendida eh a primeira na fila geral tambem
         * nao sendo a 1a eu fiz um while pra encontrar ela na geral pra poder remove-la de la tambem.
         * Sendo a 1a da geral eu apenas acerto os ponteiros pra remove-la.
         */
        if (f->inicioGeral->id != *id) {
            PONT ondeEsta = f->inicioGeral;
            while (ondeEsta != f->fimGeral) {
                if (ondeEsta->prox->id == *id) {
                    PONT excluir = ondeEsta->prox;
                    ondeEsta->prox = ondeEsta->prox->prox;
                    free(excluir);
                    resposta = true;
                    break;
                } else ondeEsta = ondeEsta->prox;
            }
        } else {
            PONT excluir = f->inicioGeral;
            *id = f->inicioGeral->id;
            f->inicioGeral = f->inicioGeral->prox;
            free(excluir);
            resposta = true;
        }
        /*
         * Aqui verifico se a fila preferencial esta vazia, com ela vazia eu nao tenho quem remover de la, e, seguindo as
         * instrucoes passadas no PDF eu preciso remover o 1o da fila geral.
         */
    } else if (f->inicioPref == NULL) {
        PONT excluir = f->inicioGeral;
        *id = f->inicioGeral->id;
        f->inicioGeral = f->inicioGeral->prox;
        free(excluir);
        resposta = true;
    }
        return resposta;
}



bool atenderPrimeiraDaFilaGeral(PFILA f, int* id){
  bool resposta = false;

    if (f->inicioGeral == NULL) return resposta;

    /*
     * Aqui verifico se o 1o da fila geral eh preferencial, sendo preferencial eu removo ele da fila geral(pois ele esta
     * sendo atendido) e depois verifico se a fila preferencial nao esta vazia(pra evitar segmentation fault),
     * a partir disso fiz um while pra procurar esse elemento na fila preferencial e removo ele de la, acertando os ponteiros.
     */
    if (f->inicioGeral->ehPreferencial == true) {
        *id = f->inicioGeral->id;
        PONT excluir = f->inicioGeral;
        f->inicioGeral = f->inicioGeral->prox;
        free(excluir);

        if (f->inicioPref != NULL) {
            PONT ondeEsta = f->inicioPref;

            while (ondeEsta != f->fimPref) {
                if (ondeEsta->prox->id == *id) {
                    PONT excluirPreferencial = ondeEsta->prox;
                    ondeEsta->prox = ondeEsta->prox->prox;
                    free(excluirPreferencial);
                    resposta = true;
                    break;
                } else ondeEsta = ondeEsta->prox;
            }
        }
        /*
         * Aqui pro caso da fila preferencial estar vazia, removo o 1o da fila geral.
         */
    } else {
        *id = f->inicioGeral->id;
        PONT excluirD = f->inicioGeral;
        f->inicioGeral = f->inicioGeral->prox;
        free(excluirD);
        resposta = true;
    }

  return resposta;

}



bool desistirDaFila(PFILA f, int id){
  bool resposta = false;

    if (!buscarID(f,id)) return resposta;

    PONT ant = f->inicioGeral;
    PONT saindo = f->inicioGeral;
    /*
     * Aqui fiz 2 while pra percorrer a fila geral e encontrar o elemento a ser removido pelo ID passado.
     * Armazeno nos 2 PONT ant(de anterior) e saindo(que eh quem esta desistindo da fila).
     * Guardo o anterior a ele pra facilitar a organizacao dos ponteiros pos-saida.
     */
 while (saindo != f->fimGeral){
     if (saindo->id == id){
         break;
     } else saindo = saindo->prox;
 }

 while (ant != f->fimGeral){
     if (ant->prox == saindo){
         break;
     } else ant = ant->prox;
 }

 /*
  * Aqui faco a mesma coisa explicada no comentario anterior so que pra fila preferencial, portanto crio uma variacao
  * dos PONT ja visto acima, e percorro a fila preferencial com o mesmo objetivo dos while anteriores.
  */
    PONT antP = f->inicioPref;
    PONT saindoP = f->inicioPref;

 if (saindo->ehPreferencial == true){

     while (saindoP->prox != f->fimPref){
         if (saindoP->id == id){
             break;
         } else saindoP = saindoP->prox;
     }

     while (antP != f->fimPref){
         if (antP->prox == saindoP){
             break;
         } else antP = antP->prox;
     }

     /*
      * Aqui fiz um if pra cada caso especifico, o primeiro eh pra caso o elemento que esta saindo seja o 1o da fila preferencial
      * se ele for o 1o, ele eh removido acertando os ponteiros necessarios, se nao for o 1o, o codigo do else roda,
      * removendo ele da lista e acertando os ponteiros necessarios tambem.
      *
      * Eh bom ressaltar o cuidado de verificar se apos a remocao a fila fica vazia, e caso fique, setando o fim para NULL(assim como o
      * inicio ja esta)
      */
     if (id == f->inicioPref->id){
         PONT sairPF = f->inicioPref;
         f->inicioPref = f->inicioPref->prox;
         free(sairPF);
         resposta = true;
         if (f->inicioPref == NULL) f->fimPref = NULL;
     } else {
         PONT excluirP = saindoP;
         antP->prox = saindoP->prox;
         free(excluirP);
         resposta = true;
     }

     /*
     * Aqui fiz um if pra cada caso especifico, o primeiro eh pra caso o elemento que esta saindo seja o 1o da fila geral
     * se ele for o 1o, ele eh removido acertando os ponteiros necessarios, se nao for o 1o, o codigo do else roda,
     * removendo ele da lista e acertando os ponteiros necessarios tambem.
     *
     */
     if (id == f->inicioGeral->id){
         PONT sairPG = f->inicioGeral;
         f->inicioGeral = f->inicioGeral->prox;
         free(sairPG);
         resposta = true;
         if (f->inicioGeral == NULL) f->fimGeral = NULL;
     } else {
         PONT excluirG = saindo;
         ant->prox = saindo->prox;
         free(excluirG);
         resposta = true;
     }

    return resposta;
     /*
      * Esse else roda para qualquer elemento saindo da fila que nao seja preferencial, o 1o IF remove ele caso ele seja
      * o 1o da fila geral, nao sendo o 1o, o codigo do else roda, removendo ele e acertando os ponteiros necessarios.
      */
 } else {
     if (f->inicioGeral->id == id){
         PONT saindoGeral = saindo;
         f->inicioGeral = saindo->prox;
         free(saindoGeral);

         if (f->inicioGeral == NULL) f->fimGeral == NULL;
         resposta = true;
     } else {
         PONT saindoGeral2 = saindo;
         ant->prox = saindo->prox;
         free(saindoGeral2);
         resposta = true;
     }
 }

  return resposta;
}

