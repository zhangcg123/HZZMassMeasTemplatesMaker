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

def run(cut,inputname,outpath,outname,var1name,vars):

	cutstring(vars,cut,inputname,os.getcwd(),var1name)
	print '\n\n\n\nmake'
	os.system('make')
	print '\n\n\nexecute'
	print outpath
	os.system('./ReadTree.exe ' + inputname + ' ' + outpath + ' ' + outname + ' ' + vars + ' '  + histosetting + ' ' + fitsetting)

dict = {
	'ggf':'/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/elescaleptdependent_lut_noetcut_0-20firstbin-median/crab_GGF125_20UL2018.root',
	'dy':'/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_LO_2018_2e_slimmed.root',
	#'dynlomc':'/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_2018_2e_slimmed.root',
	#'dydata':'/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_EGamma_Run18_2e_slimmed.root',
}

cuts = {
	'ggf':'pTL1>7&&pTL2>7&&pTL3>7&&pTL4>7&&finalState==2',
	'dy':'ele1_ptpostcorr>7&&ele2_ptpostcorr>7&&ele_et1>32&&ele_et2>20',
}
var1 = {
	'ggf':'pTL1||pTL2||pTL3||pTL4',
	'dy':'ele1_ptpostcorr||ele2_ptpostcorr',
}
vars = {
	'ggf':'pTL1/pTL2/pTL3/pTL4/finalState',
	'dy':'ele1_ptpostcorr/ele2_ptpostcorr/ele_et1/ele_et2'
}

histosetting = '100/0/120'
fitsetting = '--'
dirclass = DirTree()
dirclass.mkrootdir('dyvsggf_elescale_ptdistributions')
baseoutdir = dirclass.root
outpath = baseoutdir


for outname in ['ggf','dy']:
	inname = dict[outname]
	cut_ = cuts[outname]
	var = vars[outname]
	var1_ = var1[outname]
	run(cut_,inname,outpath,outname,var1_,var)
c = ROOT.TCanvas('c','',1400,1000)
legend = ROOT.TLegend(.45,.62,.75,.75)
legend.SetTextSize(0.04)
legend.SetLineWidth(0)
list = ['dy','ggf']
for i in range(len(list)):
        file1 = ROOT.TFile(baseoutdir + '/' + list[i] + '.root','read')
        hhh = file1.Get(list[i])
        hhh.SetDirectory(0)
        hhh.SetLineWidth(5)
        hhh.SetLineStyle(1)
        hhh.SetLineColor(i+1)
        hhh.SetStats(0)
        hhh.Scale(1/hhh.Integral())
        legend.AddEntry(hhh,list[i])
        c.cd()
        if(i==0):hhh.Draw('hist')
        if(i!=0):hhh.Draw('histsame')
        file1.Close()
legend.Draw('same')
c.SaveAs(baseoutdir + '/xxx.png')
