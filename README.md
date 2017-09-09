# Duplicate detection

This is our solution to the duplicate detection assignment at EDBT summer school
2017.

It consists of duplicate detection algorithms written in C (file
`detect_duplicates.c`), and a Python framework to test the algorithms and
compare their performances using precision, recall, f-measure and execution
time (file `detect_duplicates.py`). The Python code communicates with the C
library using `ctypes` (file `detect_duplicates_wrapper.py`).

## Usage

You need to compile the C library, then start the script with Python3 (you will
need to have numpy installed; `pip install numpy` on most Unix-based systems).

```sh
make
./detect_duplicates.py
```

## Algorithms

### Naive algorithm

By simply comparing the representation strings of each restaurant, we determine
that two restaurants are duplicates iff they have the exact same representation.
This algorithm shows a f-measure of 0.10.

### Jaro similarity

Using Jaro similarity function, we compare each field of each pair of
restaurants. We say that two restaurants are duplicates if the similarity
between their fields exceeds a certain threshold, which is determined per-field.

The thresholds were determined empirically: we tried all combinations for all
thresholds between 0.5 and 1 using a step of 0.1, then refined the coefficients
more and more as our experiments told us where the (possibly local) optima were.

In particular, we found that the `type` field is irrelevant: we got our maximum
f-measure with a type threshold of 0.

The `name` field has a threshold of 0.95, which means it is usually fairly
consistent through duplicates.

The `address` field, on the other hand, has a threshold of 0.54: we interpreted
it as a consequence of the fact that two addresses that should be the same can
be written in very different ways.

The `city` field has a threshold of 0.77, which means it is usually more
consistent than addresses, but not as much as the restaurant's name.

With this algorithm, we reached a f-measure of 0.83, with a precision of 0.96
(very few false positives) and a recall of 0.74 (quite a few false negatives).
These probably come from the duplicate restaurants that have their name written
very differently from the original record, which won't be captured because of
the high similarity threshold that we set for the restaurant's name.

### n-grams similarity

Please note that the n-grams implementation is still under development and the precision, recall, and f-measure will continue to change over the next days.

Basically, given two strings (string1 and string2), the strings are split up into grams of size n.
e.g., "andreas" results in the grams "and" "ndr" "dre" "rea" and "eas"

If over half the grams in the two strings are exact matches we claim that the two strings match.
Note: threshold subject to change.

If three fields (name, address, city, type) of the four match, then the two restaurants are declared duplicate.

If a string has a length less than the set n (e.g., n=4, but the string is "mic"), then the implementation returns a negative comparison by default.

