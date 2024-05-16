import sys
import os
import ROOT
from array import array
from multiprocessing import Process
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
sys.path.append('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/EBE/cpp+py_multiprocessing/include/')
from FileSystemClass import *
from LUTMaker import mklut

def multirun(year, plotpath, cut, isecal, ismc, L_):
	cut_list = cut.split(',')
	absetalow = cut_list[0]
	absetahigh = cut_list[1]
	deltaptlow = cut_list[2]
	deltapthigh = cut_list[3]	
	os.system('./CalibrateElectrons.exe '+year+' '+plotpath+' '+absetalow+' '+absetahigh+' '+deltaptlow+' '+deltapthigh+' '+isecal+' '+ismc+' '+str(L_[1])+' '+str(L_[2])+' '+str(L_[3]))

#arguments
year = sys.argv[1]
ismc = sys.argv[2]
assert year in ['2016pre', '2016post', '2017', '2018'] and ismc in ['mc','data'],'errrrrrrrrrrrrrrrrrrrrr'
#output dir
dirclass = DirTree()
dirclass.mkrootdir(year+'_'+ismc+'_allbins_electron_ebe_scb')
evaluationplotpath = dirclass.mksubdir('extract_ebe')
validationplotpath = dirclass.mksubdir('closure_ebe')
lookuptableplotpath = dirclass.mksubdir('lookuptable')
#first 3 bins
p1 = Process(target=multirun, args=(year,evaluationplotpath,'0,0.8,0,0.01', 'ecal', ismc, {1:1,2:1,3:1}))
p2 = Process(target=multirun, args=(year,evaluationplotpath,'0,0.8,0.01,0.015', 'ecal', ismc, {1:1,2:1,3:1}))
p3 = Process(target=multirun, args=(year,evaluationplotpath,'0,0.8,0.015,0.025', 'ecal', ismc, {1:1,2:1,3:1}))
p1.start()
p2.start()
p3.start()
p1.join()
p2.join()
p3.join()
#
dice1 = mklut(year, ismc, evaluationplotpath, True, lookuptableplotpath)
L = {}
h = ROOT.TH1D('h','',3,array('d',[0,0.01,0.015,0.025]))
x = h.GetXaxis()
L[x.FindBin((dice1[0][2]+dice1[0][3])/2.0)] = dice1[0][4]
L[x.FindBin((dice1[1][2]+dice1[1][3])/2.0)] = dice1[1][4]
L[x.FindBin((dice1[2][2]+dice1[2][3])/2.0)] = dice1[2][4]
print L
#other bins
p4 = Process(target=multirun, args=(year,evaluationplotpath,'0.8,1.0,0,0.025', 'ecal', ismc, L))
p5 = Process(target=multirun, args=(year,evaluationplotpath,'1.0,2.5,0,0.02', 'ecal', ismc, L))
p6 = Process(target=multirun, args=(year,evaluationplotpath,'1.0,2.5,0.02,0.03', 'ecal', ismc, L))
p7 = Process(target=multirun, args=(year,evaluationplotpath,'1.0,2.5,0.03,0.04', 'ecal', ismc, L))
p8 = Process(target=multirun, args=(year,evaluationplotpath,'1.0,2.5,0.04,0.06', 'ecal', ismc, L))
p9 = Process(target=multirun, args=(year,evaluationplotpath,'0.0,1.0,0.025,1', 'ecal', ismc, L))
p10 = Process(target=multirun, args=(year,evaluationplotpath, '1.0,2.5,0.06,1', 'ecal', ismc, L))
p11 = Process(target=multirun, args=(year,evaluationplotpath,'0.0,1.44,0.0,1.0', 'trk', ismc, L))
p12 = Process(target=multirun, args=(year,evaluationplotpath,'1.44,1.6,0.0,1.0', 'trk', ismc, L))
p13 = Process(target=multirun, args=(year,evaluationplotpath,'1.6,2.0,0.0,1.0', 'trk', ismc, L))
p14 = Process(target=multirun, args=(year,evaluationplotpath,'2.0,2.5,0.0,1.0', 'trk', ismc, L))
p4.start()
p5.start()
p6.start()
p7.start()
p8.start()
p9.start()
p10.start()
p11.start()
p12.start()
p13.start()
p14.start()
p4.join()
p5.join()
p6.join()
p7.join()
p8.join()
p9.join()
p10.join()
p11.join()
p12.join()
p13.join()
p14.join()
mklut(year, ismc, evaluationplotpath, False, lookuptableplotpath)
os.system('./Closure2Electrons.exe '+year+' '+ismc+' '+validationplotpath)