CXXFLAGS = -std=c++17 

.PHONY: all 
all: main

main : main.o
	g++ $(CXXFLAGS) $^ -o $@

main.o : main.cpp stb_image.h stb_image_write.h
	g++ $(CXXFLAGS) -c main.cpp -o main.o

.PHONY: run
run : main
	./main test.jpg jpg -50 0 0 1.5

.PHONY: clean
clean:
	rm -f main main.o

