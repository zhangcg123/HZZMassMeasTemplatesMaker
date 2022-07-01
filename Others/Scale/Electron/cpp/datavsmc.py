import ROOT
import os
import sys
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from PlotClass import *
import glob
import math
import numpy
ROOT.gROOT.SetBatch( 1 )
path = sys.argv[1]
low = float(sys.argv[2])
high = float(sys.argv[3])

for i in ['mass2e_corr','mass2e_uncorr']:	
	for j in ['eta','phi','pt']:
		
		path_var = path + '/' + i + '/mass_distributions/' + j + '/'
		root_lista = glob.glob(path_var +'/mc*root')
		root_listb = glob.glob(path_var +'/data*root')
		histo_lista = {}		#mc
		histo_listb = {}		#data
		histo_listup = {}
		for a in root_lista:		#mc
			f = ROOT.TFile.Open(a,'r')
			keylist = f.GetListOfKeys()
			obj = keylist[0].ReadObj()
			obj.SetDirectory(0)
			obj.SetName(keylist[0].GetName())
			histo_lista[a.split('/')[-1].split('.')[0]] = obj
			f.Close()
		
		for b in root_listb:		#data
			f = ROOT.TFile.Open(b,'r')
			keylist = f.GetListOfKeys()
			obj = keylist[0].ReadObj()
			obj.SetDirectory(0)
			obj.SetName(keylist[0].GetName())
			histo_listb[b.split('/')[-1].split('.')[0]] = obj
			f.Close()

		#uncertainties
		tot_diff = []
		xs = []
		ys = []
		dummy = []
		for bins in range(histo_lista['mc_'+j+'_neg_0'].GetNbinsX()):
			tot_diff.append(0)
			dummy.append(0.0)
			ys.append(1.0)
			xs.append(histo_lista['mc_'+j+'_neg_0'].GetBinCenter(bins+1))	
		tot_diffs = {}
		for kname in histo_lista:
			tot_diffs[kname] = tot_diff

		if i == 'mass2e_corr' or i == 'mass2e_uncorr':
			scaleup_path = path + '/mass2e_scaleup/mass_distributions/' + j + '/'
			scaledn_path = path + '/mass2e_scaledn/mass_distributions/' + j + '/'
			smearup_path = path + '/mass2e_smearup/mass_distributions/' + j + '/'
			smeardn_path = path + '/mass2e_smeardn/mass_distributions/' + j + '/'
			scup_files = glob.glob(scaleup_path + '/mc*root')
			scdn_files = glob.glob(scaledn_path + '/mc*root')
			smup_files = glob.glob(smearup_path + '/mc*root')
			smdn_files = glob.glob(smeardn_path + '/mc*root')
			files_listvar = [scup_files, scdn_files, smup_files, smdn_files]
			
			histo_scaleup = {}
			histo_scaledn = {}
			histo_smearup = {}
			histo_smeardn = {}
			histo_listvar = [histo_scaleup, histo_scaledn, histo_smearup, histo_smeardn]
	
			for file_list_n in range(len(files_listvar)):
				for file_n in files_listvar[file_list_n]:
					f = ROOT.TFile.Open(file_n,'r')
					keylist = f.GetListOfKeys()
					obj = keylist[0].ReadObj()
					obj.SetDirectory(0)
					obj.SetName(keylist[0].GetName())
					histo_listvar[file_list_n][file_n.split('/')[-1].split('.')[0]] = obj

			scup_diffs = {}
			scdn_diffs = {}
			smup_diffs = {}
			smdn_diffs = {}
			stat_diffs = {}
			def_diffs = {}
			for p in range(len(histo_listvar)):						#variations
				for q in histo_listvar[p]:						#kinematic bins for a specific variation
					for abc in histo_lista:						#kinematic bins for default
						if abc.split('_')[-1] == q.split('_')[-1]:		#match kinematic bin
							scup_diff = []
							scdn_diff = []
							smup_diff = []
							smdn_diff = []
							stat_diff = []
							def_diff = []
							a_tmp = histo_lista[abc].Clone()
							#b_tmp = histo_listb[abc.replace('mc','data')].Clone()
							#a_tmp.Divide(b_tmp)
							j_tmp = histo_listvar[p][q].Clone()
							for n in range(1,a_tmp.GetNbinsX()+1):
								if p == 0: 
									stat_diff.append(a_tmp.GetBinError(n))
									stat_diffs[q] = stat_diff
								if p == 0: 
									def_diff.append(a_tmp.GetBinContent(n))
									def_diffs[q] = def_diff
								if p == 1: 
									scup_diff.append(abs(a_tmp.GetBinContent(n)-j_tmp.GetBinContent(n)))
									scup_diffs[q] = scup_diff
								if p == 2: 
									scdn_diff.append(abs(a_tmp.GetBinContent(n)-j_tmp.GetBinContent(n)))
									scdn_diffs[q] = scdn_diff
								if p == 3: 
									smup_diff.append(abs(a_tmp.GetBinContent(n)-j_tmp.GetBinContent(n)))
									smup_diffs[q] = smup_diff
								if p == 4: 
									smdn_diff.append(abs(a_tmp.GetBinContent(n)-j_tmp.GetBinContent(n)))
									smdn_diffs[q] = smdn_diff
			
			for aa in stat_diffs:
				for dd in tot_diffs:
					if aa == dd:
						for kk in range(len(stat_diffs[aa])):
							if def_diffs[dd][kk]==0:
								def_diffs[dd][kk]=0.00000001
							tot_diffs[dd][kk] = math.sqrt(stat_diffs[aa][kk]*stat_diffs[aa][kk]+scup_diffs[aa][kk]*scup_diffs[aa][kk]+smup_diffs[aa][kk]*smup_diffs[aa][kk])/def_diffs[dd][kk]
							#print stat_diffs[aa][kk], scup_diffs[aa][kk], smup_diffs[aa][kk], tot_diffs[aa][kk]	
			for qqq in histo_lista:
				for ppp in tot_diffs:
					if qqq == ppp:
						histo_up = ROOT.TGraphErrors(len(xs), numpy.array(xs), numpy.array(ys), numpy.array(dummy), numpy.array(tot_diffs[ppp]))
						histo_up.GetXaxis().SetRangeUser(low,high)
						histo_listup[qqq] = histo_up

		for aaa in histo_lista:
			for bbb in histo_listb:
				if aaa.split('_')[-1] == bbb.split('_')[-1]:	#normaly the histogram name looks like this data_phi_neg_1
					a_tmp = histo_lista[aaa].Clone()
					ratio = histo_listb[bbb].Clone()
					b_tmp = histo_listb[bbb].Clone()
					aup_tmp = histo_listup[aaa].Clone()
					mc_mean = a_tmp.GetMean()
					mc_sigma = a_tmp.GetStdDev()
					data_mean = b_tmp.GetMean()
					data_sigma = b_tmp.GetStdDev()
					mean_mc_vs_data = data_mean/mc_mean
					sigma_mc_vs_data = data_sigma/mc_sigma
					tex = ['MC:Mean '+str(round(mc_mean,4)),'MC:#sigma '+str(round(mc_sigma,4)),'Data:Mean '+str(round(data_mean,4)),'Data:#sigma '+str(round(data_sigma,4)),'Data/MC:Mean '+str(round(mean_mc_vs_data,4)),'Data/MC:#sigma '+str(round(sigma_mc_vs_data,4))]
					#a_tmp.Rebin(5)
					#ratio.Rebin(5)
					#b_tmp.Rebin(5)
					ratio.Divide(a_tmp)
					Plot = HistogramPlot2([a_tmp,b_tmp,ratio,aup_tmp],['MC','DATA','DATA/MC'],tex,path_var + '/data_vs_mc_' + aaa,i )
