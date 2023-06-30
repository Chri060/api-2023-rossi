#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct station {
    struct station *prev;
    struct station *next;
    unsigned int pos;
    unsigned int *cars;
    unsigned int cars_size;
} Station;

typedef struct hop {
    struct hop *prev;
    struct hop *lat_l;
    struct hop *lat_r;
    Station *station;
} Hop;

typedef struct {
    unsigned int *stations;
    unsigned int len;
} Path;

Station *stations_list_start = NULL;
Station *stations_list_end = NULL;

bool add_station(unsigned int pos, unsigned int *cars, unsigned int cars_size);

bool del_station(unsigned int pos);

bool add_car(unsigned int pos, unsigned int limit);

bool del_car(unsigned int pos, unsigned int limit);

int comp_cars(const void *a, const void *b);

Path get_forward_path(unsigned int start, unsigned int end);

Path get_backward_path(unsigned int start, unsigned int end);

void cleanup_hops(Hop *base);

int main() {
    char command[19];
    while (!feof(stdin)) {
        if (scanf("%s", command) == EOF)return 0;
        switch (command[12]) {
            case 'z': { 
                unsigned int pos, cars_size;
                unsigned int *cars = malloc(sizeof(int) * 512);
                if (scanf("%d", &pos) == EOF) return -1;
                if (scanf("%d", &cars_size) == EOF) return -1;
                for (unsigned int i = 0; i < cars_size; i++) {
                    if (scanf("%d", &cars[i]) == EOF) return -1;
                }
                qsort(cars, cars_size, sizeof(int), comp_cars);
                if (add_station(pos, cars, cars_size)) printf("aggiunta\n");
                else {
                    free(cars);
                    printf("non aggiunta\n");
                }
                break;
            }
            case 'a': { 
                unsigned int pos;
                if (scanf("%d", &pos) == EOF) return -1;
                if (del_station(pos)) printf("demolita\n");
                else printf("non demolita\n");
                break;
            }
            case 'o': {
                unsigned int pos, limit;
                if (scanf("%d", &pos) == EOF) return -1;
                if (scanf("%d", &limit) == EOF) return -1;
                if (add_car(pos, limit)) printf("aggiunta\n");
                else printf("non aggiunta\n");
                break;
            }
            case '\0': {
                unsigned int pos, limit;
                if (scanf("%d", &pos) == EOF) return -1;
                if (scanf("%d", &limit) == EOF) return -1;
                if (del_car(pos, limit)) printf("rottamata\n");
                else printf("non rottamata\n");
                break;
            }
            case 'r': { 
                unsigned int start, end;
                if (scanf("%d %d", &start, &end) == EOF) return -1;
                Path path;
                if (end > start) path = get_forward_path(start, end);
                else if (end < start) path = get_backward_path(start, end);
                else {
                    printf("%d\n", start);
                    break;
                }
                if (path.stations == NULL) printf("nessun percorso\n");
                else {
                    for (unsigned int i = 0; i < path.len; i++) {
                        printf("%d", path.stations[i]);
                        if (i < path.len - 1) fputc(' ', stdout);
                    }
                    printf("\n");
                    free(path.stations);
                }
                break;
            }
        }
        if (fgetc(stdin) != '\n') return -1;
    }
}

int comp_cars(const void *a, const void *b) { return *((long *) b) - *((long *) a); }

bool add_station(unsigned int pos, unsigned int *cars, unsigned int cars_size) {
    Station *next = stations_list_start;
    Station *prev = NULL;
    Station *this = malloc(sizeof(Station));
    this->pos = pos;
    this->cars = cars;
    this->cars_size = cars_size;
    while (next != NULL && next->pos < pos) {
        prev = next;
        next = next->next;
    }
    this->next = next;
    this->prev = prev;
    if (next != NULL && next->pos == this->pos) {
        free(this);
        return false;
    }
    if (prev == NULL) stations_list_start = this;
    else  prev->next = this;
    if (next == NULL) stations_list_end = this;
    else next->prev = this;
    return true;
}

