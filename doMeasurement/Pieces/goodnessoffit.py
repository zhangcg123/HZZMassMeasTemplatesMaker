#!/usr/bin/python
import os
import sys
import math
import glob
import numpy
from multiprocessing import Pool

def runcombineformass(file_, computer, withsyst, toyn, floatothers, ranges):
		
	#t2w
	outputname = file_+'_massmodelforMH_floatothers'+floatothers+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.PhysicsModel:floatingHiggsMass --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' --PO=muAsPOI -D '+toyn+' -o ' + outputname + '.root'
	
	#custom combine line
	outputrootname =  '-n '+outputname+'_fullsimtoys_'+toyn+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_fullsimtoys_'+toyn+'_'+withsyst+'_'+computer+'.log'
	
	fixed_options1 = '-M GoodnessOfFit -P MH --setParameters MH=125.0 --floatOtherPOIs='+floatothers+' --algo=singles -D '+toyn
	
	if withsyst == 'withsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options1+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options1+' '+opt2+' | tee '+outputlogname
	#run jobs
	os.system(t2wline)
	os.system(combline)

def runcombineforwidth(file_, computer, withsyst, toyn, poi, floatothers, ranges):
	
	#t2w, custom combine input file name
	outputname = file_+ '_widthmodelfor'+poi+'_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange='+str(ranges['HiggsDecayWidth'][0])+','+str(ranges['HiggsDecayWidth'][1])+' --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' -D '+toyn+' -o ' + outputname + '.root'
	
	#custom combine line, combine output file name
	outputrootname =  '-n '+outputname+'_fullsimtoys_'+toyn+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_fullsimtoys_'+toyn+'_'+withsyst+'_'+computer+'.log'
	
	fixed_options2 = '-M GoodnessOfFit -d '+outputname+'.root -P '+poi+' --setParameters HiggsDecayWidth=0.004,MH=125.0,r=1.0 --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=singles -D '+toyn
	
	if withsyst == 'withsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options2+' '+opt1+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options2+' '+opt1+' '+opt2+' | tee '+outputlogname
	
	#run jobs
	#if not os.path.exists(outputname + '.root'):
	os.system(t2wline)
	os.system(combline)

channel = sys.argv[1]			#2e2mu 2mu2e 4e 4mu allfinalstates
computer = sys.argv[2]			#local remote
syst = sys.argv[3]			#with without syst
meas = sys.argv[4]
toyn = sys.argv[5]
floatothers = '1'
ranges = {'HiggsDecayWidth':[0,0.5],'MH':[124.9,125.1]}
assert channel in ['2e2mu','2mu2e','4mu','4e','allfinalstates'] and computer in ['local','condor'] and syst in ['withsyst','withoutsyst'] and meas in ['mass','width'], 'ERRRRRRRRRRRRRRRRRR'

if channel == '4e' or channel == '2e2mu' or channel == '2mu2e':
	if syst == 'withoutsyst':
		ranges['MH'] = [124.7,125.3]
		ranges['HiggsDecayWidth'] = [0.0,0.5]
		if channel == '4e':
			ranges['HiggsDecayWidth'] = [0.0,0.8]
	if syst == 'withsyst':
		ranges['MH'] = [124.3,125.7]
		ranges['HiggsDecayWidth'] = [0.0,1.2]
		if channel == '4e':
			ranges['HiggsDecayWidth'] = [0.0,1.8]

if channel == '4mu' or channel == 'allfinalstates':
	if syst == 'withoutsyst':
		ranges['MH'] = [124.9,125.1]
		ranges['HiggsDecayWidth'] = [0.0,0.25]
	if syst == 'withsyst':
		ranges['MH'] = [124.9,125.1]
		ranges['HiggsDecayWidth'] = [0.0,0.8]

file_list = glob.glob('*allcategs.txt')
newlist = []
for f in file_list:	#remove prefix path
	newlist.append(f.split('/')[-1])
for f in newlist:
	if channel in f:
		fname = f
opt1 = '-v 9'
opt2 = '--freezeParameters allConstrainedNuisances'
opt3 = '--job-mode condor'
opt4 = '--split-points 1'
opt5 = ''
if syst == 'withoutsyst':
	opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''
if syst == 'withsyst':
	opt5 = '--sub-opts=\'+JobFlavour=\"workday\"\''

if meas == 'width':
	runcombineforwidth(fname.split('.')[0], computer, syst, toyn, 'HiggsDecayWidth', floatothers, ranges)
if meas == 'mass':
	runcombineformass(fname.split('.')[0], computer, syst, toyn, '1', ranges)
	#runcombineforwidth(fname.split('.')[0], computer, syst, toyn, 'MH', floatothers, ranges)
