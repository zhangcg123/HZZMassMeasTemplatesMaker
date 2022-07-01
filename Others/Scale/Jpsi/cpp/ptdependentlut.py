import ROOT
import sys
import os
from array import array
def mklut(xp,xw,y,path,name):
	bins = []
	for i in range(len(xp)):
		bins.append(xp[i]-xw[i])
	bins.append(xp[-1]+xw[-1])
	lut = ROOT.TH1F('lut','',len(bins)-1,array('f',bins))
	for i in range(1,len(bins)):
		lut.SetBinContent(i,y[i-1])
	f = ROOT.TFile(path + '/' + name + '.root','recreate')
	f.cd()
	lut.Write()
	f.Close()
