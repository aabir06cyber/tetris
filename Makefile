IDIR = include
CC = gcc
CFLAGS = -I$(IDIR)
ifeq ($(OS),Windows_NT)
    # Point to the Windows version in your local lib folder
    LIBS = -L./lib -l:libraylib_win.a -lopengl32 -lgdi32 -lwinmm
else
    # Point to the Linux version in your local lib folder
    LIBS = -L./lib -l:libraylib_lin.a -lGL -lm -lpthread -ldl -lrt -lX11
endif
DEPS = include/common.h include/logic.h include/renderer.h include/input_control.h
OBJ = tetris_main.o logic.o renderer.o input_control.o

%o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
tetris: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)