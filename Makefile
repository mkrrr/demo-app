CC=$(CROSS_COMPILER)gcc
LD=$(CROSS_COMPILER)ld
CFLAGS=-Wall -Werror
OBJ = handler.o os.o answerd.o  
CHECK_OBJS = test/unit/unit_main.o test/unit/unit_handler.o

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

answerd: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm -f $(OBJ) answerd check

check: $(CHECK_OBJS) 
	$(CC) -ggdb $(LOCAL_LDFLAGS) -lcheck $(CHECK_OBJS) -o $@
