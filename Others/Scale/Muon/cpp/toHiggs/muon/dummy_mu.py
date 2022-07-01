import ROOT
import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
from PlotClass import *
dirs = DirTree()
year=sys.argv[1]
fs = sys.argv[2]
dirs.mkrootdir('muon_scale_to_higgs_'+fs+'_'+year)
path = dirs.root
#leg = TextLegend('legend')
#tex = TextLegend('txt')
command1='root -l -q -b \'RochUnc.cc("'+year+'","'+fs+'","'+path+'")\''
os.system(command1)
