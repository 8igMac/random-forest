CC = g++
CFLAGS = -I $(INCDIR) -Wall

# directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
TARGETDIR = bin

# file with path
DEPS = $(shell find $(INCDIR) -type f -name *.h)
SRC = $(shell find $(SRCDIR) -type f -name *.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRC))
TARGET = $(TARGETDIR)/main

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@mkdir -p $(BUILDDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	@mkdir -p $(TARGETDIR)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -r $(BUILDDIR) $(TARGETDIR)



