import os
import sys
import ROOT
import json
import numpy
import glob
from math import sqrt

def makeplot(mcnegpoints, mcpospoints, mcnegerrors, mcposerrors, datanegpoints, datapospoints, datanegerrors, dataposerrors, k, plotpath,what):
	print mcnegpoints
	print mcpospoints
	print datanegpoints
	print datapospoints
	if k == 'eta':
		x = [(-2.4*(3.5/4.0)),(-2.4*(2.5/4.0)),(-2.4*(1.5/4.0)),(-2.4*(0.5/4.0)),(2.4*(0.5/4.0)),(2.4*(1.5/4.0)),(2.4*(2.5/4.0)),(2.4*(3.5/4.0))]
	else:
		pi = numpy.pi
		x = [-pi*(3.5/4.0),-pi*(2.5/4.0),-pi*(1.5/4.0),-pi*(0.5/4.0),pi*(0.5/4.0),pi*(1.5/4.0),pi*(2.5/4.0),pi*(3.5/4.0)]
	xerr = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
	
	if True:
		for i in range(8):
			mcnegpoints[i] = mcnegpoints[i]-0.034
			mcpospoints[i] = mcpospoints[i]-0.034

	g_mc_neg = ROOT.TGraphErrors(len(x), numpy.array(x), numpy.array(mcnegpoints), numpy.array(xerr), numpy.array(xerr))
	g_mc_pos = ROOT.TGraphErrors(len(x), numpy.array(x), numpy.array(mcpospoints), numpy.array(xerr), numpy.array(xerr))
	g_dt_neg = ROOT.TGraphErrors(len(x), numpy.array(x), numpy.array(datanegpoints), numpy.array(xerr), numpy.array(xerr))
	g_dt_pos = ROOT.TGraphErrors(len(x), numpy.array(x), numpy.array(datapospoints), numpy.array(xerr), numpy.array(xerr))
	
	g_mc_neg.SetTitle('')
	g_mc_pos.SetTitle('')
	g_dt_neg.SetTitle('')
	g_dt_pos.SetTitle('')
		
	g_mc_neg.SetMarkerSize(1.5)
	g_mc_neg.SetMarkerColor(4)
	g_mc_neg.SetMarkerStyle(24)
	
	g_dt_neg.SetMarkerSize(1)
	g_dt_neg.SetMarkerColor(2)
	g_dt_neg.SetMarkerStyle(20)

	g_mc_pos.SetMarkerSize(1.5)
	g_mc_pos.SetMarkerColor(4)
	g_mc_pos.SetMarkerStyle(24)

	g_dt_pos.SetMarkerSize(1)
	g_dt_pos.SetMarkerColor(2)
	g_dt_pos.SetMarkerStyle(20)
	
	g_mc_pos.GetXaxis().SetTitle('#'+k)
	g_mc_pos.GetXaxis().SetTitleOffset(0.2)
	g_mc_pos.GetXaxis().SetTitleSize(0.08)
				
	if k == 'eta':
		max_ = 91.14
		min_ = 90.96
	if k == 'phi':
		max_ = 91.14
		min_ = 90.98
	g_mc_neg.GetHistogram().SetMaximum(max_)
	g_mc_neg.GetHistogram().SetMinimum(min_)
	g_mc_pos.GetHistogram().SetMaximum(max_)
	g_mc_pos.GetHistogram().SetMinimum(min_)	
	
	legend = ROOT.TLegend(.74,.82,.95,.95)
	legend.SetTextSize(0.05)
	legend.SetLineWidth(0)
	legend.SetFillColor(0)
	legend.SetBorderSize()
	legend.AddEntry(g_mc_pos,'mc')
	legend.AddEntry(g_dt_pos,'data')

	lx = ROOT.TLatex()
	lx.SetNDC()
	lx.SetTextSize(0.1)
	lx.SetTextFont(42)
	lx.SetTextAlign(23)

	c = ROOT.TCanvas('c','',1000,1000)
	#c.SetBottomMargin(0.1)
	c.cd()
	c1 = ROOT.TPad('p1','',0,0.5,1,1.0)
	c1.Draw()
	c1.cd()
	c1.SetTopMargin(0.1)
	c1.SetBottomMargin(0.05)
	g_mc_neg.Draw('ap')
	g_dt_neg.Draw('p')
	lx.DrawLatex(0.2,0.9,'#mu^{-}')
	legend.Draw('same')
	c.cd()
	c2 = ROOT.TPad('p2','',0,0.0,1,0.5)
	c2.Draw()
	c2.cd()
	c2.SetTopMargin(0.05)
	c2.SetBottomMargin(0.1)
	g_mc_pos.Draw('ap')
	g_dt_pos.Draw('p')
	lx.DrawLatex(0.2,0.9,'#mu^{+}')
	c.SaveAs(plotpath + '111_' + year + '_' + k + '_' + what + '.png')

