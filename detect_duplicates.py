#!/usr/bin/env python3

import csv
import detect_duplicates_wrapper as dedup
from detect_duplicates_wrapper import Restaurant
import numpy as np
import operator
import sys
import time


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

    if len(duplicates) == 0:
        return 0

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


def optimize_jaro(actual_duplicates):
    # optimize coefficients for jaro
    max_f_measure = 0
    best_coeffs = [] # expects tuples like (f_measure, name, address, city, type) thresholds
    for name_similarity_threshold in np.arange(.93, .97, .02):
        for address_similarity_threshold in np.arange(.5, .60, .02):
            for city_similarity_threshold in np.arange(.75, .8, .02):
                for type_similarity_threshold in [0]:
                    duplicates = dedup.jaro_coeffs(restaurants, name_similarity_threshold, address_similarity_threshold, city_similarity_threshold, type_similarity_threshold)
                    f = f_measure(duplicates, actual_duplicates)
                    if f > max_f_measure:
                        max_f_measure = f
                        best_coeffs.append((f, name_similarity_threshold, address_similarity_threshold, city_similarity_threshold, type_similarity_threshold))
                        p = precision(duplicates, actual_duplicates)
                        r = recall(duplicates, actual_duplicates)
                        print()
                        print('Found f_measure = ', f, ', precision = ', p,  ', recall = ', r, ' for these coefficients:')
                        print('    name_similarity_threshold: ', name_similarity_threshold)
                        print('    address_similarity_threshold: ', address_similarity_threshold)
                        print('    city_similarity_threshold: ', city_similarity_threshold)
                        print('    type_similarity_threshold: ', type_similarity_threshold)
                    else:
                        print('.', end='')
                        sys.stdout.flush()

    print()
    best_coeffs.sort(key= lambda x: x[0], reverse=True)
    #for x in best_coeffs:
    #    print(x)


if __name__ == '__main__':
    restaurants = []

    with open(DATASET_FILE) as dataset:
        csv_reader = csv.reader(dataset, delimiter=';')
        for restaurant in csv_reader:
            restaurants.append(Restaurant(
                restaurant[0].strip().lower(), # name
                restaurant[1].strip().lower(), # address
                restaurant[2].strip().lower(), # city
                restaurant[3].strip().lower(), # type
                int(restaurant[4][1:-1]), # id, removing the quotes
            ))

    # start by computing the actual duplicates
    actual_duplicates = get_actual_duplicates(restaurants)

    for method in dedup.DUPLICATION_DETECTION_FUNCTIONS.keys():
        print('Computing duplicates for ' + method + '... ')
        start_time = time.time()
        duplicates = dedup.duplicates_detection(method, restaurants)
        execution_time = time.time() - start_time

        print('    execution time = ', execution_time, ' seconds')
        print('    precision = ', precision(duplicates, actual_duplicates))
        print('    recall = ', recall(duplicates, actual_duplicates))
        print('    f_measure = ', f_measure(duplicates, actual_duplicates))

    #optimize_jaro(actual_duplicates)
