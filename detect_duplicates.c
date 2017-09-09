#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DATASET_SIZE 864
#define STRING_SIZE 256


typedef struct restaurant_t restaurant_t;

/**
 * Linked list
 * Struct to hold each restaurant
 */
struct restaurant_t {
    char name[STRING_SIZE];
    char address[STRING_SIZE];
    char city[STRING_SIZE];
    char type[STRING_SIZE];
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
    duplicate_t duplicates[DATASET_SIZE];
    int num_duplicates;
} duplicates_t;

static void printRestaurant(restaurant_t restaurant){
    fprintf(stdout,
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
        if(i+1<num_restaurants){
            currentRestaurant->next = calloc(1, sizeof(restaurant_t));
            currentRestaurant = currentRestaurant->next;
        } else
            currentRestaurant->next = NULL;
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
            }
        }
    }

    return duplicates;
}


int compareNGrams(char * string1, char *string2, long n){
    /* If either string is smaller than the set size of n */
    long size1 = strlen(string1);
    long size2 = strlen(string2);

    if (size1 < n || size2 < n) {
        return 0; /* There are no similar n-grams */
    }

    float frequency = 0;

    for(int i=0; i<size1 - n; i+=1) {
        for(int j=0; j<size2 - n; j+=1) {
            if(strncmp(string1+i, string2+j, n) == 0) { /*they are equal*/
                frequency++;
            }
        }
    }

    float similarity = frequency/(size1/n);

    return similarity > 0.5;
}

static int duplicateIdCmp(const void *duplicate1, const void *duplicate2){

    duplicate_t *d1 = (duplicate_t *) duplicate1;
    duplicate_t *d2 = (duplicate_t *) duplicate2;

    int id1 = d1->original_id;
    int id2 = d2->original_id;

    if (id1<id2)
        return -1;
    if (id1>id2)
        return 1;

    /* they are equal */
    return 0;
}

duplicates_t ngrams_implementation(restaurant_t * restaurants, int num_restaurants) {
    restaurant_t *restaurantList = initializeRestaurantList(restaurants, num_restaurants);
    restaurant_t *currentRestaurant = restaurantList;

    /* Testing with 3-grams */
    long n = 3;

    /* Counters to track the row number in the csv file */
    int currentRow = 1;
    int comparisonRow = 2;

    /* Initialize variables to hold duplicates */
    duplicates_t duplicates;
    duplicates.num_duplicates = 0;

    /* Let's go through the linked list ..*/
    currentRestaurant = restaurantList;
    restaurant_t *previousRestaurant = currentRestaurant;
    restaurant_t *comparisonRestaurant = restaurantList->next;

    while(currentRestaurant!=NULL) {
        while(comparisonRestaurant!=NULL) {
/*            fprintf(stderr, "Comparing %d - %s and %d - %s \n", currentRow,
                    currentRestaurant->name, comparisonRow,
                    comparisonRestaurant->name);*/
            long addressSim = compareNGrams(&currentRestaurant->address[0],
                    &comparisonRestaurant->address[0],
                    n /* size of n in ngrams */);
            long citySim = compareNGrams(&currentRestaurant->city[0],
                    &comparisonRestaurant->city[0], n);
            long nameSim = compareNGrams(&currentRestaurant->name[0],
                    &comparisonRestaurant->name[0], n);
            long typeSim = compareNGrams(&currentRestaurant->type[0],
                    &comparisonRestaurant->type[0], n);

            /* temporarily hold a pointer to the current comparison restaurant */
            restaurant_t *tmpRestaurant = comparisonRestaurant;
            /* progress the comparison restaurant */
            comparisonRestaurant = comparisonRestaurant->next;
            /* If we are above the threshold for similarity */
            if ((addressSim + citySim + nameSim + typeSim) > 3) {
                fprintf(stderr, "ngrams: duplicate detected %d\n", currentRestaurant->id);
                /* Set the id and object index in the duplicates array */
                duplicates.duplicates[duplicates.num_duplicates].original_id = currentRestaurant->id;
                duplicates.duplicates[duplicates.num_duplicates].dataset_index = comparisonRow;
                duplicates.num_duplicates++;

                /* free the detected duplicate - no need to compare it with anything again */
                /* If the list is A->B->C and we're freein'g B, let A point to C (A->C) */
                previousRestaurant->next = comparisonRestaurant;
                free(tmpRestaurant);
            } else
                previousRestaurant = tmpRestaurant;
            comparisonRow++;
        }
        if(currentRestaurant->next == NULL)
            break;
        else {
            currentRestaurant = currentRestaurant->next;
            comparisonRestaurant = currentRestaurant->next;
            currentRow++;
            comparisonRow = currentRow + 1;
        }

    }

/*    qsort(&duplicates.duplicates[0], duplicates.num_duplicates,
            sizeof(duplicate_t), duplicateIdCmp);
    for (int k = 0; k < duplicates.num_duplicates; k++) {
        fprintf(stderr,
                "nGrams_implementation: duplicates original_id %d index %d\n",
                duplicates.duplicates[k].original_id,
                duplicates.duplicates[k].dataset_index);

    }*/

    return duplicates;
}

