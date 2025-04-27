all: simple_read_struct simple_read_union simple_union simple_struct

simple_struct: simple_struct.c
	gcc -g -Wall -Wall -Wno-unused-but-set-variable -o simple_struct simple_struct.c -lrt

simple_union: simple_union.c
	gcc -g -Wall -Wall -Wno-unused-but-set-variable -o simple_union simple_union.c -lrt

simple_read_union: simple_read_union.c
	gcc -g -Wall -Wall -Wno-unused-but-set-variable -o simple_read_union simple_read_union.c -lrt

simple_read_struct: simple_read_struct.c
	gcc -g -Wall -Wall -Wno-unused-but-set-variable -o simple_read_struct simple_read_struct.c -lrt

clean:
	rm simple_struct
	rm simple_union
	rm simple_read_struct
	rm simple_read_union

