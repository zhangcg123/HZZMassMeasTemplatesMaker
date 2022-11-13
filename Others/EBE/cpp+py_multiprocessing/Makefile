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
OBJ7=CalibrateMuons.o
OBJ3=Closure2Electrons.o
OBJ5=Closure2Muons.o
OBJ4=Closure4Lepton.o

all: CalibrateElectrons Closure2Electrons Closure4Lepton Closure2Muons CalibrateMuons
	rm *.o
CalibrateMuons: CalibrateMuons.o
	$(CXX) -o CalibrateMuons.exe ${OBJ1} ${OBJ7} $(LIBS1) ${LIBS2}
CalibrateElectrons: CalibrateElectrons.o
	$(CXX) -o CalibrateElectrons.exe ${OBJ1} ${OBJ2} $(LIBS1) ${LIBS2}
Closure4Lepton: Closure4Lepton.o
	$(CXX) -o Closure4Lepton.exe ${OBJ1} ${OBJ4} $(LIBS1) ${LIBS2}
Closure2Electrons: Closure2Electrons.o
	$(CXX) -o Closure2Electrons.exe ${OBJ1} ${OBJ3} $(LIBS1) ${LIBS2}
Closure2Muons: Closure2Muons.o
	$(CXX) -o Closure2Muons.exe ${OBJ1} ${OBJ5} $(LIBS1) ${LIBS2}
Closure4Lepton.o: Closure4Lepton.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c Closure4Lepton.cc
CalibrateElectrons.o: CalibrateElectrons.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c CalibrateElectrons.cc
CalibrateMuons.o: CalibrateMuons.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c CalibrateMuons.cc
Closure2Electrons.o: Closure2Electrons.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c Closure2Electrons.cc
Closure2Muons.o: Closure2Muons.cc
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c Closure2Muons.cc

.PHONY: clean
clean:
	@rm *.exe *.o 
