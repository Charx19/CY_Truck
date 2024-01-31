EXECUTABLE=main

.DEFAULT_GOAL:=$(EXECUTABLE)

SRCS:=$(wildcard *.c)

clean:
	@rm -f *.o
	@rm -f *.h.gch
	@rm -f $(EXECUTABLE)

$(EXECUTABLE): $(SRCS)
	@gcc $(SRCS) -o $(EXECUTABLE) -lm