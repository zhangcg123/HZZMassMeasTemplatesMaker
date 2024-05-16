import os
import sys
from multiprocessing import Pool

def run(inputdir, inputfile, outputdir, job):
	os.system('./Slimmer.exe ' + inputdir + ' ' + inputfile + ' ' + outputdir + ' ' + job + ' > Dump/' + inputfile + job + '.log')

#inputdir = '/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/2018/'
#inputdir = '/eos/user/f/ferrico/www/FullRun2/20165/'

inputdir = '/eos/user/f/ferrico/www/New_Sample_Marzo2023/'
#outputdir = '/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/' 
outputdir = '/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesAug2023/' 

inputfile = sys.argv[1].split('/')[-1].split('.')[-2]

if 'TTTo2L2Nu' in inputfile or 'WZTo3LNu' in inputfile or 'DYJetsToLL_M-50' in inputfile:
	print 'skip this:', inputfile
	sys.exit()

p=Pool(10)
for i in range(10):
	p.apply_async(func=run,args=(inputdir,inputfile,outputdir,str(i)))
p.close()
p.join()

os.system('hadd OutPutTreesAug2023/'+inputfile+'.root OutPutTreesAug2023/'+inputfile+'_*root')
os.system('rm OutPutTreesAug2023/'+inputfile+'_?.root')
