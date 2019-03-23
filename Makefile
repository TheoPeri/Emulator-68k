CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -l capstone
LDFLAGS= `pkg-config --cflags gtk+-3.0` -rdynamic
LDLIBS= `pkg-config --libs gtk+-3.0`

TMP_DIR=target/tmp/
BUILD_DIR=target/build/
SRC_DIR=src/
TEST_DIR=test/

SOURCES=$(wildcard $(SRC_DIR)*.c) # all .c file
TSOURCES=$(wildcard $(TEST_DIR)*.c) # all .c file

OBJECTS=$(patsubst $(SRC_DIR)%.c, $(TMP_DIR)%.o, $(SOURCES)) # all .o for exec
DOBJECTS=$(patsubst $(SRC_DIR)%.c, $(TMP_DIR)_d_%.o, $(SOURCES)) # all .o for debug
TOBJECTS=$(patsubst $(TEST_DIR)%.c, $(TMP_DIR)%.o, $(TSOURCES)) # all .o for test

LOBJECTS=$(filter-out $(TMP_DIR)main.o, $(OBJECTS)) # all .o for the lib

exec: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -Ofast $(OBJECTS) -o $(BUILD_DIR)$@

debug: $(DOBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -g $(DOBJECTS) -o $(BUILD_DIR)$@

unit_test: $(TOBJECTS) $(LOBJECTS)
	$(CC) $(CFLAGS) -Ofast -lcriterion $(TOBJECTS) $(LOBJECTS) -o $(BUILD_DIR)$@

$(OBJECTS): $(TMP_DIR)%.o : $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -Ofast -c $< -o $@

$(DOBJECTS): $(TMP_DIR)_d_%.o : $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(LDFLAGS)  $(LDLIBS) -g -c $< -o $@

$(TOBJECTS): $(TMP_DIR)%.o : $(TEST_DIR)%.c
	$(CC) $(CFLAGS) -Ofast -lcriterion -c $< -o $@

clean:
	rm -f $(TMP_DIR)*
	rm -f $(BUILD_DIR)*
