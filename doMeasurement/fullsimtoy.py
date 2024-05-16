from multiprocessing import Pool
from multiprocessing import Manager
import commands
import glob
import time
import json
import ROOT
import sys
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from DictClass import *
import os
import re

def formulatofloat( proc ):

	for i in proc:
		tmp = proc[i]
		if isinstance(tmp,basestring):
			m = ROOT.RooRealVar('m','',125.0)
			r = ROOT.RooFormulaVar('tmp',tmp,ROOT.RooArgList(m))
			proc[i] = r.getVal()
	
def evaluator( proc ):
	
	yields = {'sig':0.0,'rbkg':0.0,'irrbkg':0.0}	
	yields['sig'] = sum(OneDimDictClass( proc, '^sigrate.*').result.values())
	yields['irrbkg'] = sum(OneDimDictClass( proc, '^bkgrate_(qq|gg)zz.*').result.values())
	yields['rbkg'] = sum(OneDimDictClass( proc, '^bkgrate_zjets.*').result.values())
	
	return yields

def run( command ):
	print 'a sampling prosses was launched ...'
	os.system( command )


def mergers(filename, subdir):
	eos = '/eos/user/c/chenguan/fullsimtoys/ftr/'+sub+'/'	
	fin = ROOT.TFile(filename,'READ')
	w = fin.Get('w').Clone()
	fin.Close()
	
	f_tmp = filename
	if 'masspdf/' in f_tmp: 
		f_tmp = f_tmp.replace('masspdf/','widthpdf/')
	if '_m_' in f_tmp:
		f_tmp = f_tmp.replace('_m_','_mkd_')
	f = f_tmp.split('/')[-1].split('_')[3][-1]		#f = category index
	
	sigin = ROOT.TFile(f_tmp.replace(basedir+'_fullsimtoy',eos).replace('input',subdir.split('_')[1]+'_sig_toys'),'READ')
	rbkgin = ROOT.TFile(f_tmp.replace(basedir+'_fullsimtoy',eos).replace('input',subdir.split('_')[1]+'_rbkg_toys'),'READ')
	irrbkgin = ROOT.TFile(f_tmp.replace(basedir+'_fullsimtoy',eos).replace('input',subdir.split('_')[1]+'_irrbkg_toys'),'READ')	
	
	ntoys = 0
	for key in sigin.GetListOfKeys():
		#if ntoys>10:break
		cl = ROOT.gROOT.GetClass(key.GetClassName())
		if (not cl.InheritsFrom("RooDataSet")): continue;
		ntoys+=1;
		
	
	d_sig_tot = sigin.Get('toy_cat'+str(f)+'_0')
	d_rbkg_tot = rbkgin.Get('toy_cat'+str(f)+'_0')
	d_irrbkg_tot = irrbkgin.Get('toy_cat'+str(f)+'_0')
	
	#d_aninclusive = sigin.Get('toy_cat'+str(f)+'_0').Clone('toy_0')
	#d_aninclusive.append(rbkgin.Get('toy_cat'+str(f)+'_0'))
	#d_aninclusive.append(irrbkgin.Get('toy_cat'+str(f)+'_0'))
	#getattr(w, 'import')(d_aninclusive)
	for i in range(1,ntoys):
		d_sig = sigin.Get('toy_cat'+str(f)+'_'+str(i))
		d_rbkg = rbkgin.Get('toy_cat'+str(f)+'_'+str(i))
		d_irrbkg = irrbkgin.Get('toy_cat'+str(f)+'_'+str(i))
		d_sig_tot.append(d_sig)
		d_rbkg_tot.append(d_rbkg)
		d_irrbkg_tot.append(d_irrbkg)

		#d_aninclusive = sigin.Get('toy_cat'+str(f)+'_'+str(i)).Clone('toy_'+str(i))
		#d_aninclusive.append(rbkgin.Get('toy_cat'+str(f)+'_'+str(i)))
		#d_aninclusive.append(irrbkgin.Get('toy_cat'+str(f)+'_'+str(i)))
		#getattr(w, 'import')(d_aninclusive)

	sigin.Close()
	rbkgin.Close()
	irrbkgin.Close()

	d_tot = d_sig_tot.Clone()
	d_tot.append(d_rbkg_tot)
	d_tot.append(d_irrbkg_tot)
	wFunc = ROOT.RooRealVar('weight','event weight',float(1.0/float(ntoys)))
	weight = d_tot.addColumn( wFunc )
	toy_tot_w = ROOT.RooDataSet('toy_total_w','toy_total_w',d_tot,d_tot.get(),"",weight.GetName())
	toy_tot_w.Print()
	getattr(w, 'import')(toy_tot_w)
	fout = ROOT.TFile(filename,'RECREATE')
	w.writeToFile(filename)
	fout.Close()

def mergetoys( subdir, fs_ ):
	files = glob.glob( basedir + '_fullsimtoy/' + subdir + '/hzz4l_' + year + '_' + fs_ + '_cat*_notag.input.root')
	mergepool=Pool(len(files))
	for f in range(len(files)):
		#mergers(files[f],f,subdir)
		mergepool.apply_async(func=mergers,args=(files[f],subdir,))
	mergepool.close()
	mergepool.join()

