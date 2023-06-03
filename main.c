#include <stdio.h>
#include <stdlib.h>

char string[11000];

struct station {
    int km;
    struct vehicle* vehicle;
    struct station* pre;
    struct station* next;
} station;

struct vehicle {
    int autonomy;
    struct vehicle* next;
} vehicle ;

struct searchNode {
    int km;
    int autonomy;
    struct searchNode* next;
    struct searchNode* prev;
} searchNode;

struct station* list = NULL;

void addStation();
void removeStation();
void addAuto();
void removeAuto();
void planPath();
void memoryCleaner(struct searchNode *searchHead);
//void stampa();

int main() {
    while (fgets(string,11000,stdin) != NULL) {
        switch (string[12]) {
            case 'z':
                addStation();
                break;
            case 'a':
                removeStation();
                break;
            case 'o':
                addAuto();
                break;
            case ' ':
                removeAuto();
                break;
            case 'r':
                planPath();
                break;
        }
    }
    fflush(stdout);
    return 0;
}

void addStation() {
    char *end = &string[18];
    int km = strtol(end, &end, 10);
    struct station *new;
    new = (struct station*) malloc(sizeof(station));
    if (list == NULL) {
        new->km = km;
        new->next = NULL;
        new->vehicle = NULL;
        new->pre = NULL;
        list = new;
        fputs("aggiunta\n", stdout);
    }
    else {
        struct station* pointer = list;
        struct station* pointerPrev = NULL;
        new->km = km;
        new->vehicle = NULL;
        new->next = NULL;
        new->pre = NULL;
        while (pointer->km < km) {
            if (pointer->next != NULL) {
                pointerPrev = pointer;
                pointer = pointer->next;
            }
            else break;
        }
        if (pointer->km == km) {
            free(new);
            fputs("non aggiunta\n", stdout);
            return;
        }
        else {
            if (pointer == list && list->km > km) {
                new->next = list;
                new->pre = NULL;
                list->pre = new;
                list = new;
            }
            else if (pointer == list && list->km < km) {
                new->pre = pointer;
                pointer->next = new;
                new->next = NULL;
            }
            else if (pointer->km > km) {
                new->next = pointer;
                pointer->pre = new;
                pointerPrev->next = new;
                new->pre = pointerPrev;
            }
            else if (pointer->km < km){
                new->next = pointer->next;
                pointer->next = new;
                pointer->next->pre = new;
                new->pre = pointer;
            }
            fputs("aggiunta\n", stdout);
        }
    }
    while (*end != '\n') {
        int autonomy = strtol(end, &end, 10);
        struct vehicle *new1 = (struct vehicle*) malloc(sizeof(vehicle));
        if (new->vehicle == NULL) {
            new1->next = NULL;
            new1->autonomy = autonomy;
            new->vehicle = new1;
        }
        else {
            struct vehicle* point = new->vehicle;
            while (point->autonomy > autonomy) {
                if (point->next != NULL) {
                    if (point->next->autonomy < autonomy) break;
                    else point = point->next;
                }
                else break;
            }
            new1->autonomy = autonomy;
            if (point == new->vehicle && point->autonomy < autonomy) {
                new1->next = new->vehicle;
                new->vehicle = new1;
            }
            else if(point == new->vehicle && point->autonomy >= autonomy) {
                new1->next = new->vehicle->next;
                new->vehicle->next = new1;
            }
            else if (point->next == NULL) {
                point->next = new1;
                new1->next = NULL;
            }
            else {
                new1->next = point->next;
                point->next = new1;
            }
        }
    }
}

