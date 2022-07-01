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
		xerr = [abs((-2.4*(3.5/4.0))-(-2.4*(2.5/4.0)))/2.0]*8
	elif k == 'phi':
		pi = numpy.pi
		x = [-pi*(3.5/4.0),-pi*(2.5/4.0),-pi*(1.5/4.0),-pi*(0.5/4.0),pi*(0.5/4.0),pi*(1.5/4.0),pi*(2.5/4.0),pi*(3.5/4.0)]
		xerr = [abs(-pi*(3.5/4.0)-(-pi*(2.5/4.0)))/2.0]*8
	else :
		mcnegpoints = mcnegpoints[:-2]
		mcpospoints = mcpospoints[:-2]
		datanegpoints = datanegpoints[:-2]
		datapospoints = datapospoints[:-2]
		x = [(20.0+5.0)/2.0,(20.0+30.0)/2.0,(30.0+40.0)/2.0,(40.0+50.0)/2.0,(50.0+60.0)/2.0,(100.0+60.0)/2.0]
		xerr = [7.5,5,5,5,5,20]
			
	print mcnegpoints
	print mcpospoints
	print datanegpoints
	print datapospoints

	if what == 'peak' or what == 'mean':
		for i in range(len(mcnegpoints)):
			mcnegpoints[i] = mcnegpoints[i]
			mcpospoints[i] = mcpospoints[i]

	g_mc_neg = ROOT.TGraphErrors(len(x), numpy.array(x), numpy.array(mcnegpoints), numpy.array(xerr), numpy.array(mcnegerrors))
	g_mc_pos = ROOT.TGraphErrors(len(x), numpy.array(x), numpy.array(mcpospoints), numpy.array(xerr), numpy.array(mcposerrors))
	g_dt_neg = ROOT.TGraphErrors(len(x), numpy.array(x), numpy.array(datanegpoints), numpy.array(xerr), numpy.array(datanegerrors))
	g_dt_pos = ROOT.TGraphErrors(len(x), numpy.array(x), numpy.array(datapospoints), numpy.array(xerr), numpy.array(dataposerrors))
	
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
	
	if k !='pt':
		g_mc_pos.GetXaxis().SetTitle('#'+k)
	if k == 'pt':
		g_mc_pos.GetXaxis().SetTitle('pt')
		g_mc_pos.GetYaxis().SetLabelSize(0)
	g_mc_pos.GetXaxis().SetTitleOffset(0.4)
	g_mc_pos.GetXaxis().SetTitleSize(0.1)
	'''			
	if what == 'peak' and k == 'eta' and 'ewk2asdef' in plotpath:
		max_ = 91.08
		min_ = 90.9
	if what == 'peak' and k == 'phi' and 'ewk2asdef' in plotpath:
		max_ = 91.14
		min_ = 90.98
	if what == 'peak' and k == 'eta' and 'defasdef' in plotpath:
		max_ = 91.08+0.054
		min_ = 90.9+0.054
	if what == 'peak' and k == 'phi' and 'defasdef' in plotpath:
		max_ = 91.14 + 0.034
		min_ = 90.98 + 0.034
	if what == 'mean' and k == 'eta':
		max_ = 90.98
		min_ = 90.92
	if what == 'mean' and k == 'phi':
		max_ = 90.96
		min_ = 90.93
	'''
	if what == 'mean' or what == 'peak':
		max_ = numpy.amax(mcnegpoints)*1.001
		min_ = numpy.amin(mcnegpoints)*0.999
	elif what == 'sigma':
		max_ = 3
		min_ = 0
	elif what == 'n1' or what == 'n2':
		max_ = numpy.amax(mcnegpoints)*1.5
		min_ = numpy.amin(mcnegpoints)*0.5
	else: 
		max_ = numpy.amax(mcnegpoints)*1.3
		min_ = numpy.amin(mcnegpoints)*0.7
	
	c = ROOT.TCanvas('c','',1000,1000)
	g_mc_neg.GetHistogram().SetMaximum(max_)
	g_mc_neg.GetHistogram().SetMinimum(min_)
	g_mc_pos.GetHistogram().SetMaximum(max_)
	g_mc_pos.GetHistogram().SetMinimum(min_)	
	
	legend = ROOT.TLegend(.74,.82,.95,.95)
	legend.SetTextSize(0.1)
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
	if k!='pt':
		c.cd()
		c1 = ROOT.TPad('p1','',0,0.55,1,1.0)
		c1.Draw()
		c1.cd()
		c1.SetTopMargin(0.1)
		c1.SetBottomMargin(0.01)
		g_mc_neg.Draw('ap')
		g_dt_neg.Draw('p')
		lx.DrawLatex(0.2,0.9,'#mu^{-}')
		legend.Draw('same')
		c.cd()
		c2 = ROOT.TPad('p2','',0,0.1,1,0.55)
		c2.Draw()
		c2.cd()
		c2.SetTopMargin(0.01)
		c2.SetBottomMargin(0.1)
		g_mc_pos.Draw('ap')
		g_dt_pos.Draw('p')
		lx.DrawLatex(0.2,0.9,'#mu^{+}')
		c.SaveAs(plotpath + '111_' + year + '_' + k + '_' + what + '.png')
	else:
		c.cd()
		c1 = ROOT.TPad('p1','',0,0,0.5,1.0)#bottom left x and y, top right x and y
		c1.Draw()
		c1.cd()
		c1.SetTopMargin(0.05)
		c1.SetBottomMargin(0.1)
		c1.SetLeftMargin(0.1)
		c1.SetRightMargin(0.01)
		g_mc_neg.Draw('ap')
		g_dt_neg.Draw('p')
		lx.DrawLatex(0.3,0.9,'#mu^{-}')
		legend.Draw('same')
		c.cd()
		c2 = ROOT.TPad('p2','',0.5,0.0,1,1)
		c2.Draw()
		c2.cd()
		c2.SetTopMargin(0.05)
		c2.SetBottomMargin(0.1)
		c2.SetRightMargin(0.1)
		c2.SetLeftMargin(0.01)
		g_mc_pos.Draw('ap')
		g_dt_pos.Draw('p')
		lx.DrawLatex(0.3,0.9,'#mu^{+}')
		c.SaveAs(plotpath + '111_' + year + '_' + k + '_' + what + '.png')
		

