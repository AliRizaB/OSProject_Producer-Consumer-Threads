# Variables for easier management
CC = gcc
CFLAGS = -Wall -Iinclude -Isrc/
LDFLAGS = -lpthread
OBJ_DIR = src/object_files
TARGET = main.exe

# List of object files with their full paths
OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/producer.o \
       $(OBJ_DIR)/consumer.o \
	   $(OBJ_DIR)/deadlockTimer.o \
       $(OBJ_DIR)/globalVariables.o \
       $(OBJ_DIR)/threadFunctions.o

# Default rule
all: $(TARGET)
	./$(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o $(OBJ_DIR)/main.o

# Rule for producer.o 
$(OBJ_DIR)/producer.o: src/thread_operations/producer.c
	$(CC) $(CFLAGS) -c src/thread_operations/producer.c -o $(OBJ_DIR)/producer.o

# Rule for consumer.o 
$(OBJ_DIR)/consumer.o: src/thread_operations/consumer.c
	$(CC) $(CFLAGS) -c src/thread_operations/consumer.c -o $(OBJ_DIR)/consumer.o

$(OBJ_DIR)/deadlockTimer.o: src/thread_operations/deadlockTimer.c
	$(CC) $(CFLAGS) -c src/thread_operations/deadlockTimer.c -o $(OBJ_DIR)/deadlockTimer.o

# Rule for globalVariables.o
$(OBJ_DIR)/globalVariables.o: src/global_attributes/globalVariables.c
	$(CC) $(CFLAGS) -c src/global_attributes/globalVariables.c -o $(OBJ_DIR)/globalVariables.o

# Rule for threadFunctions.o 
$(OBJ_DIR)/threadFunctions.o: src/thread_functions/threadFunctions.c
	$(CC) $(CFLAGS) -c src/thread_functions/threadFunctions.c -o $(OBJ_DIR)/threadFunctions.o

# Clean up
clean:
	-del /f /q src\object_files\*.o $(TARGET)
	@cls