void removeStation() {
    char *end = &string[19];
    int km = strtol(end, &end, 10);
    struct station* pointer = list;
    struct station* pointerPrev = NULL;
    if (pointer == NULL) {
        fputs("non demolita\n", stdout);
        return;
    }
    while (pointer->km != km) {
        if (pointer->next != NULL) {
            pointerPrev = pointer;
            pointer = pointer->next;
        }
        else break;
    }
    if (pointer->km != km) fputs("non demolita\n", stdout);
    else if (pointer->km == km) {
        if (pointerPrev != NULL && pointer->next != NULL) {
            pointerPrev->next = pointer->next;
        } else if (pointerPrev == NULL && pointer->next == NULL) list = NULL;
        else if (pointerPrev == NULL) list = pointer->next;
        else if (pointer->next == NULL) pointerPrev->next = NULL;

        if (pointer->vehicle == NULL) {
            fputs("demolita\n", stdout);
            free(pointer);
            return;
        }
        else {
            struct vehicle *vehicle1;
            struct vehicle *vehicle2 = NULL;
            vehicle1 = pointer->vehicle;
            free(pointer);
            while (vehicle1 != NULL) {
                if (vehicle1->next == NULL) {
                    free(vehicle1);
                    break;
                } else {
                    vehicle2 = vehicle1;
                    vehicle1 = vehicle1->next;
                    free(vehicle2);
                }
            }
            fputs("demolita\n", stdout);
        }
    }
}

void addAuto() {
    char *end = &string[14];
    int km = strtol(end, &end, 10);
    int autonomy = strtol(end, &end, 10);
    struct station* pointer = list;
    if (pointer == NULL) {
        fputs("non aggiunta\n", stdout);
        return;
    }
    while (pointer->km != km && pointer->next != NULL) pointer = pointer->next;
    if (pointer->km != km) fputs("non aggiunta\n", stdout);
    else {
        struct vehicle *new1;
        new1 = (struct vehicle*) malloc(sizeof(vehicle));
        if (pointer->vehicle == NULL) {
            new1->next = NULL;
            new1->autonomy = autonomy;
            pointer->vehicle = new1;
        }
        else {
            struct vehicle* point = pointer->vehicle;
            while (point->autonomy > autonomy) {
                if (point->next != NULL) {
                    if (point->next->autonomy < autonomy) break;
                    else point = point->next;
                }
                else break;
            }
            new1->autonomy = autonomy;
            if (point == pointer->vehicle && point->autonomy < autonomy) {
                new1->next = pointer->vehicle;
                pointer->vehicle = new1;
            }
            else if(point == pointer->vehicle && point->autonomy >= autonomy) {
                new1->next = pointer->vehicle->next;
                pointer->vehicle->next = new1;
            }
            else if (point->next == NULL) {
                point->next = new1;
                new1->next = NULL;
            }
            else {
                new1->next = point->next;
                point->next = new1;
            }
        }
        fputs("aggiunta\n", stdout);
    }
}

void removeAuto() {
    char *end = &string[13];
    int km = strtol(end, &end, 10);
    int autonomy = strtol(end, &end, 10);
    struct station* pointer = list;
    while (pointer->km != km) {
        if (pointer->next != NULL) pointer = pointer->next;
        else break;
    }
    if (pointer->km != km) fputs("non rottamata\n", stdout);
    else if (pointer->km == km) {
        struct vehicle *vehicle1;
        struct vehicle *vehicle2 = NULL;
        if (pointer->vehicle != NULL) {
            vehicle1 = pointer->vehicle;
            while (vehicle1->autonomy > autonomy) {
                if (vehicle1->next != NULL) {
                    vehicle2 = vehicle1;
                    vehicle1 = vehicle1->next;
                } else break;
            }
            if (vehicle1->autonomy == autonomy) {
                if (pointer->vehicle == vehicle1) {
                    pointer->vehicle = vehicle1->next;
                    free(vehicle1);
                }
                else if (vehicle1->next == NULL) {
                    vehicle2->next = NULL;
                    free(vehicle1);
                }
                else {
                    vehicle2->next = vehicle1->next;
                    free(vehicle1);
                }
                fputs("rottamata\n", stdout);
            } else fputs("non rottamata\n", stdout);
        }
        else fputs("non rottamata\n", stdout);
    }
}

