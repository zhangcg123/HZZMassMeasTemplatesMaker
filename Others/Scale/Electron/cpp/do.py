import os
import sys
import time
import glob
import numpy as np
from multiprocessing import Pool
from multiprocessing import Manager
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
sys.path.append('.')
from ptdependentlut import *
from FileSystemClass import *
from DictClass import *
from PlotClass import *

def multirun(year, fs_, isdata, mass_name, low, high, init_mean, buffer_):
	
	command = './ReadTree.exe '+year+' '+isdata+' '+mass_name+' '+low+' '+high+' '+init_mean+' '+buffer_+' '+plotpath+'/'+mass_name+'/ ' + fs_
	print command
	os.system(command)

#arguments holder
os.system('make')
year = sys.argv[1]
fs = sys.argv[2]
low = sys.argv[3]
high = sys.argv[4]
init_mean = sys.argv[5]
buffer_ = sys.argv[6]
if fs == '2mu':mass_list = ['mass2mu_ewk2','mass2mu_ewk_dt','mass2mu_zpt_dt','mass2mu_dm_dt','mass2mu_ewk2']
if fs == '2e':mass_list = ['mass2e_uncorr','mass2e_corr','mass2e_scaleup','mass2e_scaledn','mass2e_smearup','mass2e_smeardn']

if True:
	dirclass = DirTree()
	dirclass.mkrootdir(year+'_'+fs+'_'+low+'_'+high+'_'+init_mean+'_'+buffer_+'_combr9binscuton1p2_fixbwall_floatm_lo_vs_data_2kfft_doublecbwithcmp_iso0p35sip4_withetcut3220_zpt+pileupweightedmc_ptcut5k_fsr_bw91p19_2p49')
	for name in mass_list:
		dirclass.mksubdir(name)
		dirclass.mksubdir(name+'/fitplots/')
		dirclass.mksubdir(name+'/mass_distributions/')
		k_list = ['eta','phi','pt']
		for i in k_list:
			dirclass.mksubdir(name+'/fitplots/'+i)
			dirclass.mksubdir(name+'/mass_distributions/'+i)

#	subsubdir_list = ['angle_distributions','mass_distributions','zpt_distributions','zpz_distributions','zphi_distributions','zeta_distributions','fix_pt_distributions','free_pt_distributions','lepton_distributions','pterr_distributions','pterrold_distributions']
#	for i in subsubdir_list:
#		dirclass.mksubdir(name+'/'+i)
#		for j in ['_1-5bin','_neg_edge_vs_middle','_pos_edge_vs_middle','_pos_vs_neg','_datavsmc']:
#			for j_ in ['pt','eta','phi']:
#				dirclass.mksubdir(name+'/'+i+'/'+j_+j)

	descrip = fs + ' xx'
	dirclass.mkdescrip(descrip)
	plotpath = dirclass.root

#multiple run
	if fs == '2e':
		p1=Pool(len(mass_list))
		for i in mass_list:
			p1.apply_async(func=multirun,args=(year,fs,'mc',i,low,high,init_mean,buffer_,))
		p1.close()
		p1.join()
		p2=Pool(len(mass_list))
		for i in mass_list:
			p2.apply_async(func=multirun,args=(year,fs,'data',i,low,high,init_mean,buffer_,))
		p2.close()
		p2.join()

	if fs == '2mu':
		mc_tmp = 'mc'
		p1 = Pool(len(mass_list))
		for i in range(len(mass_list)):
			if i == len(mass_list)-1:
				mc_tmp = 'data'
			p1.apply_async(func=multirun,args=(year,fs,mc_tmp,mass_list[i],low,high,init_mean,buffer_,))
		p1.close()
		p1.join()
