import os
import sys
sys.path.append('/afs/cern.ch/work/c/chenguan/private/')
from HTMLClass import *
import json
year = sys.argv[1]
seed = sys.argv[2]
os.system("jq -rs 'reduce .[] as $item ({}; . * $item)' ./"+year+"*withsyst_"+seed+".json > results/"+year+"_withsyst_"+seed+"_summary.json")
os.system("jq -rs 'reduce .[] as $item ({}; . * $item)' ./"+year+"*withoutsyst_"+seed+".json > results/"+year+"_withoutsyst_"+seed+"_summary.json")
with open("./results/"+year+"_withsyst_"+seed+"_summary.json",'r') as fwithsyst:
	dics_withsyst = json.load(fwithsyst)
fwithsyst.close()
with open("./results/"+year+"_withoutsyst_"+seed+"_summary.json",'r') as fwithoutsyst:
	dics_withoutsyst = json.load(fwithoutsyst)
fwithoutsyst.close()
type_ = ['reco','refit','bs','bsrefit']
kd = ['m','mkd']
cat = ['1','9']
list_s = []
for t in type_:
	for k in kd:
		for c in cat:
			list_ = []
			key = year+'_'+t+'_'+k+'_'+c
			list_.append(key)
			list_.append(dics_withsyst[key]['center'])
			list_.append(dics_wihtoutsyst[key]['center'])
			list_.append(dics_withsyst[key]['onesig'])
			list_.append(dics_withoutsyst[key]['onesig'])
			list_.append(sqrt(dics_withsyst[key]['onesig']*dics_withsyst[key]['onesig']-dics_withoutsyst[key]['onesig']*dics_withoutsyst[key]['onesig']))
			list_s.append(list_)
myhtml = HTMLClass(year+' '+seed)
myhtml.sttable('300px',['center_stat+syst','center_stat','cl68_stat+syst','cl68_stat','syst'])
for i in list_s:
	myhtml.fillrow(i)
myhtml.endtable()
myhtml.endfile()
string = myhtml.check()
f = open('./results/'+year+'_'+seed+'.html','w')
f.write(string)
f.close()
