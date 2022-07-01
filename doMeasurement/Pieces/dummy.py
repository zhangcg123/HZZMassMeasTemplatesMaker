import ROOT
import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
dirs = DirTree()
dirs.mkrootdir('testtoygenerateonly')
path = dirs.root
f = ROOT.TFile(sys.argv[1],'read')
w = f.Get('toys/toy_asimov')
m = ROOT.RooRealVar('CMS_zz4l_mass','CMS_zz4l_mass',105,140)
m.setBins(100)
plot = m.frame()
w.plotOn(plot)
c = ROOT.TCanvas('c','',1400,1000)
plot.Draw()
c.SaveAs(path+'/toygenerateonly.png')
