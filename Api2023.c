#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxM 512

typedef struct Stazione {
    int distanza;
    int numAuto;
    int automobili[MaxM];
    int maxAutonomia;
    int tappe;
    struct Stazione *next, *prev;
} Stazione ;

typedef struct lista{
    int num;
    struct lista *next, *prev;
} lista;

/*1*/ Stazione* aggiungiStazione(int staz, Stazione *inizio, int nAuto, int* automobili, int m, Stazione **tail);
/*2*/ Stazione* demolisciStazione(int staz, Stazione *inizio, int m, Stazione **tail);
/*3*/ Stazione* aggiungiAuto(Stazione *inizio, int staz, int automobile, Stazione *ultima, int m, Stazione **tail);
/*4*/ void rottamaAuto(Stazione *inizio, int staz, int automobile, int m, Stazione **tail);
/*5*/ void pianifica(Stazione *inizio, int partenza, int arrivo, int m, Stazione **tail);

/*6*/ int trovaMax(int *array);
/*7*/ lista* inserisci1(lista *head, int num);
/*8*/ lista* inserisci2(lista *head, int num);

/*9*/ void pianificaPercorso1(Stazione *inizio, int partenza, int arrivo, int m, Stazione **tail);
/*10*/ void pianificaPercorso2(Stazione *inizio, int partenza, int arrivo);

int main() {
    char parolaChiave[21];
    int i, stazione, numeroAuto, *autom, automobile, partenza, arrivo, mediana = 0;

    Stazione *inizioAutostrada=NULL, *iter, *next, *ultima = NULL, *tail = NULL;
    while (scanf("%s", parolaChiave)==1){

        if (!strcmp(parolaChiave, "aggiungi-stazione")){
            i=0;

            if(scanf("%d", &stazione)==1){};
            if(scanf("%d", &numeroAuto)==1){};

            autom = (int*)malloc(sizeof (int)*numeroAuto);

            while (i<numeroAuto){
                if(scanf("%d", &autom[i])==1){};
                i++;
            }

            inizioAutostrada = aggiungiStazione(stazione, inizioAutostrada, numeroAuto, autom, mediana, &tail);

            mediana = (tail->distanza + inizioAutostrada->distanza)/2;

            free(autom);

        } else if (!strcmp(parolaChiave, "demolisci-stazione")){

            if(scanf("%d", &stazione)==1){};
            inizioAutostrada = demolisciStazione(stazione, inizioAutostrada, mediana, &tail);

        } else if (!strcmp(parolaChiave, "aggiungi-auto")){

            if(scanf("%d %d", &stazione, &automobile)==1){};
            ultima = aggiungiAuto(inizioAutostrada, stazione, automobile, ultima, mediana, &tail);

        } else if (!strcmp(parolaChiave, "rottama-auto")) {

            if(scanf("%d %d", &stazione, &automobile)==1){};
            rottamaAuto(inizioAutostrada, stazione, automobile, mediana, &tail);

        } else if (!strcmp(parolaChiave, "pianifica-percorso")){

            if(scanf("%d %d", &partenza, &arrivo)==1){};
            pianifica(inizioAutostrada, partenza, arrivo, mediana, &tail);

        } else {
            break;
        }
    }

    iter = inizioAutostrada;
    while (iter) {
        next = iter->next;
        free(iter);
        iter = next;
    }

    return 0;

}

