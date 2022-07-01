import os
import sys
import glob
do = sys.argv[1]
prefix = os.getcwd().split('/')[-1]
finalstates = 'allfinalstates'
categories = 'allcategs'
model = 'widthmodelforMH'
floatothers = 'floatothers1'
massrange = 'MH_124_126'
widthrange = 'width_0_3p0'
seed = 'seed1234567'
syst = 'withoutsyst'
jobid = 'unknowid'

dir_ = prefix+'_'+finalstates+'_'+categories+'_'+model+'_'+floatothers+'_'+massrange+'_'+widthrange+'_'+seed+'_'+syst+'_condor'+jobid
os.mkdir(dir_)

outlist = glob.glob('./*'+finalstates+'*'+categories+'*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'*'+jobid+'*out')
errlist = glob.glob('./*'+finalstates+'*'+categories+'*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'*'+jobid+'*err')
loglist = glob.glob('./*'+finalstates+'*'+categories+'*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'*'+jobid+'*log')
rootlist = glob.glob('./*'+finalstates+'*'+categories+'*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'*'+'*root')#no job id within root out put files
condorsh = glob.glob('./*'+finalstates+'*'+categories+'*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'*sh')
condorsub = glob.glob('./*'+finalstates+'*'+categories+'*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'*sub')
txt2wsp = glob.glob('./*'+finalstates+'*'+categories+'*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'.root')
combineoutputs = glob.glob('./*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'*combineoutput*.root')#finalstates and categories name is not here, should be updated
combineoutputs = combineoutputs + glob.glob('./*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'.pdf')
combineoutputs = combineoutputs + glob.glob('./*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'.root')
combineoutputs = combineoutputs + glob.glob('./*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'.png')
print '###########################################################################################'
print ''
print 'template:'+'./*'+finalstates+'*'+categories+'*'+model+'*'+floatothers+'*'+widthrange+'*'+massrange+'*'+seed+'*'+syst+'*'+jobid
print ''
print '###########################################################################################'
print ''
print '#####################condor.out'
for i in outlist:
	print i
	if do == '1':os.system('mv '+i+' '+dir_)
print ''
print '####################condor.err'
for i in errlist:
	print i
	if do == '1':os.system('mv '+i+' '+dir_)
print ''
print '####################condor.log'
for i in loglist:
	print i
	if do == '1':os.system('mv '+i+' '+dir_)
print ''
print '####################condor.root'
for i in rootlist:
	print i
	if do == '1':os.system('mv '+i+' '+dir_)
print ''
print '####################condor.sh'
for i in condorsh:
	print i
	if do == '1':os.system('mv '+i+' '+dir_)
print ''
print '####################condor.sub'
for i in condorsub:
	print i
	if do == '1':os.system('mv '+i+' '+dir_)
print ''
print '####################txt2wsp'
for i in txt2wsp:
	print i
	#if do == 1:os.system('mv '+i+' '+dir_)
print ''
print '####################combineoutputs'
for i in combineoutputs:
	print i
	if do == '1':os.system('mv '+i+' '+dir_)
