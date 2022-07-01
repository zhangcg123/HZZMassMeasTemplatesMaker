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

def run(cut_,massname_,inputname,outpath,outname):
	masscut = masslow+'<'+massname_+'<'+masshigh	
	vars = 'RelIso1/RelIso2/Sip1/Sip2/e1_isee/e2_isee/e1_iseb/e2_iseb/'+massname_+'/ele1_ptpostcorr/ele2_ptpostcorr/lep1_ecalDriven/lep2_ecalDriven/ele_r91/ele_r92/ele_et1/ele_et2/eta1/eta2'
	cut_ = cut_ + ' && ' + masscut
	cutstring(vars,cut_,inputname,os.getcwd(),massname_)
	print '\n\n\n\nmake'
	os.system('make')
	print '\n\n\nexecute'
	print outpath
	os.system('./ReadTree.exe ' + inputname + ' ' + outpath + ' ' + outname + ' ' + vars + ' '  + histosetting + ' ' + fitsetting )

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

year = sys.argv[1]
masslow = sys.argv[2]
masshigh = sys.argv[3]
histosetting = str(int((float(masshigh)-float(masslow)))*100)+'/'+masslow+'/'+masshigh
fitsetting = '91.19/2.49/-0.3/1.0/1.0/10'
dirclass = DirTree()
dirclass.mkrootdir(year+'_elescale_scb_lo_vs_data_'+masslow+'_'+masshigh)
baseoutdir = dirclass.root

commoncut = 'RelIso1<0.35 && RelIso2<0.35 && Sip1<4 && Sip2<4 && lep1_ecalDriven==1 && lep2_ecalDriven==1 && -2.5<eta1<2.5 && -2.5<eta2<2.5'
ptcut = 'ele_et1>32 && ele_et2>20 && ele1_ptpostcorr<5000 && ele2_ptpostcorr<5000'
r9high = 'ele_r91>0.96 && ele_r92>0.96'
r9low = 'ele_r91<0.96 && ele_r92<0.96'
ebcut = 'e1_isee==0||1:0:0:1 && e2_isee==1 && e1_iseb==1 && e2_iseb==0'
eecut = 'e1_isee==1 && e2_isee==1 && e1_iseb==0 && e2_iseb==0'
bbcut = 'e1_isee==0 && e2_isee==0 && e1_iseb==1 && e2_iseb==1'

bbhigh = commoncut + ' && ' + ptcut + ' && ' + r9high + ' && ' + bbcut
bblow = bbhigh.replace(r9high,r9low)
eehigh = bbhigh.replace(bbcut,eecut)
eelow = eehigh.replace(r9high,r9low)
ebhigh = bbhigh.replace(bbcut,ebcut)
eblow = ebhigh.replace(r9high,r9low)
cutsdict = {'BBHIGH':bbhigh,'BBLOW':bblow,'EEHIGH':eehigh,'EELOW':eelow,'EBHIGH':ebhigh,'EBLOW':eblow}

for cut in cutsdict:
	dirclass.mksubdir(cut)
	outpath = baseoutdir + '/' + cut + '/'
	for massname in ['mass2e_corr_data','mass2e_corr_mc','mass2e_scaleup_mc','mass2e_smearup_mc']:
		outname = cut+'_'+massname
		mcname = massname.split('_')[-1]
		massname = massname.split('_')[0]+'_'+massname.split('_')[1]
		inname = basedir + dict[year+'_'+mcname]
		run(cutsdict[cut],massname,inname,outpath,outname)
		#p=Pool(4)
		#p.apply_async(func=run,args=(cut,massname,inname,outpath,outname,))
	#p.close()
	#p.join()
#baseoutdir = '/eos/user/c/chenguan/www/TESTPLOTS/17_Apr_2022/timestamp_15_17_08_776960_2017_elescale_nlo_vs_data_80_100/'
fulldict = {}
for name__ in ['BBHIGH','BBLOW','EEHIGH','EELOW','EBHIGH','EBLOW']:
	path = baseoutdir + '/' + name__ + '/'
	sys.path.append(path)
	files=glob.glob(path + '*.py')
	for f in files:
		tmp_file = __import__(f.split('/')[-1].split('.')[0],globals(),locals(),['info'],-1)
		for p in tmp_file.info:
			fulldict[p] = tmp_file.info[p]

