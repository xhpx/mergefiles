TARGET: mergefile
OBJECTS:mergefile.o


%.o: %.c
	gcc  -g -c  $< -o $@

$(TARGET): $(OBJECTS)
	gcc -g -o $@  -Wall $(OBJECTS) 

clean:
	rm -f *.o $(TARGET)
