all : main epweb-dk
.PHONY : all
CXX=g++
CC=gcc
LINK.o = $(LINK.cc)
# CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++20 -fcoroutines -g
CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++20 -fcoroutines 
CFLAGS=-lpthread

LD=g++

OBJ1 = main.o io_context.o socket_accept_operation.o socket_recv_operation.o socket_send_operation.o socket.o

OBJ2 = epweb-dk.o

main: ${OBJ1}

epweb-dk : $(OBJ2)
	$(CC) -o epweb-dk $(OBJ2) $(CFLAGS)


$(OBJ2):%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY:clean
clean:
	${RM} ${OBJ1} $(OBJ2) main epweb-dk
