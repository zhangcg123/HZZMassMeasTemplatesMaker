import os
import sys
import glob

def combcards(f_):	
	fs_all = glob.glob(f_+'/*allfinalstates_allcategs.txt')		
	fs_4mu = glob.glob(f_+'/*4mu_allcategs.txt')
	fs_4e = glob.glob(f_+'/*4e_allcategs.txt')
	fs_2e2mu = glob.glob(f_+'/*2e2mu_allcategs.txt')
	fs_2mu2e = glob.glob(f_+'/*2mu2e_allcategs.txt')
	file_list = {'run2_all':fs_all,'run2_4mu':fs_4mu,'run2_4e':fs_4e,'run2_2e2mu':fs_2e2mu,'run2_2mu2e':fs_2mu2e}
	for fs in file_list:
		string = 'combineCards.py '
		for f in file_list[fs]:
			string = string + f + ' '
		string = string + '> ' + f_ + '/'+fs+'.txt'
		print string
		os.system(string)
'''
for d in ['reco_m_1_notag_widthpdf','bs_m_1_notag_widthpdf','bsrefit_m_1_notag_widthpdf','bsrefit_m_9_notag_widthpdf',
	'bsrefit_mkd_9_notag_widthpdf']:
	if os.path.exists('run2_'+d):
		continue
	else:
		os.system('mkdir run2_'+d)
		for year in ['20160','20165','2017','2018']:
			os.system('cp -r '+year+'_'+d+'/condor_inputfiles/*root run2_'+d+'/')
			os.system('cp -r '+year+'_'+d+'/condor_inputfiles/hzz4l_'+year+'_*_allcategs_13TeV.txt run2_'+d+'/')
'''
#files=glob.glob('run2_*widthpdf')
#for f in files:
#	combcards(f)
fs_all = glob.glob('*allfinalstates_allcategs_13TeV.txt')		
fs_4mu = glob.glob('*4mu_allcategs_13TeV.txt')
fs_4e = glob.glob('*4e_allcategs_13TeV.txt')
fs_2e2mu = glob.glob('*2e2mu_allcategs_13TeV.txt')
fs_2mu2e = glob.glob('*2mu2e_allcategs_13TeV.txt')
file_list = {'run2_hzz_all':fs_all,'run2_hzz_4mu':fs_4mu,'run2_hzz_4e':fs_4e,'run2_hzz_2e2mu':fs_2e2mu,'run2_hzz_2mu2e':fs_2mu2e}
for fs in file_list:
	string = 'combineCards.py '
	for f in file_list[fs]:
		string = string + f + ' '
	string = string + ' > ' + fs + '_13TeV.txt'
	print string
	os.system(string)