/*1*/
Stazione* aggiungiStazione(int staz, Stazione *inizio, int nAuto, int *automobili, int m, Stazione **tail) {
    Stazione *nuovaStazione, *tmp, *tmp2;
    int i;
    if (inizio == NULL) {
        nuovaStazione = malloc(sizeof(Stazione));
        nuovaStazione->prev = NULL;
        nuovaStazione->next = NULL;
        nuovaStazione->numAuto = nAuto;
        nuovaStazione->maxAutonomia = 0;
        nuovaStazione->distanza = staz;
        nuovaStazione->tappe = 0;
        for (i = 0; i < nAuto; i++) {
            nuovaStazione->automobili[i] = automobili[i];
            if (automobili[i] > nuovaStazione->maxAutonomia) {
                nuovaStazione->maxAutonomia = automobili[i];
            }
        }
        for (; i < MaxM; i++) {
            nuovaStazione->automobili[i] = -1;
        }

        printf("aggiunta\n");
        inizio = nuovaStazione;
        *tail = nuovaStazione;
        return inizio;
    }

    nuovaStazione = malloc(sizeof(Stazione));

    if (staz <= m || m == inizio->distanza) {
        for (tmp = inizio; tmp; tmp = tmp->next) {
            if (tmp->distanza == staz) {
                printf("non aggiunta\n");
                free(nuovaStazione);
                return inizio;
            }
            tmp2 = tmp;
            if (tmp->distanza > staz)
                break;
        }

        if (tmp == NULL) {
            nuovaStazione->next = NULL;
            nuovaStazione->prev = tmp2;
            tmp2->next = nuovaStazione;

        } else if (tmp->prev == NULL) {
            nuovaStazione->prev = NULL;
            nuovaStazione->next = tmp;
            tmp->prev = nuovaStazione;
        } else {
            nuovaStazione->prev = tmp->prev;
            nuovaStazione->next = tmp;
            tmp->prev->next = nuovaStazione;
            tmp->prev = nuovaStazione;
        }

        nuovaStazione->numAuto = nAuto;
        nuovaStazione->maxAutonomia = 0;
        nuovaStazione->distanza = staz;
        nuovaStazione->tappe = 0;

        for (i = 0; i < MaxM; i++) {
            if (i < nAuto) {
                nuovaStazione->automobili[i] = automobili[i];
                if (automobili[i] > nuovaStazione->maxAutonomia) {
                    nuovaStazione->maxAutonomia = automobili[i];
                }
            } else if (nAuto == 0 || i >= nAuto) {
                nuovaStazione->automobili[i] = -1;
            }
        }

        printf("aggiunta\n");

        if (nuovaStazione->distanza < inizio->distanza) {
            inizio = nuovaStazione;
        }

        tmp = *tail;
        if (staz > tmp->distanza)
            *tail = nuovaStazione;

        return inizio;

    } else {
        tmp = *tail;
        tmp2 = tmp;
        for (tmp = *tail; tmp; tmp = tmp->prev) {
            if (tmp->distanza == staz) {
                printf("non aggiunta\n");
                free(nuovaStazione);
                return inizio;
            }
            tmp2 = tmp;
            if (tmp->distanza < staz)
                break;
        }

        if (tmp == NULL) {
            nuovaStazione->prev = NULL;
            nuovaStazione->next = tmp2;
            tmp2->prev = nuovaStazione;
        } else if (tmp->next == NULL) {
            nuovaStazione->next = NULL;
            nuovaStazione->prev = tmp;
            tmp->next = nuovaStazione;
            //*tail = nuovaStazione;
        } else {
            nuovaStazione->prev = tmp;
            nuovaStazione->next = tmp->next;
            tmp->next->prev = nuovaStazione;
            tmp->next = nuovaStazione;
        }

        nuovaStazione->numAuto = nAuto;
        nuovaStazione->maxAutonomia = 0;
        nuovaStazione->distanza = staz;
        nuovaStazione->tappe = 0;

        for (i = 0; i < MaxM; i++) {
            if (i < nAuto) {
                nuovaStazione->automobili[i] = automobili[i];
                if (automobili[i] > nuovaStazione->maxAutonomia) {
                    nuovaStazione->maxAutonomia = automobili[i];
                }
            } else if (nAuto == 0 || i >= nAuto) {
                nuovaStazione->automobili[i] = -1;
            }
        }

        printf("aggiunta\n");

        if (staz < inizio->distanza)
            inizio = nuovaStazione;

        tmp = *tail;
        if (staz > tmp->distanza)
            *tail = nuovaStazione;

        return inizio;
    }

}

