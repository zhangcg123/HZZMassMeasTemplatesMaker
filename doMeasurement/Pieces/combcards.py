import os
import sys
import glob

def combmasserrcategs(year, fs):
	filelist = glob.glob('hzz4l_'+year+'_'+fs+'_cat?_*tag_13TeV.txt')
	if len(filelist)==1:
		print 'only 1 category, do not need to comb cards, but rename the cards'
		os.system('mv '+filelist[0]+' hzz4l_'+year+'_'+fs+'_allcategs_13TeV.txt')
		return
	if len(filelist)==0:
		print 'no ' + year + ' cards'
		return
	commandstr = 'combineCards.py '
	for name in filelist:
		commandstr = commandstr + ' ' + name
	commandstr = commandstr + ' > hzz4l_' + year + '_' + fs + '_allcategs_13TeV.txt'
	os.system(commandstr)

def combfinalstates(year):
	filelist = glob.glob('hzz4l_'+year+'_??_cat?_*tag_13TeV.txt')			#4e_cat0-9_vbf/untag/notag.txt
	filelist = filelist + glob.glob('hzz4l_'+year+'_???_cat?_*tag_13TeV.txt')	#4mu_cat0-9_vbf/untag/notag.txt
	filelist = filelist + glob.glob('hzz4l_'+year+'_?????_cat?_*tag_13TeV.txt')	#2e2mu/2mu2e_cat0-9_vbf/untag/notag.txt
	if len(filelist)==0:
		print 'no ' + year + ' cards'
		return
	commandstr = 'combineCards.py '
	for name in filelist:
		commandstr = commandstr + ' ' + name
	commandstr = commandstr + ' > hzz4l_' + year + '_allfinalstates_allcategs_13TeV.txt'	#for a specific year, this command combine all final states and all mass err categories
	os.system(commandstr)

def makedirforcondor():
	os.system('mkdir condor_inputfiles')	#originally I wrote a script for condor job submition, right now treat it as a backup
	os.system('cp *_13TeV.input.root condor_inputfiles/')
	os.system('cp *allcategs_13TeV.txt condor_inputfiles/')

if os.path.exists('./condor_inputfiles'):
	print 'this script has been run!'
	sys.exit()

for j in ['20160','20165','2017','2018']:
	combfinalstates(j)
	for i in ['4mu','4e','2e2mu','2mu2e']:
		combmasserrcategs(j, i)
makedirforcondor()
