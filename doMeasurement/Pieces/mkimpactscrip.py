import numpy
import math
import os
import sys
import ROOT
import glob
import commands
sys.path.append('../')
from costumeimpact import getpull
import pandas

def gethead():
	head = '#!/bin/sh\nulimit -s unlimited\nset -e\ncd /afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/CMSSW_10_6_12/src\nexport SCRAM_ARCH=slc7_amd64_gcc700\nsource /cvmfs/cms.cern.ch/cmsset_default.sh\neval `scramv1 runtime -sh`\n'
	wkd = os.getcwd()
	head = head + 'cd ' + wkd + '\n'
	return head

def getpois( file_, computer, poi, floatothers, ranges ):
	# t2w
	outputname = file_+ '_widthmodelfor'+poi+'_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange='+str(ranges['HiggsDecayWidth'][0])+','+str(ranges['HiggsDecayWidth'][1])+' --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' -o ' + outputname + '.root'
	os.system( t2wline )
	# extract all vars name from t2w output file, then return a list
	nuis = []
	f = ROOT.TFile( outputname+'.root','r' )
	w = f.Get('w')
	a = w.obj('ModelConfig').GetNuisanceParameters()
	iter = a.createIterator()
	var = iter.Next()
	while var:
		nuis.append( var.GetName() )
		var = iter.Next()
	f.Close()
	return nuis + ['r','MH','HiggsDecayWidth']

def getstep2( logname, rootname, var, name1, poi='HiggsDecayWidth' ):#move condor files to eso dir
	# get all above likelihood scans, summary to one json
	#target dir
	cwd = os.getcwd().split('/')[-1]
	targetdir = '/eos/user/c/chenguan/www/'+cwd+'/'
	os.system('mkdir '+targetdir)
	os.system('cp /eos/user/c/chenguan/www/index.php /eos/user/c/chenguan/www/'+cwd)	
	# merge
	string  = ''
	for v in range(len(rootname)):
		fs = glob.glob('*'+rootname[v]+'*root')
		targetfile = 'hadd '+var[v]+'_'+name1+'.root '
		for ff in fs:
			targetfile = targetfile + ff + ' '
		#print targetfile
		os.system(targetfile)
		os.system( 'mv '+var[v]+'_'+name1+'.root '+targetdir )
		poiname = ''
		if name1 == 'nuisance': poiname = var[v]
		if name1 == poi: poiname = poi
		out = commands.getstatusoutput( 'python /afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/Pieces/plot1DScan.py '+targetdir+'/'+var[v]+'_'+name1+'.root --POI '+poiname+' -o '+var[v]+'_'+name1 )
		os.system( 'mv '+var[v]+'_'+name1+'.png '+targetdir )
		string = string + var[v]+'_'+name1+':'+out[-1].split('\n')[-1]+'\n'
		os.system('mv *'+rootname[v]+'* '+targetdir)
		os.system('mv *'+logname[v]+'* '+targetdir)
	# move
	os.system('mv *out '+targetdir)
	os.system('mv *err '+targetdir)
	os.system('mv *log '+targetdir)
	os.system('mv condor*sh '+targetdir)
	os.system('mv condor*sub '+targetdir)
	with open(name1+'_summary.txt','w+') as fo:
		fo.write(string)
	fo.close() 

def getstep4( file_, computer, poi, floatothers, ranges ):
	cwd = os.getcwd().split('/')[-1]
	targetdir = '/eos/user/c/chenguan/www/'+cwd+'/'
	allvars = getpois( file_, computer, poi, floatothers, ranges )
	for DN in ['DN','UP']:
		string = ''
		for aa in allvars:
			if aa == 'HiggsDecayWidth' or aa == 'MH' or aa == 'r': continue
			fs = glob.glob('higgs*'+aa+'*'+DN+'*')
			targetfile = 'hadd '+aa+'_'+DN+'.root '
			for fff in fs:
				targetfile = targetfile + fff + ' '
			#os.system(targetfile)
			os.system('mv '+aa+'_'+DN+'.root '+targetdir )
			out = commands.getstatusoutput( 'python /afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/Pieces/plot1DScan.py '+targetdir+'/'+aa+'_'+DN+'.root --POI '+poi+' -o '+aa+'_'+DN )
			os.system('mv '+aa+'_'+DN+'.png '+targetdir)
			os.system('mv higgs*'+aa+'_Impact_'+DN+'*.root '+targetdir)
			#os.system('mv higgs*'+aa+'_Impact_'+DN+'*.log '+targetdir)
			string = string + aa +'_'+DN+':'+out[-1].split('\n')[-1]+'\n'
		os.system('mv *out '+targetdir)
		os.system('mv *err '+targetdir)
		os.system('mv *log '+targetdir)
		os.system('mv condor*sub '+targetdir)
		os.system('mv condor*sh '+targetdir)	
		with open('impact_'+DN+'.txt','w+') as fo:
			fo.write(string)
		fo.close()

