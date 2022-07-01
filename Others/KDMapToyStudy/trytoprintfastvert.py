import sys
import ROOT
import os
import glob
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *

dirs = DirTree()
dirs.mkrootdir('try_to_print_or_draw_fast2d_'+sys.argv[1])
path = dirs.root

suffix = ''
if 'KDTrue' in sys.argv[1]: suffix = '_opt'

c = ROOT.TCanvas('c','',1500,1000)
c.cd()
t = ROOT.TLatex()
t.SetNDC()
t.SetTextFont( 62 )
t.SetTextColor( 36 )
t.SetTextSize( 0.08 )
t.SetTextAlign( 12 )

x = ROOT.RooRealVar('CMS_zz4l_mass','',105,140)
y = ROOT.RooRealVar('melaLD','',0,1)
x.setBins(70)
y.setBins(100)

f = ROOT.TFile('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/datacardsOutputs_'+sys.argv[1]+'/2018_bsrefit_mkd_1_notag_widthpdf/hzz4l_2018_allfinalstates_allcategs_widthmodelforHiggsDecayWidth_floatothers1_width_0_0p5_MH_124p9_125p1.root')
w = f.Get('w')
fastpdf = w.obj('bkgTemplateMorphPdf_zjets_2018_4mu_0_notag'+suffix)
#fastpdf = w.obj('sigTemplateMorphPdf_ggH_2018_4mu_0_notag'+suffix)
fastpdf.Print()
hh = fastpdf.createHistogram('',x,ROOT.RooFit.YVar(y))
hh.Draw('surf')
t.DrawLatex(0.1,0.7,str(hh.Integral()))
c.SaveAs(path+'/fastpdf.png')
