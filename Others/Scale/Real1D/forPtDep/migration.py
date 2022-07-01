import os
import sys
sys.path.append('/afs/cern.ch/work/c/chenguan/private/')
from Table import *

def mktb(row,type_,what):
	
	tb = Table(type_+'_'+what)
	first = []
	if not 'ratio' in what:
		first.append(type_+'_'+what)
	else:
		first.append(type_+'_entries(%)')
	for i in range(len(row)):
		first.append(type_+'_gen-bin'+str(i))
	tb.sttable('left',first)
	for i in range(len(row)):
		tb.fillrow(row[i])
	tb.endtable()
	tb.endfile()
		
	with open(path+'/'+type_+'_'+what+'.html','w') as f:
		f.write(tb.substr)

	
path = sys.argv[1]
what = sys.argv[2]
sys.path.append(path)

typelist = ['pt_pos','pt_neg','eta_pos','eta_neg','phi_pos','phi_neg']
for i in typelist:
	dic_tmp = __import__(i ,globals(),locals(),['info'],-1)
	rows = []
	k=8
	if 'pt' in i:
		k=6
	for n in range(k):
		arow = []
		arow.append(i+'_reco-bin'+str(n))
		for m in range(k):
			arow.append(round(dic_tmp.info[i+'_'+what+'_'+str(n)+'_'+str(m)],3))
		rows.append(arow)
	mktb(rows,i,what)
	if what == 'entries':
		for j in rows:
			a = 0
			for k in range(1,len(j)):
				a +=j[k]
			for k in range(1,len(j)):
				j[k] = round(j[k]/a*100,3)
		mktb(rows,i,what+'_ratio')
