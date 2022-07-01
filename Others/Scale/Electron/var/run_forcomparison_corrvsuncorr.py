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

def run(cut,inputname,outpath,outname,var1name):
	vars = 'RelIso1/RelIso2/Sip1/Sip2/ele1_ptpostcorr/ele2_ptpostcorr/lep1_ecalDriven/lep2_ecalDriven/ele_et1/ele_et2/eta1/eta2/'+var1name
	cutstring(vars,cut,inputname,os.getcwd(),var1name)
	print '\n\n\n\nmake'
	os.system('make')
	print '\n\n\nexecute'
	print outpath
	os.system('./ReadTree.exe ' + inputname + ' ' + outpath + ' ' + outname + ' ' + vars + ' '  + histosetting + ' ' + fitsetting)

dict = {
	'2016preVFP_lomc':'crab_DYJetsToLL_M50_LO_2016APV_2e_slimmed.root',
	'2016preVFP_nlomc':'crab_DYJetsToLL_M50_2016APV_2e_slimmed.root',
	'2016preVFP_data':'crab_SingleElectron_Run2016_UL2016HIPM_2e_slimmed.root',
	
	'2016postVFP_lomc':'crab_DYJetsToLL_M50_LO_2016_2e_slimmed.root',
	'2016postVFP_nlomc':'crab_DYJetsToLL_M50_2016_2e_slimmed.root',
	'2016postVFP_data':'crab_SingleElectron_Run2016_UL2016_2e_slimmed.root',
	
	'2017_lomc':'crab_DYJetsToLL_M50_LO_2017_2e_slimmed.root',
	'2017_nlomc':'crab_DYJetsToLL_M50_2017_new_2e_slimmed.root',
	'2017_data':'crab_SingleElectron_Run17_2e_slimmed.root',
	
	'2018_lomc':'crab_DYJetsToLL_M50_LO_2018_2e_slimmed.root',
	'2018_nlomc':'crab_DYJetsToLL_M50_2018_2e_slimmed.root',
	'2018_data':'crab_EGamma_Run18_2e_slimmed.root',
}
basedir = '/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/'

var1 = 'mass2e_corr_fsr'
year = sys.argv[1]
histosetting = '100/80/100'
fitsetting = '--'
dirclass = DirTree()
dirclass.mkrootdir(year+'_nlo_vs_data_elescale_'+var1)
baseoutdir = dirclass.root
cut_ = 'RelIso1<0.35 && RelIso2<0.35 && Sip1<4 && Sip2<4 && lep1_ecalDriven==1 && lep2_ecalDriven==1 && -2.5<eta1<2.5 && -2.5<eta2<2.5 && ' + histosetting.split('/')[-2]+'<'+var1+'<'+histosetting.split('/')[-1] + ' && ele_et1>32 && ele_et2>20 && ele1_ptpostcorr<5000 && ele2_ptpostcorr<5000'
outpath = baseoutdir


for outname in ['nlomc','data']:
	inname = basedir + dict[year+'_'+outname]
	run(cut_,inname,outpath,outname,var1)

file1 = ROOT.TFile(baseoutdir + '/nlomc.root','read')
file2 = ROOT.TFile(baseoutdir + '/data.root','read')
h1 = file1.Get('nlomc')
h2 = file2.Get('data')
 
h1.SetStats(0)
h2.SetStats(0)
h1.Scale(1.0/h1.Integral())
h2.Scale(1.0/h2.Integral())

mean1 = round(h1.GetMean(),4)
sigma1 = round(h1.GetStdDev(),4)
mean2 = round(h2.GetMean(),4)
sigma2 = round(h2.GetStdDev(),4)
meanratio = round((mean1/mean2),4)
sigmaratio = round((sigma1/sigma2),4)
tex = ['    MC mean: '+str(mean1)+' #sigma: '+str(sigma1),'    DATA mean: '+str(mean2)+' #sigma: '+str(sigma2)]
h3 = h1.Clone()
h4 = h2.Clone()
h3.Divide(h4)
h3.SetStats(0)
HistogramPlot2([h1,h2,h3],['MC','DATA','ratio'],tex,baseoutdir+year+'_'+var1,'massZ')
