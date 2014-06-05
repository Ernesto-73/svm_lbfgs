# Author: Javier Zhang
# Date: 3-25-2013

CXX 		= g++
CC 			= $(CXX)
CPPFLAGS 	= -O3 -g -D DEBUG
CXXFLAGS 	= $(INCLUDE)
SOURCES 	= ./src/
HEADERS 	= ./include/
OBJS 		= ./obj/
LIBS 		= ./lib/
BIN 		= ./bin/
INCLUDE 	= -I$(HEADERS)
LIBRARY 	= -L$(LIBS)
CPP_OBJS 	= $(OBJS)array.o $(OBJS)minconf_tmp.o $(OBJS)polyinterp.o $(OBJS)svm.o

CPP_TEST 	= $(SOURCES)test.cpp
CPP_LR		= $(SOURCES)lr.cpp
CPP_LS		= $(SOURCES)ls.cpp
CPP_SVM		= $(SOURCES)main.cpp

LIBA = $(LIBS)libminconf_tmp.a

$(OBJS)%.o : $(SOURCES)%.cpp $(HEADERS)%.h
	@echo Building $@...
	@$(CC) -c $< -o $@ $(CXXFLAGS) $(CPPFLAGS)

$(LIBA): $(CPP_OBJS)
	@echo Building library...
	@ar rs $(LIBA) $(CPP_OBJS)

svm: $(CPP_SVM) $(LIBA)
	@echo Building train executable file ...
	@$(CC) $(CPP_SVM) -o $(BIN)svm $(CPPFLAGS) $(LIBRARY) -lminconf_tmp $(CXXFLAGS)	
	
clean:
	-@rm $(OBJS)*.o
	-@rm $(LIBS)*.a
	-@rm $(BIN)svm
