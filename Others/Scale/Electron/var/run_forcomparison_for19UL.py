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
	vars = var1name
	cutstring(vars,cut,inputname,os.getcwd(),var1name)
	print '\n\n\n\nmake'
	os.system('make')
	print '\n\n\nexecute'
	print outpath
	os.system('./ReadTree.exe ' + inputname + ' ' + outpath + ' ' + outname + ' ' + vars + ' '  + histosetting + ' ' + fitsetting)

dict = {
	'2016preVFP_20ULlomc':'crab_DYJetsToLL_M50_LO_2016APV_2e_slimmed.root',
	'2016preVFP_20ULnlomc':'crab_DYJetsToLL_M50_2016APV_2e_slimmed.root',
	'2016preVFP_20ULdata':'crab_SingleElectron_Run2016_UL2016HIPM_2e_slimmed.root',
	
	'2016postVFP_20ULlomc':'crab_DYJetsToLL_M50_LO_2016_2e_slimmed.root',
	'2016postVFP_20ULnlomc':'crab_DYJetsToLL_M50_2016_2e_slimmed.root',
	'2016postVFP_20ULdata':'crab_SingleElectron_Run2016_UL2016_2e_slimmed.root',
	
	'2017_20ULlomc':'crab_DYJetsToLL_M50_LO_2017_2e_slimmed.root',
	'2017_19ULnlomc':'DY_amcatnlo_skimmed_2e_2017.root',
	'2017_20ULnlomc':'crab_DYJetsToLL_M50_2017_new_2e_slimmed.root',
	'2017_20ULdata':'crab_SingleElectron_Run17_2e_slimmed.root',
	
	'2018_20ULlomc':'crab_DYJetsToLL_M50_LO_2018_2e_slimmed.root',
	'2018_20ULnlomc':'crab_DYJetsToLL_M50_2018_2e_slimmed.root',
	'2018_19ULnlomc':'DY_amcatnlo_skimmed_2e_2018.root',
	'2018_20ULdata':'crab_EGamma_Run18_2e_slimmed.root',
}
basedir = '/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/'

var1 = 'GENmassZ'
year = sys.argv[1]
histosetting = '400/80/100'
fitsetting = '--'
dirclass = DirTree()
dirclass.mkrootdir(year+'_19ULvs20UL_nlo_elescale_'+var1)
baseoutdir = dirclass.root
cut_ = histosetting.split('/')[-2]+'<'+var1+'<'+histosetting.split('/')[-1]
outpath = baseoutdir


for outname in ['20ULnlo','19ULnlo']:
	inname = basedir + dict[year+'_'+outname+'mc']
	if '19UL' in outname:
		var1 = 'GENmass2l'
		cut_ = cut_.replace('GENmassZ','GENmass2l')
	run(cut_,inname,outpath,outname,var1)
var1 = 'GENmassZ'
file1 = ROOT.TFile(baseoutdir + '/20ULnlo.root','read')
file2 = ROOT.TFile(baseoutdir + '/19ULnlo.root','read')
h1 = file1.Get('20ULnlo')
h2 = file2.Get('19ULnlo')
 
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
tex = ['20ULamcatnlo mean: '+str(mean1)+' #sigma: '+str(sigma1),'19ULamcatnlo mean: '+str(mean2)+' #sigma: '+str(sigma2)]
h3 = h1.Clone()
h4 = h2.Clone()
h3.Divide(h4)
h3.SetStats(0)
HistogramPlot2([h1,h2,h3],['20UL'+year+'_amcatnlo','19UL'+year+'_amcatnlo','ratio'],tex,baseoutdir+year+'_'+var1,var1)
