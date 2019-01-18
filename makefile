#CC=g++ -std=c++17
CC=g++

DEPS = \
	src/utils/assert.h \
	src/utils/ncurses_raii.h \
	src/board/board_model.h \
	src/board/cell_model.h \
	src/board/board_view.h \
	src/solver/solver.h \
	src/def.h \


OBJ =	\
	./obj/ncurses_raii.o \
	./obj/assert.o \
	./obj/board_view.o \
	./obj/solver.o

MAIN_OBJ = \
	./obj/main.o \
	$(OBJ)

TEST_OBJ = \
	./obj/catch.o \
	./obj/cell_model_spec.o \
	./obj/assert_spec.o \
	$(OBJ)

LIBS = \
	-lncurses \
	-lunwind-x86_64 \
	-lunwind

VPATH=./src:./src/utils:./src/board:./src/solver:./tests

obj/%.o: %.cpp $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

build: dirs main

dirs: obj

obj:
	mkdir obj

main: $(MAIN_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test: $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm main obj/*.o

rebuild: clean build
