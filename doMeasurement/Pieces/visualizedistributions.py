import ROOT
import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
from PlotClass import *

dirs = DirTree()
dirs.mkrootdir('test_dummy')
path = dirs.root

f = ROOT.TFile.Open(sys.argv[1],'read')

prefit_data = f.Get('shapes_prefit/'+f.Get('shapes_prefit').GetListOfKeys()[0].GetName()+'/data')
prefit_func = f.Get('shapes_prefit/'+f.Get('shapes_prefit').GetListOfKeys()[0].GetName()+'/total')

postfit_data = f.Get('shapes_fit_s/'+f.Get('shapes_fit_s').GetListOfKeys()[0].GetName()+'/data')
postfit_func = f.Get('shapes_fit_s/'+f.Get('shapes_fit_s').GetListOfKeys()[0].GetName()+'/total')

c = ROOT.TCanvas('c','',1000,1000)
prefit_data.Draw('ap')
prefit_func.Draw('samehist')
c.SaveAs( path + '/prefit_'+f.Get('shapes_prefit').GetListOfKeys()[0].GetName()+'.png')
c.Clear()

postfit_data.Draw('ap')
postfit_func.Draw('samehist')
c.SaveAs( path + '/postfit_'+f.Get('shapes_prefit').GetListOfKeys()[0].GetName()+'.png')
