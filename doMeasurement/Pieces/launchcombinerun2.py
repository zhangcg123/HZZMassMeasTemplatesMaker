#!/usr/bin/python
import os
import sys
import math
import glob
import numpy

def runcombineformass(file_, computer, withsyst, totcat, seed):
		
	#orgnize job scale
	if computer == 'local':
		max_ = 10000
	else:
		max_ = 40000000
	events=str(int(math.ceil(max_/int(totcat))))
	
	numpy.random.seed(seed)
	rm = numpy.random.randint(0,100000)
		
	#t2w
	outputname = file_+'_massmeas'
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.PhysicsModel:floatingHiggsMass --PO higgsMassRange=115,135 --PO=muAsPOI -o ' + outputname + '.root'
	
	#custom combine line
	outputrootname =  '-n '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer+'.log'
	taskname = '--task-name '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	fixed_options1 = '-M MultiDimFit -d '+outputname+'.root -m 125.0 -P MH --setParameters MH=125.0 --floatOtherPOIs=1 --algo=singles --expectSignal=1 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	fixed_options2 = '-M MultiDimFit -d '+outputname+'.root -m 125.0 -P MH --setParameters MH=125.0 --floatOtherPOIs=1 --algo=grid --points=10 --expectSignal=1 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	
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

def runcombineforwidth(file_,computer, withsyst, totcat, seed):
	
	#orgnize job scale
	if computer == 'local':
		max_ = 10000
	else:
		max_ = 4000000
	events=str(int(math.ceil(max_/int(totcat))))
	
	numpy.random.seed(seed)
	rm = numpy.random.randint(0,100000)

	#t2w
	outputname = file_+ '_widthmeas'
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange=0.0,3.0 --PO higgsMassRange=105,140 -o ' + outputname + '.root'
	
	#custom combine line
	outputrootname =  '-n '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer+'.log'
	taskname = '--task-name '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	fixed_options = '-M MultiDimFit -d '+outputname+'.root -m 125.0 -P HiggsDecayWidth --setParameters HiggsDecayWidth=0.004,MH=125.0 --floatOtherPOIs=0 --algo=grid --points=10 --expectSignal=1 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	
	if withsyst == 'withsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options+' '+opt1+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options+' '+opt1+' '+opt2+' | tee '+outputlogname
	if withsyst == 'withsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt2+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	
	#run jobs
	if not os.path.exists(outputname + '.root'):
		os.system(t2wline)
	os.system(combline)

def runcombineformasswidth(file_,computer, withsyst, totcat, seed):
	
	if computer == 'local': return
	
	opt4 = '--split-points 10'
	opt5 = '--sub-opts=\'+JobFlavour=\"nextweek\"\''

	#orgnize job sale
	if computer == 'local':
		max_ = 10000
	if computer =='condor':
		max_ = 40000000
	events=str(int(math.ceil(max_/int(totcat))))

	numpy.random.seed(seed)
	rm = numpy.random.randint(0,100000)

	#t2w
	outputname = file_+ '_masswidth'
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange=0.0,3.0 --PO higgsMassRange=122,128 -o ' + outputname + '.root'
	
	#custom combine line
	outputrootname =  '-n '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer+'.log'
	taskname = '--task-name '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+withsyst+'_'+computer
	fixed_options = '-M MultiDimFit -d '+outputname+'.root -m 125.0 -P HiggsDecayWidth -P MH --setParameters HiggsDecayWidth=0.004,MH=125.0 --floatOtherPOIs=0 --algo=grid --points=1800 --expectSignal=1 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	
	if withsyst == 'withsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+'_'+fixed_options+' '+opt2+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	
	os.system(t2wline)
	os.system(combline)

channel = sys.argv[1]			#2e2mu 2mu2e 4e 4mu allfinalstates
computer = sys.argv[2]			#local remote
syst = sys.argv[3]			#with without syst
meas = sys.argv[4]
seed = int(sys.argv[5])
assert channel in ['allfinalstates'] and computer in ['local','condor'] and syst in ['withsyst','withoutsyst'] and meas in ['mass','width','masswidth'], 'ERRRRRRRRRRRRRRRRRR'

if not os.path.exists('run2_allfinalstates_allcategs.txt'):
	os.system('combineCards.py hzz4l_2016_allfinalstates_allcategs.txt hzz4l_2017_allfinalstates_allcategs.txt hzz4l_2018_allfinalstates_allcategs.txt > run2_allfinalstates_allcategs.txt')

file_list = glob.glob('run2*allfinalstates_allcategs*.txt')
newlist = []
for f in file_list:
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
opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''

if meas == 'width':
	runcombineforwidth(fname.split('.')[0], computer, syst, totcat, seed)
if meas == 'mass':
	runcombineformass(fname.split('.')[0], computer, syst, totcat, seed)
if meas == 'masswidth':
	runcombineformasswidth(fname.split('.')[0], computer, syst, totcat, seed)
