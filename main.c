#include <stdio.h>
#include <stdlib.h>

char string[255];

struct station {
    int km;
    struct vehicle* vehicle;
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

struct station* addStation(struct station* head);
struct station* removeStation(struct station* head);
struct station* addAuto(struct station* head);
struct station* removeAuto(struct station* head);
void planPath(struct station* head);

int main() {
    struct station* list = NULL;
    while (fgets(string,255,stdin) != NULL) {
        switch (string[12]) {
            case 'z':
                list = addStation(list);
                break;
            case 'a':
                list = removeStation(list);
                break;
            case 'o':
                list = addAuto(list);
                break;
            case ' ':
                list = removeAuto(list);
                break;
            case 'r':
                planPath(list);
                break;
        }
    }
    return 0;
}

struct station* addStation(struct station* head) {
    char *end = &string[18];
    int km = strtol(end, &end, 10);
    struct station *new;
    struct station* pointer = head;
    struct station* pointerPrev = NULL;
    if (head == NULL) {
        new = (struct station*) malloc(sizeof(station));
        new->km = km;
        new->next = NULL;
        new->vehicle = NULL;
        head = new;
        fputs("aggiunta\n", stdout);
    }
    else {
        while (pointer->km < km) {
            if (pointer->next != NULL) {
                pointerPrev = pointer;
                pointer = pointer->next;
            }
            else break;
        }
        if (pointer->km == km) {
            fputs("non aggiunta\n", stdout);
            return head;
        }
        else {
            new = (struct station *) malloc(sizeof(station));
            new->km = km;
            new->vehicle = NULL;
            if (pointer == head && new->km < head->km) {
                new->next = head;
                head = new;
            }
            else if (pointer->next == NULL) {
                pointer->next = new;
                new->next = NULL;
            }
            else {
                pointerPrev->next = new;
                new->next = pointer;
            }
            fputs("aggiunta\n", stdout);
        }
    }
    while (*end != '\n') {
        int autonomy = strtol(end, &end, 10);
        if (new->vehicle == NULL) {
            struct vehicle *new1;
            new1 = (struct vehicle*) malloc(sizeof(vehicle));
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
            struct vehicle *new1;
            new1 = (struct vehicle*) malloc(sizeof(vehicle));
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
    return head;
}

struct station* removeStation(struct station* head) {
    char *end = &string[19];
    int km = strtol(end, &end, 10);
    struct station* pointer = head;
    struct station* pointerPrev = NULL;
    if (pointer == NULL) {
        fputs("non demolita\n", stdout);
        return head;
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
        struct vehicle *vehicle1;
        struct vehicle *vehicle2;
        if (pointerPrev != NULL && pointer->next != NULL) {
            pointerPrev->next = pointer->next;
        } else if (pointerPrev == NULL && pointer->next == NULL) head = NULL;
        else if (pointerPrev == NULL) {
            head = pointer->next;
        } else if (pointer->next == NULL) pointerPrev->next = NULL;

        if (pointer->vehicle == NULL) {
            fputs("demolita\n", stdout);
            return head;
        }
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
    return head;
}

struct station* addAuto(struct station* head) {
    char *end = &string[14];
    int km = strtol(end, &end, 10);
    int autonomy = strtol(end, &end, 10);
    struct station* pointer = head;
    if (pointer == NULL) {
        fputs("non aggiunta\n", stdout);
        return head;
    }
    while (pointer->km != km && pointer->next != NULL) pointer = pointer->next;
    if (pointer->km != km) fputs("non aggiunta\n", stdout);
    else {
        if (pointer->vehicle == NULL) {
            struct vehicle *new1;
            new1 = (struct vehicle*) malloc(sizeof(vehicle));
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
            struct vehicle *new1;
            new1 = (struct vehicle*) malloc(sizeof(vehicle));
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
    return head;
}

struct station* removeAuto(struct station* head) {
    char *end = &string[13];
    int km = strtol(end, &end, 10);
    int autonomy = strtol(end, &end, 10);
    struct station* pointer = head;
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
    return head;
}

void planPath(struct station* head) {
    char *end = &string[19];
    int start = strtol(end, &end, 10);
    int arrive = strtol(end, &end, 10);
    int distance = abs(start - arrive);
    if (start == arrive) fprintf(stdout, "%i\n", start);
    else if (start < arrive) {
        struct station *pointer = head;
        while (pointer->km != start) pointer = pointer->next;
        if (pointer->vehicle->autonomy >= distance) {
            fprintf(stdout, "%i %i\n", start, arrive);
            return;
        }
        else {
            struct searchNode *headList;
            headList = (struct searchNode *) malloc(sizeof(searchNode));
            headList->km = pointer->km;
            headList->autonomy = pointer->vehicle->autonomy;
            headList->next = NULL;
            headList->prev = NULL;
            pointer = pointer->next;
            struct searchNode *actualNode = headList;
            while (pointer->km <= arrive) {
                struct searchNode *newNode;
                newNode = (struct searchNode *) malloc(sizeof(searchNode));
                newNode->km = pointer->km;
                newNode->autonomy = pointer->vehicle->autonomy;
                newNode->next = NULL;
                newNode->prev = NULL;
                actualNode->next = newNode;
                actualNode = newNode;
                if (pointer->next != NULL) pointer = pointer->next;
                else break;
            }
            actualNode = headList;
            struct searchNode *successiveNode;
            while (actualNode->km <= arrive) {
                successiveNode = actualNode->next;
                while (actualNode->autonomy >= (successiveNode->km - actualNode->km)) {
                    if (successiveNode->prev == NULL) {
                        successiveNode->prev = actualNode;
                    }
                    if (successiveNode->km == arrive) {
                        int i = 0;
                        int stations[200];
                        struct searchNode* search = successiveNode;
                        while (search->km != start) {
                            stations[i] = search->km;
                            i++;
                            if (search->prev != NULL) search = search->prev;
                            else break;
                        }
                        search = headList;
                        if (stations[i - 1] - start <= headList->autonomy && stations[0] == arrive) {
                            printf("%i ", start);
                            for (int k = i - 1; k != 0; k--) printf("%i ", stations[k]);
                            printf("%i\n", arrive);
                            return;
                        }
                    }
                    if (successiveNode->next != NULL) successiveNode = successiveNode->next;
                    else break;
                }
                if (actualNode->next != NULL) {
                    actualNode = actualNode->next;
                    if (actualNode->next != NULL) successiveNode = actualNode->next;
                    else break;
                }
                else break;
            }
            fputs("nessun percorso\n", stdout);
            while (headList != NULL) {
                successiveNode = headList;
                if (headList->next != NULL) {
                    headList = headList->next;
                    free(successiveNode);
                }
                else {
                    free(headList);
                    break;
                }
            }
            return;
        }
    }
    else {
        struct station *pointer = head;
        while (pointer->km != arrive) pointer = pointer->next;
        struct station* goal = head;
        while (goal->km != start) goal = goal->next;
        if (goal->km - pointer->km <= goal->vehicle->autonomy) {
            fprintf(stdout, "%i %i\n", start, arrive);
            return;
        }
        else {
            struct searchNode *headList;
            headList = (struct searchNode *) malloc(sizeof(searchNode));
            headList->km = pointer->km;
            headList->autonomy = pointer->vehicle->autonomy;
            headList->next = NULL;
            headList->prev = NULL;
            pointer = pointer->next;
            struct searchNode *actualNode = headList;
            while (pointer->km <= start) {
                struct searchNode *newNode;
                newNode = (struct searchNode *) malloc(sizeof(searchNode));
                newNode->km = pointer->km;
                newNode->autonomy = pointer->vehicle->autonomy;
                newNode->next = NULL;
                newNode->prev = NULL;
                actualNode->next = newNode;
                actualNode = newNode;
                if (pointer->next != NULL) pointer = pointer->next;
                else break;
            }
            actualNode = headList;
            struct searchNode *successiveNode = actualNode->next;
            while (actualNode->km <= start) {
                successiveNode = actualNode->next;
                while (successiveNode->autonomy >= (successiveNode->km - actualNode->km)) {
                    if (successiveNode->prev == NULL) {
                        successiveNode->prev = actualNode;
                    }
                    if (successiveNode->km == start) {
                        int i = 0;
                        int stations[200];
                        struct searchNode* search = successiveNode;
                        while (search->km != arrive) {
                            stations[i] = search->km;
                            i++;
                            if (search->prev != NULL) search = search->prev;
                            else break;
                        }
                        search = headList;
                        while (search->km != stations[i - 1]) search = search->next;
                        if (stations[i - 1] - arrive <= search->autonomy && stations[0] == start) {
                            printf("%i ", start);
                            for (int k = 1; k != i; k++) printf("%i ", stations[k]);
                            printf("%i\n", arrive);
                            return;
                        }
                    }
                    if (successiveNode->next != NULL) successiveNode = successiveNode->next;
                    else break;
                }
                if (actualNode->next != NULL && actualNode->next->next != NULL) {
                    actualNode = actualNode->next;
                    successiveNode = actualNode->next;
                }
                else break;
            }
            fputs("nessun percorso\n", stdout);
            while (headList != NULL) {
                successiveNode = headList;
                if (headList->next != NULL) {
                    headList = headList->next;
                    free(successiveNode);
                }
                else {
                    free(headList);
                    break;
                }
            }
            return;
        }
    }
}