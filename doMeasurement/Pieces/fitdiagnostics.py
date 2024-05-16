#!/usr/bin/python
import os
import sys
import math
import glob
import numpy

def runcombineforwidthimpacts(file_, computer, totcat, seed, poi, ranges, step):

	#orgnize job scale
	if computer == 'local':
		max_ = 4000
	else:
		max_ = 800000
	events=str(int(math.ceil(max_/int(totcat))))

	numpy.random.seed(seed)
	rm = numpy.random.randint(0,100000)

	#t2w custom combine input file name
	outputname = file_+ '_widthmodelfor'+poi+'_Impacts_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange='+str(ranges['HiggsDecayWidth'][0])+','+str(ranges['HiggsDecayWidth'][1])+' --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' -o ' + outputname + '.root'
	
	#custom combine line, combine output file name
	outputrootname =  '-n '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+computer
	outputlogname = outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+computer+'.log'
	taskname = '--task-name '+outputname+'_'+events+'x'+totcat+'_seed'+str(seed)+'_'+computer
	step1_options = ' -M Impacts -d '+outputname+'.root -m 125.38 --doInitialFit --robustFit=1 --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)	#otherpois should be float here?
	step2_options = ' -M Impacts -d '+outputname+'.root -m 125.38 --doFits --robustFit=0 --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	
	if computer == 'local':
		combline_step1 = 'combineTool.py '+step1_options+' '+opt1
		combline_step2 = 'combineTool.py '+step2_options+' '+opt1
	if computer == 'condor':
		combline_step1 = 'combineTool.py '+step1_options+' '+opt3+' '+opt5+' '+taskname+'_initialFit'
		combline_step2 = 'combineTool.py '+step2_options+' '+opt3+' '+opt5+' '+taskname+'_doFits'
	#run jobs
	#if not os.path.exists(outputname + '.root'):
	os.system(t2wline)
	if step == 'step1':
		os.system(combline_step1)
	if step == 'step2':
		os.system(combline_step2)

channel = sys.argv[1]			#2e2mu 2mu2e 4e 4mu all
computer = sys.argv[2]			#local remote
meas = sys.argv[3]
seed = int(sys.argv[4])
step_ = sys.argv[5]
ranges = {'HiggsDecayWidth':[0,2.5],'MH':[124.38,126.38]}
assert channel in ['2e2mu','2mu2e','4mu','4e','all'] and computer in ['local','condor'] and meas in ['massmodelformassimpacts','widthmodelforwidthimpacts','widthmodelforwidthimpacts'] and step_ in ['step1','step2'], 'ERRRRRRRRRRRRRRRRRR'
if '4e' in channel:
	ranges['HiggsDecayWidth'] = [0,1.5]
	ranges['MH'] = [124.6,125.4]
if '2e2mu' in channel or '2mu2e' in channel:
	ranges['HiggsDecayWidth'] = [0,1]
	ranges['MH'] = [124.7,125.3]

file_list = glob.glob('run2_hzz_all_13TeV.txt')
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
print fname
print totcat

opt1 = '-v 3'
opt3 = '--job-mode condor'
opt5 = '--sub-opts=\'+JobFlavour=\"testmatch\"\''

if meas == 'widthmodelforwidthimpacts':
	runcombineforwidthimpacts(fname.split('.')[0], computer, totcat, seed, 'HiggsDecayWidth', ranges, step_)