def stepbystep( subdir, fs_, step ):
	
	if step[0] == 1:
		#step1, clone the workspaces and datacards
		if not os.path.exists(basedir + '_fullsimtoy/' + subdir ):
			os.makedirs( basedir + '_fullsimtoy/' + subdir )
		ws = glob.glob( basedir + '/' + subdir +'/hzz4l_' + year + '_' + fs_ + '_cat*_notag.input.root')
		for w in ws:
			os.system('cp ' + w + ' ' + basedir + '_fullsimtoy/' + subdir + '/')
		
		if 'm_9' in subdir or 'mkd_9' in subdir:
			dcs = glob.glob( basedir + '/' + subdir +'/hzz4l_' + year + '_' + fs_ + '_cat*_notag.txt')
		if 'm_1' in subdir or 'mkd_1' in subdir:
			dcs = glob.glob( basedir + '/' + subdir +'/hzz4l_' + year + '_' + fs_ + '_allcategs.txt')
		for dc in dcs:
			targetname = dc.split('/')[-1]
			if 'allcategs.txt' in targetname: targetname = targetname.replace('allcategs','cat0_notag')
			newstr = ''
			with open(dc,'r') as fin:
				for line in fin:
					newline = line
					if 'observation' in line:
						newline = line.split(' ')[0] + ' ' + '-1\n'
					newstr = newstr + newline
			fin.close()
			with open( basedir + '_fullsimtoy/' + subdir + '/' + targetname,'w') as fout:
				fout.write(newstr)
				fout.close()
		print 'step1 done ...'

	if step[1] == 1:	
		print ''''''''
		#step2, get event norm
		proc = {}
		adj = subdir.split('_')[0] + '_' + subdir.split('_')[1] + '_' + subdir.split('_')[3] + '_' + subdir.split('_')[4]
		f = glob.glob('datacardsInputs_hllhc3kfb14tevprojection_35x20kdmap_newzx/'+adj+'/*params.json')
		with open(f[0],'r') as fin:
			params = json.load(fin)
		for i in params:
			if re.search('^.*rate.*'+fs_+'.*$', i):
				proc[i] = params[i]
		formulatofloat( proc )
		yields = evaluator( proc )	
		print yields
		#step3, sampling
		outputpath = '/eos/user/c/chenguan/fullsimtoys/ftr/'+'/'+sub+'/'+subdir+'/'
		type_ = subdir.split('_')[1]
		categs_ = subdir.split('_')[3]
		sampling_pool = Pool(len(yields))
		for process in ['rbkg','sig','irrbkg']:		##sig rbkg irrbkg
			command = './../Others/ToyStudy/createToys.exe '+year+' '+process+' '+fs_+' '+categs_+' '+type_+' '+str(yields[process])+' 0 '+outputpath + ' | tee ' + outputpath + 'hzz4l_'+year+'_'+process+'_'+fs_+'_toys.log'
			sampling_pool.apply_async(func=run,args=(command,))
		sampling_pool.close()
		sampling_pool.join()
		print 'step2 done ...'
	
	if step[2] == 1 :	
		#step3, replace the dataset in workspaces with new fullsim toys
		mergetoys( subdir, fs_ )	
		os.system('cp ./Pieces/combcards.py ' + basedir + '_fullsimtoy/'+subdir+'/')
		os.system('cp ./Pieces/launchcombine_fullsimtoy.py ' + basedir + '_fullsimtoy/'+subdir+'/')
		#os.system('cp ./Pieces/launchcombine.py ' + basedir + '_fullsimtoy/'+subdir+'/')

basedir = sys.argv[1]
target = '2018_bsrefit_mkd_9_notag_widthpdf'
year = '2018'
fs = ['4e','4mu','2e2mu','2mu2e']
steps = [1,0,1]
sub = 'setcache20ksetdefault35'

if '/' in basedir:basedir = basedir.rstrip('/')

#deploy arguments for stepbystep function
if '_widthpdf' in target and '_mkd_' in target:
	if not os.path.exists('/eos/user/c/chenguan/fullsimtoys/ftr/'+sub+'/' + target ):
		os.makedirs('/eos/user/c/chenguan/fullsimtoys/ftr/'+sub+'/'+ target )
if '_masspdf' in target or '_m_' in target:
	if not os.path.exists('/eos/user/c/chenguan/fullsimtoys/ftr/'+sub+'/' + target.replace('mass','width').replace('_m_','_mkd_' ) ):
		print 'need to run ' + target.replace('mass','width').replace('_m_','_mkd_') + ' first...'
		sys.exit()
	else:
		steps = [1,0,1]

if not basedir+'/'+target in glob.glob(basedir + '/*'):
	print 'no target in original dir..., need to run makecards.py first...'
	sys.exit()

print 'pre check has been done, launch the jobs...'
#if os.path.exists(basedir + '_fullsimtoy'):os.system('rm -rf ' + basedir + '_fullsimtoy')	
for k in fs:
	stepbystep( target, k, steps )
