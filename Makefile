all:
	gcc -shared -fPIC -o detect_duplicates.so detect_duplicates.c

clean:
	rm detect_duplicates.so
