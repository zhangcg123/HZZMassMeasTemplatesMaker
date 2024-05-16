#!/usr/bin/python
import os
import sys
import math
import glob
import numpy
import ROOT
import json
def extractor( prename ):
	with open('snapshot.json','r') as fin:
		dictionary = json.load(fin)
	a = []	
	for name in dictionary:
		a.append(','+name+'='+str(dictionary[name]))
	atolist = ''.join(a).strip(',')
	return atolist

def runcombineforpoiscan(file_, computer, totcat, seed, poi, ranges, wosyst, obs):

	#t2w custom combine input file name
	outputname = file_+ '_massmodelfor'+poi+'_Scan_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.PhysicsModel:floatingHiggsMass --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' --PO=muAsPOI -o ' + outputname + '.root'
	os.system(t2wline)
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
	trackstr = ','.join(nuis)
	trackP = ' --trackParameters '+trackstr
	trackE = ' --trackErrors '+trackstr
	
	#custom combine line, combine output file name
	outputrootname =  ' -n '+outputname+'_'+wosyst+'_'+computer+'_'+obs
	outputlogname = outputname+'_'+wosyst+'_'+computer+'_'+obs+'.log'
	taskname = ' --task-name '+outputname+'_'+wosyst+'_'+computer+'_'+obs

	fixed_options = '-M MultiDimFit '+outputrootname+' -m 125.38 -P '+poi+' --floatOtherPOIs=1 --robustFit=0 --saveInactivePOI 1 --algo=grid --points=20 --split-points 10'
	if obs == 'obs' and wosyst == '':
		fixed_options = fixed_options + ' -d '+outputname+'.root --setParameters MH=125.38,r=1.0 --saveWorkspace'
	if obs == 'exp' and wosyst == '':
		fixed_options = fixed_options + ' -d '+outputname+'.root --setParameters MH=125.38,r=1.0 -t -1'
	if obs == 'obs' and wosyst == 'nosyst':
		#setparams = extractor( obs )
		#print 'the snapshot root file needs to be specified'
		#exit()
		fixed_options = fixed_options + ' -d postfitsnapshot.root ' + opt2 +' --snapshotName MultiDimFit'
	if obs == 'exp' and wosyst == 'nosyst':
		#setparams = extractor( obs )
		fixed_options = fixed_options + ' -d '+outputname+'.root ' + opt2 +' --setParameters MH=125.38,r=1.0 -t -1'
			
	combline_step1 = 'combineTool.py '+fixed_options+' '+opt3+' '+opt5+' '+taskname#+' --dry-run'
		
	#run jobs
	os.system(combline_step1)

channel = os.getcwd().split('/')[-1].split('_')[-1]
computer = 'condor'			#local remote
meas = 'widthmodelforpoiscan'
seed = 123
ranges = {'HiggsDecayWidth':[0.001,4.0],'MH':[123.0,128.0]}
assert channel in ['2e2mu','2mu2e','4mu','4e','all'] and computer in ['local','condor'], 'ERRRRRRRRRRRRRRRRRR'

cwd = os.getcwd().split('/')[-1]
yy = cwd.split('_')[0]
fffs = cwd.split('_')[-1]
fname = ''
if yy == 'run2':
	fname = 'run2_hzz_'+fffs+'_13TeV.txt'
elif fffs == 'all' and '201' in yy:
        fffs = 'allfinalstates'
	fname = 'hzz4l_'+yy+'_'+fffs+'_allcategs_13TeV.txt'
elif fffs != 'all' and '201' in yy:
	fname = 'hzz4l_'+yy+'_'+fffs+'_allcategs_13TeV.txt'
elif 'run1' == yy:
	fname = 'run1_'+fffs+'_13TeV.txt'
elif 'run12' == yy:
	fname = 'run12_hzz_'+fffs+'_13TeV.txt'


if yy == '20160':
        if fffs == '4mu':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == '4e':
		ranges['MH'] = [110.0,130.0]
                ranges['HiggsDecayWidth'] = [0.001,20]
        if fffs == '2e2mu':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == '2mu2e':
		ranges['MH'] = [120.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,5.0]
        if fffs == 'allfinalstates':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
if yy == '20165':
        if fffs == '4mu':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == '4e':
		ranges['MH'] = [122.0,129.0]
                ranges['HiggsDecayWidth'] = [0.001,10.0]
        if fffs == '2e2mu':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == '2mu2e':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == 'allfinalstates':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
if yy == '2017':
        if fffs == '4mu':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == '4e':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == '2e2mu':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == '2mu2e':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == 'allfinalstates':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
if yy == '2018':
        if fffs == '4mu':
		ranges['MH'] = [124.0,126.0]
                ranges['HiggsDecayWidth'] = [0.001,3.0]
        if fffs == '4e':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == '2e2mu':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == '2mu2e':
		ranges['MH'] = [124.0,126.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
        if fffs == 'allfinalstates':
		ranges['MH'] = [123.0,128.0]
                ranges['HiggsDecayWidth'] = [0.001,4.0]
if yy == 'run2' or yy == 'run12':
	if fffs == '4mu':
		ranges['MH'] = [123.0,128.0]
		ranges['HiggsDecayWidth'] = [0.001,4.0]
	if fffs == '4e':
		ranges['MH'] = [123.0,128.0]
		ranges['HiggsDecayWidth'] = [0.001,10]
	if fffs == '2e2mu':
		ranges['MH'] = [123.0,128.0]
		ranges['HiggsDecayWidth'] = [0.001,4.0]
	if fffs == '2mu2e':
		ranges['MH'] = [123.0,128.0]
		ranges['HiggsDecayWidth'] = [0.001,4.0]
	if fffs == 'all':
		ranges['MH'] = [123.0,128.0]
		ranges['HiggsDecayWidth'] = [0.001,3.0]
if yy == 'run1':
	if fffs == '4mu':
		ranges['MH'] = [120.0,130.0]
	if fffs == '4e':
		ranges['MH'] = [120.0,130.0]
	if fffs == '2e2mu':
		ranges['MH'] = [120.0,130.0]
	if fffs == '2mu2e':
		ranges['MH'] = [123.0,128.0]
	if fffs == 'all':
		ranges['MH'] = [123.0,128.0]


file_list = glob.glob(fname)
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
opt2 = '--freezeParameters allConstrainedNuisances'
opt3 = '--job-mode condor'
opt5 = ''
if yy != 'run2' and channel != 'all':
	opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''
if yy != 'run2' and channel == 'all':
	opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''
if yy == 'run2' and channel != 'all':
	opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''
if yy == 'run2' and channel == 'all':
	opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''

opt5 = '--sub-opts=\'+JobFlavour=\"espresso\"\''

obs_ = os.getcwd().split('/')[-2].lower()
wosyst_ = os.getcwd().split('/')[-1].split('_')[-2] if os.getcwd().split('/')[-1].split('_')[-2] == 'nosyst' else ''

if meas == 'widthmodelforpoiscan':
	runcombineforpoiscan(fname.split('.')[0], computer, totcat, seed, 'MH', ranges, wosyst_, obs_ )
