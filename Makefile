PROGNAME := app
INCDIR := include
SRCDIR := src
LIBDIR := lib
LIBS := -lhg
OUTDIR := build
TARGET := $(OUTDIR)/$(PROGNAME)
SRCS := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/$(LIBDIR)/*.cpp)
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(SRCS)))
#$(warning $(OBJS))

CC = g++
CFLAGS = -Wall -std=c++11 -pthread -I $(INCDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OUTDIR)/%.o:%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

all: clean $(TARGET)

clean:
	rm -rf $(OUTDIR)

run:
	open -a HGDisplayer
	./$(OUTDIR)/app