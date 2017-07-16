UTF8_VALIDATE_DEBUG ?=

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -fPIC
LDFLAGS = -L.
CPPFLAGS = -Iinclude

ifeq ($(UTF8_VALIDATE_DEBUG),1)
DEBUG_CFLAGS += -g -O0
else
DEBUG_CFLAGS += -O2
endif

PREFIX ?= /usr/local

TARGET = libutf8-validate.so
TESTS = test-utf8-validate

LIB_SRC = src/utf8-validate.c
LIB_OBJ = $(LIB_SRC:%.c=%.o)
LIB_COV_OBJ = $(LIB_SRC:%.c=%.cov.o)

TEST_SRC = tests/test-utf8-validate.c
TEST_OBJ = $(TEST_SRC:%.c=%.cov.o)
TEST_LIB = libutf8-validate_test.a

ALL_OBJ = $(LIB_OBJ) $(TEST_OBJ) $(COV_OBJ)
GCNO = $(ALL_OBJ:%.o=%.gcno)
GCDA = $(ALL_OBJ:%.o=%.gcda)

all: $(TARGET) $(TESTS)

$(TARGET): $(LIB_OBJ)
	$(CC) -o $@ $(LIB_OBJ) $(LDFLAGS) -shared $(LIBS)

$(TEST_LIB): $(LIB_COV_OBJ)
	ar rcs $@ $^

%.cov.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -O0 -g --coverage -c $< -o $@

%.o : %.c
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) $(CPPFLAGS) -c $< -o $@

$(TESTS): $(TEST_LIB) $(TEST_OBJ)
	$(CC) -o $@ $(TEST_OBJ) $(TEST_LIB) $(LDFLAGS) --coverage $(LIBS)

run_tests: $(TESTS)
	./$(TESTS)

run_valgrind: $(TESTS)
	LD_LIBRARY_PATH=. valgrind --leak-check=full ./$(TESTS)

run_gdb: $(TESTS)
	LD_LIBRARY_PATH=. gdb ./$(TESTS)

coverage: run_tests
	lcov -c -b . -d src -d tests --output-file coverage/lcov.raw; \
	lcov -r coverage/lcov.raw "/usr/include/*" --output-file coverage/lcov.info; \
	genhtml coverage/lcov.info --output-directory coverage

install: all
	install -d $(PREFIX)/lib
	install -m 755 $(TARGET) $(PREFIX)/lib/
	install -d $(PREFIX)/include/
	install -m 644 include/*.h $(PREFIX)/include/

clean:
	rm -rf $(TARGET) $(TESTS) $(TEST_LIB) $(ALL_OBJ) $(GCNO) $(GCDA) coverage/*

.PHONY: all clean run_tests run_valgrind run_gdb coverage
