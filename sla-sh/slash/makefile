# Compiler flags: all warnings + debugger meta-data
CFLAGS = -Wall -g

# The final executable program file, i.e. name of our program
BIN = slash

# Object files from which $BIN depends
# OBJS = module1.o module2.o module3.o

# This default rule compiles the executable program
$(BIN): $(OBJS) $(BIN).c
	$(CC) $(CFLAGS) $(BIN).c -o $(BIN)
	# $(CC) $(CFLAGS) $(OBJS) $(BIN).c -o $(BIN)

# This rule compiles each module into its object file
%.o: %.c %.h
	$(CC) -c $(CFLAGS) $(DEFS) $< -o $@

clean:
	rm -f *~ *.o $(BIN)