def getstep1( file_, computer, poi, floatothers, ranges, seed, step2=False ):
	allvars = getpois( file_, computer, poi, floatothers, ranges )
	# '--algo=singles' treats other pois as unconstrainted nuisances, so it should be equal to '--algo=grid --floatOtherPOIs=1'
	outputname = file_+ '_widthmodelfor'+poi+'_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	roots = []
	logs = []
	for ii,a in enumerate(allvars):
		outputrootname = '-n '+outputname.replace(poi,a)+'_'+computer#in the step1, scan all parameters
		outputlogname = outputname.replace(poi,a)+'_'+computer+'.log'
		if step2:
			logs.append(outputlogname)
			roots.append('higgsCombine'+outputrootname.strip('-n '))
		if not step2:
			nn = '30'
			rangestr = ' --setParameterRanges '+a+'=-2.0,2.0'
			if a == 'HiggsDecayWidth':
				nn = '300'
				rangestr = ''
			if a == 'MH' or a == 'r':
				rangestr = ''
			if '_sigma_e_' in a:
				rangestr = ' --setParameterRanges '+a+'=-0.2,0.2'
			if '_sigma_m_' in a:
				rangestr = ' --setParameterRanges '+a+'=-0.06,0.06'
			taskname = '--task-name '+outputname.replace(poi,a)+'_'+computer
			fixed_opt = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P '+a+' '+rangestr+' --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points='+nn+' --saveInactivePOI=1'
			cmbl = 'combineTool.py '+outputrootname+' '+fixed_opt+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
			os.system( cmbl )
	'''
	#for uncertanities break down freeze each of nuisances
	improots = []
	implogs = []
	for ii,a in enumerate( allvars ):
		outputrootname = '-n '+outputname.replace(poi,a+'Impact')+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer
		outputlogname = outputname.replace(poi,a+'Impact')+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer+'.log'
		if step2:
			implogs.append(outputlogname)
			improots.append('higgsCombine'+outputrootname.strip('-n '))
		if not step2:
			taskname = '--task-name '+outputname.replace(poi,a+'Impact')+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer
			fixed_opt = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P '+poi+' --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --freezeParameters '+a+' --robustFit=0 --algo=grid --points=10 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
			cmbl = 'combineTool.py '+outputrootname+' '+fixed_opt+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
			os.system( cmbl )
	'''
	'''
	#for uncertainties break down freeze all nuisances
	outputrootname = '-n '+outputname.replace(poi,'None')+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer
	outputlogname = outputname.replace(poi,'None')+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer+'.log'
	if step2:
		implogs.append(outputlogname)
		improots.append('higgsCombine'+outputrootname.strip('-n '))
	if not step2:	
		taskname = '--task-name '+outputname.replace(poi,'None')+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer
		fixed_opt = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P '+poi+' --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --freezeParameters allConstrainedNuisances --robustFit=0 --algo=grid --points=10 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
		cmbl = 'combineTool.py '+outputrootname+' '+fixed_opt+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
		os.system( cmbl )
	'''
	if step2:
		getstep2( logs, roots, allvars, 'nuisance' )

