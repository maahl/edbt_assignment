#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct restaurant_t {
    char name[256];
    char address[256];
    char city[256];
    char type[256];
    int id;
} restaurant_t;


typedef struct duplicate_t {
    int id;
    int parent_id; // by convention, the restaurant with the lowest id is the parent
} duplicate_t;


typedef struct duplicates_t {
    duplicate_t duplicates[864];
    int num_duplicates;
} duplicates_t;


// From a list of restaurants, return the list of duplicates
duplicates_t naive_implementation(restaurant_t * restaurants, int num_restaurants) {
    // TODO

    // print the input
    for(int i=0; i<num_restaurants; i++) {
        printf(
            "NAME: %s ; ADDRESS: %s ; CITY: %s ; TYPE: %s ; ID: %d\n", 
            restaurants[i].name,
            restaurants[i].address,
            restaurants[i].city,
            restaurants[i].type,
            restaurants[i].id
        );
    }

    // generate a dummy list of duplicates
    duplicates_t duplicates;
    duplicates.num_duplicates = 0;

    for(int i=0; i<5; i++) {
        duplicate_t duplicate;
        duplicate.id = i * 2;
        duplicate.parent_id = i * 1;

        duplicates.duplicates[i] = duplicate;
        duplicates.num_duplicates++;
    }

    return duplicates;
}


int main() {
    return 0;
}
