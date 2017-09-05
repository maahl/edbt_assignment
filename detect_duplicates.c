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
    // silence the compiler warning about this useless argument
    // note: it is useless because the parsed string is null terminated
    (void) num_bytes;

    static int current_field_id = 0;
    restaurant_t * restaurant = (restaurant_t *) data;

    switch(current_field_id) {
        case 0:
            // initialize the restaurant's name
            strcpy(restaurant->name, parsed_data);
            restaurant->address[0] = '\0';
            restaurant->city[0] = '\0';
            restaurant->type[0] = '\0';
            restaurant->id = 0;
            break;
        case 1:
            // initialize the restaurant's address
            strcpy(restaurant->address, parsed_data);
            break;
        case 2:
            // initialize the restaurant's city
            strcpy(restaurant->city, parsed_data);
            break;
        case 3:
            // initialize the restaurant's type
            strcpy(restaurant->type, parsed_data);
            break;
        case 4: ; // fix for weird C behaviour, see https://stackoverflow.com/a/18496437/2451259
            // initialize the restaurant's id
            // we first need to remove the quotes around the id (who had this idea?)

            // this removes the first quote
            char* id = (char*) parsed_data + 1;
            // this finds the index of the second quote and replaces it with \0
            *(strchr(id, '\'')) = '\0';

            restaurant->id = atoi(id);
            break;
        default:
            // this should never happen
            exit(EXIT_FAILURE);
    }

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

    size_t buffer_size = 1024;
    char* buffer;;
    size_t bytes_read;
    int i = 0;
    FILE *fp = fopen(DATASET_FILE, "rb");

    // initialize the list of restaurants
    while((bytes_read = getline(&buffer, &buffer_size, fp)) > 0) {
        csv_parse(&p, &buffer, bytes_read, &init_restaurant, NULL, (void *) &(restaurants[i]));

        printf("ORIGINAL DATA:  %s", buffer);
        printf("PARSED DATA:    ");
        print_restaurant(restaurants[i]);
        printf("--\n");
        i++;
    }

    fclose(fp);

    // deinit the csv parser
    csv_fini(&p, NULL, NULL, NULL);

    return 0;
}
