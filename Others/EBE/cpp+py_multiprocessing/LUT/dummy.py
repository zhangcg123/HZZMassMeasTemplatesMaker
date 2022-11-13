import ROOT
import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
from PlotClass import *
dirs = DirTree()
dirs.mkrootdir('test_printlut')
path = dirs.root


f = ROOT.TFile(sys.argv[1],'read')
lut = f.Get(sys.argv[2])
lut.Print()
c = ROOT.TCanvas('c','',1000,1000)
lut.Draw('text')
c.SaveAs(path + '/xxx.png')
