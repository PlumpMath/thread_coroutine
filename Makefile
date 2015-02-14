TC_BUILD_PATH ?= .

CC = gcc

LUA_INC := -I/usr/include/lua5.2
LUA_LIBS := -llua5.2

TC_INC := -Isrc
TC_LIBS := -lpthread
CFLAGS := -g -O2 -Wall

SRC = tc_module.c tc_main.c

all: \
	tc

$(TC_BUILD_PATH)/tc: $(foreach v, $(SRC), src/$(v))
	$(CC) $(CFLAGS) -o $@ $^ $(TC_INC) $(LUA_INC) $(LUA_LIBS) $(TC_LIBS)

clean:
	rm $(TC_BUILD_PATH)/tc
