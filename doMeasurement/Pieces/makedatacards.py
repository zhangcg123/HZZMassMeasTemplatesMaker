import os
import sys
import glob
sys.path.append('./include/')
from inputReader import *
from datacardClass import *
from multiprocessing import Pool

def makedir(dic, updatejson=True):
	
	year = dic['year']
	type_ = dic['type']											#reco refit bs bsrefit
	totcat = dic['totcat']
	tag = dic['tag']
	fft = dic['FFT']
	inputs = dic['inputdir']

	kdmname = ''
	if dic['isdm'] and dic['iskd']: kdmname = 'mdmkd_'
	if dic['isdm'] and not dic['iskd']: kdmname = 'mdm_'
	if not dic['isdm'] and dic['iskd']: kdmname = 'mkd_'
	if not dic['isdm'] and not dic['iskd']: kdmname = 'm_'
	
	mname = ''
	if dic['monly']: mname = '_masspdf'
	if not dic['monly'] and dic['FFT']: mname = '_widthpdf'
	if not dic['monly'] and not dic['FFT']: mname = '_widthpdf_approx'
	
	#for output dir
	name2 = year + '_' + type_ + '_' + kdmname + str(totcat) + '_' + dic['tag'] + mname
	os.system('mkdir -p datacardsOutputs_KD'+str(dic['interp'])+'_'+inputs+'_'+dic['suffix']+'/' + name2)
	os.system('rm -rf datacardsOutputs_KD'+str(dic['interp'])+'_'+inputs+'_'+dic['suffix']+'/' + name2 + '/*')
	os.system('cp Pieces/launchcombine.py datacardsOutputs_KD'+str(dic['interp'])+'_'+inputs+'_'+dic['suffix']+'/' + name2 + '/')
	os.system('cp Pieces/combcards.py datacardsOutputs_KD'+str(dic['interp'])+'_'+inputs+'_'+dic['suffix']+'/' + name2 + '/')

	#for input dir
	name1 = year + '_' + type_ + '_' + str(totcat) + '_' + dic['tag']
	if os.path.exists('datacardsInputs_' + inputs + '/' + name1):
		os.system('rm -rf datacardsInputs_'+inputs+'/'+name1+'/*')
	else:
		os.system('mkdir -p datacardsInputs_'+inputs + '/' + name1)	
	#copy to input dir	
	if dic['tag'] == 'notag':
		name3 = year + '_' + str(totcat) + 'categs_' + type_ + '_notag'
		os.system('cp /eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+inputs+'/JSON/' + name3 + '/*params.json datacardsInputs_'+inputs+'/' + name1 + '/')
		os.system('cp /eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+inputs+'/JSON/' + name3 + '/*root datacardsInputs_'+inputs+'/' + name1 + '/')
	if dic['tag'] != 'notag':
		name3 = year + '_' + str(totcat) + 'categs_' + type_ + '_untag'
		name4 = year + '_' + str(totcat) + 'categs_' + type_ + '_vbftag'
		os.system('cp /eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+inputs+'/JSON/' + name3 + '/*params.json datacardsInputs_'+inputs+'/' + name1 + '/')
		os.system('cp /eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+inputs+'/JSON/' + name3 + '/*root datacardsInputs_'+inputs+'/' + name1 + '/')
		os.system('cp /eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+inputs+'/JSON/' + name4 + '/*params.json datacardsInputs_'+inputs+'/' + name1 + '/')
		os.system('cp /eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+inputs+'/JSON/' + name4 + '/*root datacardsInputs_'+inputs+'/' + name1 + '/')

def makecards(dic, fs):
	mh = 125.0
	year = dic['year']
	
	myReader = inputReader('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/commoninputs/SM_inputs_13TeV/inputs_'+fs+'.txt')
	myReader.readInputs()
	theInputs = myReader.getInputs()
	
	totcat = int(dic['totcat'])
	iskd = dic['iskd']
	isdm = dic['isdm']
	type_ = dic['type']
	fft = dic['FFT']

	indir = 'datacardsInputs_'+dic['inputdir']+'/' + year + '_' + type_ + '_' + str(totcat) + '_' + dic['tag']           
	
	kdmname = ''
	if dic['isdm'] and dic['iskd']: kdmname = 'mdmkd_'
	if dic['isdm'] and not dic['iskd']: kdmname = 'mdm_'
	if not dic['isdm'] and dic['iskd']: kdmname = 'mkd_'
	if not dic['isdm'] and not dic['iskd']: kdmname = 'm_'
	
	mname = ''
	if dic['monly']: mname = '_masspdf'
	if not dic['monly'] and dic['FFT']: mname = '_widthpdf'
	if not dic['monly'] and not dic['FFT']: mname = '_widthpdf_approx'
	outdir = 'datacardsOutputs_KD'+str(dic['interp'])+'_'+dic['inputdir']+'_'+dic['suffix']+'/' + year + '_' + type_ + '_' + kdmname + str(totcat) + '_' + dic['tag'] + mname

	monly = dic['monly']

	#make cards
	myClass = datacardClass()
	myClass.loadIncludes()

	for thecat in range(totcat):
		if dic['tag'] == 'notag': myClass.makeCardsWorkspaces(mh, year, theInputs, totcat, thecat, iskd, isdm, type_, indir, outdir, False, 0, monly, fft, dic['interp'])
		if dic['tag'] != 'notag':
			 myClass.makeCardsWorkspaces(mh, year, theInputs, totcat, thecat, iskd, isdm, type_, indir, outdir, True, 0, monly, fft, dic['interp'])
			 myClass.makeCardsWorkspaces(mh, year, theInputs, totcat, thecat, iskd, isdm, type_, indir, outdir, True, 1, monly, fft, dic['interp'])



dic = {	
	'year' : sys.argv[1],
	'totcat' : sys.argv[2],
	'iskd' : False,
	'isdm' : False,
	'type' : sys.argv[3],
	'tag' : 'notag',
	'monly' : False,
	'FFT' : True,
	'inputdir' : sys.argv[4],
	'interp' : sys.argv[5],
	'suffix' : sys.argv[6]
}
assert int(dic['totcat'])<14 and dic['tag'] in ['notag','vbftag'] and dic['type'] in ['reco','refit','bs','bsrefit'], 'ERRORRRRRRRRRRRRRRRRRRRRRRRRRR'

if not os.path.exists('/eos/user/c/chenguan/www/HZZMASSTEMPLATES_'+dic['inputdir']):
	print 'no input HZZ TEMPLATES dir in eos'
	sys.exit()

for kd in [False,True]:
	for dm in [False]:
		dic['iskd'] = kd
		dic['isdm'] = dm
		makedir(dic)
		p=Pool(1)
		for fs in ['4mu','4e','2e2mu','2mu2e']:
			p.apply_async(func=makecards(dic,fs,))
		p.close()
		p.join()