#the main jobs have been done, extrac output information below
t0 = time.clock()
#closure plot
'''
if year == '2016pre':
	plotpath = '/eos/user/c/chenguan/www/TESTPLOTS/31_Mar_2022/timestamp_15_08_12_616363_2016pre_2e_80_100_-0.5_0.2_fixdoublecountissue_2kfft_doublecbwithbkg_iso3p5sip4dxy0p5_withtriggercut_withetcut_noweightedmc_ptcut5k_mc2p44_nofsr_mcewk2-34_vs_dataewk2/'
if year == '2016post':
	plotpath = '/eos/user/c/chenguan/www/TESTPLOTS/31_Mar_2022/timestamp_15_23_38_465621_2016post_2e_80_100_-0.5_0.2_fixdoublecountissue_2kfft_doublecbwithbkg_iso3p5sip4dxy0p5_withtriggercut_withetcut_noweightedmc_ptcut5k_mc2p44_nofsr_mcewk2-34_vs_dataewk2/'
if year == '2017':
	plotpath = '/eos/user/c/chenguan/www/TESTPLOTS/31_Mar_2022/timestamp_15_31_48_742409_2017_2e_80_100_-0.5_0.2_fixdoublecountissue_2kfft_doublecbwithbkg_iso3p5sip4dxy0p5_withtriggercut_withetcut_noweightedmc_ptcut5k_mc2p44_nofsr_mcewk2-34_vs_dataewk2/'
if year == '2018':
	plotpath = '/eos/user/c/chenguan/www/TESTPLOTS/31_Mar_2022/timestamp_15_49_13_887134_2018_2e_80_100_-0.5_0.2_fixdoublecountissue_2kfft_doublecbwithbkg_iso3p5sip4dxy0p5_withtriggercut_withetcut_noweightedmc_ptcut5k_mc2p44_nofsr_mcewk2-34_vs_dataewk2/'
'''
#plotpath = '/eos/user/c/chenguan/www/TESTPLOTS/17_Mar_2022/timestamp_03_30_33_572167_2018_2e_80_100_-0.1_0.2/'
#extract all imformation
fulldict = {}
for dd in mass_list:
	path = plotpath + '/' + dd + '/'
	sys.path.append( path )
	files = glob.glob(path + '*.py')
	for f in files:	
		tmp_file = __import__(f.split('/')[-1].split('.')[0],globals(),locals(),['info'],-1)
		for p in tmp_file.info:
			fulldict[p] = tmp_file.info[p]	#all information here
t1 = time.clock()
	
pattern = year+'_(mc|data)_mass2e_(corr|uncorr|scaleup|scaledn|smearup|smeardn)_(eta|phi|pt)_neg_[0-9]_(median|mean|peak|sigma|sigmaerr|sd|peakerr|xposition|xpositionerr)$'#for electrons, merge + and -, but neg still used here
if fs == '2mu':
	pattern = year+'_(mc|data)_mass2mu_(ewk2|zpt_dt|dm_dt|ewk_dt)_(eta|phi|pt)_(neg|pos)_[0-9]_(median|mean|sigma|sigmaerr|sd|peak|peakerr|xposition|xpositionerr)$'

standard = 'mass'+fs+'_corr'

if fs == '2mu':
	standard = 'mass'+fs+'_ewk2'

varlist = ['mass2e_scaleup','mass2e_scaledn','mass2e_smearup','mass2e_smeardn']
if fs == '2mu':
	varlist = ['mass2mu_ewk_dt','mass2mu_zpt_dt','mass2mu_dm_dt']

kandc = ['eta_neg','phi_neg','pt_neg']	#kinematic and charge
if fs == '2mu':
	kandc = ['eta_neg','eta_pos','phi_neg','phi_pos','pt_pos','pt_neg']

#extract needed vars	
mydictclass = OneDimDictClass(fulldict,pattern)
datamc = {}
for d in ['mc','data']:
	datamc[d] = {}
	for m in mass_list:
		datamc[d][m] = {}
		for var in ['sd','sigma','sigmaerr','median','mean','peak','peakerr','xposition','xpositionerr']:
			datamc[d][m][var] = {}
			for c in kandc:
				if 'pt' in c:
					tmp = OneDimDictClass(mydictclass.result,year+'_'+d+'_'+m+'_'+c+'_[0-4]_'+var+'$')
				elif 'phi' in c:
					tmp = OneDimDictClass(mydictclass.result,year+'_'+d+'_'+m+'_'+c+'_[0-9]_'+var+'$')
				else:
					tmp = OneDimDictClass(mydictclass.result,year+'_'+d+'_'+m+'_'+c+'_[0-9]_'+var+'$')
				tmp.sortbykey()
				datamc[d][m][var][c] = tmp.result_sorted	#all useful information here, and sorted already

