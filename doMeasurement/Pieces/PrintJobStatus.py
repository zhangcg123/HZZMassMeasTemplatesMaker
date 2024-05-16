import sys
import os
import glob
var = os.popen('condor_q').read()
status_list = var.split('\n')
status_list_new = []
for i in range(10):
	print ''
for i in status_list:
	if i.startswith('chenguan'):
		status_list_new.append(i)
for i in status_list_new:
	onejob = {}
	onejob['owner'] = i.split()[0]
	onejob['batch'] = i.split()[2]
	onejob['done'] = i.split()[5]
	onejob['run'] = i.split()[6]
	onejob['idel'] = i.split()[7]
	onejob['total'] = i.split()[8]
		
	dirlist = glob.glob(os.getcwd()+'/201*')
	for f in dirlist:
		filelist = glob.glob(f+'/*condor.*.log')
		for t in filelist:
			if onejob['batch'] in t:
				fsname = t.split('/')[-1].split('_')[2]
				syst = t.split('/')[-1].split('_')[14]
				seed = t.split('/')[-1].split('_')[13]
				floatothers = t.split('/')[-1].split('_')[5]
				model = t.split('/')[-1].split('_')[4]
				print t.split('/')[-2], fsname, model, floatothers, syst, seed, 'done/run:		'+onejob['done']+'/'+onejob['run']
				break
for i in range(10):
	print ''	
