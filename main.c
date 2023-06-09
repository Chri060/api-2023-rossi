#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct station {
    int km;
    struct vehicle* vehicle;
    struct station* pre;
    struct station* next;
} station;

typedef struct vehicle {
    int autonomy;
    struct vehicle* next;
} vehicle ;

typedef struct searchNode {
    int km;
    int autonomy;
    struct searchNode* next;
} searchNode;

typedef struct bfsNode {
    int km;
    int autonomy;
    int iteration;
    struct bfsNode* next;
    struct bfsNode* pre;
} bfsNode;

station* list;

void addStation();
void removeStation();
void addAuto();
void removeAuto();
void planPath();

int main() {
    char string[532];
    //freopen("/Users/christianrossi/Documents/UTM/API/open_testcases/open_9.txt","r",stdin);
    while (fgetc(stdin) != EOF) {
        if (!scanf("%s", string)) return -1;
        switch (string[11]) {
            case 'z':
                addStation();
                fgetc(stdin);
                break;
            case 'a':
                removeStation();
                fgetc(stdin);
                break;
            case 'o':
                addAuto();
                fgetc(stdin);
                break;
            case 'r':
                planPath();
                fgetc(stdin);
                break;
            default:
                removeAuto();
                fgetc(stdin);
                break;
        }
    }
    fflush(stdout);
    return 0;
}

void addStation() {
    bool added = true;
    int km;
    if (!scanf("%i", &km)) return;
    station *stationPointer = list;
    station *newStation = (station *) malloc(sizeof(station));
    newStation->km = km;
    newStation->vehicle = NULL;
    newStation->pre = NULL;
    newStation->next = NULL;
    if (list == NULL) list = newStation;
    else if (newStation->km < stationPointer->km) {
        newStation->next = list;
        list->pre = newStation;
        list = newStation;
    } else {
        while (stationPointer->next != NULL && stationPointer->next->km <= newStation->km) stationPointer = stationPointer->next;
        if (stationPointer->km == newStation->km) added = false;
        else {
            if (stationPointer->next != NULL) {
                stationPointer->next->pre = newStation;
                newStation->next = stationPointer->next;
            }
            stationPointer->next = newStation;
            newStation->pre = stationPointer;
        }
    }
    if (added == true) {
        int length, autonomy;
        if (!scanf("%i", &length)) return;
        for (int i = 0; i < length; i++) {
            if (!scanf("%i", &autonomy)) return;
            vehicle* vehiclePointer = newStation->vehicle;
            vehicle* newVehicle = malloc(sizeof(vehicle));
            newVehicle->autonomy = autonomy;
            newVehicle->next = NULL;
            if (newStation->vehicle == NULL) newStation->vehicle = newVehicle;
            else if (newStation->vehicle->autonomy < newVehicle->autonomy) {
                newVehicle->next = newStation->vehicle;
                newStation->vehicle = newVehicle;
            } else {
                while (vehiclePointer->next != NULL && vehiclePointer->next->autonomy > newVehicle->autonomy) vehiclePointer = vehiclePointer->next;
                if (vehiclePointer->next != NULL) newVehicle->next = vehiclePointer->next;
                vehiclePointer->next = newVehicle;
            }
        }
    }
    else {
        int length, autonomy;
        if (!scanf("%i", &length)) return;
        for (int i = 0; i < length; i++) {
            if (!scanf("%i", &autonomy)) return;
        }
    }
    if (added == false) {
        free(newStation);
        fputs("non aggiunta\n", stdout);
    }
    else fputs("aggiunta\n", stdout);
}

