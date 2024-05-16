import glob
import sys
import os

targetdir = sys.argv[1]
step = sys.argv[2]

def secondlast():
	year = os.getcwd().split('/')[-1].strip('/').split('_')[0]
	finalstate = os.getcwd().split('/')[-1].strip('/').split('_')[-1]
	f = glob.glob('hzz4l_'+year+'_'+finalstate+'_allcategs_13TeV_widthmodelforHiggsDecayWidth_Impacts_*.root')[0]	
	os.system('combineTool.py -M Impacts -d '+f+' -m 125.38 --output impacts.json')


def last():
	year = os.getcwd().split('/')[-1].strip('/').split('_')[0]
	finalstate = os.getcwd().split('/')[-1].strip('/').split('_')[-1]	
	os.system('plotImpacts.py -i impacts.json --POI HiggsDecayWidth -o '+year+'_'+finalstate+'_baseline_width_impacts')
	os.system('plotImpacts.py -i impacts.json --POI MH -o '+year+'_'+finalstate+'_baseline_MH_impacts')
	os.system('plotImpacts.py -i impacts.json --POI r -o '+year+'_'+finalstate+'_baseline_r_impacts')
	os.system('pdftoppm -png -rx 1000 -ry 1000 '+year+'_'+finalstate+'_baseline_width_impacts.pdf '+year+'_'+finalstate+'_baseline_width_impacts')
	os.system('pdftoppm -png -rx 1000 -ry 1000 '+year+'_'+finalstate+'_baseline_MH_impacts.pdf '+year+'_'+finalstate+'_baseline_MH_impacts')
	os.system('pdftoppm -png -rx 1000 -ry 1000 '+year+'_'+finalstate+'_baseline_r_impacts.pdf '+year+'_'+finalstate+'_baseline_r_impacts')


def cleaner(targetdir_):

	year = os.getcwd().split('/')[-1].strip('/').split('_')[0]
	finalstate = os.getcwd().split('/')[-1].strip('/').split('_')[-1]
	files = glob.glob('*err') + glob.glob('*out') + glob.glob('higgs*root') + glob.glob('condor_hzz_*') + glob.glob('*log') + glob.glob('impacts.json') + glob.glob('*.pdf') + glob.glob('*.png') + glob.glob('hzz4l_'+year+'_'+finalstate+'_allcategs_13TeV_widthmodelforHiggsDecayWidth_Impacts_*.root')
	for f in files:
		os.system('mv '+f+' '+targetdir_+'/')
	
if step=='step1':
	secondlast()
if step=='step2':
	last()
if step=='step3':
	cleaner(targetdir)
