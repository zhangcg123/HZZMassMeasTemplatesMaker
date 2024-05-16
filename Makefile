CXX=g++
CFLAGS=-O2 -Wall

INS=-I./include
INSS=-I$(ROOFITSYS)/include
INSSS=-I${CMSSW_BASE}/src/HiggsAnalysis/CombinedLimit/interface

CFLAGS = $(shell root-config --cflags)

LIBS1 = $(shell root-config --libs)
LIBS2 = -L$(ROOFITSYS)/lib -lRooFit -lRooFitCore -lRooStats

OBJ1 = ${CMSSW_BASE}/lib/slc7_amd64_gcc700/libHiggsAnalysisCombinedLimit.so
OBJ2=GetBounds.o
OBJ3=SignalShape.o
OBJ4=VHShape.o
OBJ5=BKGShape.o
OBJ6=EventYield.o
OBJ7=BKGEventYield.o
OBJ8=KDMap.o
OBJ9=PatchForWH.o
OBJ10=ZXEventYield.o
OBJ11=DmHisto.o

#all: ZXEventYield
#	rm *.o

all: GetBounds SignalShape VHShape BKGShape EventYield BKGEventYield KDMap PatchForWH ZXEventYield DmHisto
	rm *.o

GetBounds: GetBounds.o
	$(CXX) -o GetBounds.exe ${OBJ1} ${OBJ2} $(LIBS1) ${LIBS2}

SignalShape: SignalShape.o
	$(CXX) -o SignalShape.exe ${OBJ1} ${OBJ3} $(LIBS1) ${LIBS2}

VHShape: VHShape.o
	$(CXX) -o VHShape.exe ${OBJ1} ${OBJ4} $(LIBS1) ${LIBS2}

BKGShape: BKGShape.o
	$(CXX) -o BKGShape.exe ${OBJ1} ${OBJ5} $(LIBS1) ${LIBS2}

EventYield: EventYield.o
	$(CXX) -o EventYield.exe ${OBJ1} ${OBJ6} $(LIBS1) ${LIBS2}

BKGEventYield: BKGEventYield.o
	$(CXX) -o BKGEventYield.exe ${OBJ1} ${OBJ7} $(LIBS1) ${LIBS2}

PatchForWH: PatchForWH.o
	$(CXX) -o PatchForWH.exe ${OBJ1} ${OBJ9} $(LIBS1) ${LIBS2}

KDMap:KDMap.o
	$(CXX) -o KDMap.exe ${OBJ1} ${OBJ8} $(LIBS1) ${LIBS2}

ZXEventYield: ZXEventYield.o
	$(CXX) -o ZXEventYield.exe ${OBJ1} ${OBJ10} $(LIBS1) ${LIBS2}
	
DmHisto: DmHisto.o
	$(CXX) -o DmHisto.exe ${OBJ1} ${OBJ11} $(LIBS1) ${LIBS2}

GetBounds.o: GetBounds.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c GetBounds.cpp

SignalShape.o: SignalShape.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c SignalShape.cpp

VHShape.o: VHShape.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c VHShape.cpp

BKGShape.o: BKGShape.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c BKGShape.cpp

EventYield.o: EventYield.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c EventYield.cpp

BKGEventYield.o: BKGEventYield.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c BKGEventYield.cpp

KDMap.o: KDMap.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c KDMap.cpp

PatchForWH.o: PatchForWH.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c PatchForWH.cpp

ZXEventYield.o: ZXEventYield.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c ZXEventYield.cpp

DmHisto.o: DmHisto.cpp
	$(CXX) $(CFLAGS) $(INS) $(INSS) ${INSSS} -c DmHisto.cpp

.PHONY: clean
clean:
	@rm *.exe *.o 

#the value of 'root-config --cflags' -pthread -std=c++1z -m64 -I/cvmfs/cms.cern.ch/slc6_amd64_gcc700/cms/cmssw/CMSSW_10_2_13/external/slc6_amd64_gcc700/bin/../../../../../../../slc6_amd64_gcc700/lcg/root/6.12.07-gnimlf5/include
#the value of 'root-config --glibs' -L/usr/lib64/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -lm -ldl -rdynamic
