import os
import sys
from multiprocessing import Pool

def run(inputdir, inputfile, outputdir, job):
	os.system('./Slimmer.exe ' + inputdir + ' ' + inputfile + ' ' + outputdir + ' ' + job + ' > Dump/' + inputfile + job + '.log')

inputdir = '/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/2018/'
#inputdir = '/eos/user/f/ferrico/www/FullRun2/20165/'
outputdir = '/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/' 
inputfile = sys.argv[1].split('/')[-1].split('.')[-2]
if 'TTTo2L2Nu' in inputfile or 'WZTo3LNu' in inputfile or 'DYJetsToLL_M-50' in inputfile:
	print 'skip this:', inputfile
	sys.exit()

p=Pool(10)
for i in range(10):
	p.apply_async(func=run,args=(inputdir,inputfile,outputdir,str(i)))
p.close()
p.join()
os.system('hadd OutPutTrees/'+inputfile+'.root OutPutTrees/'+inputfile+'_*root')
os.system('rm OutPutTrees/'+inputfile+'_?.root')
