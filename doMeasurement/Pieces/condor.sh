#!/bin/bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
tar -xf CMSSW_10_6_12.tgz
cd CMSSW_10_6_12/src/
scramv1 b ProjectRename
eval `scramv1 runtime -sh`
echo $CMSSW_BASE "is the CMSSW I am using on the local worker node"
cd -
python launchcombinerun2.py $1 $2 $3 $4 $5 
