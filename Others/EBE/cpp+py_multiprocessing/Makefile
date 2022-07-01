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
OBJ2=Calibrator.o
OBJ3=Closure2Lepton.o

all: Calibrator Closure2Lepton
	rm *.o

Calibrator: Calibrator.o
	$(CXX) -o Calibrator.exe ${OBJ1} ${OBJ2} $(LIBS1) ${LIBS2}
Closure2Lepton: Closure2Lepton.o
	$(CXX) -o Closure2Lepton.exe ${OBJ1} ${OBJ3} $(LIBS1) ${LIBS2}
Calibrator.o: Calibrator.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c Calibrator.cc
Closure2Lepton.o: Closure2Lepton.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c Closure2Lepton.cc

.PHONY: clean
clean:
	@rm *.exe *.o 
