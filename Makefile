CC = g++
CFLAGS = -I $(INCDIR)

# directories
SRCDIR = src
INCDIR = include
BUILDDIR = build

# file with path
DEPS = $(shell find $(INCDIR) -type f -name *.h)
SRC = $(shell find $(SRCDIR) -type f -name *.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRC))
TARGET = bin/main

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -r $(BUILDDIR)/* $(TARGET)



