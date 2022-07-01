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

def run(cut_,inputname,outpath,outname,histogramsetting):
	vars = 'RelIso1/RelIso2/Sip1/Sip2/'+massname+'/ele1_ptpostcorr/ele2_ptpostcorr/lep1_ecalDriven/lep2_ecalDriven/ele_et1/ele_et2/eta1/eta2/ele_r91/ele_r92'
	cutstring(vars,cut_,inputname,os.getcwd(),massname)
	print '\n\n\n\nmake'
	os.system('make')
	print '\n\n\nexecute'
	print outpath
	os.system('./ReadTree.exe ' + inputname + ' ' + outpath + ' ' + outname + ' ' + vars + ' '  + histogramsetting + ' ' + fitsetting )

dict = {
	#'2016pre_mc':'crab_DYJetsToLL_M50_LO_2016APV_2e_slimmed.root',
	#'2016pre_mc':'crab_DYJetsToLL_M50_2016APV_2e_slimmed.root',
	'2016pre_data':'crab_SingleElectron_Run2016_UL2016HIPM_2e_slimmed.root',
	
	#'2016post_mc':'crab_DYJetsToLL_M50_LO_2016_2e_slimmed.root',
	#'2016post_mc':'crab_DYJetsToLL_M50_2016_2e_slimmed.root',
	'2016post_data':'crab_SingleElectron_Run2016_UL2016_2e_slimmed.root',
	
	#'2017_mc':'crab_DYJetsToLL_M50_LO_2017_2e_slimmed.root',
	#'2017_mc':'crab_DYJetsToLL_M50_2017_new_2e_slimmed.root',
	'2017_data':'crab_SingleElectron_Run17_2e_slimmed.root',
	
	#'2018_mc':'crab_DYJetsToLL_M50_LO_2018_2e_slimmed.root',
	#'2018_mc':'crab_DYJetsToLL_M50_2018_2e_slimmed.root',
	'2018_data':'crab_EGamma_Run18_2e_slimmed.root',
}
basedir = '/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/'

year = sys.argv[1]
massname = 'mass2e_corr_fsr'
histogramsetting = ''
fitsetting = '91.19/2.49/-0.3/1.0/1.0/1.0/10/10'
dirclass = DirTree()
dirclass.mkrootdir(year+'_elescale_1bin_syncr9cutwith_bwxdcb+exp_fsr')
baseoutdir = dirclass.root

commoncut = 'RelIso1<0.35 && RelIso2<0.35 && Sip1<4 && Sip2<4 && lep1_ecalDriven==1 && lep2_ecalDriven==1 && -2.5<eta1<2.5 && -2.5<eta2<2.5'
ptcut = 'ele_et1>32 && ele_et2>20 && ele1_ptpostcorr<5000 && ele2_ptpostcorr<5000'
r9high = 'ele_r91>0.96 && ele_r92>0.96'
r9low = 'ele_r91<0.96 && ele_r92<0.96'
ebcut = 'e1_isee==0||1:0:0:1 && e2_isee==1 && e1_iseb==1 && e2_iseb==0'
eecut = 'e1_isee==1 && e2_isee==1 && e1_iseb==0 && e2_iseb==0'
bbcut = 'e1_isee==0 && e2_isee==0 && e1_iseb==1 && e2_iseb==1'

commoncut = commoncut + ' && ' + ptcut + ' && 0.5<ele_r91<1.1 && 0.5<ele_r92<1.1' 
masscut = ['60<'+massname+'<120','70<'+massname+'<110','80<'+massname+'<100']
cutsdict = {'60_120':commoncut+' && '+masscut[0],'70_110':commoncut+' && '+masscut[1],'80_100':commoncut+' && '+masscut[2]}

outpath = baseoutdir
for cut in cutsdict:
		masslow = cut.split('_')[0]
		masshigh = cut.split('_')[1]
		histogramsetting = str(int((float(masshigh)-float(masslow)))*100)+'/'+masslow+'/'+masshigh
		outname = cut
		inname = basedir + dict[year]
		run(cutsdict[cut],inname,outpath,outname,histogramsetting)

#baseoutdir = '/eos/user/c/chenguan/www/TESTPLOTS/19_Apr_2022/timestamp_14_40_31_012020_2016pre_mc_elescale_1bin_bwxdcb+exp_fsr/'
fulldict = {}
path = baseoutdir + '/'
sys.path.append(path)
files=glob.glob(path + '*.py')
for f in files:
	tmp_file = __import__(f.split('/')[-1].split('.')[0],globals(),locals(),['info'],-1)
	for p in tmp_file.info:
		fulldict[p] = tmp_file.info[p]

mc_peak_var = []
mc_sigma_var = []
for name in ['60_120','70_110','80_100']:
	for name_ in fulldict:
		if name in name_:
				if name_.endswith('peak'):
					mc_peak_var.append(fulldict[name_])
				if name_.endswith('sigma'):
					mc_sigma_var.append(fulldict[name_])

xposition = [0.5,1.5,2.5]
xerr = [0.5,0.5,0.5]
dummy = [0,0,0]

print mc_peak_var
print mc_sigma_var
ytitle = 'Z_{ee}^{mass}' 
mcconfig = {'title':'','ytitle':ytitle,'mksize':'1.5','mkstyle':'24','mkcolor':'4','xlabelsize':'0.04','xtitleoff':'0.5',
                'ytitleoff':'0.6','ytitlesize':'0.07','xtitlesize':'0.08'}
dtconfig = {'title':'','ytitle':ytitle,'mksize':'1','mkstyle':'20','mkcolor':'2','xlabelsize':'0.04','xtitleoff':'0.5',
                'ytitleoff':'0.6','ytitlesize':'0.07','xtitlesize':'0.08'}
barconfig = {'title':'','ytitle':'(data-mc)/mc(%)','mksize':'1','mkstyle':'21','mkcolor':'1','xlabelsize':'0.1','xtitleoff':'1.0','ylabelsize':'0.08',
                'ytitleoff':'0.45','ytitlesize':'0.1','xtitlesize':'0.15'}
xlabel = ['60-120','70-110','80-100']

obj1 = ListToGraph(xposition,mc_peak_var,xerr,dummy,mcconfig).g
for i in range(len(xlabel)):
	obj1.GetXaxis().SetBinLabel(obj1.GetXaxis().FindBin(xposition[i]),xlabel[i])
obj1.GetXaxis().LabelsOption('h')
obj1.GetXaxis().SetLabelOffset(0.02)
tmp = ScatterPlot0([obj1],['data'],['-'],baseoutdir+'peak',year.split('_')[0])

mcconfig['ytitle'] = 'Z_{ee}^{#sigma}'
dtconfig['ytitle'] = 'Z_{ee}^{#sigma}'
obj1 = ListToGraph(xposition,mc_sigma_var,xerr,dummy,mcconfig).g
for i in range(len(xlabel)):
	obj1.GetXaxis().SetBinLabel(obj1.GetXaxis().FindBin(xposition[i]),xlabel[i])
obj1.GetXaxis().LabelsOption('h')
obj1.GetXaxis().SetLabelOffset(0.02)
tmp = ScatterPlot0([obj1],['data'],['-'],baseoutdir+'sigma',year.split('_')[0])
