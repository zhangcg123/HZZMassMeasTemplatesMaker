import ROOT
import sys
import os
xbin = sys.argv[1]
ybin = sys.argv[2]
h = ROOT.TH2D("h_mzzD","",int(xbin),105,140,int(ybin),0,1.0)
#make a flat 
for x in range(1,h.GetNbinsX()+1):
	for y in range(1,h.GetNbinsY()+1):
		h.SetBinContent(x,y,1.0)
#slice norm


for fsname in ['4e','4mu','2e2mu','2mu2e']:
	f = ROOT.TFile.Open('Dbackground_ZX_'+fsname+'.root','RECREATE')
	f.cd()
	h_ = h.Clone()
	h_.Write()
	h_up = h.Clone()
	h_up.SetName('h_mzzD_up')
	h_up.Write()
	h_dn = h.Clone()
	h_dn.SetName('h_mzzD_dn')
	h_dn.Write()
	f.Close()
