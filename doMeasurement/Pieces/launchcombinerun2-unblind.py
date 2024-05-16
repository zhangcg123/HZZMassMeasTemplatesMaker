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
	fixed_options2 = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P MH --setParameters MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points=10 -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	
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

def runcombineforwidth1dscan(file_,computer, withsyst, totcat, seed, poi, floatothers, ranges):
	
	#orgnize job scale
	if computer == 'local':
		max_ = 4000
	else:
		max_ = 100000
	events=str(int(math.ceil(max_/int(totcat))))
	
	numpy.random.seed(seed)
	rm = numpy.random.randint(0,100000)

	#t2w, custom combine input file name
	outputname = file_+ '_widthmodelfor'+poi+'_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
	t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange='+str(ranges['HiggsDecayWidth'][0])+','+str(ranges['HiggsDecayWidth'][1])+' --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' -o ' + outputname + '.root'
	
	#custom combine line, combine output file name
	outputrootname =  '-n '+outputname+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_'+withsyst+'_'+computer+'.log'
	taskname = '--task-name '+outputname+'_'+withsyst+'_'+computer
	fixed_options = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P '+poi+' --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points=10'
	
	if withsyst == 'withsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options+' '+opt1+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'local':
		combline = 'combine '+outputrootname+' '+fixed_options+' '+opt1+' '+opt2+' | tee '+outputlogname
	if withsyst == 'withsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt2+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	
	os.system(t2wline)
	os.system(combline)
	
	with open(outputlogname.replace('.log','.cmd'),'w+') as fo:
		fo.write(t2wline)
		fo.write('\n')
		fo.write(combline)
	fo.close()
	

def runcombineforwidthresult( file_, computer, withsyst, totcat, seed, poi, floatothers, ranges, robust ):
	
	#orgnize job scale
        if computer == 'local':
                max_ = 4000
        else:
                max_ = 100000
        events=str(int(math.ceil(max_/int(totcat))))
 
        numpy.random.seed(seed)
        rm = numpy.random.randint(0,100000)
 
        #t2w custom combine input file name
        outputname = file_+ '_widthmodelfor'+poi+'_Impacts_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
        t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange='+str(ranges['HiggsDecayWidth'][0])+','+str(ranges['HiggsDecayWidth'][1])+' --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' -o ' + outputname + '.root' 
     
        #custom combine line, combine output file name
        outputrootname =  '-n '+outputname+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+withsyst+'_'+computer
        outputlogname = outputname+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+withsyst+'_'+computer+'.log'
        taskname = '--task-name '+outputname+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+withsyst+'_'+computer
        fixed_options = ' -M MultiDimFit -d '+outputname+'.root -m 125.38 -P '+poi+' --robustFit='+str(robust)+' --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1 --floatOtherPOIs='+floatothers+' --algo=singles -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
	
	if withsyst == 'withsyst' and computer == 'condor' and robust == 1:
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt3+' '+opt5+' '+taskname+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'condor' and robust ==1:
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt2+' '+opt3+' '+opt5+' '+taskname+' | tee '+outputlogname

	os.system(t2wline)
        os.system(combline)
        with open(outputlogname.replace('.log','.cmd'),'w+') as fo:
		fo.write(t2wline)
       	 	fo.write(combline)
	fo.close()

