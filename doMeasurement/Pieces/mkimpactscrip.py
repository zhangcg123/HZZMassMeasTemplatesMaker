import numpy
import math
import os
import sys
import ROOT
import glob
import commands

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


def getstep2( logname, rootname, var, name1, poi='HiggsDecayWidth' ):
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
	with open(name1+'_summary.txt','w+') as fo:
		fo.write(string)
	fo.close() 

def getstep1( file_, computer, poi, floatothers, ranges, seed, step2=False ):
	
	allvars = getpois( file_, computer, poi, floatothers, ranges )

	# '--algo=singles' treats other pois as unconstrainted nuisances, so it should be equal to '--algo=grid --floatOtherPOIs=1'
	
	max_ = 4000
	events=str(int(math.ceil(max_/int(totcat))))
	
	numpy.random.seed(seed)
	rm = numpy.random.randint(0,100000)

	outputname = file_+ '_widthmodelfor'+poi+'_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')

	roots = []
	logs = []

	for ii,a in enumerate(allvars):
		outputrootname = '-n '+outputname.replace(poi,a)+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer
		outputlogname = outputname.replace(poi,a)+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer+'.log'
		if step2:
			logs.append(outputlogname)
			roots.append('higgsCombine'+outputrootname.strip('-n '))
		if not step2:
			taskname = '--task-name '+outputname.replace(poi,a)+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer
			fixed_opt = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P '+a+' --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points=10 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
			cmbl = 'combineTool.py '+outputrootname+' '+fixed_opt+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
			os.system( cmbl )

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

	if step2:
		#getstep2( logs, roots, allvars, 'nuisance' )
		allvars.append('None')
		getstep2( implogs, improots, allvars, poi )


def getstep3( file_, computer, poi, floatothers, ranges, seed ):
	# '-P HiggsDecayWidth --algo=grid --floatOtherPOIs=1 --setParameters onenui=fixed_value_from_above --freezeParameters theonenui'
	pass

channel = sys.argv[1]
computer = sys.argv[2]
meas = sys.argv[3]
seed = int(sys.argv[4])
floatothers = '1'
ranges ={'MH':[124.38,126.38],'HiggsDecayWidth':[0.0001,1.0]}
if channel == 'all':
                ranges['MH'] = [124.38,126.38]
                ranges['HiggsDecayWidth'] = [0.0001,0.4]
if channel == '4mu':
                ranges['MH'] = [124.38,126.38]
                ranges['HiggsDecayWidth'] = [0.0001,0.5]
if channel == '4e': 
                ranges['MH'] = [122.38,128.38]
                ranges['HiggsDecayWidth'] = [0.0001,1.3]
if channel == '2e2mu':
                ranges['MH'] = [122.38,128.38]
                ranges['HiggsDecayWidth'] = [0.0001,1.0]
if channel == '2mu2e':
                ranges['MH'] = [122.38,128.38]
                ranges['HiggsDecayWidth'] = [0.0001,1.0]

fname = [f.split('/')[-1] for f in glob.glob('run2*.txt') if channel in f.split('/')[-1]][-1]
with open(fname,'r') as fin:
	totcat = [ l.split()[1] for l in fin if l.startswith('imax')][-1] 

opt1 = '-v 3'
opt2 = '--freezeParameters'
opt3 = '--job-mode condor'
opt4 = '--split-points 1'
opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''

#getstep1( fname.split('/')[-1].split('.')[0], 'condor', 'HiggsDecayWidth', floatothers, ranges, seed )
getstep1( fname.split('/')[-1].split('.')[0], 'condor', 'HiggsDecayWidth', floatothers, ranges, seed, True )
