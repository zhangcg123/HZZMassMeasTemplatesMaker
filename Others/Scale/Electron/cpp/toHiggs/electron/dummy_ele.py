import ROOT
import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
from PlotClass import *
dirs = DirTree()
year=sys.argv[1]
dirs.mkrootdir('electron_scale_to_higgs'+year)
path = dirs.root
#leg = TextLegend('legend')
#tex = TextLegend('txt')
command1='root -l -q -b \'ScaleSmearUnc.cc("'+year+'","'+path+'")\''
os.system(command1)
