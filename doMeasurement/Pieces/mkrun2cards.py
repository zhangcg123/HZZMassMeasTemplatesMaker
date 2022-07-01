import os
import sys
import glob
for d in ['reco_m_1_notag_widthmeas_fft','reco_m_9_notag_widthmeas_fft','reco_mkd_1_notag_widthmeas_fft','reco_mkd_9_notag_widthmeas_fft',
	'refit_m_1_notag_widthmeas_fft','refit_m_9_notag_widthmeas_fft','refit_mkd_1_notag_widthmeas_fft','refit_mkd_9_notag_widthmeas_fft']:
	os.system('mkdir run2_'+d)
	for year in ['2016','2017','2018']:
		os.system('cp -r '+year+'_'+d+'/condor_inputfiles/*root run2_'+d+'/')
		os.system('cp -r '+year+'_'+d+'/condor_inputfiles/hzz4l_'+year+'_allfinalstates_allcategs.txt run2_'+d+'/')