def runcombineforwidthimpact(file_, computer, withsyst, totcat, seed, poi, floatothers, ranges, robust, step):
	
	#orgnize job scale
        if computer == 'local':
                max_ = 4000
        else:
                max_ = 100000
        events=str(int(math.ceil(max_/int(totcat))))
 
        numpy.random.seed(seed)
        rm = numpy.random.randint(0,100000)
 
        #t2w custom combine input file name
        outputname = file_+ '_widthmodelfor'+poi+'_Impacts_floatothers'+floatothers+'_width_'+str(ranges['HiggsDecayWidth'][0])[0:6].replace('.','p')+'_'+str(ranges['HiggsDecayWidth'][1])[0:6].replace('.','p')+'_MH_'+str(ranges['MH'][0])[0:6].replace('.','p')+'_'+str(ranges['MH'][1])[0:6].replace('.','p')
        t2wline = 'text2workspace.py ' + file_ + '.txt -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange='+str(ranges['HiggsDecayWidth'][0])+','+str(ranges['HiggsDecayWidth'][1])+' --PO higgsMassRange='+str(ranges['MH'][0])+','+str(ranges['MH'][1])+' -o ' + outputname + '.root' 
     
        #custom combine line, combine output file name
        outputrootname =  '-n '+outputname+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer
        outputlogname = outputname+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer+'.log'
        taskname = '--task-name '+outputname+'_'+events+'x'+totcat+'_seed'+str(rm)+'_'+computer
        step1_options = ' -M Impacts -d '+outputname+'.root -m 125.38 --doInitialFit --robustFit=1 --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1 --floatOtherPOIs='+floatothers+' -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm) #otherpois should be float here?
        step2_options = ' -M Impacts -d '+outputname+'.root -m 125.38 --doFits --robustFit=1 --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1 --floatOtherPOIs='+floatothers+' -t -1 --X-rtd TMCSO_PseudoAsimov='+events+' --X-rtd TMCSO_AdaptivePseudoAsimov=0 -s '+str(rm)
     
        if computer == 'local':
                combline_step1 = 'combineTool.py '+step1_options+' '+opt1
                combline_step2 = 'combineTool.py '+step2_options+' '+opt1
        if computer == 'condor':
                combline_step1 = 'combineTool.py '+step1_options+' '+opt3+' '+opt5+' '+taskname+'_initialFit'
                combline_step2 = 'combineTool.py '+step2_options+' '+opt3+' '+opt5+' '+taskname+'_doFits'
	os.system(t2wline)
        if step == 'step1':
               os.system(combline_step1)
        if step == 'step2':
               os.system(combline_step2)
        with open(outputlogname.replace('.log','.cmd'),'w+') as fo:
		fo.write(t2wline)
       	 	fo.write(combline_step1)
	        fo.write(combline_step2)
	fo.close()

def runcombineformasswidth(file_,computer, withsyst, totcat, seed):
	
	if computer == 'local': return
	
	opt4 = '--split-points 1'
	opt5 = '--sub-opts=\'+JobFlavour=\"workday\"\''
	ranges['MH'] = [124.88,125.88]
	ranges['HiggsDecayWidth'] = [0.0,1.0]

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
	outputrootname =  '-n '+outputname+'_'+withsyst+'_'+computer
	outputlogname = outputname+'_'+withsyst+'_'+computer+'.log'
	taskname = '--task-name '+outputname+'_'+withsyst+'_'+computer
	fixed_options = '-M MultiDimFit -d '+outputname+'.root -m 125.38 -P HiggsDecayWidth -P MH --setParameters HiggsDecayWidth=0.004,MH=125.38,r=1.0 --floatOtherPOIs='+floatothers+' --robustFit=0 --algo=grid --points=600'
	
	if withsyst == 'withsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	if withsyst == 'withoutsyst' and computer == 'condor':
		combline = 'combineTool.py '+outputrootname+' '+fixed_options+' '+opt2+' '+opt3+' '+opt4+' '+opt5+' '+taskname+' | tee '+outputlogname
	
	#run jobs
	#if not os.path.exists(outputname + '.root'):
	os.system(t2wline)
	os.system(combline)

channel = sys.argv[1]			#2e2mu 2mu2e 4e 4mu all
computer = sys.argv[2]			#local remote
syst = sys.argv[3]			#with without syst
meas = sys.argv[4]
seed = int(sys.argv[5])
algo = sys.argv[6]
rob = 1
step = 'step1'
floatothers = '1'
ranges = {'HiggsDecayWidth':[0,2.5],'MH':[124.38,126.38]}
assert channel in ['2e2mu','2mu2e','4mu','4e','all'] and computer in ['local','condor'] and syst in ['withsyst','withoutsyst'] and meas in ['mass','width','masswidth'], 'ERRRRRRRRRRRRRRRRRR'

if channel == 'all':
	if algo == 'grid':
		ranges['MH'] = [124.38,126.38]
		ranges['HiggsDecayWidth'] = [0.0001,0.8] 
	if algo == 'impact' or algo == 'singles':
		ranges['MH'] = [124.38,126.38]
		ranges['HiggsDecayWidth'] = [0.0001,0.4]
