# Makefile for Sol
#
# Copyright (C) 2012 Heidelberg University
#
# Author: Sascha Fendrich
#
# This file is part of Sol.
# 
# Sol is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Sol is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with Sol.  If not, see <http://www.gnu.org/licenses/>.

INC=-Itiny_log
LIB=-Ltiny_log
OBJS=learner.o weight_vector.o data_set.o model.o sparse_data_format.o sparse_vector.o
BINARIES=tiny_log/libtiny_log.a sol-bin sol-mucl sol-mulab 

CXXFLAGS=-O3 #-pg #-static 

all: $(BINARIES)

tiny_log/Makefile:
	$(error "tiny_log/Makefile is missing. Please check if the\
           submodules are initialized correctly in .gitmodules.\
           If not, try 'make submodules'")

tiny_log/libtiny_log.a: tiny_log/Makefile
	cd tiny_log && make

.PHONY: submodules
submodules:
	git submodule init
	git submodule update

sol-bin: binary.cpp learner_binary.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(INC) $(LIB) -o $@ $^ -lboost_program_options -ltiny_log

sol-mucl: multiclass.cpp learner_multiclass.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(INC) $(LIB) -o $@ $^ -lboost_program_options -ltiny_log

sol-mulab: multilabel.cpp learner_multilabel.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(INC) $(LIB) -o $@ $^ -lboost_program_options -ltiny_log

%.o: %.cpp
	$(CXX) $(INC) -c $<

.PHONY:
clean:
	$(RM) $(BINARIES) *.o