def mkpeakunc(dic,lis,what):

	diffdic = {}
	for i in dic:
		diffdic[i] = []
		for j in range(8):
			diffdic[i].append(abs(dic[i][j]-dic['mass2mu_corr'][j]))
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
	
	year = plotpath.split('/')[-2].split('_')[0]
	low = plotpath.split('/')[-2].split('_')[2]
	high = plotpath.split('/')[-2].split('_')[3]
	print year, low, high
		
	sys.path.append(plotpath + '/')		
	files = glob.glob(plotpath + '/*/*py')
	for i in files:
		i = i.rsplit('/',1)[0]
		sys.path.append(i)

	dic = {}
	for f in files:
		dic_tmp = __import__(f.split('/')[-1].split('.')[0],globals(),locals(),['info'],-1)
		print dic_tmp.info
		for p in dic_tmp.info:
			dic[p] = dic_tmp.info[p]
	for i in dic:
		print i, dic[i]
	masslist = ['mass2mu_corr','mass2mu_ewk_dt','mass2mu_zpt_dt','mass2mu_dm_dt']
	
	peak_points = {}
	mean_points = {}
	peakerr_points = {}
	sigma_points = {}
	a1_points = {}
	a2_points = {}
	n1_points = {}
	n2_points = {}
	shift_points = {}
	for isdata in ['data','mc']:
		peak_points[isdata] = {}
		mean_points[isdata] = {}
		peakerr_points[isdata] = {}
		sigma_points[isdata] = {}
		a1_points[isdata] = {}
		a2_points[isdata] = {}
		n1_points[isdata] = {}
		n2_points[isdata] = {}
		shift_points[isdata] = {}
		for type_ in ['eta','phi','pt']:
			peak_points[isdata][type_] = {}
			mean_points[isdata][type_] = {}
			peakerr_points[isdata][type_] = {}
			sigma_points[isdata][type_] = {}
			a1_points[isdata][type_] = {}
			a2_points[isdata][type_] = {}
			n1_points[isdata][type_] = {}
			n2_points[isdata][type_] = {}
			shift_points[isdata][type_] = {}
			for charge in ['pos','neg']:
				peak_points[isdata][type_][charge] = {}
				mean_points[isdata][type_][charge] = {}
				peakerr_points[isdata][type_][charge] = {}
				sigma_points[isdata][type_][charge] = {}
				a1_points[isdata][type_][charge] = {}
				a2_points[isdata][type_][charge] = {}
				n1_points[isdata][type_][charge] = {}
				n2_points[isdata][type_][charge] = {}
				shift_points[isdata][type_][charge] = {}
				if isdata == 'data':
					for mass in ['mass2mu_corr']:
						peak_points[isdata][type_][charge][mass] = []
						mean_points[isdata][type_][charge][mass] = []
						peakerr_points[isdata][type_][charge][mass] = []
						sigma_points[isdata][type_][charge][mass] = []
						a1_points[isdata][type_][charge][mass] = []
						a2_points[isdata][type_][charge][mass] = []
						n1_points[isdata][type_][charge][mass] = []
						n2_points[isdata][type_][charge][mass] = []
						shift_points[isdata][type_][charge][mass] = []
						for i in range(8):
							if dic.has_key(year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peak'):

								peak_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peak'])
								peakerr_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peakerr'])
								mean_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_mean'])
								sigma_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_sigma'])
								a1_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_a1'])
								n1_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_n1'])
								a2_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_a2'])
								n2_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_n2'])
								
								shift_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_shift'])
							else:
								peak_points[isdata][type_][charge][mass].append(-9999.99)
								peakerr_points[isdata][type_][charge][mass].append(-9999.99)
								mean_points[isdata][type_][charge][mass].append(-9999.99)
								sigma_points[isdata][type_][charge][mass].append(-9999.99)
								a1_points[isdata][type_][charge][mass].append(-9999.99)
								a2_points[isdata][type_][charge][mass].append(-9999.99)
								n1_points[isdata][type_][charge][mass].append(-9999.99)
								n2_points[isdata][type_][charge][mass].append(-9999.99)
								shift_points[isdata][type_][charge][mass].append(-9999.99)
				if isdata == 'mc':
					for mass in masslist:
						peak_points[isdata][type_][charge][mass] = []
						mean_points[isdata][type_][charge][mass] = []
						peakerr_points[isdata][type_][charge][mass] = []
						sigma_points[isdata][type_][charge][mass] = []
						a1_points[isdata][type_][charge][mass] = []
						a2_points[isdata][type_][charge][mass] = []
						n1_points[isdata][type_][charge][mass] = []
						n2_points[isdata][type_][charge][mass] = []
						shift_points[isdata][type_][charge][mass] = []
						for i in range(8):
							if dic.has_key(year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peak'):
								peak_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peak'])
								peakerr_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_peakerr'])
								mean_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_mean'])
								sigma_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_sigma'])
								a1_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_a1'])
								n1_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_n1'])
								a2_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_a2'])
								n2_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_n2'])

								shift_points[isdata][type_][charge][mass].append(dic[year+'_'+isdata+'_'+mass+'_'+type_+'_'+charge+'_'+str(i)+'_'+low+'_'+high+'_shift'])
							else:
								peak_points[isdata][type_][charge][mass].append(-9999.99)
								peakerr_points[isdata][type_][charge][mass].append(-9999.99)
								mean_points[isdata][type_][charge][mass].append(-9999.99)
								sigma_points[isdata][type_][charge][mass].append(-9999.99)
								a1_points[isdata][type_][charge][mass].append(-9999.99)
								a2_points[isdata][type_][charge][mass].append(-9999.99)
								n1_points[isdata][type_][charge][mass].append(-9999.99)
								n2_points[isdata][type_][charge][mass].append(-9999.99)
								shift_points[isdata][type_][charge][mass].append(-9999.99)
	
	#make unc
	peakunc = {}
	meanunc = {}
	dataunc = {}
	for type_ in ['eta','phi','pt']:
		peakunc[type_]={}
		meanunc[type_]={}
		dataunc[type_]={}
		for charge in ['pos','neg']:
			peakunc[type_][charge]=mkpeakunc(peak_points['mc'][type_][charge],peakerr_points['mc'][type_][charge]['mass2mu_corr'],'peak')
			meanunc[type_][charge]=mkpeakunc(mean_points['mc'][type_][charge],peakerr_points['mc'][type_][charge]['mass2mu_corr'],'mean')
			dataunc[type_][charge]=mkpeakunc(peak_points['data'][type_][charge],peakerr_points['data'][type_][charge]['mass2mu_corr'],'peakerr')
	
	#mk plot
	dummy=[0,0,0,0,0,0,0,0]
	for type_ in ['eta','phi','pt']:
		makeplot(peak_points['mc'][type_]['neg']['mass2mu_corr'],peak_points['mc'][type_]['pos']['mass2mu_corr'],peakunc[type_]['neg'],peakunc[type_]['pos'],peak_points['data'][type_]['neg']['mass2mu_corr'],peak_points['data'][type_]['pos']['mass2mu_corr'],dataunc[type_]['neg'],dataunc[type_]['pos'],type_,plotpath,'peak')
		makeplot(mean_points['mc'][type_]['neg']['mass2mu_corr'],mean_points['mc'][type_]['pos']['mass2mu_corr'],meanunc[type_]['neg'],meanunc[type_]['pos'],mean_points['data'][type_]['neg']['mass2mu_corr'],mean_points['data'][type_]['pos']['mass2mu_corr'],dummy,dummy,type_,plotpath,'mean')
		makeplot(sigma_points['mc'][type_]['neg']['mass2mu_corr'],sigma_points['mc'][type_]['pos']['mass2mu_corr'],dummy,dummy,sigma_points['data'][type_]['neg']['mass2mu_corr'],sigma_points['data'][type_]['pos']['mass2mu_corr'],dummy,dummy,type_,plotpath,'sigma')
		
		#makeplot(a1_points['mc'][type_]['neg']['mass2mu_corr'],a1_points['mc'][type_]['pos']['mass2mu_corr'],dummy,dummy,a1_points['data'][type_]['neg']['mass2mu_corr'],a1_points['data'][type_]['pos']['mass2mu_corr'],dummy,dummy,type_,plotpath,'a1')

		#makeplot(a2_points['mc'][type_]['neg']['mass2mu_corr'],a2_points['mc'][type_]['pos']['mass2mu_corr'],dummy,dummy,a2_points['data'][type_]['neg']['mass2mu_corr'],a2_points['data'][type_]['pos']['mass2mu_corr'],dummy,dummy,type_,plotpath,'a2')

		#makeplot(n1_points['mc'][type_]['neg']['mass2mu_corr'],n1_points['mc'][type_]['pos']['mass2mu_corr'],dummy,dummy,n1_points['data'][type_]['neg']['mass2mu_corr'],n1_points['data'][type_]['pos']['mass2mu_corr'],dummy,dummy,type_,plotpath,'n1')
		
		#makeplot(n2_points['mc'][type_]['neg']['mass2mu_corr'],n2_points['mc'][type_]['pos']['mass2mu_corr'],dummy,dummy,n2_points['data'][type_]['neg']['mass2mu_corr'],n2_points['data'][type_]['pos']['mass2mu_corr'],dummy,dummy,type_,plotpath,'n2')
		
		#makeplot(shift_points['mc'][type_]['neg']['mass2mu_corr'],shift_points['mc'][type_]['pos']['mass2mu_corr'],dummy,dummy,shift_points['data'][type_]['neg']['mass2mu_corr'],shift_points['data'][type_]['pos']['mass2mu_corr'],dummy,dummy,type_,plotpath,'shift')

year = sys.argv[1]
low = sys.argv[2]
high = sys.argv[3]
init_mean = sys.argv[4]
buffer_ = sys.argv[5]
plotpath = sys.argv[6]
assert year in ['2016a','2016b','2017','2018'], 'ERRORRRRRRRRRRRRRRRRRRRRRR'
extractlist(plotpath)