mc_peak_var = []
mc_sigma_var = []
mc_peak_scaleup = []
mc_sigma_smearup = []
mc_peak_err = []
mc_sigma_err = []
data_peak_var = []
data_sigma_var = []
for name in ['BBHIGH','BBLOW','EEHIGH','EELOW','EBHIGH','EBLOW']:
	for name_ in fulldict:
		if name in name_:
			if '_mc_' in name_:
				if name_.endswith('peak') and '_corr_' in name_:
					mc_peak_var.append(fulldict[name_])
				if name_.endswith('sigma') and '_corr_' in name_:
					mc_sigma_var.append(fulldict[name_])
				if name_.endswith('peakerr') and '_corr_' in name_:
					mc_peak_err.append(fulldict[name_])
				if name_.endswith('sigmaerr') and '_corr_' in name_:
					mc_sigma_err.append(fulldict[name_])
				if '_scaleup_' in name_ and name_.endswith('peak'):
					mc_peak_scaleup.append(fulldict[name_])
				if '_smearup_' in name_ and name_.endswith('sigma'):
					mc_sigma_smearup.append(fulldict[name_])
			if '_data_' in name_:
				if '_data_' in name_ and name_.endswith('peak'):
					data_peak_var.append(fulldict[name_])
				if '_data_' in name_ and name_.endswith('sigma'):
					data_sigma_var.append(fulldict[name_])
peak_diff_scaleup = ListDiff(mc_peak_var,mc_peak_scaleup).result
sigma_diff_smearup = ListDiff(mc_sigma_var,mc_sigma_smearup).result

peak_errbar = ListQrtSum([peak_diff_scaleup,mc_peak_err]).result
sigma_errbar = ListQrtSum([sigma_diff_smearup,mc_sigma_err]).result

peak_ratio = RelDiffinPresentage(data_peak_var,mc_peak_var).result
sigma_ratio = RelDiffinPresentage(data_sigma_var,mc_sigma_var).result

peak_ratioerr = Division(peak_errbar,mc_peak_var).result
sigma_ratioerr = Division(sigma_errbar,mc_sigma_var).result

for dd in range(len(peak_ratioerr)):
	peak_ratioerr[dd] = peak_ratioerr[dd]*100
	sigma_ratioerr[dd] = sigma_ratioerr[dd]*100

xposition = [0.5,1.5,2.5,3.5,4.5,5.5]
xerr = [0.5,0.5,0.5,0.5,0.5,0.5]
dummy = [0,0,0,0,0,0]

print mc_peak_var
print data_peak_var
ytitle = 'Z_{ee}^{mass}' 
mcconfig = {'title':'','ytitle':ytitle,'mksize':'1.5','mkstyle':'24','mkcolor':'4','xlabelsize':'0.04','xtitleoff':'0.5',
                'ytitleoff':'0.6','ytitlesize':'0.07','xtitlesize':'0.08'}
dtconfig = {'title':'','ytitle':ytitle,'mksize':'1','mkstyle':'20','mkcolor':'2','xlabelsize':'0.04','xtitleoff':'0.5',
                'ytitleoff':'0.6','ytitlesize':'0.07','xtitlesize':'0.08'}
barconfig = {'title':'','ytitle':'(data-mc)/mc(%)','mksize':'1','mkstyle':'21','mkcolor':'1','xlabelsize':'0.1','xtitleoff':'1.0','ylabelsize':'0.08',
                'ytitleoff':'0.45','ytitlesize':'0.1','xtitlesize':'0.15'}
xlabel = ['BBHIGHR9','BBLOWR9','EEHIGHR9','EELOWR9','EBHIGHR9','EBLOWR9']

obj1 = ListToGraph(xposition,mc_peak_var,xerr,peak_errbar,mcconfig).g
obj2 = ListToGraph(xposition,data_peak_var,xerr,dummy,dtconfig).g
obj3 = ListToGraph(xposition,peak_ratio,xerr,peak_ratioerr,barconfig).g
#obj3.GetXaxis().CenterTitle(True)
for i in range(len(xlabel)):
	obj3.GetXaxis().SetBinLabel(obj3.GetXaxis().FindBin(xposition[i]),xlabel[i])
obj3.GetXaxis().LabelsOption('h')
obj3.GetXaxis().SetLabelOffset(0.02)
tmp = ScatterPlot1([obj1,obj2,obj3],['MC','DATA'],['-'],baseoutdir+'peak',year)

mcconfig['ytitle'] = 'Z_{ee}^{#sigma}'
dtconfig['ytitle'] = 'Z_{ee}^{#sigma}'
obj1 = ListToGraph(xposition,mc_sigma_var,xerr,sigma_errbar,mcconfig).g
obj2 = ListToGraph(xposition,data_sigma_var,xerr,dummy,dtconfig).g
obj3 = ListToGraph(xposition,sigma_ratio,xerr,sigma_ratioerr,barconfig).g
#obj3.GetXaxis().CenterTitle(True)
for i in range(len(xlabel)):
	obj3.GetXaxis().SetBinLabel(obj3.GetXaxis().FindBin(xposition[i]),xlabel[i])
obj3.GetXaxis().LabelsOption('h')
obj3.GetXaxis().SetLabelOffset(0.02)
tmp = ScatterPlot1([obj1,obj2,obj3],['MC','DATA'],['-'],baseoutdir+'sigma',year)
