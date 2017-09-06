#!/usr/bin/env python3

import csv
import detect_duplicates_wrapper as dedup
from detect_duplicates_wrapper import Restaurant

DATASET_FILE = 'restaurant_dataset.csv'

if __name__ == '__main__':
    restaurants = []

    with open(DATASET_FILE) as dataset:
        csv_reader = csv.reader(dataset, delimiter=';')
        for restaurant in csv_reader:
            restaurants.append(Restaurant(
                restaurant[0], # name
                restaurant[1], # address
                restaurant[2], # city
                restaurant[3], # type
                int(restaurant[4][1:-1]), # id, removing the quotes
            ))

    dedup.naive_implementation(restaurants)
    print(len(restaurants))
