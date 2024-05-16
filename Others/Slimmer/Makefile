CXX=g++
CFLAGS=-O2 -Wall

INS=-I./include
INSS=-I$(ROOFITSYS)/include
INSSS=-I${CMSSW_BASE}/src/HiggsAnalysis/CombinedLimit/interface
INSSSS=-I/afs/cern.ch/work/c/chenguan/private

CFLAGS = $(shell root-config --cflags)

LIBS1 = $(shell root-config --libs)
LIBS2 = -L$(ROOFITSYS)/lib -lRooFit -lRooFitCore -lRooStats

OBJ1 = ${CMSSW_BASE}/lib/slc7_amd64_gcc700/libHiggsAnalysisCombinedLimit.so
OBJ2=Slimmer.o

all: Calibrator
	rm *.o

Calibrator: Slimmer.o
	$(CXX) -o Slimmer.exe ${OBJ1} ${OBJ2} $(LIBS1) ${LIBS2}

Slimmer.o: Slimmer.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c Slimmer.cc


.PHONY: clean
clean:
	@rm *.exe *.o 
