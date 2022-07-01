import ROOT
import os
import sys
import json
import numpy
sys.path.append('/afs/cern.ch/work/c/chenguan/private/')
from realtimeinfo import *
ROOT.gROOT.SetBatch(1)
def ReadTree(inputpath, isdata, low, high):
	
	descripdir = 'triggerstudy'
	outputdir = realtimedir(descripdir, False)
	lead = ROOT.TH1D('lead','',100,0,100)
	sub = ROOT.TH1D('sub','',100,0,100)

	fi = ROOT.TFile.Open(inputpath,'READ')
	t = fi.Get('passedEvents')
	for i in range(t.GetEntries()):
		if i>10000:break
		if i%10000 == 0: print i, '/', t.GetEntries()
		t.GetEntry(i)
		if t.mu1_hzzid and t.mu2_hzzid and (t.trigger24matching1 or t.trigger24matching2) and t.RelIso1 < 0.35 and t.RelIso2 < 0.35 and t.Sip1 < 4.0 and t.Sip2 < 4.0 and t.pt1_corr > 25 and t.pt2_corr > 5 and abs(t.eta1) < 2.4 and abs(t.eta2) < 2.4 and t.mass2mu_corr < float(high) and t.mass2mu_corr > float(low):
			if t.pt2_corr < 25:
				if t.trigger24matching1: lead.Fill(t.pt2_corr)
				if t.trigger24matching2: sub.Fill(t.pt2_corr)	
	lead.SetStats(0)
	sub.SetStats(0)
	lead.SetLineColor(2)
	sub.SetLineColor(4)
	lead.SetLineWidth(6)
	sub.SetLineWidth(6)

	leg = initiatelegend()
	leg.AddEntry(lead,'leading muons')
	leg.AddEntry(sub,'sub-leading muons')

	c = ROOT.TCanvas('c','',1400,1000)
	c.cd()
	lead.Draw('hist')
	sub.Draw('histsame')
	leg.Draw('same')
	outputname = year+'_'+isdata+'_'+str(low)+'_'+str(high)+'_1'
	mkdescrip(outputdir+outputname,'at low pt range, who fired the trigger isomu24')
	c.SaveAs(outputdir+outputname+'.png')

year = sys.argv[1]
isdata = sys.argv[2]
low = float(sys.argv[3])
high = float(sys.argv[4])
print year, isdata, low, high
assert isdata in ['mc','data'] and year in ['2016a','2016b','2017','2018'], 'ERRORRRRRRRRRRR'
#input path
if isdata == 'mc': subname = 'LO.root'
if isdata == 'data': subname = 'SingleMu.root'
inputpath = inputpath = '/eos/user/c/chenguan/Tuples/MuonScaleUsed/19UL_EWK2ASDEF/' + year + '_HZZ_'+subname 
ReadTree(inputpath, isdata, low, high)
