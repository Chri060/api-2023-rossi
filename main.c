#include <stdio.h>
#include <stdlib.h>

char string[255];

struct station {
    int kilometre;
    struct vehicle* vehicle;
    struct station* next;
    struct station* previous;
} station;

struct vehicle {
    int autonomy;
    struct vehicle* next;
} vehicle ;

struct searchNode {
    int array[3];
    struct searchNode* list[3];
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
    struct station *new;
    int km = strtol(end, &end, 10);
    struct station* pointer = head;
    if (head == NULL) {
        new = (struct station*) malloc(sizeof(station));
        new->kilometre = km;
        new->previous = NULL;
        new->next = NULL;
        new->vehicle = NULL;
        head = new;
        fputs("aggiunta\n", stdout);
    }
    else {
        while (pointer->kilometre < km) {
            if (pointer->next != NULL) pointer = pointer->next;
            else break;
        }
        if (pointer->kilometre == km) {
            fputs("non aggiunta\n", stdout);
            return head;
        }
        else {
            new = (struct station *) malloc(sizeof(station));
            new->kilometre = km;
            new->vehicle = NULL;
            if (pointer == head && new->kilometre < head->kilometre) {
                new->previous = NULL;
                new->next = head;
                head->previous = new;
                head = new;
            }
            else if (pointer->next == NULL) {
                new->previous = pointer;
                pointer->next = new;
                new->next = NULL;
            }
            else {
                pointer->previous->next = new;
                new->previous = pointer->previous;
                pointer->previous = new;
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
    if (pointer != NULL) {
        while (pointer->kilometre != km) {
            if (pointer->next != NULL) pointer = pointer->next;
            else break;
        }
        if (pointer->kilometre != km) fputs("non demolita\n", stdout);
        else if (pointer->kilometre == km) {
            struct vehicle *vehicle1;
            struct vehicle *vehicle2;
            if (pointer->previous != NULL && pointer->next != NULL) {
                pointer->previous->next = pointer->next;
                pointer->next->previous = pointer->previous;
            } else if (pointer->previous == NULL && pointer->next == NULL) head = NULL;
            else if (pointer->previous == NULL) {
                head = pointer->next;
                head->previous = NULL;
            } else if (pointer->next == NULL) pointer->previous->next = NULL;

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
    }
    else fputs("non demolita\n", stdout);
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
    while (pointer->kilometre != km && pointer->next != NULL) pointer = pointer->next;
    if (pointer->kilometre != km) fputs("non aggiunta\n", stdout);
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
    while (pointer->kilometre != km) {
        if (pointer->next != NULL) pointer = pointer->next;
        else break;
    }
    if (pointer->kilometre != km) fputs("non rottamata\n", stdout);
    else if (pointer->kilometre == km) {
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
        while (pointer->kilometre != start) pointer = pointer->next;
        if (pointer->vehicle->autonomy >= distance) {
            fprintf(stdout, "%i %i\n", start, arrive);
            return;
        }
        else {
            struct searchNode *headList;
            headList = (struct searchNode *) malloc(sizeof(searchNode));
            headList->array[0] = pointer->kilometre;
            headList->array[1] = pointer->vehicle->autonomy;
            headList->array[2] = 0;
            headList->list[0] = NULL;
            headList->list[1] = NULL;
            headList->list[2] = NULL;
            pointer = pointer->next;
            struct searchNode *actualNode = headList;
            while (pointer->kilometre <= arrive) {
                struct searchNode *newNode;
                newNode = (struct searchNode *) malloc(sizeof(searchNode));
                newNode->array[0] = pointer->kilometre;
                newNode->array[1] = pointer->vehicle->autonomy;
                newNode->array[2] = 0;
                newNode->list[0] = NULL;
                newNode->list[1] = actualNode;
                newNode->list[2] = NULL;
                actualNode->list[0] = newNode;
                actualNode = newNode;
                if (pointer->next != NULL) pointer = pointer->next;
                else break;
            }
            actualNode = headList;
            struct searchNode *successiveNode = actualNode->list[0];
            while (actualNode->array[0] <= arrive) {
                while (actualNode->array[1] >= (successiveNode->array[0] - actualNode->array[0])) {
                    if (successiveNode->array[2] == 0) {
                        successiveNode->list[2] = actualNode;
                        successiveNode->array[2] = 1;
                    }
                    if (successiveNode->array[0] == arrive) {
                        int i = 0;
                        int stations[200];
                        struct searchNode* search = successiveNode;
                        while (search->array[0] != start) {
                            stations[i] = search->array[0];
                            i++;
                            if (search->list[2] != NULL) search = search->list[2];
                            else break;
                        }
                        search = headList;
                        while (search->array[0] != stations[i - 1]) search = search->list[0];
                        if (stations[i - 1] - start <= search->array[1] && stations[0] == arrive) {
                            printf("%i ", start);
                            for (int k = i - 1; k != 0; k--) printf("%i ", stations[k]);
                            printf("%i\n", arrive);
                            return;
                        }
                    }
                    if (successiveNode->list[0] != NULL) successiveNode = successiveNode->list[0];
                    else break;
                }
                if (actualNode->list[0] != NULL) {
                    actualNode = actualNode->list[0];
                    if (actualNode->list[0] != NULL) successiveNode = actualNode->list[0];
                    else break;
                }
                else break;
            }
            fputs("nessun percorso\n", stdout);
            while (headList != NULL) {
                successiveNode = headList;
                if (headList->list[0] != NULL) {
                    headList = headList->list[0];
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
        while (pointer->kilometre != arrive) pointer = pointer->next;
        struct station* goal = head;
        while (goal->kilometre != start) goal = goal->next;
        if (goal->kilometre - pointer->kilometre <= goal->vehicle->autonomy) {
            fprintf(stdout, "%i %i\n", start, arrive);
            return;
        }
        else {
            struct searchNode *headList;
            headList = (struct searchNode *) malloc(sizeof(searchNode));
            headList->array[0] = pointer->kilometre;
            headList->array[1] = pointer->vehicle->autonomy;
            headList->array[2] = 0;
            headList->list[0] = NULL;
            headList->list[1] = NULL;
            headList->list[2] = NULL;
            pointer = pointer->next;
            struct searchNode *actualNode = headList;
            while (pointer->kilometre <= start) {
                struct searchNode *newNode;
                newNode = (struct searchNode *) malloc(sizeof(searchNode));
                newNode->array[0] = pointer->kilometre;
                newNode->array[1] = pointer->vehicle->autonomy;
                newNode->array[2] = 0;
                newNode->list[0] = NULL;
                newNode->list[1] = actualNode;
                newNode->list[2] = NULL;
                actualNode->list[0] = newNode;
                actualNode = newNode;
                if (pointer->next != NULL) pointer = pointer->next;
                else break;
            }
            actualNode = headList;
            struct searchNode *successiveNode = actualNode->list[0];
            while (actualNode->array[0] <= start) {
                while (successiveNode->array[1] >= (successiveNode->array[0] - actualNode->array[0])) {
                    if (successiveNode->array[2] == 0) {
                        successiveNode->list[2] = actualNode;
                        successiveNode->array[2] = 1;
                    }
                    if (successiveNode->array[0] == start) {
                        int i = 0;
                        int stations[200];
                        struct searchNode* search = successiveNode;
                        while (search->array[0] != arrive) {
                            stations[i] = search->array[0];
                            i++;
                            if (search->list[2] != NULL) search = search->list[2];
                            else break;
                        }
                        search = headList;
                        while (search->array[0] != stations[i - 1]) search = search->list[0];
                        if (stations[i - 1] - arrive <= search->array[1] && stations[0] == start) {
                            printf("%i ", start);
                            for (int k = 1; k != i; k++) printf("%i ", stations[k]);
                            printf("%i\n", arrive);
                            return;
                        }
                    }
                    if (successiveNode->list[0] != NULL) successiveNode = successiveNode->list[0];
                    else break;
                }
                if (actualNode->list[0] != NULL) {
                    actualNode = actualNode->list[0];
                    if (actualNode->list[0] != NULL) successiveNode = actualNode->list[0];
                    else break;
                }
                else break;
            }
            fputs("nessun percorso\n", stdout);
            while (headList != NULL) {
                successiveNode = headList;
                if (headList->list[0] != NULL) {
                    headList = headList->list[0];
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