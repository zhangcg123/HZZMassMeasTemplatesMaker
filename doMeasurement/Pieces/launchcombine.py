#!/usr/bin/python
import os
import sys
import math
import glob
import numpy

def runcombineformass(file_, computer, withsyst, totcat, seed, floatothers, ranges):
		
	#orgnize job scale
	if computer == 'local':
		max_ = 40000
	else:
		max_ = 1000000
	events=str(int(math.ceil(max_/int(totcat))))
	
	numpy.random.seed(seed)
	rm = numpy.random.randint(0,100000)
		
	#t2w
	outputname = file_+'_massmodelforMH_floatothers'+floatothers+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.PhysicsModel:floatingHiggsMass --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' --PO=muAsPOI -o ' + outputname + '.root'
	
	#custom combine line
	outputrootname =  '-n '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer+'.log'
	taskname = '--task-name '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	fixed_options1 = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P MH --setParameters MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --algo=singles -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	fixed_options2 = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P MH --setParameters MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points=20 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	
	if withsyst == 'withsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options1+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options1+' '+opt2+' | tee '+outputlogname
	if withsyst == 'withsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options2+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options2+' '+opt2+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname 
	
	#run jobs
	#if not os.path.exists(outputname + '.root'):
	os.system(t2wline)
	os.system(combline)

def runcombineforwidth(file_,computer, withsyst, totcat, seed, poi, floatothers, ranges):
	
	#orgnize job scale
	if computer == 'local':
		max_ = 10000
	else:
		max_ = 1000000
	events=str(int(math.ceil(max_/int(totcat))))
	
	numpy.random.seed(seed)
	rm = numpy.random.randint(0,100000)

	#t2w, custom combine input file name
	outputname = file_+ '_widthmodelfor'+poi+'_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange='+str(ranges['HiggsDecayWidth'][0])+','+str(ranges['HiggsDecayWidth'][1])+' --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' -o ' + outputname + '.root'
	
	#custom combine line, combine output file name
	outputrootname =  '-n '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer+'.log'
	taskname = '--task-name '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	fixed_options = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P '+poi+' --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points=10 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	
	if withsyst == 'withsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options+' '+opt1+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options+' '+opt1+' '+opt2+' | tee '+outputlogname
	if withsyst == 'withsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt2+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	
	#run jobs
	#if not os.path.exists(outputname + '.root'):
	os.system(t2wline)
	os.system(combline)

def runcombineformasswidth(file_,computer, withsyst, totcat, seed):
	
	if computer == 'local': return
	
	opt4 = '--split-points 1'
	opt5 = '--sub-opts=\'+JobFlavour=\"tomorrow\"\''
	ranges['MH'] = [125.28,125.48]
	ranges['HiggsDecayWidth'] = [0.0,0.3]

	#orgnize job sale
	if computer == 'local':
		max_ = 10000
	if computer =='condor':
		max_ = 1000000
	events=str(int(math.ceil(max_/int(totcat))))

	numpy.random.seed(seed)
	rm = numpy.random.randint(0,100000)

	#t2w
	outputname = file_+ '_widthmodelforMassWidth_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange='+str(ranges['HiggsDecayWidth'][0])+','+str(ranges['HiggsDecayWidth'][1])+' --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' -o ' + outputname + '.root'
	
	#custom combine line
	outputrootname =  '-n '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer+'.log'
	taskname = '--task-name '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	fixed_options = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P HiggsDecayWidth -P MH --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points=600 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	
	if withsyst == 'withsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt2+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	
	#run jobs
	if not os.path.exists(outputname + '.root'):
		os.system(t2wline)
	os.system(combline)

channel = sys.argv[1]			#2e2mu 2mu2e 4e 4mu allfinalstates
computer = sys.argv[2]			#local remote
syst = sys.argv[3]			#with without syst
meas = sys.argv[4]
seed = int(sys.argv[5])
floatothers = '1'
ranges = {'HiggsDecayWidth':[0,2.5],'MH':[124.38,126.38]}
assert channel in ['2e2mu','2mu2e','4mu','4e','allfinalstates'] and computer in ['local','condor'] and syst in ['withsyst','withoutsyst'] and meas in ['mass','width','masswidth'], 'ERRRRRRRRRRRRRRRRRR'

if channel == '4e' or channel == '2e2mu' or channel == '2mu2e':
	if syst == 'withoutsyst':
		ranges['MH'] = [122.38,128.38]
		ranges['HiggsDecayWidth'] = [0.0,5.0]
		if channel == '4e':
			ranges['HiggsDecayWidth'] = [0.0,8.0]
	if syst == 'withsyst':
		ranges['MH'] = [121.38,129.38]
		ranges['HiggsDecayWidth'] = [0.0,8.0]
		if channel == '4e':
			ranges['HiggsDecayWidth'] = [0.0,10.0]

if channel == '4mu' or channel == 'allfinalstates':
	if syst == 'withoutsyst':
		ranges['MH'] = [124.38,126.38]
		ranges['HiggsDecayWidth'] = [0.0,2.5]
	if syst == 'withsyst':
		ranges['MH'] = [124.38,126.38]
		ranges['HiggsDecayWidth'] = [0.0,2.5]
	currentdir = os.getcwd().split('/')[-1]
	if '20160' in currentdir and '_m_1_' in currentdir:
		ranges['MH'] = [122.38,128.38]
		ranges['HiggsDecayWidth'] = [0.0,3.5]
	if '20165' in currentdir and '_m_1_' in currentdir:
		ranges['MH'] = [122.38,128.38]
		ranges['HiggsDecayWidth'] = [0.0,3.5]

file_list = glob.glob('*allcategs_13TeV.txt')
newlist = []
for f in file_list:	#remove prefix path
	newlist.append(f.split('/')[-1])
for f in newlist:
	if channel in f:
		fname = f
		with open(f,'r') as fin:
			for line in fin:
				if line.startswith('imax'):
					totcat = line.split()[1] 
opt1 = '-v 3'
opt2 = '--freezeParameters allConstrainedNuisances'
opt3 = '--job-mode condor'
opt4 = '--split-points 1'
opt5 = ''
if syst == 'withoutsyst':
	opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''
if syst == 'withsyst':
	opt5 = '--sub-opts=\'+JobFlavour=\"workday\"\''

if meas == 'width':
	runcombineforwidth(fname.split('.')[0], computer, syst, totcat, seed, 'HiggsDecayWidth', floatothers, ranges)
if meas == 'mass':
	runcombineformass(fname.split('.')[0], computer, syst, totcat, seed, '1', ranges)
	#runcombineforwidth(fname.split('.')[0], computer, syst, totcat, seed, 'MH', floatothers, ranges)
if meas == 'masswidth':
	runcombineformasswidth(fname.split('.')[0], computer, syst, totcat, seed)
