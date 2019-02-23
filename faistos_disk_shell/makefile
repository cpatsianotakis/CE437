CC = gcc 
OBJ = build/main.o build/tcl/tcl.o build/readline/readline.o build/cubes/cube_functions.o build/cubes/cube_list.o build/graph/graph.o build/graph/graph_data_structure.o build/division/division_algebric.o build/r_kernels/r_kernels.o #build/espresso/espresso.o build/espresso/ucp.o
LIBS = -ltcl8.6 -lreadline

fds: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS)

build/main.o: src/main.c src/main.h
	@ mkdir -p build
	$(CC) -Wall -g src/main.c -o $@ $(LIBS) -c

build/tcl/tcl.o: src/tcl/tcl.c src/tcl/tcl.h
	@ mkdir -p build/tcl
	$(CC) -Wall -g src/tcl/tcl.c -o $@ $(LIBS) -c

build/readline/readline.o: src/readline/readline.c src/readline/readline.h
	@ mkdir -p build/readline
	$(CC) -Wall -g src/readline/readline.c -o $@ $(LIBS) -c

build/cubes/cube_functions.o: src/cubes/cube_functions.c src/cubes/cubes.h build/cubes/cube_list.o
	@ mkdir -p build/cubes
	$(CC) -Wall -g src/cubes/cube_functions.c -o $@ $(LIBS) -c

build/cubes/cube_list.o: src/cubes/cube_list.c src/cubes/cubes.h
	@ mkdir -p build/cubes
	$(CC) -Wall -g src/cubes/cube_list.c -o $@ -c

build/graph/graph.o: src/graph/graph.c src/graph/graph.h build/graph/graph_data_structure.o
	@ mkdir -p build/graph
	$(CC) -Wall -g src/graph/graph.c -o $@ -c

build/graph/graph_data_structure.o: src/graph/graph_data_structure.c src/graph/graph.h
	@ mkdir -p build/graph
	$(CC) -Wall -g src/graph/graph_data_structure.c -o $@ -c

#build/espresso/espresso.o: src/espresso/espresso.c src/espresso/espresso.h
#	@ mkdir -p build/espresso
#	$(CC) -Wall -g src/espresso/espresso.c -o $@ -c
#
#build/espresso/ucp.o: src/espresso/ucp.c src/espresso/espresso.h
#	@ mkdir -p build/espresso
#	$(CC) -Wall -g src/espresso/ucp.c -o $@ -c

build/division/division_algebric.o: src/division/division_algebric.c src/division/division_algebric.h
	@ mkdir -p build/division
	$(CC) -Wall -g src/division/division_algebric.c -o $@ -c

build/r_kernels/r_kernels.o: src/r_kernels/r_kernels.c src/r_kernels/r_kernels.h
	@ mkdir -p build/r_kernels
	$(CC) -Wall -g src/r_kernels/r_kernels.c -o $@ -c

clean: 
	rm -rf build
	rm fds
