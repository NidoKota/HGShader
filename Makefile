#再帰的にディレクトリ内のファイルを探してコンパイルする

PROGNAME := app
INCDIR := include
SRCDIR := src
LIBS := -lhg
OUTDIR := build
TARGET := $(OUTDIR)/$(PROGNAME)
SRCDIRS := $(shell find $(SRCDIR) -type d)
SRCS := $(foreach var, $(SRCDIRS), $(wildcard $(var)/*.cpp))
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(SRCS)))

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