bool del_station(unsigned int pos) {
    Station *this = stations_list_start;
    while (this != NULL && this->pos < pos) this = this->next;
    if (this != NULL && this->pos == pos) {
        if (this->prev != NULL) this->prev->next = this->next;
        else stations_list_start = this->next;
        if (this->next != NULL) this->next->prev = this->prev;
        else stations_list_end = this->prev;
        free(this->cars);
        free(this);
        return true;
    }
    return false;
}

bool add_car(unsigned int pos, unsigned int limit) {
    Station *this = stations_list_start;
    while (this->pos < pos && this->next != NULL) this = this->next;
    if (this->pos != pos) return false;
    if (this->cars_size == 512) return false;
    unsigned int curr_max = this->cars_size > 0 ? this->cars[0] : limit;
    this->cars[0] = limit > curr_max ? limit : curr_max;
    this->cars[this->cars_size++] = limit > curr_max ? curr_max : limit;
    return true;
}

bool del_car(unsigned int pos, unsigned int limit) {
    Station *this = stations_list_start;
    while (this->pos < pos && this->next != NULL) this = this->next;
    if (this->pos != pos) return false;
    unsigned int i = 0;
    bool removed = false;
    for (; i < this->cars_size; i++) {
        if (this->cars[i] == limit) {
            removed = true;
            this->cars[i] = this->cars[--this->cars_size];
            break;
        }
    }
    if (removed && i == 0) qsort(this->cars, this->cars_size, sizeof(int), comp_cars);
    return removed;
}

Path get_forward_path(unsigned int start, unsigned int end) {
    unsigned int iterations = 1;
    Hop *prev_layer_start = NULL;
    Hop *curr_layer_start = NULL;
    Hop *curr_layer_hop = NULL;
    Hop *prev_layer_hop = NULL;
    Station *curr_station = stations_list_start;
    while (curr_station->pos < start) curr_station = curr_station->next;
    prev_layer_hop = malloc(sizeof(Hop));
    prev_layer_hop->station = curr_station;
    prev_layer_hop->lat_l = NULL;
    prev_layer_hop->lat_r = NULL;
    prev_layer_hop->prev = NULL;
    prev_layer_start = prev_layer_hop;
    curr_station = curr_station->next;
    bool found = false;
    while (curr_station != NULL) {
        if (curr_station->pos - prev_layer_hop->station->pos <=
            (prev_layer_hop->station->cars_size > 0 ?
            prev_layer_hop->station->cars[0] : 0)) {
            if (curr_layer_start == NULL) {
                curr_layer_hop = malloc(sizeof(Hop));
                curr_layer_hop->lat_l = NULL;
                curr_layer_hop->lat_r = NULL;
                curr_layer_hop->station = curr_station;
                curr_layer_hop->prev = prev_layer_hop;
                curr_layer_start = curr_layer_hop;
            } else {
                curr_layer_hop->lat_r = malloc(sizeof(Hop));
                Hop *temp_prev_hop = curr_layer_hop;
                curr_layer_hop = curr_layer_hop->lat_r;
                curr_layer_hop->lat_l = temp_prev_hop;
                curr_layer_hop->lat_r = NULL;
                curr_layer_hop->station = curr_station;
                curr_layer_hop->prev = prev_layer_hop;
            }
            if (curr_layer_hop->station->pos == end) {
                found = true;
                iterations++;
                break;
            }
            curr_station = curr_station->next;
        } else {
            if (prev_layer_hop->lat_r != NULL) prev_layer_hop = prev_layer_hop->lat_r;
            } else {
                if (curr_layer_start == NULL) break;
                prev_layer_hop = curr_layer_start;
                prev_layer_start = curr_layer_start;
                curr_layer_start = NULL;
                curr_layer_hop = NULL;
                iterations++;
            }
        }
    }
    if (found) {
        unsigned int *hops = malloc(iterations * sizeof(unsigned int));
        for (unsigned int i = iterations; i > 0; i--) {
            hops[i - 1] = curr_layer_hop->station->pos;
            curr_layer_hop = curr_layer_hop->prev;
        }
        Path path;
        path.stations = hops;
        path.len = iterations;
        cleanup_hops(curr_layer_start);
        return path;
    } else {
        Path path;
        path.stations = NULL;
        path.len = 0;
        cleanup_hops(curr_layer_start != NULL ? curr_layer_start : prev_layer_start);
        return path;
    }
}