/*2*/
Stazione* demolisciStazione(int staz, Stazione *inizio, int m, Stazione **tail){
    Stazione *tmp;

    if (staz <= m) {
        for (tmp = inizio; tmp; tmp = tmp->next) {
            if (tmp->distanza == staz) {
                break;
            }
            if (tmp->distanza > staz) {
                printf("non demolita\n");
                return inizio;
            }
        }

        if (tmp == NULL) {
            printf("non demolita\n");
            return inizio;
        }

        if (tmp->prev == NULL && tmp->next == NULL) {
            free(tmp);
            printf("demolita\n");
            return NULL;
        } else if (tmp->prev == NULL) {
            inizio = inizio->next;
            tmp->next->prev = NULL;
            free(tmp);
        } else if (tmp->next == NULL) {
            tmp->prev->next = NULL;
            *tail = tmp->prev;
            free(tmp);
        } else {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
            free(tmp);
        }

        printf("demolita\n");
        return inizio;

    } else {

        for (tmp = *tail; tmp; tmp = tmp->prev) {
            if (tmp->distanza == staz) {
                break;
            }
            if (tmp->distanza < staz) {
                printf("non demolita\n");
                return inizio;
            }
        }

        if (tmp == NULL) {
            printf("non demolita\n");
            return inizio;
        }

        if (tmp->prev == NULL && tmp->next == NULL) {
            free(tmp);
            printf("demolita\n");
            return NULL;
        } else if (tmp->prev == NULL) {
            inizio = inizio->next;
            tmp->next->prev = NULL;
            free(tmp);
        } else if (tmp->next == NULL) {
            tmp->prev->next = NULL;
            *tail = tmp->prev;
            free(tmp);
        } else {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
            free(tmp);
        }

        printf("demolita\n");
        return inizio;


    }
}

/*3*/
Stazione* aggiungiAuto(Stazione *inizio, int staz, int automobile, Stazione *ultima, int m, Stazione **tail){
    Stazione *tmp;
    int i;

    if (ultima!=NULL){
        if (ultima->distanza == staz){
            for(i=0; ultima->automobili[i]!=-1; i++);
            if (i==MaxM){
                printf("non aggiunta\n");
                return NULL;
            }

            ultima->automobili[i]=automobile;

            if (automobile > ultima->maxAutonomia){
                ultima->maxAutonomia = automobile;
            }
            ultima->numAuto++;
            printf("aggiunta\n");
            return ultima;
        }
    }

    if (staz <= m) {
        for (tmp = inizio; tmp; tmp = tmp->next) {
            if (tmp->distanza == staz) {
                break;
            }
            if (tmp->distanza > staz) {
                printf("non aggiunta\n");
                return NULL;
            }
        }

        if (tmp == NULL) {
            printf("non aggiunta\n");
            return NULL;
        }

        for (i = 0; tmp->automobili[i] != -1; i++);
        if (i == MaxM) {
            printf("non aggiunta\n");
            return NULL;
        }

        tmp->automobili[i] = automobile;

        if (automobile > tmp->maxAutonomia) {
            tmp->maxAutonomia = automobile;
        }
        tmp->numAuto++;
        printf("aggiunta\n");

        return tmp;

    } else {

        for (tmp = *tail; tmp; tmp = tmp->prev) {
            if (tmp->distanza == staz) {
                break;
            }
            if (tmp->distanza < staz) {
                printf("non aggiunta\n");
                return NULL;
            }
        }

        if (tmp == NULL) {
            printf("non aggiunta\n");
            return NULL;
        }

        for (i = 0; tmp->automobili[i] != -1; i++);
        if (i == MaxM) {
            printf("non aggiunta\n");
            return NULL;
        }

        tmp->automobili[i] = automobile;

        if (automobile > tmp->maxAutonomia) {
            tmp->maxAutonomia = automobile;
        }
        tmp->numAuto++;
        printf("aggiunta\n");

        return tmp;

    }
}

