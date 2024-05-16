import ROOT
import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
from PlotClass import *
dirs = DirTree()
dirs.mkrootdir('test_dummy')
path = dirs.root


f_old = ROOT.TFile('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/20160/ZZTo4L_M125_20160_skimmed.root','read')
t_old = f_old.Get("passedEvents")
h_old = ROOT.TH1F('h_old','',100,0,10)
t_old.Draw('mass4lErrREFIT_vtx_BS>>h_old','passedFullSelection==1')
h_old.SetLineWidth(4)
h_old.SetLineColor(2)

f_new = ROOT.TFile('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMatch2023/ZZTo4L_M125_20160_skimmed.root ','read')
t_new = f_new.Get("passedEvents")
h_new = ROOT.TH1F('h_new','',100,0,10)
t_new.Draw('mass4lErrREFIT_vtx_BS>>h_new','passedFullSelection==1')
h_new.SetLineWidth(4)
h_new.SetLineColor(4)

c = ROOT.TCanvas('c','',1000,1000)
h_new.Draw('hist')
h_old.Draw('histsame')
#add legend
leg = ROOT.TLegend(0.7,0.7,0.9,0.9)
leg.AddEntry(h_old,'old','l')
leg.AddEntry(h_new,'new','l')
leg.Draw()
#close stat box
h_new.SetStats(0)
h_old.SetStats(0)

c.SaveAs(path+'/test.png')
