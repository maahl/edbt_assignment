#!/usr/bin/env python3

import csv
import detect_duplicates_wrapper as dedup
from detect_duplicates_wrapper import Restaurant

DATASET_FILE = 'restaurant_dataset.csv'

if __name__ == '__main__':
    restaurants = [
        Restaurant('rubicon', '558 sacramento st.', 'san francisco', 'american', 508),
        Restaurant('halcyon', '151 w. 54th st.  in the rihga royal hotel', 'new york', 'american', 260)
    ]
    print(dedup.naive_implementation(restaurants))
