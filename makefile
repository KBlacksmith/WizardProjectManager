GCC=g++
OBJECTS=wizard.o validations.o help.o functions.o
CFLAGS=-std=c++20 -c -o
TARGET=bin
SRC=src
default: all
all: clean build
clean: 
	rm -rf $(TARGET)
build: dir $(OBJECTS)
	$(GCC) -o $(TARGET)/wizard.out $(TARGET)/*.o
	$(RM) $(TARGET)/*.o
dir: 
	mkdir $(TARGET)
wizard.o: 
	$(GCC) $(CFLAGS) $(TARGET)/wizard.o $(SRC)/wizard.cpp
validations.o: 
	$(GCC) $(CFLAGS) $(TARGET)/validations.o $(SRC)/validations.cpp
help.o: 
	$(GCC) $(CFLAGS) $(TARGET)/help.o $(SRC)/help.cpp
functions.o: 
	$(GCC) $(CFLAGS) $(TARGET)/functions.o $(SRC)/functions.cpp
run: 
	./$(TARGET)/wizard.out