void removeStation() {
    int km;
    if (!scanf("%i", &km)) return;
    if (list == NULL) {
        fputs("non demolita\n", stdout);
        return;
    }
    station *previousStation = NULL, *currentStation = NULL, *nextStation = NULL;
    currentStation = list;
    nextStation = currentStation->next;
    while (currentStation->km < km && nextStation != NULL) {
        previousStation = currentStation;
        currentStation = nextStation;
        nextStation = nextStation->next;
    }
    if (currentStation->km != km) {
        fputs("non demolita\n", stdout);
        return;
    }
    if (previousStation != NULL) previousStation->next = nextStation;
    else list = nextStation;
    if (nextStation != NULL) nextStation->pre = previousStation;
    vehicle *toDemolish;
    vehicle *nextInLine = currentStation->vehicle;
    while (nextInLine != NULL) {
        toDemolish = nextInLine;
        nextInLine = nextInLine->next;
        free(toDemolish);
    }
    free(currentStation);
    fputs("demolita\n", stdout);
}

void addAuto() {
    int km, autonomy;
    if (!scanf("%i", &km)) return;
    if (!scanf("%i", &autonomy)) return;
    station* stationPointer = list;
    while (stationPointer->next != NULL && stationPointer->next->km <= km) stationPointer = stationPointer->next;
    if (stationPointer->km == km){
        vehicle* vehiclePointer = stationPointer->vehicle;
        vehicle* newVehicle = malloc(sizeof(vehicle));
        newVehicle->autonomy = autonomy;
        newVehicle->next = NULL;
        if (stationPointer->vehicle == NULL) stationPointer->vehicle = newVehicle;
        else if (stationPointer->vehicle->autonomy < newVehicle->autonomy) {
            newVehicle->next = stationPointer->vehicle;
            stationPointer->vehicle = newVehicle;
        } else {
            while (vehiclePointer->next != NULL && vehiclePointer->next->autonomy > newVehicle->autonomy) vehiclePointer = vehiclePointer->next;
            if (vehiclePointer->next != NULL) newVehicle->next = vehiclePointer->next;
            vehiclePointer->next = newVehicle;
        }
        fputs("aggiunta\n", stdout);
    }
    else fputs("non aggiunta\n", stdout);
}

void removeAuto() {
    int km, autonomy;
    bool removed = false;
    if (!scanf("%i", &km)) return;
    if (!scanf("%i", &autonomy)) return;
    station* stationPointer = list;
    while (stationPointer->next != NULL && stationPointer->next->km <= km) stationPointer = stationPointer->next;
    if (stationPointer->km == km){
        vehicle* vehiclePointer = stationPointer->vehicle;
        if (stationPointer->vehicle->autonomy == autonomy) {
            stationPointer->vehicle = vehiclePointer->next;
            free(vehiclePointer);
            removed = true;
        }
        else {
            while (vehiclePointer->next != NULL && vehiclePointer->next->autonomy > autonomy) vehiclePointer = vehiclePointer->next;
            if (vehiclePointer->next != NULL && vehiclePointer->next->autonomy == autonomy) {
                vehicle* toFree = vehiclePointer->next;
                vehiclePointer->next = vehiclePointer->next->next;
                free(toFree);
                removed = true;
            }
        }
    }
    if (removed == false) fputs("non rottamata\n", stdout);
    else fputs("rottamata\n", stdout);
}

