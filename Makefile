#!/usr/bin/make
##
#  file: Makefile
#  date: 2017.09.05
#
#

#_______________________________________________________________________________
color		= 1
src_dir		= src
include_dir	= include
bin_dir		= bin
build_dir	= $(src_dir)/build

opencv_dir	= $(HOME)/local
opencv_include	= -I$(opencv_dir)/include
opencv_libs	= -L$(opencv_dir)/lib \
	-lopencv_core \
	-lopencv_highgui \
	-lopencv_imgproc \
	-lopencv_dnn

CXX	= g++
CFLAGS	= -g -O3 -Wall

FLAGS	= $(CFLAGS) -I$(include_dir) $(opencv_include)
LIBS	= $(opencv_libs)

sources	= $(wildcard $(src_dir)/*.cc)
headers	= $(wildcard $(include_dir)/*.hh)
deps	= $(sources:$(src_dir)/%.cc=$(build_dir)/%.d)
objs	= $(sources:$(src_dir)/%.cc=$(build_dir)/%.o)

target	= $(bin_dir)/AlphaDNN

echo	= /bin/echo -e
mkdir	= mkdir -p
mv	= mv -f
rm	= rm -rfv
# escape sequence
ifeq ($(color),1)
beep		= \007
black		= \033[0;30;1m
blue		= \033[0;34;1m
green		= \033[0;32;1m
cyan		= \033[0;36;1m
red		= \033[0;31;1m
purple		= \033[0;35;1m
brown		= \033[0;33;1m
light_gray	= \033[0;37;1m
dark_gray	= \033[1;30;1m
light_blue	= \033[1;34;1m
light_green	= \033[1;32;1m
light_cyan	= \033[1;36;1m
light_red	= \033[1;31;1m
light_purple	= \033[1;35;1m
yellow		= \033[1;33;1m
white		= \033[1;37m
default_color	= \033[0m
endif

#_______________________________________________________________________________
.PHONY: all clean

all: $(target)

-include $(deps)

$(target): $(objs)
	@ $(echo) "$(purple)=== Linking $@ $(default_color)"
	@ $(mkdir) $(bin_dir)
	$(CXX) $(FLAGS) -o $@ $^ $(LIBS)

$(build_dir)/%.o: $(src_dir)/%.cc
	@ $(echo) "$(green)=== Compiling $@ $(default_color)"
	@ $(mkdir) $(build_dir)
	$(CXX) $(FLAGS) -o $@ -MMD -c $<

clean:
	@ $(echo) "$(green)=== Cleaning $(default_color)"
	@ $(rm) $(build_dir)
	@ find . \( -name "*~" -o -name "\#*\#" \) -exec $(rm) \{\} \;
