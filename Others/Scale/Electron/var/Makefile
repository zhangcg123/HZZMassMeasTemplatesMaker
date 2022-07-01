CXX=g++
CFLAGS=-O2 -Wall
CFLAGS = $(shell root-config --cflags)

LIBS1 = $(shell root-config --libs)
LIBS2 = -L$(ROOFITSYS)/lib -lRooFit -lRooFitCore -lRooStats

INS=-I./common/
INSS=-I$(ROOFITSYS)/include
INSSS=-I${CMSSW_BASE}/src/HiggsAnalysis/CombinedLimit/interface

OBJ1 = ${CMSSW_BASE}/lib/slc7_amd64_gcc700/libHiggsAnalysisCombinedLimit.so
OBJ2=ReadTree.o

all: ReadTree
	rm *.o

ReadTree: ReadTree.o
	$(CXX) -o ReadTree.exe ${OBJ1} ${OBJ2} ${LIBS1} ${LIBS2}

ReadTree.o: ReadTree.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) $(INSSS) -c ReadTree.cpp

.PHONY: clean
clean:
	@rm *.exe *.o 