/*4*/
void rottamaAuto(Stazione *inizio, int staz, int automobile, int m, Stazione **tail){
    Stazione *tmp;
    int i, flag;

    if (staz <= m) {
        for (tmp = inizio; tmp; tmp = tmp->next) {
            if (tmp->distanza == staz)
                break;
            if (tmp->distanza > staz) {
                printf("non rottamata\n");
                return;
            }
        }

        if (tmp == NULL) {
            printf("non rottamata\n");
            return;
        }

        for (i = 0, flag = 0; i < MaxM; i++) {
            if (tmp->automobili[i] == automobile) {
                flag = 1;
                tmp->automobili[i] = -1;
                break;
            }
        }

        if (flag) {
            tmp->numAuto--;
            if (tmp->maxAutonomia == automobile)
                tmp->maxAutonomia = trovaMax(tmp->automobili);
            printf("rottamata\n");
        } else {
            printf("non rottamata\n");
        }
    } else {
        for (tmp = *tail; tmp; tmp = tmp->prev) {
            if (tmp->distanza == staz)
                break;
            if (tmp->distanza < staz) {
                printf("non rottamata\n");
                return;
            }
        }

        if (tmp == NULL) {
            printf("non rottamata\n");
            return;
        }

        for (i = 0, flag = 0; i < MaxM; i++) {
            if (tmp->automobili[i] == automobile) {
                flag = 1;
                tmp->automobili[i] = -1;
                break;
            }
        }

        if (flag) {
            tmp->numAuto--;
            if (tmp->maxAutonomia == automobile)
                tmp->maxAutonomia = trovaMax(tmp->automobili);
            printf("rottamata\n");
        } else {
            printf("non rottamata\n");
        }
    }
}

/*5*/
void pianifica(Stazione *inizio, int partenza, int arrivo, int m, Stazione **tail){
    if (partenza == arrivo){
        printf("%d\n", partenza);
        return;

    } else if (partenza < arrivo){
        pianificaPercorso1(inizio, partenza, arrivo, m, tail);

    } else if (partenza > arrivo){
        pianificaPercorso2(inizio, partenza, arrivo);

    }

}

/*6*/
int trovaMax(int *array){
    int i, max;
    for (i=0, max=0; i<MaxM; i++){
        if (array[i]>=max){
            max = array[i];
        }
    }
    return max;
}

/*9*/ lista* inserisci1(lista *head, int num){
    lista *tmp, *nuovoNodo;

    if (head == NULL){
        head = malloc(sizeof (lista));
        head->next = NULL;
        head->prev = NULL;
        head->num = num;
        return head;
    }

    for (tmp=head; tmp; tmp = tmp->next){
        if (tmp->next->num > num){
            break;
        }
    }
    nuovoNodo = malloc(sizeof (lista));
    nuovoNodo->prev = tmp;
    nuovoNodo->next = tmp->next;
    nuovoNodo->num = num;
    tmp->next = nuovoNodo;

    return head;
}

/*8*/ lista* inserisci2(lista *head, int num){
    lista *tmp, *nuovoNodo;

    if (head == NULL){
        head = malloc(sizeof (lista));
        head->next = NULL;
        head->prev = NULL;
        head->num = num;
        return head;
    }

    for (tmp=head; tmp; tmp = tmp->next){
        if (tmp->next->num < num){
            break;
        }
    }
    nuovoNodo = malloc(sizeof (lista));
    nuovoNodo->prev = tmp;
    nuovoNodo->next = tmp->next;
    nuovoNodo->num = num;
    tmp->next = nuovoNodo;

    return head;

}

