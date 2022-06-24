CFLAGS=-Wall -Wextra -Werror -std=c11 -ggdb

image-filters: main.c image-filters.c image-filters.c
	$(CC) $(CFLAGS) -o image-filters main.c image-filters.c
