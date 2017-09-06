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
 * id: The ID of the restaurant with duplicates in the csv file
 * num_duplicates: How many duplicates we've detected for this restaurant
 * index[]: Array of
*/
typedef struct duplicate_t {
    int id;
    int num_duplicates;
    int index[863]; // TODO: change to pointer and optimize with calloc's etc.
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
        printRestaurant(*currentRestaurant);
        currentRestaurant = currentRestaurant->next;
    }
    return startRestaurant;
}

// From a list of restaurants, return the list of duplicates
duplicates_t naive_implementation(restaurant_t * restaurants, int num_restaurants) {
    // TODO

    restaurant_t *restaurantList = initializeRestaurantList(restaurants, num_restaurants);
    restaurant_t *currentRestaurant = restaurantList;
    // print the input
    for(int i=0; i<num_restaurants; i++) {
        printRestaurant(*currentRestaurant);
        currentRestaurant = currentRestaurant->next;
    }
    duplicates_t duplicates;
    return duplicates;
}


int main() {
    return 0;
}