void planPath() {
    char *end = &string[19];
    int start = strtol(end, &end, 10);
    int arrive = strtol(end, &end, 10);
    if (start == arrive) fprintf(stdout, "%i\n", start);
    else if (start < arrive) {
        struct station *pointer = list;
        while (pointer->km != start) pointer = pointer->next;
        if (pointer->vehicle->autonomy >= arrive - start) {
            fprintf(stdout, "%i %i\n", start, arrive);
            return;
        } else {
            struct searchNode *headList;
            headList = (struct searchNode *) malloc(sizeof(searchNode));
            headList->km = pointer->km;
            headList->autonomy = pointer->vehicle->autonomy;
            headList->next = NULL;
            headList->prev = NULL;
            struct searchNode *node = headList;
            struct searchNode *actualNode = headList;
            struct searchNode *newNode;
            pointer = pointer->next;
            while (pointer->km <= arrive) {
                if (pointer->km - actualNode->km <= actualNode->autonomy) {
                    newNode = (struct searchNode *) malloc(sizeof(searchNode));
                    newNode->km = pointer->km;
                    newNode->autonomy = pointer->vehicle->autonomy;
                    newNode->next = NULL;
                    newNode->prev = actualNode;
                    node->next = newNode;
                    node = newNode;
                    pointer = pointer->next;
                }
                else {
                    if (actualNode->next != NULL) actualNode = actualNode->next;
                    else break;
                }
                if (node->km == arrive) {
                    int stations[200], i;
                    struct searchNode* search = node;
                    for (i = 0; search->prev != NULL; i++) {
                        stations[i] = search->km;
                        search = search->prev;
                    }
                    if (search->km == start) {
                        printf("%i ", start);
                        for (int k = i - 1; k != 0; k--) printf("%i ", stations[k]);
                        printf("%i\n", arrive);
                        memoryCleaner(headList);
                        return;
                    }
                }
            }
            fputs("nessun percorso\n", stdout);
            memoryCleaner(headList);
            return;
        }
    } else if (start > arrive){
        struct station *pointer = list;
        while (pointer->km != start) pointer = pointer->next;
        if (pointer->vehicle->autonomy >= start - arrive) {
            fprintf(stdout, "%i %i\n", start, arrive);
            return;
        } else {
            struct searchNode *headList;
            headList = (struct searchNode *) malloc(sizeof(searchNode));
            headList->km = pointer->km;
            headList->autonomy = pointer->vehicle->autonomy;
            headList->next = NULL;
            headList->prev = NULL;
            struct searchNode *node = headList;
            struct searchNode *actualNode = headList;
            struct searchNode *newNode;
            struct station *rewindNode;
            while (pointer->km >= arrive) {
                rewindNode = pointer;
                while (actualNode->km - rewindNode->km <= actualNode->autonomy && rewindNode->km != arrive) rewindNode = rewindNode->pre;
                if (actualNode->km - rewindNode->km > actualNode->autonomy) {
                    rewindNode = rewindNode->next;
                }
                struct station* save = rewindNode;
                while (rewindNode != pointer) {
                    newNode = (struct searchNode *) malloc(sizeof(searchNode));
                    newNode->km = rewindNode->km;
                    newNode->autonomy = rewindNode->vehicle->autonomy;
                    newNode->next = NULL;
                    newNode->prev = actualNode;
                    node->next = newNode;
                    node = newNode;
                    if (rewindNode->next != NULL) rewindNode = rewindNode->next;
                    else break;
                    if (node->km == arrive) {
                        int stations[200], i;
                        struct searchNode* search = node;
                        for (i = 0; search->prev != NULL; i++) {
                            stations[i] = search->km;
                            search = search->prev;
                        }
                        if (search->km == start) {
                            printf("%i ", start);
                            for (int k = i - 1; k != 0; k--) printf("%i ", stations[k]);
                            printf("%i\n", arrive);
                            memoryCleaner(headList);
                            return;
                        }
                    }

                }
                pointer = save;
                if (actualNode->next != NULL) actualNode = actualNode->next;
                else break;
            }
            fputs("nessun percorso\n", stdout);
            memoryCleaner(headList);
            return;
        }
    }
}

void memoryCleaner(struct searchNode *searchHead) {
    struct searchNode *node = NULL;
    while (searchHead != NULL) {
        node = searchHead;
        if (searchHead->next != NULL) {
            searchHead = searchHead->next;
            free(node);
        }
        else {
            free(searchHead);
            break;
        }
    }
}
/*
void stampa() {
    struct station *li;
    li = list;
    while (1) {
        if (li->vehicle != NULL) {
            printf("%i %i\n", li->km, li->vehicle->autonomy);
        }
        else printf("%i\n", li->km);
        if (li->next != NULL) li = li->next;
        else break;
    }
}
 */