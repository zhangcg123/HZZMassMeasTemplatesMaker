import ROOT
import sys
import os
import glob
from array import array
import numpy as np
def mklut(year, ismc, inputpath, isfirst3, outputpath__):
	files = glob.glob(inputpath + '/*txt')
	e1dic = []
	e3dic = []
	for i in files:
		ii = i.split('/')[-1].split('.t')[0].split('_')[-1]
		ecal = i.split('/')[-1].split('.t')[0].split('_')[0]
		etalow = i.split('/')[-1].split('.t')[0].split('_')[2]
		etahigh = i.split('/')[-1].split('.t')[0].split('_')[3]
		deltaptlow = i.split('/')[-1].split('.t')[0].split('_')[5]
		deltapthigh = i.split('/')[-1].split('.t')[0].split('_')[6]
		if ecal == 'ecal':
			list_ = [float(etalow)+0.0001,float(etahigh)-0.0001,float(deltaptlow)+0.0001,float(deltapthigh)-0.0001,float(ii)]
			e1dic.append(list_)	
		if ecal == 'trk':
			list_ = [7.0+0.001,100.0-0.001,float(etalow)+0.0001,float(etahigh)-0.0001,float(ii)]
			e3dic.append(list_)
	if isfirst3 == True:
		return e1dic
	e1 = ROOT.TH2D('e1','e1',3,array('d',[0, 0.8, 1.0, 2.5]),8,array('d',[0, 0.01, 0.015, 0.02, 0.025, 0.03, 0.04, 0.06, 1]))
	e3 = ROOT.TH2D('e3','e3',1,array('d',[7, 100]),4,array('d',[0,1.44,1.6,2,2.5]))
	e1x = e1.GetXaxis()
	e1y = e1.GetYaxis()
	e3x = e3.GetXaxis()
	e3y = e3.GetYaxis()
	for i in e1dic:
		for j in np.arange(i[0],i[1],0.001):
			for k in np.arange(i[2],i[3],0.001):
				x = e1x.FindBin(j)
				y = e1y.FindBin(k)
				e1.SetBinContent(x,y,i[4])
	for i in e3dic:
		for j in np.arange(i[2],i[3],0.01):
			y = e3y.FindBin(j)
			e3.SetBinContent(1,y,i[4])
	for i in range(1,e1.GetXaxis().GetNbins()+1):
		for j in range(1,e1.GetYaxis().GetNbins()+1):
			print e1.GetXaxis().GetBinCenter(i), e1.GetYaxis().GetBinCenter(j), e1.GetBinContent(i,j)
	print '========================================='
	for i in range(1,e3.GetYaxis().GetNbins()+1):
		print e3.GetYaxis().GetBinCenter(i), e3.GetBinContent(1,i)
	outputpath = glob.glob(outputpath__)
	if len(outputpath) != 1:
		print 'errr'
		sys.exists()
	else:
		outputpath_ = outputpath[0]	
		c = ROOT.TCanvas('c','',1000,1000)
		c.cd()
		e1.Draw('TEXT')
		c.SaveAs(outputpath_ + '/'+year+'_'+ismc+'_e1.png')
		c.Clear()
		e3.Draw('TEXT')
		c.SaveAs(outputpath_ + '/'+year+'_'+ismc+'_e3.png')
		fe3 = ROOT.TFile(outputpath_+'/'+year+'_'+ismc+'_e3.root','RECREATE')
		fe3.cd()
		e3.Write()
		fe3.Close()
		os.system('cp '+outputpath_+'/'+year+'_'+ismc+'_e3.root LUT/')
		fe1 = ROOT.TFile(outputpath_+'/'+year+'_'+ismc+'_e1.root','RECREATE')
		fe1.cd()
		e1.Write()
		fe1.Close()
		os.system('cp '+outputpath_+'/'+year+'_'+ismc+'_e1.root LUT/')
