#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct restaurant_t restaurant_t;

/**
 * Linked list
 * Struct to hold each restaurant
 */
struct restaurant_t {
    char name[256];
    char address[256];
    char city[256];
    char type[256];
    int id;
    restaurant_t *next; /* pointer to next restaurant in linked list */
};

/**
 * original_id: The ID of the restaurant this is a duplicate of
 * By convention, the first restaurant encountered is the original one and the others are duplicates
 * index: index of the duplicate in the dataset
*/
typedef struct duplicate_t {
    int original_id;
    int dataset_index;
} duplicate_t;

/**
 * Struct to hold all found duplicates
 * duplicates: Each restaurant with a duplicate. See the duplicate_t struct.
 * num_duplicates: Total number of duplicates found.
 */
typedef struct duplicates_t {
    duplicate_t duplicates[864];
    int num_duplicates;
} duplicates_t;

static void printRestaurant(restaurant_t restaurant){
    fprintf(stderr,
                "NAME: %s ; ADDRESS: %s ; CITY: %s ; TYPE: %s ; ID: %d\n",
                restaurant.name,
                restaurant.address,
                restaurant.city,
                restaurant.type,
                restaurant.id
            );
}

/**
 * Initialize a linked list of the restaurants from an array
 * of restaurants because we don't want to do it in Python
 */
restaurant_t *initializeRestaurantList(restaurant_t *restaurants, int num_restaurants){
    restaurant_t *startRestaurant, *currentRestaurant, *nextRestaurant;
    startRestaurant = calloc(1, sizeof(restaurant_t));
    currentRestaurant = startRestaurant;

    for (int i=0; i<num_restaurants; i++){
        memcpy(currentRestaurant, &restaurants[i], sizeof(restaurant_t));
        currentRestaurant->next = calloc(1, sizeof(restaurant_t));
        currentRestaurant = currentRestaurant->next;
    }

    currentRestaurant = startRestaurant;
    for (int j=0; j<num_restaurants; j++){
        currentRestaurant = currentRestaurant->next;
    }
    return startRestaurant;
}

// From a list of restaurants, return the list of duplicates
// Naive implementation: just compare the strings litterally
duplicates_t naive_implementation(restaurant_t * restaurants, int num_restaurants) {
    restaurant_t *restaurantList = initializeRestaurantList(restaurants, num_restaurants);
    restaurant_t *currentRestaurant = restaurantList;

    for(int i=0; i<num_restaurants; i++) {
        currentRestaurant = currentRestaurant->next;
    }
    duplicates_t duplicates;
    duplicates.num_duplicates = 0;

    for(int i=0; i<num_restaurants; i++) {
        for(int j=i+1; j<num_restaurants; j++) {
            restaurant_t ri = restaurants[i];
            restaurant_t rj = restaurants[j];

            if(
                strcmp(ri.name, rj.name) == 0 &&
                strcmp(ri.address, rj.address) == 0 &&
                strcmp(ri.city, rj.city) == 0 &&
                strcmp(ri.type, rj.type) == 0
              ) {
                duplicates.duplicates[duplicates.num_duplicates].original_id = ri.id;
                duplicates.duplicates[duplicates.num_duplicates].dataset_index = j;
                duplicates.num_duplicates++;
                duplicates.num_duplicates++;
            }
        }
    }

    return duplicates;
}


int main() {
    return 0;
}