float max(float x, float y) { return x > y ? x : y; }
float min(float x, float y) { return x < y ? x : y; }


// https://en.wikipedia.org/wiki/Jaro%E2%80%93Winkler_distance
float jaro_similarity(char s1[STRING_SIZE], char s2[STRING_SIZE]) {
    float l1 = strlen(s1);
    float l2 = strlen(s2);

    char s1_common[STRING_SIZE];
    int s1_common_len = 0;
    char s2_common[STRING_SIZE];
    int s2_common_len = 0;

    float max_distance = .5 * max(l1, l2) - 1;

    float c = 0; // number of common characters between s1 and s2
    float t = 0; // number of transpositions between s1 and s2

    // two characters match if they're present in both strings not too far apart
    for(int i=0; i<min(l1, l2); i++) {
        char c1 = s1[i];
        for(int j=max(0, i - max_distance); j < min(l2, i + max_distance); j++) {
            char c2 = s2[j];
            if(c2 == c1) {
                s1_common[s1_common_len] = c1;
                s1_common_len++;
                s2_common[s2_common_len] = c2;
                s2_common_len++;
                c++;
                break;
            }
        }
    }
    s1_common[s1_common_len] = '\0';
    s2_common[s2_common_len] = '\0';

    // compute number of transpositions
    for(int i=0; i<min(l1, l2); i++) {
        if(s1_common[i] != s2_common[i]) {
            t++;
        }
    }

    if(c == 0) { return 0; }

    float similarity = (c / l1 + c / l2 + (c - .5 * t) / c) / 3.;

    return similarity;
}


// resolve entity using jaro similarity
// two restaurants are considered duplicates if the similarity of their fields exceeds a specified per-field threshold
duplicates_t jaro_similarity_entity_resolution_with_coeffs(restaurant_t * restaurants, int num_restaurants, float name_similarity_threshold, float address_similarity_threshold, float city_similarity_threshold, float type_similarity_threshold) {
    duplicates_t duplicates;
    duplicates.num_duplicates = 0;

    for(int i=0; i<num_restaurants; i++) {
        for(int j=i+1; j<num_restaurants; j++) {
            restaurant_t ri = restaurants[i];
            restaurant_t rj = restaurants[j];

            // compute individual similarity for each field
            float name_sim = jaro_similarity(ri.name, rj.name);
            float address_sim = jaro_similarity(ri.address, rj.address);
            float city_sim = jaro_similarity(ri.city, rj.city);
            float type_sim = jaro_similarity(ri.type, rj.type);

            if(
                name_sim >= name_similarity_threshold &&
                address_sim >= address_similarity_threshold &&
                city_sim >= city_similarity_threshold &&
                type_sim >= type_similarity_threshold
            ) {
                duplicates.duplicates[duplicates.num_duplicates].original_id = ri.id;
                duplicates.duplicates[duplicates.num_duplicates].dataset_index = j;
                duplicates.num_duplicates++;
            }
        }
    }

    return duplicates;
}


duplicates_t jaro_similarity_entity_resolution(restaurant_t * restaurants, int num_restaurants) {
    return jaro_similarity_entity_resolution_with_coeffs(restaurants, num_restaurants, .95, .54, .77, 0);
}


int main() {
    return 0;
}
