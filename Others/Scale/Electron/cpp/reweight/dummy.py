import ROOT
import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
from PlotClass import *
dirs = DirTree()
year = sys.argv[1]
dirs.mkrootdir(year+'lozptreweighttodata_etcut3220_sip4iso3p5_ptcut5k')
path = dirs.root
#leg = TextLegend('legend')
#tex = TextLegend('txt')
command1='root -l -q -b \'DoZPtY.C("'+year+'","'+path+'")\''
command2='root -l -q -b \'Check_ReWeight.cc("'+year+'","'+path+'")\''
os.system(command1)
os.system(command2)