#evaluate erros
peak_var = {}
mean_var = {}
median_var = {}
sigma_var = {}
sd_var = {}
for ddd in ['mc','data']:
	peak_var[ddd] = {}
	mean_var[ddd] = {}
	median_var[ddd] = {}
	sigma_var[ddd] = {}
	sd_var[ddd] = {}
	for mmm in varlist:
		peak_var[ddd][mmm] = {}
		mean_var[ddd][mmm] = {}
		median_var[ddd][mmm] = {}
		sigma_var[ddd][mmm] = {}
		sd_var[ddd][mmm] = {}
		for ccc in kandc:
			peak_var[ddd][mmm][ccc] = ListDiff(datamc[ddd][mmm]['peak'][ccc],datamc[ddd][standard]['peak'][ccc]).result
			mean_var[ddd][mmm][ccc] = ListDiff(datamc[ddd][mmm]['mean'][ccc],datamc[ddd][standard]['mean'][ccc]).result
			median_var[ddd][mmm][ccc] = ListDiff(datamc[ddd][mmm]['median'][ccc],datamc[ddd][standard]['median'][ccc]).result
			sigma_var[ddd][mmm][ccc] = ListDiff(datamc[ddd][mmm]['sigma'][ccc],datamc[ddd][standard]['sigma'][ccc]).result
			sd_var[ddd][mmm][ccc] = ListDiff(datamc[ddd][mmm]['sd'][ccc],datamc[ddd][standard]['sd'][ccc]).result

peak_err = {}
mean_err = {}
median_err = {}
sigma_err = {}
sd_err = {}
for d_ in ['mc']:
	peak_err[d_] = {}
	mean_err[d_] = {}
	median_err[d_] = {}
	sigma_err[d_] = {}
	sd_err[d_] = {}
	for ud in ['up']:
		peak_err[d_][ud] = {}
		mean_err[d_][ud] = {}
		median_err[d_][ud] = {}
		sigma_err[d_][ud] = {}
		sd_err[d_][ud] = {}
		for k in kandc:
			if fs == '2e':
				peak_err[d_][ud][k] = ListQrtSum([peak_var[d_]['mass2e_scaleup'][k],peak_var[d_]['mass2e_smearup'][k],datamc['data']['mass2e_corr']['peakerr'][k],datamc[d_]['mass2e_corr']['peakerr'][k]]).result
				mean_err[d_][ud][k] = ListQrtSum([mean_var[d_]['mass2e_scaleup'][k],mean_var[d_]['mass2e_smearup'][k]]).result
				median_err[d_][ud][k] = ListQrtSum([median_var[d_]['mass2e_scaleup'][k],median_var[d_]['mass2e_smearup'][k]]).result
				#sigma_err[d_][ud][k] = ListQrtSum([sigma_var[d_]['mass2e_scaleup'][k],sigma_var[d_]['mass2e_smearup'][k],datamc['data']['mass2e_corr']['sigmaerr'][k],datamc[d_]['mass2e_corr']['sigmaerr'][k]]).result
				sigma_err[d_][ud][k] = ListQrtSum([sigma_var[d_]['mass2e_scaleup'][k],sigma_var[d_]['mass2e_smearup'][k]]).result
				sd_err[d_][ud][k] = ListQrtSum([sd_var[d_]['mass2e_scaleup'][k],sd_var[d_]['mass2e_smearup'][k]]).result
			if fs == '2mu':
				peak_err[d_][ud][k] = ListQrtSum([peak_var[d_]['mass2mu_ewk_dt'][k],peak_var[d_]['mass2mu_zpt_dt'][k],peak_var[d_]['mass2mu_dm_dt'][k],datamc[d_]['mass2mu_ewk2']['peakerr'][k]]).result
				mean_err[d_][ud][k] = ListQrtSum([mean_var[d_]['mass2mu_ewk_dt'][k],mean_var[d_]['mass2mu_zpt_dt'][k],mean_var[d_]['mass2mu_dm_dt'][k]]).result
				median_err[d_][ud][k] = ListQrtSum([median_var[d_]['mass2mu_ewk_dt'][k],median_var[d_]['mass2mu_zpt_dt'][k],median_var[d_]['mass2mu_dm_dt'][k]]).result
				sigma_err[d_][ud][k] = ListQrtSum([sigma_var[d_]['mass2mu_ewk_dt'][k],sigma_var[d_]['mass2mu_zpt_dt'][k],sigma_var[d_]['mass2mu_dm_dt'][k]]).result
				sd_err[d_][ud][k] = ListQrtSum([sd_var[d_]['mass2mu_ewk_dt'][k],sd_var[d_]['mass2mu_zpt_dt'][k],sd_var[d_]['mass2mu_dm_dt'][k]]).result