/*9*/ void pianificaPercorso1(Stazione *inizio, int partenza, int arrivo, int m, Stazione **t){
    Stazione  *tmp, *nuovoArrivo, *p, *a;
    lista *head, *tail, *iter, *next;
    int done, i, j;

    if (partenza <= m) {
        for (tmp = inizio; tmp; tmp = tmp->next) {
            if (tmp->distanza == partenza) {
                break;
            }
            if (tmp->distanza > partenza) {
                printf("nessun percorso\n");
                return;
            }
        }
    } else {
        for (tmp = *t; tmp; tmp = tmp->prev) {
            if (tmp->distanza == partenza) {
                break;
            }
            if (tmp->distanza < partenza) {
                printf("nessun percorso\n");
                return;
            }
        }
    }
    p = tmp;

    for (; tmp; tmp = tmp->next){
        if (tmp->distanza == arrivo){
            break;
        }
        if (tmp->distanza > arrivo){
            printf("nessun percorso\n");
            return;
        }
    }
    a = tmp;

    if (p->distanza + p->maxAutonomia >= a->distanza){
        printf("%d %d\n", partenza, arrivo);
        return;
    }

    if (p->maxAutonomia == -1){
        printf("nessun percorso\n");
        return;
    }

    head = malloc(sizeof (lista));
    tail = malloc(sizeof (lista));
    head->next = tail;
    head->prev = NULL;
    head->num = partenza;
    tail->next = NULL;
    tail->prev = head;
    tail->num = arrivo;

    nuovoArrivo = a;
    done = 0;

    i=0;
    while (done == 0) {
        i++;
        if (p->distanza + p->maxAutonomia >= nuovoArrivo->distanza){
            done = 1;
            break;
        }
        for (tmp = p, j=0; tmp; tmp = tmp->next, j++) {
            if (tmp->distanza + tmp->maxAutonomia >= nuovoArrivo->distanza) {
                if (tmp->distanza == nuovoArrivo->distanza) {
                    printf("nessun percorso\n");
                    iter = head;
                    while (iter) {
                        next = iter->next;
                        free(iter);
                        iter = next;
                    }
                    return;
                }
                if (tmp->distanza != head->num){
                    head = inserisci1(head, tmp->distanza);
                }
                nuovoArrivo = tmp;
                break;
            }
        }
    }

    for (iter = head; iter; iter = next){
        printf("%d ", iter->num);
        next = iter->next;
        free(iter);
    }
    printf("\n");

    return;

}

/*10*/ void pianificaPercorso2(Stazione *inizio, int partenza, int arrivo){
    Stazione *p, *a, *tmp, *tmp2;
    int nTappe, distanzaRaggiungibile, nuovaDistRaggiungibile;
    lista *head, *tail, *iter, *next;

    for (tmp = inizio; tmp; tmp = tmp->next){
        if (tmp->distanza == arrivo){
            break;
        }
        if (tmp->distanza > arrivo){
            printf("nessun percorso\n");
            return;
        }
    }
    a = tmp;
    for (tmp = a; tmp; tmp = tmp->next){
        if (tmp->distanza == partenza){
            break;
        }
        if (tmp->distanza > partenza){
            printf("nessun percorso\n");
            return;
        }
    }
    p = tmp;

    if (p->distanza - p->maxAutonomia <= a->distanza){
        printf("%d %d\n", partenza, arrivo);
        return;
    }

    head = malloc(sizeof (lista));
    tail = malloc(sizeof (lista));
    head->next = tail;
    head->prev = NULL;
    head->num = partenza;
    tail->next = NULL;
    tail->prev = head;
    tail->num = arrivo;

    distanzaRaggiungibile = p->distanza - p->maxAutonomia;
    nuovaDistRaggiungibile = distanzaRaggiungibile;
    nTappe = 1;

    for(tmp = p->prev; tmp; tmp = tmp->prev){
        if(tmp->distanza < nuovaDistRaggiungibile){
            printf("nessun percorso\n");
            iter = head;
            while (iter) {
                next = iter->next;
                free(iter);
                iter = next;
            }
            return;
        }
        if (tmp->distanza >= distanzaRaggiungibile){
            tmp->tappe = nTappe;
        } else {
            nTappe++;
            tmp->tappe = nTappe;
            distanzaRaggiungibile = nuovaDistRaggiungibile;
        }
        if (tmp->distanza - tmp->maxAutonomia < nuovaDistRaggiungibile){
            nuovaDistRaggiungibile = tmp->distanza - tmp->maxAutonomia;
        }
        if (tmp->distanza == a->distanza){
            break;
        }
    }

    a->tappe = 0;
    tmp2 = a;

    for(tmp = a; tmp->distanza != p->distanza; tmp = tmp->next){
        if (tmp->distanza != a->distanza && tmp->tappe == nTappe-1 && tmp->distanza - tmp->maxAutonomia <= tmp2->distanza){
            head = inserisci2(head, tmp->distanza);
            nTappe--;
            tmp2 = tmp;
        }
        tmp->tappe = 0;
    }

    for (iter = head; iter; iter = next){
        printf("%d ", iter->num);
        next = iter->next;
        free(iter);
    }
    printf("\n");

    return;

}