def mkpeakunc(dic,lis,what):
	diffdic = {}
	for i in dic:
		diffdic[i] = []
		for j in range(8):
			diffdic[i].append(abs(dic[i][j]-dic['mass2mu_ewk2'][j]))
	q = []
	for i in range(8):
		tmp = 0
		for j in diffdic:
			tmp += diffdic[j][i]*diffdic[j][i]
		if what == 'peak' or what == 'peakerr':
			q.append(sqrt(tmp+lis[i]*lis[i]))
		if what == 'mean':
			q.append(sqrt(tmp))
	return q

def extractlist(plotpath):
	year = plotpath.split('/')[-2].split('_')[-3]
	low = plotpath.split('/')[-2].split('_')[-2]
	high = plotpath.split('/')[-2].split('_')[-1]
	
	files = glob.glob(plotpath + '*json')
	dic = {}
	for f in files:
		with open(f,'r') as f:
			dic.update(json.load(f))
	
	masslist = ['mass2mu_ewk2','mass2mu_ewk_dt','mass2mu_zpt_dt','mass2mu_dm_dt']
	
	peak_points = {}
	mean_points = {}
	peakerr_points = {}
	for isdata in ['data','mc']:
		peak_points[isdata] = {}
		mean_points[isdata] = {}
		peakerr_points[isdata] = {}
		for type_ in ['eta','phi']:
			peak_points[isdata][type_] = {}
			mean_points[isdata][type_] = {}
			peakerr_points[isdata][type_] = {}
			for charge in ['pos','neg']:
				peak_points[isdata][type_][charge] = {}
				mean_points[isdata][type_][charge] = {}
				peakerr_points[isdata][type_][charge] = {}
				if isdata == 'data':
					for mass in ['mass2mu_ewk2']:
						peak_points[isdata][type_][charge][mass] = []
						mean_points[isdata][type_][charge][mass] = []
						peakerr_points[isdata][type_][charge][mass] = []
						for i in range(8):
							if dic.has_key(year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peak'):
								peak_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peak'])
								peakerr_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peakerr'])
								mean_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_mean'])
							else:
								peak_points[isdata][type_][charge][mass].append(-9999.99)
								peakerr_points[isdata][type_][charge][mass].append(-9999.99)
								mean_points[isdata][type_][charge][mass].append(-9999.99)
				if isdata == 'mc':
					for mass in masslist:
						peak_points[isdata][type_][charge][mass] = []
						mean_points[isdata][type_][charge][mass] = []
						peakerr_points[isdata][type_][charge][mass] = []
						for i in range(8):
							if dic.has_key(year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peak'):
								peak_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peak'])
								peakerr_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peakerr'])
								mean_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_mean'])
							else:
								peak_points[isdata][type_][charge][mass].append(-9999.99)
								peakerr_points[isdata][type_][charge][mass].append(-9999.99)
								mean_points[isdata][type_][charge][mass].append(-9999.99)

	#make unc
	peakunc = {}
	meanunc = {}
	dataunc = {}
	for type_ in ['eta','phi']:
		peakunc[type_]={}
		meanunc[type_]={}
		dataunc[type_]={}
		for charge in ['pos','neg']:
			peakunc[type_][charge]=mkpeakunc(peak_points['mc'][type_][charge],peakerr_points['mc'][type_][charge]['mass2mu_ewk2'],'peak')
			meanunc[type_][charge]=mkpeakunc(mean_points['mc'][type_][charge],peakerr_points['mc'][type_][charge]['mass2mu_ewk2'],'mean')
			dataunc[type_][charge]=mkpeakunc(peak_points['data'][type_][charge],peakerr_points['data'][type_][charge]['mass2mu_ewk2'],'peakerr')
	
	#mk plot
	dummy=[0,0,0,0,0,0,0,0]
	for type_ in ['eta','phi']:
		makeplot(peak_points['mc'][type_]['neg']['mass2mu_ewk2'],peak_points['mc'][type_]['pos']['mass2mu_ewk2'],peakunc[type_]['neg'],peakunc[type_]['pos'],peak_points['data'][type_]['neg']['mass2mu_ewk2'],peak_points['data'][type_]['pos']['mass2mu_ewk2'],dataunc[type_]['neg'],dataunc[type_]['pos'],type_,plotpath,'peak')
		#makeplot(mean_points['mc'][type_]['neg']['mass2mu_ewk2'],mean_points['mc'][type_]['pos']['mass2mu_ewk2'],meanunc[type_]['neg'],meanunc[type_]['pos'],mean_points['data'][type_]['neg']['mass2mu_ewk2'],peak_points['data'][type_]['pos']['mass2mu_ewk2'],dummy,dummy,type_,plotpath,'mean')



year = sys.argv[1]
low = sys.argv[2]
high = sys.argv[3]
assert year in ['2016a','2016b','2017','2018'], 'ERRORRRRRRRRRRRRRRRRRRRRRR'
plotpath = '/eos/user/c/chenguan/www/vertex/ScaleUnc/Real1D/19UL/unbinned/'+year+'_'+low+'_'+high+'/'
extractlist(plotpath)


