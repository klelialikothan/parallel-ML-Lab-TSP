name = Parallel_ML_lab_3_2
src = $(wildcard *.cpp)
obj = $(src:/c=.o)

CC = g++
CFLAGS = -std=c++17 -O2 -march=native -mtune=native -fopt-info
LIBFLAGS = -lm -fopenmp

compile: $(name)
$(name): $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFLAGS)

run:
	./$(name)
