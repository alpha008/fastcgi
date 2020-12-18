src = $(wildcard *.c)
src := $(filter-out cJSON.c, $(src))
targets = $(patsubst %.c, %, $(src))
INCLUDE = 
CC = gcc
#标志位置中有些需要加上带上开源源文件一起编译
CFLAGS =  -g  
#-Wall
LIBS  = -lm -lstdc++   -lfcgi
all:$(targets)

$(targets):%:%.c
	$(CC) $(INCLUDE) $(CFLAGS)  $< -o $@ $(LIBS)

.PHONY:clean all
clean:
	-rm -rf $(targets) 