if fs == '2mu':
	#shift mc ewk2 
	for qwe in ['eta_pos','eta_neg','phi_neg','phi_pos','pt_pos','pt_neg']:
		for ewq in range(len(datamc['mc']['mass2mu_ewk2']['peak'][qwe])):
			datamc['mc']['mass2mu_ewk2']['peak'][qwe][ewq] = datamc['mc']['mass2mu_ewk2']['peak'][qwe][ewq] - 0.034
			datamc['mc']['mass2mu_ewk2']['mean'][qwe][ewq] = datamc['mc']['mass2mu_ewk2']['mean'][qwe][ewq] - 0.034
			datamc['mc']['mass2mu_ewk2']['median'][qwe][ewq] = datamc['mc']['mass2mu_ewk2']['median'][qwe][ewq] - 0.034
			#do not work for mean
		
#ratio bar
peak_bars = {}
mean_bars = {}
median_bars = {}
peak_barserr = {}
mean_barserr = {}
median_barserr = {}
sd_bars = {}
sd_barserr = {}
sigma_bars = {}
sigma_barserr = {}
for ii in kandc:
	peak_bars[ii] = RelDiffinPresentage(datamc['data'][standard]['peak'][ii],datamc['mc'][standard]['peak'][ii]).result
	mean_bars[ii] = RelDiffinPresentage(datamc['data'][standard]['mean'][ii],datamc['mc'][standard]['mean'][ii]).result
	median_bars[ii] = RelDiffinPresentage(datamc['data'][standard]['median'][ii],datamc['mc'][standard]['median'][ii]).result
	sd_bars[ii] = RelDiffinPresentage(datamc['data'][standard]['sd'][ii],datamc['mc'][standard]['sd'][ii]).result
	sigma_bars[ii] = RelDiffinPresentage(datamc['data'][standard]['sigma'][ii],datamc['mc'][standard]['sigma'][ii]).result
for iii in kandc:
	peak_barserr[iii] = Division(peak_err['mc']['up'][iii],datamc['mc'][standard]['peak'][iii]).result
	mean_barserr[iii] = Division(mean_err['mc']['up'][iii],datamc['mc'][standard]['mean'][iii]).result
	median_barserr[iii] = Division(mean_err['mc']['up'][iii],datamc['mc'][standard]['median'][iii]).result
	sigma_barserr[iii] = Division(sigma_err['mc']['up'][iii],datamc['mc'][standard]['sigma'][iii]).result
	sd_barserr[iii] = Division(sd_err['mc']['up'][iii],datamc['mc'][standard]['sd'][iii]).result
for iiii in peak_barserr:
	for jjjj in range(len(peak_barserr[iiii])):
		peak_barserr[iiii][jjjj] = peak_barserr[iiii][jjjj]*100
		mean_barserr[iiii][jjjj] = mean_barserr[iiii][jjjj]*100
		median_barserr[iiii][jjjj] = median_barserr[iiii][jjjj]*100
		sigma_barserr[iiii][jjjj] = sigma_barserr[iiii][jjjj]*100
		sd_barserr[iiii][jjjj] = sd_barserr[iiii][jjjj]*100

ytitle = 'Z_{ee}^{peak}'
if fs == '2mu':ytitle = 'Z_{\#mu#mu}^{peak}'
tex = ['e^{-}','e^{+}']
if fs == '2mu':tex = ['#mu^{-}','#mu^{+}']
mcconfig = {'title':'','ytitle':ytitle,'mksize':'1.5','mkstyle':'24','mkcolor':'4','xlabelsize':'0.04','xtitleoff':'0.5',
		'ytitleoff':'0.6','ytitlesize':'0.07','xtitlesize':'0.08'}
