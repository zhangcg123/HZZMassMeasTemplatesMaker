import glob
import json
import ROOT
import pandas
import commands
def reader( name_mask, yy, fs):
	files = glob.glob('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/datacardsOutputs_KDTrue_testnew_setBins700setBins98kcache/'+name_mask[0]+'/'+yy+name_mask[1]+fs+'/fitDiag*root')[0]
	f = ROOT.TFile.Open(files,'r')
	fitresult = f.Get('fit_s')
	r_value = fitresult.floatParsFinal().find('r').getVal()
	pre_norm = f.Get('norm_prefit')
	pos_norm = f.Get('norm_fit_s')
	names = [ a.GetName().split('/')[-1] for a in pre_norm ]
	names.extend(['sig','bkg','tot','obs','(obs-bkg)/sig','r'])
		
	pre_proc = [ a.getVal() for a in pre_norm ]
	pre_sig = sum([ a.getVal() for a in pre_norm if a.GetName().endswith('hzz')])
	pre_bkg = sum([ a.getVal() for a in pre_norm if a.GetName().split('/')[-1].startswith('bkg')])
	pre_tot = pre_sig + pre_bkg
	
	pos_proc = [ a.getVal() for a in pos_norm ]
	pos_sig = sum([ a.getVal() for a in pos_norm if a.GetName().endswith('hzz')]) 
	pos_bkg = sum([ a.getVal() for a in pos_norm if a.GetName().split('/')[-1].startswith('bkg')]) 
	pos_tot = pos_sig + pos_bkg 

	pre_proc.extend([ pre_sig, pre_bkg, pre_tot ])
	pos_proc.extend([ pos_sig, pos_bkg, pos_tot ])
	
	
	obs = 0	
	files = glob.glob('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/datacardsOutputs_KDTrue_testnew_setBins700setBins98kcache/'+name_mask[0]+'/'+yy+name_mask[1]+fs+'/hzz4l_'+yy+'_'+fs+'_allcategs_13TeV.txt')[0]
	obs = int(commands.getstatusoutput('cat '+files+' | grep observation')[1].rstrip(' ').split(' ')[1])
	pre_proc.extend([ obs, (obs-pre_bkg)/pre_sig, 1])
	pos_proc.extend([ obs, (obs-pos_bkg)/pos_sig, r_value])

	ratio = [ (pos_proc[i]/pre_proc[i]) for i in range(len(pre_proc)) ]

	pd = pandas.DataFrame({'name':names,'pre-fit':pre_proc,'pos-fit':pos_proc, 'ratio(pos/pre)':ratio})	
	return pd[['name','pre-fit','pos-fit','ratio(pos/pre)']]
	
	

def hook( masks, results):
	for yy in ['20160','20165','2017','2018']:
		for fs in ['4mu','4e','2e2mu','2mu2e']:
			reco = reader( masks[0], yy, fs )
			bsrefit = reader( masks[1], yy, fs )
			result = pandas.concat( [reco,bsrefit], axis=1)
			print '\n\n\n'
			print yy, fs
			print result

hook( [['FitDiag_RECO_1D_1Categ','_reco_m_1_notag_widthpdf_'],['FitDiag_BSREFIT_1D_1Categ','_bsrefit_m_1_notag_widthpdf_']], {} )
