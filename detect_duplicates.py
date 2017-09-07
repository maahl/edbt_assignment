#!/usr/bin/env python3

import csv
import detect_duplicates_wrapper as dedup
from detect_duplicates_wrapper import Restaurant
import operator


DATASET_FILE = 'restaurant_dataset.csv'


def true_positives(duplicates, actual_duplicates):
    tp = []

    for d in duplicates:
        if (d[0], d[1]) in actual_duplicates:
            tp.append(d)

    return tp


def false_negatives(duplicates, actual_duplicates):
    fn = []
    for ad in actual_duplicates:
        if (ad[0], ad[1]) not in duplicates:
            fn.append(ad)

    return fn


def false_positives(duplicates, actual_duplicates):
    fp = []
    for d in duplicates:
        if (d[0], d[1]) not in actual_duplicates:
            fp.append(d)

    return fp


def union(set1, set2):
    result = []
    for x in set1 + set2:
        if x not in result:
            result.append(x)

    return result


def recall(duplicates, actual_duplicates):
    tp = true_positives(duplicates, actual_duplicates)

    return len(tp) / len(actual_duplicates)


def precision(duplicates, actual_duplicates):
    tp = true_positives(duplicates, actual_duplicates)

    return len(tp) / len(duplicates)


def f_measure(duplicates, actual_duplicates):
    r = recall(duplicates, actual_duplicates)
    p = precision(duplicates, actual_duplicates)

    if r + p == 0:
        return 0

    return (2 * r * p) / (r + p)

def get_actual_duplicates(restaurants):
    seen = []
    duplicates = []
    for i, r in enumerate(restaurants):
        if r.restaurant_id not in seen:
            seen.append(r.restaurant_id)
        else:
            duplicates.append((r.restaurant_id, i))

    duplicates.sort(key = lambda x: x[0])
    return duplicates


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

    # start by computing the actual duplicates
    actual_duplicates = get_actual_duplicates(restaurants)

    for method in dedup.DUPLICATION_DETECTION_FUNCTIONS.keys():
        print('Computing duplicates for ' + method + '... ', end='')
        duplicates = dedup.duplicates_detection(method, restaurants)

        print('f_measure = ' + str(f_measure(duplicates, actual_duplicates)))