dtconfig = {'title':'','ytitle':ytitle,'mksize':'1','mkstyle':'20','mkcolor':'2','xlabelsize':'0.04','xtitleoff':'0.5',
		'ytitleoff':'0.6','ytitlesize':'0.07','xtitlesize':'0.08'}
barconfig = {'title':'','ytitle':'(data-mc)/mc(%)','mksize':'1','mkstyle':'21','mkcolor':'1','xlabelsize':'0.1','xtitleoff':'0.99','ylabelsize':'0.08',
		'ytitleoff':'0.45','ytitlesize':'0.1','xtitlesize':'0.13'}

for kkk in ['phi','eta','pt']:
	dummy = [0.0]*len(peak_err['mc']['up'][kkk+'_neg'])
	#mcconfig['xtitle'] = k
	#dtconfig['xtitle'] = k
	if kkk == 'eta':
		kk_ = '#'+kkk
	if kkk == 'pt':
		kk_ = 'p_{T}'
	if kkk == 'phi':
		kk_ = 'R^{lep1+lep2}_{9}'
	barconfig['xtitle'] = kk_
	mcconfig['xtitle'] = kk_
	dtconfig['xtitle'] = kk_
	arrange = '2r'
	if kkk == 'pt': arrange = '2c'
	if fs == '2mu':
		mcconfig['ytitle'] = 'Z_{#mu#mu}^{peak}'
		dtconfig['ytitle'] = 'Z_{#mu#mu}^{peak}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['peak'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],peak_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['peak'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g	
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_pos'],datamc['mc'][standard]['peak'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],peak_err['mc']['up'][kkk+'_pos'],mcconfig).g
		obj4 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_pos'],datamc['data'][standard]['peak'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		tmp = ScatterPlot2([obj1,obj2,obj3,obj4],['mc','data'],tex,arrange,plotpath+'peak_'+kkk)

		mcconfig['ytitle'] = 'Z_{#mu#mu}^{mean}'
		dtconfig['ytitle'] = 'Z_{#mu#mu}^{mean}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['mean'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],mean_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['mean'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g	
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_pos'],datamc['mc'][standard]['mean'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],mean_err['mc']['up'][kkk+'_pos'],mcconfig).g
		obj4 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_pos'],datamc['data'][standard]['mean'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		tmp = ScatterPlot2([obj1,obj2,obj3,obj4],['mc','data'],tex,arrange,plotpath+'mean_'+kkk)

		mcconfig['ytitle'] = 'Z_{#mu#mu}^{median}'
		dtconfig['ytitle'] = 'Z_{#mu#mu}^{median}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['median'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],median_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['median'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g	
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_pos'],datamc['mc'][standard]['median'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],median_err['mc']['up'][kkk+'_pos'],mcconfig).g
		obj4 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_pos'],datamc['data'][standard]['median'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		tmp = ScatterPlot2([obj1,obj2,obj3,obj4],['mc','data'],tex,arrange,plotpath+'median_'+kkk)

		mcconfig['ytitle'] = 'Z_{#mu#mu}^{#sigma}^{fit}'
		dtconfig['ytitle'] = 'Z_{#mu#mu}^{#sigma}^{fit}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['sigma'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],sigma_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['sigma'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g	
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_pos'],datamc['mc'][standard]['sigma'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],sigma_err['mc']['up'][kkk+'_pos'],mcconfig).g
		obj4 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_pos'],datamc['data'][standard]['sigma'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		tmp = ScatterPlot2([obj1,obj2,obj3,obj4],['mc','data'],tex,arrange,plotpath+'sigma_'+kkk)
		
		mcconfig['ytitle'] = 'Z_{#mu#mu}^{#sigma}'
		dtconfig['ytitle'] = 'Z_{#mu#mu}^{#sigma}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['sd'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],sd_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['sd'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g	
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_pos'],datamc['mc'][standard]['sd'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],sd_err['mc']['up'][kkk+'_pos'],mcconfig).g
		obj4 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_pos'],datamc['data'][standard]['sd'][kkk+'_pos'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		tmp = ScatterPlot2([obj1,obj2,obj3,obj4],['mc','data'],tex,arrange,plotpath+'sd_'+kkk)

	
	if fs == '2e':
		mcconfig['ytitle'] = 'Z_{ee}^{mass}'
		dtconfig['ytitle'] = 'Z_{ee}^{mass}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['peak'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],peak_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['peak'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],peak_bars[kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],peak_barserr[kkk+'_neg'],barconfig).g
		obj3.GetXaxis().CenterTitle(True)
		tmp = ScatterPlot1([obj1,obj2,obj3],['MC','DATA'],['-'],plotpath+'peak_'+kkk,year)
		
		mcconfig['ytitle'] = 'Z_{ee}^{mean}'
		dtconfig['ytitle'] = 'Z_{ee}^{mean}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['mean'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],mean_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['mean'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],mean_bars[kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],mean_barserr[kkk+'_neg'],barconfig).g
		obj3.GetXaxis().CenterTitle(True)
		tmp = ScatterPlot1([obj1,obj2,obj3],['mc','data'],['-'],plotpath+'mean_'+kkk,year)

		mcconfig['ytitle'] = 'Z_{ee}^{median}'
		dtconfig['ytitle'] = 'Z_{ee}^{median}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['median'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],median_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['median'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],median_bars[kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],median_barserr[kkk+'_neg'],barconfig).g
		obj3.GetXaxis().CenterTitle(True)
		tmp = ScatterPlot1([obj1,obj2,obj3],['mc','data'],['-'],plotpath+'median_'+kkk,year)

		mcconfig['ytitle'] = 'Z_{ee}^{#sigma}'
		dtconfig['ytitle'] = 'Z_{ee}^{#sigma}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['sigma'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],sigma_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['sigma'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],sigma_bars[kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],sigma_barserr[kkk+'_neg'],barconfig).g
		obj3.GetXaxis().CenterTitle(True)
		tmp = ScatterPlot1([obj1,obj2,obj3],['MC','DATA'],['-'],plotpath+'sigma_'+kkk,year)

		mcconfig['ytitle'] = 'Z_{ee}^{#sigma}'
		dtconfig['ytitle'] = 'Z_{ee}^{#sigma}'
		obj1 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],datamc['mc'][standard]['sd'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],sd_err['mc']['up'][kkk+'_neg'],mcconfig).g
		obj2 = ListToGraph(datamc['data'][standard]['xposition'][kkk+'_neg'],datamc['data'][standard]['sd'][kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],dummy,dtconfig).g
		obj3 = ListToGraph(datamc['mc'][standard]['xposition'][kkk+'_neg'],sd_bars[kkk+'_neg'],datamc['mc'][standard]['xpositionerr'][kkk+'_neg'],sd_barserr[kkk+'_neg'],barconfig).g
		obj3.GetXaxis().CenterTitle(True)
		tmp = ScatterPlot1([obj1,obj2,obj3],['mc','data'],['-'],plotpath+'sd_'+kkk,year)

t2 = time.clock()
print t1-t0, t2-t1
if fs == '2e':
	os.system('python datavsmc.py '+plotpath+' '+str(low)+' '+str(high))

#ptdependent lut
#mklut(datamc['mc'][standard]['xposition']['pt_neg'],datamc['mc'][standard]['xpositionerr']['pt_neg'],median_bars['pt_neg'],plotpath,'median_pt')
#mklut(datamc['mc'][standard]['xposition']['pt_neg'],datamc['mc'][standard]['xpositionerr']['pt_neg'],sd_bars['pt_neg'],plotpath,'sd_pt')
#mklut(datamc['mc'][standard]['xposition']['pt_neg'],datamc['mc'][standard]['xpositionerr']['pt_neg'],peak_bars['pt_neg'],plotpath,'peak_pt')
#mklut(datamc['mc'][standard]['xposition']['pt_neg'],datamc['mc'][standard]['xpositionerr']['pt_neg'],sigma_bars['pt_neg'],plotpath,'sigma_pt')
