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

def run(cut_,inputname,outpath,outname,var1name):
	vars = 'RelIso1/RelIso2/Sip1/Sip2/ele1_ptpostcorr/ele2_ptpostcorr/lep1_ecalDriven/lep2_ecalDriven/ele_r91/ele_r92/ele_et1/ele_et2/eta1/eta2/mass2e_corr'
	cut_ = cut_
	cutstring(vars,cut_,inputname,os.getcwd(),var1name)
	print '\n\n\n\nmake'
	os.system('make')
	print '\n\n\nexecute'
	print outpath
	os.system('./ReadTree.exe ' + inputname + ' ' + outpath + ' ' + outname + ' ' + vars + ' '  + histosetting + ' ' + fitsetting)

dict = {
	'2016pre_lomc':'crab_DYJetsToLL_M50_LO_2016APV_2e_slimmed.root',
	'2016pre_nlomc':'crab_DYJetsToLL_M50_2016APV_2e_slimmed.root',
	'2016pre_data':'crab_SingleElectron_Run2016_UL2016HIPM_2e_slimmed.root',
	
	'2016post_lomc':'crab_DYJetsToLL_M50_LO_2016_2e_slimmed.root',
	'2016post_nlomc':'crab_DYJetsToLL_M50_2016_2e_slimmed.root',
	'2016post_data':'crab_SingleElectron_Run2016_UL2016_2e_slimmed.root',
	
	'2017_lomc':'crab_DYJetsToLL_M50_LO_2017_2e_slimmed.root',
	'2017_nlomc':'crab_DYJetsToLL_M50_2017_new_2e_slimmed.root',
	'2017_data':'crab_SingleElectron_Run17_2e_slimmed.root',
	
	'2018_lomc':'crab_DYJetsToLL_M50_LO_2018_2e_slimmed.root',
	'2018_nlomc':'crab_DYJetsToLL_M50_2018_2e_slimmed.root',
	'2018_data':'crab_EGamma_Run18_2e_slimmed.root',
}
basedir = '/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/'

var1 = 'ele_r91+ele_r92'
#year = sys.argv[1]
histosetting = '100/1.0/2.2'
fitsetting = '--'
dirclass = DirTree()
dirclass.mkrootdir('lo_vs_years_elescale_'+var1)
baseoutdir = dirclass.root

commoncut = 'RelIso1<0.35 && RelIso2<0.35 && Sip1<4 && Sip2<4 && lep1_ecalDriven==1 && lep2_ecalDriven==1 && -2.5<eta1<2.5 && -2.5<eta2<2.5 && 60<mass2e_corr<120'
ptcut = 'ele_et1>32 && ele_et2>20 && ele1_ptpostcorr<5000 && ele2_ptpostcorr<5000 && 0.5<ele_r91<1.1 && 0.5<ele_r92<1.1'
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
ys = ['2016pre_data','2016post_data','2017_data','2018_data']
cutsdict = {'inclusive':bbhigh}
for y in ys:
	outpath = baseoutdir
	outname1 = 'inclusive_'+y
	inname = basedir + dict[y]
	run(cutsdict['inclusive'],inname,outpath,outname1,var1)

c = ROOT.TCanvas('c','',1400,1000)
legend = ROOT.TLegend(.15,.62,.45,.75)
legend.SetTextSize(0.02)
legend.SetLineWidth(0)
for i in range(len(ys)):
	file1 = ROOT.TFile(baseoutdir + '/inclusive_' + ys[i] + '.root','read')
	hhh = file1.Get('inclusive_'+ys[i])
	hhh.SetDirectory(0)
	hhh.SetLineWidth(2)
	hhh.SetLineStyle(1)
	hhh.SetLineColor(i+1)
	hhh.SetStats(0)
	hhh.Scale(1/hhh.Integral())
	legend.AddEntry(hhh,ys[i])
	c.cd()
	if(i==0):hhh.Draw('hist')
	if(i!=0):hhh.Draw('histsame')
	file1.Close()
legend.Draw('same')
c.SaveAs(baseoutdir + '/xxx.png')
