CC = clang++
#CFLAGS = -std=c90 -Weverything -Werror -pedantic-errors
PASS = /home/jackjia/Dropbox/workspace/llvm/project/pass/build/skeleton/libSkeletonPass.so
INC=-I /usr/local/polysync/include/

CFLAGS = -std=c++1y -Xclang -load -Xclang $(PASS)
BCFLAGS = -std=c++1y -c -emit-llvm

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
BYTECODES = $(addprefix bin/,$(notdir $(SOURCES:.cpp=.bc)))



TARGET = hello

bc:$(BYTECODES)
		echo "==Complete==\n"
bin/%.bc:src/%.cpp
	$(CC) $(INC) $(BCFLAGS) -o $@ $^

#%.o: %.c
#		$(CC) $(CFLAGS)  -c $^
#bc:$(SOURCES)
#		$(CC) $(INC) $(BCFLAGS) $^

clean:
	rm -rf $(BYTECODES) $(OBJECTS) $(TARGET)
.PHONY: clean
	clean:
		rm -f $(OBJECTS) $(TARGET) *.bc
		Sign up for free