void planPath() {
    int start, arrive;
    if (!scanf("%i", &start)) return;
    if (!scanf("%i", &arrive)) return;
    if (start == arrive) {
        printf("%i\n", start);
        return;
    }
    else if (start > arrive) {
        station* pointer = list;
        while (pointer->km != arrive) {
            pointer = pointer->next;
        }
        if (pointer->vehicle->autonomy >= start - arrive) {
            printf("%i %i\n", start, arrive);
            return;
        }
        while (pointer->km != start) {
            pointer = pointer->next;
        }
        bfsNode * headNode = malloc(sizeof(bfsNode));
        headNode->km = pointer->km;
        headNode->autonomy = pointer->vehicle->autonomy;
        headNode->iteration = 0;
        headNode->pre = NULL;
        headNode->next = NULL;
        bfsNode* activeNode = headNode;
        bfsNode* actualPosition = headNode;
        bfsNode* rewindNode = headNode;
        pointer = pointer->pre;
        int iteration = 0;
        bool found = false;
        while (1) {
            while (activeNode->km - pointer->km <= activeNode->autonomy) {
                bfsNode * newNode = malloc(sizeof(bfsNode));
                newNode->km = pointer->km;
                newNode->iteration = activeNode->iteration + 1;
                newNode->autonomy = pointer->vehicle->autonomy;
                newNode->pre = activeNode;
                newNode->next = NULL;
                if (rewindNode->next == NULL) {
                    actualPosition->next = newNode;
                    actualPosition = newNode;
                }
                else if (newNode->km < rewindNode->next->km) {
                    newNode->next = rewindNode->next;
                    rewindNode->next = newNode;
                }
                else {
                    bfsNode* treePointer = rewindNode;
                    while (treePointer->next != NULL && treePointer->next->km && treePointer->next->iteration == newNode->iteration) treePointer = treePointer->next;
                    if (treePointer->next == NULL) treePointer->next = newNode;
                    else {
                        newNode->next = treePointer->next;
                        treePointer->next = newNode;
                    }
                }
                if (newNode->km == arrive) {
                    found = true;
                    activeNode = newNode;
                    break;
                }
                pointer = pointer->pre;
            }
            if (found == true) break;
            if (activeNode->next != NULL) activeNode = activeNode->next;
            else break;
            if (activeNode->iteration > iteration) {
                iteration = activeNode->iteration;
                rewindNode = actualPosition;
            }
        }
        if (found == true) {
            int array[364];
            int i = 0;
            while (1) {
                array[i] = activeNode->km;
                if (activeNode->pre != NULL) activeNode = activeNode->pre;
                else break;
                i++;
            }
            while (1) {
                printf("%i ", array[i]);
                i--;
                if (i == 0) break;
            }
            printf("%i\n", array[0]);
        }
        else fputs("nessun percorso\n", stdout);
        bfsNode* prev;
        while (1) {
            prev = headNode;
            if (headNode->next != NULL) headNode = headNode->next;
            else {
                free(headNode);
                break;
            }
            free (prev);
        }
    }
    else if (start < arrive) {
        station* pointer = list;
        while (pointer->km != start) pointer = pointer->next;
        if (pointer->vehicle->autonomy >= arrive - start) {
            printf("%i %i\n", start, arrive);
            return;
        }
        while (pointer->km != arrive) pointer = pointer->next;
        searchNode* headNode = malloc(sizeof(searchNode));
        headNode->next = NULL;
        headNode->km = pointer->km;
        headNode->autonomy = pointer->vehicle->autonomy;
        searchNode* actualNode = headNode;
        pointer = pointer->pre;
        while (1) {
            if (pointer->vehicle->autonomy < actualNode->km - pointer->km) {
                if (pointer->km == start) break;
                pointer = pointer->pre;
            }
            else {
                searchNode *newNode = malloc(sizeof(searchNode));
                newNode->next = NULL;
                newNode->km = pointer->km;
                newNode->autonomy = pointer->vehicle->autonomy;
                while (actualNode->next != NULL && newNode->autonomy >= actualNode->next->km - newNode->km) {
                    searchNode *temp = actualNode;
                    if (actualNode->next != NULL) actualNode = actualNode->next;
                    else break;
                    free(temp);
                }
                newNode->next = actualNode;
                actualNode = newNode;
                pointer = pointer->pre;
                if (actualNode->km == start) break;
            }
        }
        if (actualNode->km == start) {
            while (1) {
                if (actualNode->km == arrive) {
                    printf("%i\n", actualNode->km);
                    free(actualNode);
                    break;
                }
                else {
                    printf("%i ", actualNode->km);
                }
                searchNode* temp = actualNode;
                actualNode = actualNode->next;
                free(temp);
            }
        }
        else {
            while (1) {
                searchNode* temp = actualNode;
                if (actualNode->next == NULL)  {
                    free(actualNode);
                    break;
                }
                else actualNode = actualNode->next;
                free(temp);
            }
            fputs("nessun percorso\n", stdout);
        }
    }
}