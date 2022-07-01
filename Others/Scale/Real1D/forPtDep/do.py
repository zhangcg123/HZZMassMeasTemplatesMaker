import os
import sys
from multiprocessing import Pool
from datetime import datetime
sys.path.append('/afs/cern.ch/work/c/chenguan/private/')
from FileSystemClass import *

def multirun(year, mass_name, low, high, init_mean, buffer_):
	
	isdata = mass_name.split('_')[-1]
	if 'mc' in mass_name:
		mass_name_ = mass_name[:-3]
	if 'data' in mass_name:
		mass_name_ = mass_name[:-5]
	command = './ReadTree.exe '+year+' '+isdata+' '+mass_name_+' '+low+' '+high+' '+init_mean+' '+buffer_+' '+plotpath+'/'+mass_name+'/'
	os.system(command)

year = sys.argv[1]
low = sys.argv[2]
high = sys.argv[3]
init_mean = sys.argv[4]
buffer_ = sys.argv[5]
mass_list = ['mass2mu_corr_mc']#,'mass2mu_corr_data']

dirclass = DirTree()
dirclass.mkrootdir(year+'_ewk2asdef_'+low+'_'+high+'_'+init_mean+'_'+buffer_)
for name in mass_list:
	dirclass.mksubdir(name)
	dirclass.mksubdir(name+'/fitplots/')

	k_list = ['eta','phi','pt']
	for i in k_list:
		dirclass.mksubdir(name+'/fitplots/'+i)
		
	subsubdir_list = ['mass_distributions','fix_pt_distributions','free_pt_distributions','pterr_distributions','pterrold_distributions','pull_distributions','migration']
	for i in subsubdir_list:
		dirclass.mksubdir(name+'/'+i)
		for j in ['_1-5bin','_neg_edge_vs_middle','_pos_edge_vs_middle','_pos_vs_neg','_datavsmc']:
			for j_ in ['pt','eta','phi']:
				dirclass.mksubdir(name+'/'+i+'/'+j_+j)
descrip = 'muon scale: mc->bin gen fill gen'
dirclass.mkdescrip(descrip)

plotpath = dirclass.root
p=Pool(1)
for i in mass_list:
	p.apply_async(func=multirun,args=(year,i,low,high,init_mean,buffer_,))
p.close()
p.join()
#os.system('python datavsmc.py '+plotpath+'/mass2mu_ewk2_mc/ '+plotpath+'/mass2mu_ewk2_data/')
os.system('python closureplots.py '+year+' '+low+' '+high+' '+init_mean+' '+buffer_+' '+plotpath)
