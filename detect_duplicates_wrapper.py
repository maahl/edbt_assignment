import ctypes
import os


# wrapper for struct restaurant_t

class _Restaurant(ctypes.Structure):
    pass

_Restaurant._fields_ = [
    ('name', ctypes.c_char * 256),
    ('address', ctypes.c_char * 256),
    ('city', ctypes.c_char * 256),
    ('type', ctypes.c_char * 256),
    ('id', ctypes.c_int),
    ('next', ctypes.POINTER(_Restaurant)),
]

# corresponding python class
class Restaurant():
    def __init__(self, name, address, city, restaurant_type, restaurant_id):
        self.name = name
        self.address = address
        self.city = city
        self.restaurant_type = restaurant_type
        self.restaurant_id = restaurant_id


# wrapper for struct duplicate_t
class _Duplicate(ctypes.Structure):
    _fields_ = [
        ('id', ctypes.c_int),
        ('parent_id', ctypes.c_int),
    ]


# wrapper for classe duplicates_t
class _Duplicates(ctypes.Structure):
    _fields_ = [
        ('duplicates', ctypes.ARRAY(_Duplicate, 864)),
        ('num_duplicates', ctypes.c_int),
    ]


# library functions declaration
lib_path = os.path.join(os.path.dirname(os.path.realpath(os.path.abspath(__file__))), 'detect_duplicates.so')
_detect_duplicates = ctypes.CDLL(lib_path)

DUPLICATION_DETECTION_FUNCTIONS = {
    'naive': _detect_duplicates.naive_implementation,
}

for f in DUPLICATION_DETECTION_FUNCTIONS.values():
    f.argtypes = [
        ctypes.POINTER(_Restaurant),
        ctypes.c_int
    ]
    f.restype = _Duplicates


def duplicates_detection(method, restaurants):
    '''
    Generic wrapper for all duplication detection methods implemented
    '''
    restaurants_array = (_Restaurant * len(restaurants))()
    for i in range(len(restaurants)):
        r = restaurants[i]
        restaurants_array[i] = _Restaurant(
            r.name.encode(),
            r.address.encode(),
            r.city.encode(),
            r.restaurant_type.encode(),
            r.restaurant_id,
        )

    result = DUPLICATION_DETECTION_FUNCTIONS[method](
        ctypes.cast(restaurants_array, ctypes.POINTER(_Restaurant)),
        ctypes.c_int(len(restaurants))
    )

    duplicates = []
    for i in range(result.num_duplicates):
        d = result.duplicates[i]
        duplicates.append((d.id, d.parent_id))

    return duplicates