def getstep3( file_, poi, floatothers, ranges ):#run impact
	
	# '-P HiggsDecayWidth --algo=grid --floatOtherPOIs=1 --setParameters onenui=fixed_value_from_above --freezeParameters theonenui'
	cwd = os.getcwd().split('/')[-1]	
	targetdir = '/eos/user/c/chenguan/www/'+cwd + '/'
	inputfilename = file_+ '_widthmodelfor'+poi+'_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')+'.root'
	allvars = getpois( file_, computer, poi, floatothers, ranges )
	os.system('cp '+inputfilename+' '+targetdir)
	os.system('cp nuisance_summary.txt /eos/user/c/chenguan/www/'+cwd+'/')
	results = getpull( targetdir, inputfilename )
	print results
	
	for vname in allvars:
		if vname == 'HiggsDecayWidth' or vname == 'r' or vname == 'MH':continue
		vup = ''
		vdn = ''	
		for index, row in results.iterrows():
			if vname == row['name']:
				vup = str(row['post-max'])
				vdn = str(row['post-min'])
		inputrootname = file_+ '_widthmodelfor'+poi+'_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
		
		outputname = file_+ '_widthmodelfor_'+vname+'_Impact_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')	
		
		outputrootname = '-n '+outputname.replace('Impact','Impact_UP')+'_condor'	
		outputlogname = outputname.replace('Impact','Impact_UP')+'_condor.log'
		taskname = '--task-name '+outputname.replace('Impact','Impact_UP')+'_condor'	
		fixed_opt = '-M MultiDimFit -d '+inputrootname+'.root -m 125.38 -P '+poi+' --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0,'+vname+'='+vup+' --freezeParameters '+vname+' --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points=50'
		cmbl = 'combineTool.py '+outputrootname+' '+fixed_opt+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' --dry-run | tee '+outputlogname
		os.system(cmbl)

		outputrootname = '-n '+outputname.replace('Impact','Impact_DN')+'_condor'
		outputlogname = outputname.replace('Impact','Impact_DN')+'_condor.log'
		taskname = '--task-name '+outputname.replace('Impact','Impact_DN')+'_condor'
		fixed_opt = '-M MultiDimFit -d '+inputrootname+'.root -m 125.38 -P '+poi+' --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0,'+vname+'='+vdn+' --freezeParameters '+vname+' --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points=50'
		cmbl = 'combineTool.py '+outputrootname+' '+fixed_opt+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' --dry-run | tee '+outputlogname
		os.system(cmbl)

	
		
channel = sys.argv[1]
computer = sys.argv[2]
meas = sys.argv[3]
seed = int(sys.argv[4])
floatothers = '1'
ranges ={'MH':[124.38,126.38],'HiggsDecayWidth':[0.0001,1.0]}
if channel == 'all':
                ranges['MH'] = [124.38,126.38]
                ranges['HiggsDecayWidth'] = [0.0004,0.8]
if channel == '4mu':
                ranges['MH'] = [124.0,126.5]
                ranges['HiggsDecayWidth'] = [0.001,1.0]
if channel == '4e': 
                ranges['MH'] = [124.0,127.0]
                ranges['HiggsDecayWidth'] = [0.001,10.0]
if channel == '2e2mu':
                ranges['MH'] = [122.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,5.0]
if channel == '2mu2e':
                ranges['MH'] = [122.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,1.0]

fname = [f.split('/')[-1] for f in glob.glob('hzz4l_20165_2mu2e_allcategs_13TeV.txt') if channel in f.split('/')[-1]][-1]
with open(fname,'r') as fin:
	totcat = [ l.split()[1] for l in fin if l.startswith('imax')][-1] 

opt1 = '-v 3'
opt2 = '--freezeParameters'
opt3 = '--job-mode condor'
opt4 = '--split-points 10'
opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''

#getstep1( fname.split('/')[-1].split('.')[0], 'condor', 'HiggsDecayWidth', floatothers, ranges, seed )
#getstep1( fname.split('/')[-1].split('.')[0], 'condor', 'HiggsDecayWidth', floatothers, ranges, seed, True )
#getstep3( fname.split('/')[-1].split('.')[0], 'MH', floatothers, ranges )#run impact
getstep4( fname.split('/')[-1].split('.')[0], 'condor', 'MH', floatothers, ranges )
