from multiprocessing import Pool
import ROOT
import glob
import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
from PlotClass import *
from DictClass import *
from CutClass import *

def run(cut_,inputname,outpath,outname,var1name,var2name):
	vars = 'RelIso1/RelIso2/Sip1/Sip2/ele1_ptpostcorr/ele2_ptpostcorr/lep1_ecalDriven/lep2_ecalDriven/ele_r91/ele_r92/ele_et1/ele_et2/eta1/eta2/mass2e_corr'
	cut_ = cut_
	cutstring2d(vars,cut_,inputname,os.getcwd(),var1name,var2name)
	print '\n\n\n\nmake'
	os.system('make')
	print '\n\n\nexecute'
	print outpath
	os.system('./ReadTree.exe ' + inputname + ' ' + outpath + ' ' + outname + ' ' + vars + ' '  + histosetting)

dict = {
	'2016pre_mc':'crab_DYJetsToLL_M50_LO_2016APV_2e_slimmed.root',
	#'2016pre_mc':'crab_DYJetsToLL_M50_2016APV_2e_slimmed.root',
	'2016pre_data':'crab_SingleElectron_Run2016_UL2016HIPM_2e_slimmed.root',
	
	'2016post_mc':'crab_DYJetsToLL_M50_LO_2016_2e_slimmed.root',
	#'2016post_mc':'crab_DYJetsToLL_M50_2016_2e_slimmed.root',
	'2016post_data':'crab_SingleElectron_Run2016_UL2016_2e_slimmed.root',
	
	'2017_mc':'crab_DYJetsToLL_M50_LO_2017_2e_slimmed.root',
	#'2017_mc':'crab_DYJetsToLL_M50_2017_new_2e_slimmed.root',
	'2017_data':'crab_SingleElectron_Run17_2e_slimmed.root',
	
	'2018_mc':'crab_DYJetsToLL_M50_2018_2e_slimmed.root',
	'2018_data':'crab_EGamma_Run18_2e_slimmed.root',
}
basedir = '/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/'

var1 = 'ele1_ptpostcorr'
var2 = 'ele2_ptpostcorr'
year = sys.argv[1]
mcname = sys.argv[2]
histosetting = '100/0/120/100/0/120'
dirclass = DirTree()
dirclass.mkrootdir(year+'_'+mcname+'_elescale_'+var1+'_vs_'+var2)
baseoutdir = dirclass.root

commoncut = 'RelIso1<0.35 && RelIso2<0.35 && Sip1<4 && Sip2<4 && lep1_ecalDriven==1 && lep2_ecalDriven==1 && -2.5<eta1<2.5 && -2.5<eta2<2.5 && 60<mass2e_corr<120'
ptcut = 'ele_et1>32 && ele_et2>20 && ele1_ptpostcorr<5000 && ele2_ptpostcorr<5000 && ele_r91<100 && ele_r92<100'
r9high = 'ele_r91>0.96 && ele_r92>0.96'
r9low = 'ele_r91<0.96 && ele_r92<0.96'
ebcut = 'e1_isee==0||1:0:0:1 && e2_isee==1 && e1_iseb==1 && e2_iseb==0'
eecut = 'e1_isee==1 && e2_isee==1 && e1_iseb==0 && e2_iseb==0'
bbcut = 'e1_isee==0 && e2_isee==0 && e1_iseb==1 && e2_iseb==1'

bbhigh = commoncut + ' && ' + ptcut
'''
bblow = bbhigh.replace(r9high,r9low)
eehigh = bbhigh.replace(bbcut,eecut)
eelow = eehigh.replace(r9high,r9low)
ebhigh = bbhigh.replace(bbcut,ebcut)
eblow = ebhigh.replace(r9high,r9low)
cutsdict = {'BBHIGH':bbhigh,'BBLOW':bblow,'EEHIGH':eehigh,'EELOW':eelow,'EBHIGH':ebhigh,'EBLOW':eblow}
'''
cutsdict = {'inclusive':bbhigh}
for cut in cutsdict:
	outpath = baseoutdir
	outname = cut+'_'+var1+'_'+var2
	inname = basedir + dict[year+'_'+mcname]
	run(cutsdict[cut],inname,outpath,outname,var1,var2)
