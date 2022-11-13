import sys
import os
import ROOT
from array import array
from multiprocessing import Process
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
sys.path.append('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/EBE/cpp+py_multiprocessing/include/')
from FileSystemClass import *

#arguments
year = sys.argv[1]
assert year in ['20160', '20165', '2017', '2018'],'errrrrrrrrrrrrrrrrrrrrr'

#output dir
dirclass = DirTree()
dirclass.mkrootdir(year+'_closure_with_4lepton_events_remove00')

path = dirclass.root
os.system('./Closure4Lepton.exe '+year+' '+path)