Path get_backward_path(unsigned int start, unsigned int end) {
    unsigned int iterations = 1;
    Hop *prev_layer_start = NULL;
    Hop *curr_layer_start = NULL;
    Hop *curr_layer_hop = NULL;
    Hop *prev_layer_hop = NULL;
    Station *curr_station = stations_list_end;
    while (curr_station->pos > start) curr_station = curr_station->prev;
    prev_layer_hop = malloc(sizeof(Hop));
    prev_layer_hop->station = curr_station;
    prev_layer_hop->lat_l = NULL;
    prev_layer_hop->lat_r = NULL;
    prev_layer_hop->prev = NULL;
    prev_layer_start = prev_layer_hop;
    curr_station = curr_station->prev;
    bool found = false;
    while (curr_station != NULL) {
        if (prev_layer_hop->station->pos - curr_station->pos <=
            (prev_layer_hop->station->cars_size > 0 ?
            prev_layer_hop->station->cars[0] : 0)) {
            if (curr_layer_start == NULL) {
                curr_layer_hop = malloc(sizeof(Hop));
                curr_layer_hop->lat_l = NULL;
                curr_layer_hop->lat_r = NULL;
                curr_layer_hop->station = curr_station;
                curr_layer_hop->prev = prev_layer_hop;
                curr_layer_start = curr_layer_hop;
            } else {
                curr_layer_hop->lat_r = malloc(sizeof(Hop));
                Hop *temp_prev_hop = curr_layer_hop;
                curr_layer_hop = curr_layer_hop->lat_r;
                curr_layer_hop->lat_l = temp_prev_hop;
                curr_layer_hop->lat_r = NULL;
                curr_layer_hop->station = curr_station;
                curr_layer_hop->prev = prev_layer_hop;
                curr_layer_start = curr_layer_hop;
            }
            if (curr_layer_hop->station->pos == end) {
                found = true;
                iterations++;
                break;
            }
            curr_station = curr_station->prev;
        } else {
            if (prev_layer_hop->lat_l != NULL) prev_layer_hop = prev_layer_hop->lat_l;
            else {
                if (curr_layer_start == NULL) break;
                prev_layer_hop = curr_layer_start;
                prev_layer_start = curr_layer_start;
                curr_layer_start = NULL;
                curr_layer_hop = NULL;
                iterations++;
            }
        }
    }
    if (found) {
        unsigned int *hops = malloc(iterations * sizeof(unsigned int));
        for (unsigned int i = iterations; i > 0; i--) {
            hops[i - 1] = curr_layer_hop->station->pos;
            curr_layer_hop = curr_layer_hop->prev;
        }
        Path path;
        path.stations = hops;
        path.len = iterations;
        cleanup_hops(curr_layer_start);
        return path;
    } else {
        Path path;
        path.stations = NULL;
        path.len = 0;
        cleanup_hops(curr_layer_start != NULL ? curr_layer_start : prev_layer_start);
        return path;
    }
}

void cleanup_hops(Hop *base) {
    while (base->lat_l != NULL) base = base->lat_l;
    Hop *prev_layer = base->prev != NULL ? base->prev : NULL;
    Hop *curr = base;
    while (curr) {
        Hop *to_free = curr;
        if (curr->lat_r != NULL) curr = curr->lat_r;
        else {
            curr = prev_layer;
            prev_layer = prev_layer != NULL ? prev_layer->prev : NULL;
            if (curr != NULL) {
                while (curr->lat_l != NULL) curr = curr->lat_l;
            }
        }
        free(to_free);
    }
}
