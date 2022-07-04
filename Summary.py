import os
import sys
import json
import glob

file = sys.argv[1]
files = glob.glob('/eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+file+'/JSON/*_1categs_reco_notag/*params.json')

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
	del all_info[e]

ps = ['ggh','vbf','wph','wmh','zh','tth','bbh','thq','qqzz','ggzz','zjets']
fs = ['4mu','4e','2e2mu','2mu2e']
ys = ['20160','20165','2017','2018']
cs = ['_0_']

#initialize the table
final = {}
for y in ys:
	final[y] = {}
	for p in ps:
		final[y][p] = {}
		for f in fs:
			final[y][p][f] = -1
#fill the table
for y in final:
	for p in final[y]:
		for f in final[y][p]:
			for name in all_info:
				if '_'+p+'_'+y+'_'+f+'_0_notag' in name:
					final[y][p][f] = all_info[name]

sig = 0
bkg = 0
qqzz = 0
ggzz = 0
zjets = 0
for y in final:
	for p in final[y]:
		for f in final[y][p]:
			if p in ['ggh','vbf','zh','wph','wmh','tth','bbh','thq']:
				sig = sig + final[y][p][f]
			if p in ['qqzz','ggzz','zjets']:
				bkg = bkg + final[y][p][f]
			if p == 'qqzz': qqzz = qqzz + final[y][p][f]
			if p == 'ggzz': ggzz = ggzz + final[y][p][f]
			if p == 'zjets': zjets = zjets + final[y][p][f]
print 'sig:',sig
print 'bkg:',bkg
print 'qqzz', qqzz
print 'ggzz', ggzz
print 'zjets', zjets
