import ROOT
import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
from PlotClass import *
dirs = DirTree()
dirs.mkrootdir('compare_kdmap_between_run1_run2')
path = dirs.root

input_run1 = './run1/comb_hzz4l.root'
input_run2true = './run2true/hzz4l_20160_allfinalstates_allcategs_13TeV_massmodelforMH_floatothers1_MH_124p38_126p38.root'
input_run2false = './run2false/hzz4l_20160_allfinalstates_allcategs_13TeV_massmodelforMH_floatothers1_MH_124p38_126p38.root'

obj_run1 = 'sigTemplateMorphPdf_ggH_1_7'
obj_run2true = 'sigTemplateMorphPdf_ggH_20160_4mu_0_notag_opt'
obj_run2false = 'sigTemplateMorphPdf_ggH_20160_4mu_0_notag'

file_run1 = ROOT.TFile(input_run1,'read')
file_run2true = ROOT.TFile(input_run2true,'read')
file_run2false = ROOT.TFile(input_run2false,'read')

ws_run1 = file_run1.Get('w')
ws_run2true = file_run2true.Get('w')
ws_run2false = file_run2false.Get('w')

fast2d_run1 = ws_run1.obj(obj_run1)
fast2d_run2true = ws_run2true.obj(obj_run2true)
fast2d_run2false = ws_run2false.obj(obj_run2false)


print '\n\n\n\n'
print 'run1'
fast2d_run1.Print()
print '\n\n\n\n'
print 'run2true'
fast2d_run2true.Print()
print '\n\n\n\n'
print 'run2false'
fast2d_run2false.Print()


x = ROOT.RooRealVar('CMS_zz4l_mass','',105,140)
y = ROOT.RooRealVar('melaLD','',0,1)

hhrun1 = fast2d_run1.createHistogram('',x,ROOT.RooFit.YVar(y))
hhrun2true = fast2d_run2true.createHistogram('',x,ROOT.RooFit.YVar(y))
hhrun2false = fast2d_run2false.createHistogram('',x,ROOT.RooFit.YVar(y))

print '\n\n\n\n'
print hhrun1.Integral()
print hhrun2true.Integral()
print hhrun2false.Integral()