if channel == '4mu':
	if algo == 'grid':
		ranges['MH'] = [124.38,126.38]
		ranges['HiggsDecayWidth'] = [0.0001,1.0]
	if algo == 'impact' or algo == 'singles':
		ranges['MH'] = [124.38,126.38]
		ranges['HiggsDecayWidth'] = [0.0001,0.5]
if channel == '4e':
	if algo == 'grid':
		ranges['MH'] = [122.38,128.38]
		ranges['HiggsDecayWidth'] = [0.0001,3.0]
	if algo == 'impact' or algo == 'singles':
		ranges['MH'] = [122.38,128.38]
		ranges['HiggsDecayWidth'] = [0.0001,1.3]
if channel == '2e2mu':
	if algo == 'grid':
		ranges['MH'] = [122.38,128.38]
		ranges['HiggsDecayWidth'] = [0.0001,2.0]
	if algo == 'impact' or algo == 'singles':
		ranges['MH'] = [122.38,128.38]
		ranges['HiggsDecayWidth'] = [0.0001,1.0]
if channel == '2mu2e':
	if algo == 'grid':
		ranges['MH'] = [122.38,128.38]
		ranges['HiggsDecayWidth'] = [0.0001,2.0]
	if algo == 'impact' or algo == 'singles':
		ranges['MH'] = [122.38,128.38]
		ranges['HiggsDecayWidth'] = [0.0001,1.0]

fname = [f.split('/')[-1] for f in glob.glob('run2*.txt') if channel in f.split('/')[-1]][-1]
with open(fname,'r') as fin:
	totcat = [ l.split()[1] for l in fin if l.startswith('imax')][-1]

opt1 = '-v 3'
opt2 = '--freezeParameters allConstrainedNuisances'
opt3 = '--job-mode condor'
opt4 = '--split-points 1'
opt5 = ''

if algo == 'grid':
	if computer == 'local':
		sys.exit( ' large jobs must be run with condor ')
	if rob == 1:
		sys.exit(' Do not know what would happen turning robustFit on under grid algorithm!!' )
	if syst == 'withoutsyst':
		opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''
	if syst == 'withsyst':
		opt5 = '--sub-opts=\'+JobFlavour=\"longlunch\"\''

if algo == 'impact':
	if computer == 'local':
		sys.exit( ' large jobs must be run with condor ')
	if rob == 0:
		sys.exit(' singles algo would try to find negtive limit if minos used' )
	if syst == 'withoutsyst':
		sys.exit(' impact algo + withoutsyst does not make sence ')
	opt5 = '--sub-opts=\'+JobFlavour=\"nextweek\"\''

if algo == 'singles':
	if rob == 0 and computer == 'condor':
		sys.exit(' small jobs can should at lxplus interactively ')
	if rob == 1 and computer == 'local':
		sys.exit( ' large jobs must be run with condor ')
	if syst == 'withsyst':
		opt5 = '--sub-opts=\'+JobFlavour=\"nextweek\"\''
	if syst == 'withoutsyst':
		opt5 = '--sub-opts=\'+JobFlavour=\"testmatch\"\''

if meas == 'width' and algo == 'grid':
	runcombineforwidth1dscan(fname.split('.')[0], computer, syst, totcat, seed, 'HiggsDecayWidth', floatothers, ranges)
if meas == 'width' and algo == 'impact':
	runcombineforwidthimpact(fname.split('.')[0], computer, syst, totcat, seed, 'HiggsDecayWidth', floatothers, ranges, rob, step)
if meas == 'width' and algo == 'singles':
	runcombineforwidthresult(fname.split('.')[0], computer, syst, totcat, seed, 'HiggsDecayWidth', floatothers, ranges, rob)

if meas == 'mass':
	runcombineformass(fname.split('.')[0], computer, syst, totcat, seed, '1', ranges)
	#runcombineforwidth(fname.split('.')[0], computer, syst, totcat, seed, 'MH', floatothers, ranges)
if meas == 'masswidth':
	runcombineformasswidth(fname.split('.')[0], computer, syst, totcat, seed)
