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
OBJ2=CalibrateElectrons.o
OBJ3=Closure2Lepton.o
OBJ4=Closure4Lepton.o

all: CalibrateElectrons Closure2Lepton Closure4Lepton
	rm *.o

CalibrateElectrons: CalibrateElectrons.o
	$(CXX) -o CalibrateElectrons.exe ${OBJ1} ${OBJ2} $(LIBS1) ${LIBS2}
Closure4Lepton: Closure4Lepton.o
	$(CXX) -o Closure4Lepton.exe ${OBJ1} ${OBJ4} $(LIBS1) ${LIBS2}
Closure2Lepton: Closure2Lepton.o
	$(CXX) -o Closure2Lepton.exe ${OBJ1} ${OBJ3} $(LIBS1) ${LIBS2}
Closure4Lepton.o: Closure4Lepton.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c Closure4Lepton.cc
CalibrateElectrons.o: CalibrateElectrons.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c CalibrateElectrons.cc
Closure2Lepton.o: Closure2Lepton.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c Closure2Lepton.cc

.PHONY: clean
clean:
	@rm *.exe *.o 
