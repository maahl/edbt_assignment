#include "csv.h"
#include "string.h"

typedef struct csv_parser csv_parser;

const char DATASET_FILE[] = "restaurant_dataset.csv";

typedef struct restaurant_t {
    char name[256];
    char address[256];
    char city[256];
    char type[256];
    int id;
} restaurant_t;

void print_restaurant(restaurant_t restaurant) {
    printf("NAME: %s ; ADDRESS: %s ; CITY: %s ; TYPE: %s ; ID: %d\n", restaurant.name, restaurant.address, restaurant.city, restaurant.type, restaurant.id);
}

void init_restaurant(void * parsed_data, size_t num_bytes, void * data) {
    static int current_field_id = 0;
    restaurant_t * restaurant = (restaurant_t *) data;

    // initialize the restaurant's name
    if(current_field_id == 0) {
        strcpy(restaurant->name, parsed_data);
        restaurant->address[0] = '\0';
        restaurant->city[0] = '\0';
        restaurant->type[0] = '\0';
        restaurant->id = 0;
    }

    printf("%d\n", current_field_id);
    current_field_id = (current_field_id + 1) % 5;
}

int main() {
    csv_parser p;
    // initialize the csv parser
    if (csv_init(&p, CSV_STRICT | CSV_APPEND_NULL) != 0) {
        fprintf(stderr, "Failed to initialize csv parser\n");
        exit(EXIT_FAILURE);
    }
    csv_set_delim(&p, ';');

    restaurant_t restaurants[863];

    char buf[1024];
    FILE *fp = fopen(DATASET_FILE, "rb");
    size_t bytes_read;
    int i = 0;
    // initialize the list of restaurants
    while((bytes_read = fread(buf, 1, 1024, fp)) > 0) {
        csv_parse(&p, buf, bytes_read, &init_restaurant, NULL, (void *) &(restaurants[i]));

        print_restaurant(restaurants[i]);
        i++;
    }

    fclose(fp);

    // deinit the csv parser
    csv_fini(&p, NULL, NULL, NULL);

    return 0;
}
