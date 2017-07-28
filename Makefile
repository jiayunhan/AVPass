CC = clang++
#CFLAGS = -std=c90 -Weverything -Werror -pedantic-errors

PSYNC_HOME = /usr/local/polysync
ROS_HOME = /opt/ros/kinetic
AUTOWARE_HOME = /
PASS_NAME = skeleton
PASS = /home/jackjia/Dropbox/workspace/llvm/project/pass/build/skeleton/libSkeletonPass.so
INC=-I $(PSYNC_HOME)/include \
	-I $(PSYNC_HOME)/include/deps \
	-I $(PSYNC_HOME)/include/deps/dcps/C/SAC \
	-I $(PSYNC_HOME)/include/deps/sys \
	-I /usr/include/glib-2.0 \
	-I /usr/lib/x86_64-linux-gnu/glib-2.0/include \
	-I $(PSYNC_HOME)/plugins \
	-I $(PSYNC_HOME)/pdm \
	-I $(ROS_HOME)/include 

CFLAGS = -std=c++1y -Xclang -load -Xclang $(PASS)
BCFLAGS = -std=c++1y -c -emit-llvm

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
BYTECODES = $(addprefix bin/,$(notdir $(SOURCES:.cpp=.bc)))



#TARGET = hello

bc:$(BYTECODES)
		echo "==Complete==\n"

pass:$(BYTECODES)
	opt -load $(PASS) -$(PASS_NAME) < bin/SocketReader.bc > code-inst.bc


bin/%.bc:src/%.cpp
	$(CC) $(INC) $(BCFLAGS) -o $@ $^

#%.o: %.c
#		$(CC) $(CFLAGS)  -c $^
#bc:$(SOURCES)
#		$(CC) $(INC) $(BCFLAGS) $^

clean:
	rm -rf bin/*
.PHONY: clean
	clean:
		rm -f $(OBJECTS) $(TARGET) *.bc
		Sign up for free
