# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Directories
INCDIR = include
SRCDIR = src
OBJDIR = obj

# Files
DEPS = $(INCDIR)/search.h                              # Header file dependencies
SRCS = $(SRCDIR)/cgrep.c $(SRCDIR)/linked_lists.c $(SRCDIR)/directory_search.c   # Source files
OBJS = $(OBJDIR)/cgrep.o $(OBJDIR)/linked_lists.o $(OBJDIR)/directory_search.o   # Object files

# Output binary name
TARGET = cgrep  # Updated for Windows executable

# Rule to compile object files from source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -I$(INCDIR)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# Clean up object files and executable
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

# Rule to create obj directory if needed
$(OBJDIR):
	@if not exist $(OBJDIR) mkdir $(OBJDIR)

# Default target
all: $(OBJDIR) $(TARGET)
