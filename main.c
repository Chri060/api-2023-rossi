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



struct station* addStation(struct station* head);
struct station* removeStation(struct station* head);
struct station* addAuto(struct station* head);
struct station* removeAuto(struct station* head);
void planPath();


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
                planPath();
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
    }
    else {
        while (pointer->kilometre < km) {
            if (pointer->next != NULL) pointer = pointer->next;
            else break;
        }

        if (pointer->kilometre == km) {
            fputs("non-aggiunta\n", stdout);
            return head;
        }
        else {
            new = (struct station *) malloc(sizeof(station));
            new->kilometre = km;
            new->vehicle = NULL;
            if (pointer == head) {
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
                    if (point->next->autonomy < autonomy) {
                        break;
                    }
                    else {
                        point = point->next;
                    }
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
    fputs("aggiunta\n", stdout);
    return head;
}











struct station* removeStation(struct station* head) {
    return head;
}

struct station* addAuto(struct station* head) {
    return head;
}

struct station* removeAuto(struct station* head) {
    return head;
}

void planPath() {

}
