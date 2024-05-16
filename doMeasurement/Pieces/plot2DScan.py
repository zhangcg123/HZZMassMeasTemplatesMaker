import ROOT
import sys
import os
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetPalette(109)
def contour(path, name):
	commandline = 'root -l -q -b /afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/Pieces/contours2D.cxx\'("'+path+'","'+name+'")\''
	os.system(commandline)

def colz(path, name):
	f = ROOT.TFile.Open(path+'/'+name+'.root','READ')
	t = f.Get('limit')
	h2d = ROOT.TH2D('h2d','',60,124,126,60,0,3)
	for i in range(1,t.GetEntries()):
		t.GetEntry(i)
		if t.quantileExpected==-1: continue
		binx = h2d.GetXaxis().FindBin(t.MH)
		biny = h2d.GetYaxis().FindBin(t.HiggsDecayWidth)
		h2d.SetBinContent(binx,biny,2*t.deltaNLL)
	c = ROOT.TCanvas('c','',1400,1000)
	c.cd()
	h2d.SetStats(0)
	h2d.Draw('colz')
	c.SaveAs(path + name + '_2D_colz.png')

path_ = os.getcwd()+'/'
name_ = sys.argv[1].split('/')[-1].split('.root')[-2]
contour(path_,name_)
