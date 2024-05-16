import sys
import os
import ROOT
from array import array
from multiprocessing import Process
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
sys.path.append('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/EBE/cpp+py_multiprocessing/include/')
from FileSystemClass import *
import glob
import collections
import numpy as np

def multirun(year, plotpath, cut, ismc):
	cut_list = cut.split(',')
	absetalow = cut_list[0]
	absetahigh = cut_list[1]
	pterrlow = cut_list[2]
	pterrhigh = cut_list[3]
	os.system('./CalibrateMuons.exe '+year+' '+plotpath+' '+absetalow+' '+absetahigh+' '+pterrlow+' '+pterrhigh+' '+ismc)

def mklut(year, ismc, evaluationplotpath, True, lookuptableplotpath):
	files = glob.glob(evaluationplotpath + '/*txt')
	m1dict = []	
	etabounds = []
	pterrbounds = [0.0]
	for f in files:
		lambda_ = float(f.split('/')[-1].strip('.txt').split('_')[6])
		etalow = float(f.split('/')[-1].strip('.txt').split('_')[1]) 
		etabounds.append(etalow)
		etahigh = float(f.split('/')[-1].strip('.txt').split('_')[2])
		pterrlow = float(f.split('/')[-1].strip('.txt').split('_')[4])
		#pterrbounds.append(pterrlow)
		pterrhigh = float(f.split('/')[-1].strip('.txt').split('_')[5])
		list_ = [ etalow+0.0001, etahigh-0.0001, pterrlow+0.0001, pterrhigh-0.0001, lambda_ ]	
		m1dict.append(list_)
	etabounds.append(2.5)
	etabounds.sort()	
	pterrbounds.append(1)
	pterrbounds.sort()	
	print etabounds, pterrbounds
	lut = ROOT.TH2F("ebe_mu","",len(etabounds)-1,array('f',etabounds),len(pterrbounds)-1,array('f',pterrbounds) )
	e1x = lut.GetXaxis()
	e1y = lut.GetYaxis()	
	for i in m1dict:
		for j in np.arange(i[0],i[1],0.001):
			for k in np.arange(i[2],i[3],0.001):
				x = e1x.FindBin(j)
				y = e1y.FindBin(k)
				lut.SetBinContent(x,y,i[4])
	fo = ROOT.TFile( lookuptableplotpath + '/ebe_mu.root' ,'recreate')
	lut.Write()
	fo.Close()
	c = ROOT.TCanvas('c','',1000,1000)
	lut.Draw('text')
	c.SaveAs( lookuptableplotpath + '/ebe_mu.png' )

#arguments
year = sys.argv[1]
ismc = sys.argv[2]
assert year in ['2016pre', '2016post', '2017', '2018'] and ismc in ['mc','data'],'errrrrrrrrrrrrrrrrrrrrr'
#output dir
dirclass = DirTree()
dirclass.mkrootdir(year+'_'+ismc+'_allbins_muon_ebe_withvxbs_dscb')
path = dirclass.root
evaluationplotpath = dirclass.mksubdir('extract_ebe')
validationplotpath = dirclass.mksubdir('closure_ebe')
lookuptableplotpath = dirclass.mksubdir('lookuptable')
#first 3 bins
p1 = Process(target=multirun, args=(year,evaluationplotpath,'0,0.9,0,1', ismc))
p2 = Process(target=multirun, args=(year,evaluationplotpath,'0.9,1.8,0,1', ismc))
p3 = Process(target=multirun, args=(year,evaluationplotpath,'1.8,2.4,0,1', ismc))
'''
p4 = Process(target=multirun, args=(year,evaluationplotpath,'0.4,0.9,0.011,1', ismc))
p5 = Process(target=multirun, args=(year,evaluationplotpath,'0.9,1.8,0,0.015', ismc))
p6 = Process(target=multirun, args=(year,evaluationplotpath,'0.9,1.8,0.015,1', ismc))
p7 = Process(target=multirun, args=(year,evaluationplotpath,'1.8,2.4,0,1', ismc))
'''
p1.start()
#p2.start()
#p3.start()
#p4.start()
#p5.start()
#p6.start()
#p7.start()
p1.join()
#p2.join()
#p3.join()
#p4.join()
#p5.join()
#p6.join()
#p7.join()
#
#dice1 = mklut(year, ismc, evaluationplotpath, True, lookuptableplotpath)
#os.system('python run2muclosure.py '+path+' '+year+' '+ismc)
