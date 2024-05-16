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
OBJ2=createToys.o

all: createToys
	rm *.o


createToys: createToys.o
	$(CXX) -o createToys.exe ${OBJ1} ${OBJ2} $(LIBS1) ${LIBS2}

createToys.o: createToys.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} ${INSSSS} -c createToys.cpp


.PHONY: clean
clean:
	@rm *.exe *.o 

#the value of 'root-config --cflags' -pthread -std=c++1z -m64 -I/cvmfs/cms.cern.ch/slc6_amd64_gcc700/cms/cmssw/CMSSW_10_2_13/external/slc6_amd64_gcc700/bin/../../../../../../../slc6_amd64_gcc700/lcg/root/6.12.07-gnimlf5/include
#the value of 'root-config --glibs' -L/usr/lib64/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -lm -ldl -rdynamic
