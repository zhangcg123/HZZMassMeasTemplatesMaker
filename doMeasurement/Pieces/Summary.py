import os
import sys
import json
import glob
import pandas as pd
import numpy as np
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/pytotextopdf/')
from autolatex import *

def tabs(match, suffix):
	
	files = glob.glob('/eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+suffix+'/JSON/*'+match+'notag/*params.json')
	all_info = {}
	for f in files:
		with open(f,'r') as p:
			tmp_info = json.load(p)
		all_info.update(tmp_info)
	
	trash = []
	for e in all_info:
		if 'abs_sigrate_' in e or 'bkgrate_' in e:continue
		trash.append(e)
	for e in trash:
		del all_info[e]		#all rates info
	
	ps = ['ggh','vbf','wph','wmh','zh','tth','bbh','thq','qqzz','ggzz','zjets']
	fs = ['4mu','4e','2e2mu','2mu2e']
	ys = ['20160','20165','2017','2018']
	cs = []
	if '9categs' in t:
		for i in range(0,9):
			cs.append(str(i))
	if '1categs' in t:
		cs.append(0)

	#initialize the table
	final = {}
	for c in cs:
		final[c] = {}
		for y in ys:
			final[c][y] = {}
			for f in fs:
				final[c][y][f] = []

	#fill the table
	for c in cs:
		for y in ys:
			for f in fs:
				for p in ps:
					for name in all_info:
						if '_'+p+'_'+y+'_'+f+'_'+str(c)+'_notag' in name:
							final[c][y][f].append(round(all_info[name],2))
		
	#to pandas dataframe
	string = ''
	results = []
	for c in cs:
		ds = []
		for y in ys:
			for f in fs:
				temp = pd.DataFrame({y+'_'+str(c)+'_'+f:np.array(final[c][y][f])},index=ps)
				temp.loc['tot. exp.'] = temp.sum(axis=0,numeric_only=True)
				ds.append(temp)
	
		result = pd.concat(ds,1)
		result.loc[:,'tot. exp.'] = result.sum(axis=1,numeric_only=True)	
		results.append('\n' + result.to_latex() + '\n')
	
	return results

suffix = sys.argv[1]
path = '/eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+suffix+'/'
myclass = autolatexclass(path,'hig-21-019-summary','hig-21-019-summary','2022','article')
#target file
suffix = sys.argv[1]
for t in ['_1categs_reco_','_1categs_bs_','_1categs_bsrefit_','_9categs_bsrefit_']:
	myclass.section('',t.replace('_',' ') ,'')
	myclass.section('sub','event yields','')
	tbs = tabs(t,suffix)
	for i in tbs:
		myclass.table(i)
	myclass.section('sub','mass distributions','')
	myclass.section('sub','xxx','')
myclass.end()

