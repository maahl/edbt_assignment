#include "csv.h"

typedef struct csv_parser csv_parser;

const char DATASET_FILE[] = "restaurant_dataset.csv";

int main() {
    csv_parser p;
    // initialize the csv parser
    if (csv_init(&p, CSV_STRICT) != 0) {
        fprintf(stderr, "Failed to initialize csv parser\n");
        exit(EXIT_FAILURE);
    }

    // deinit the csv parser
    csv_fini(&p, NULL, NULL, NULL);

    return 0;
}
