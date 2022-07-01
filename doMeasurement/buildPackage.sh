#!/bin/bash

echo "Building Package..."

cd include
rm *.so *.d *.pcm
echo HiggsCSandWidth
root -l -b -q buildHiggsCSandWidth.C
echo HiggsCSandWidthFermi
root -l -b -q buildHiggsCSandWidthFermi.C 
echo HiggsCSandWidthSM4
root -l -b -q buildHiggsCSandWidthSM4.C    
#echo tdrstyle
#root -l -b -q buildtdrstyle.C
cd ../
mkdir datacardsInputs
mkdir datacardsOutputs
cp -r /afs/cern.ch/user/c/chenguan/public/CMSdata_REFIT ./
cp -r /afs/cern.ch/user/c/chenguan/public/CMSdata ./
cp -r /afs/cern.ch/user/c/chenguan/public/SM_inputs_13TeV datacardsInputs/
cp /afs/cern.ch/user/c/chenguan/public/*root datacardsInputs/

echo "Done"
echo "Happy Hunting!"
