CC=gcc
CFLAGS=-Wall -g

SRC_DIR=ppm
BUILD_DIR=build

SOURCES=$(SRC_DIR)/*.c
HEADERS=$(SRC_DIR)/*.h

SHARED_LIB=libppmp.so

all: mainapp $(BUILD_DIR)/$(SHARED_LIB)

$(BUILD_DIR)/libppmp.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SOURCES) -o $(BUILD_DIR)/libppmp.o

$(BUILD_DIR)/$(SHARED_LIB): $(SOURCES) $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $(SOURCES) -lc

mainapp: $(BUILD_DIR)/$(SHARED_LIB) main.c
	$(CC) $(CFLAGS) -o $@ $^ -L$(BUILD_DIR) -lppmp

clean:
	rm $(BUILD_DIR)/*