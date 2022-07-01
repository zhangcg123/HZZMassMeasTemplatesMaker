import os
import sys
from multiprocessing import Pool

def run(inputdir, inputfile, outputdir, job):
	os.system('./Slimmer.exe ' + inputdir + ' ' + inputfile + ' ' + outputdir + ' ' + job + ' > Dump/' + inputfile + job + '.log')

#inputdir = '/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/'
inputdir = '/afs/cern.ch/user/c/chenguan/public/zxsectionfromfilippo/'
outputdir = '/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/ToyStudy/SlimForToyStudy/OutPutTrees/' 
inputfile = sys.argv[1].split('/')[-1].split('.')[-2]
p=Pool(10)
for i in range(10):
	p.apply_async(func=run,args=(inputdir,inputfile,outputdir,str(i)))
p.close()
p.join()
os.system('hadd OutPutTrees/'+inputfile+'.root OutPutTrees/'+inputfile+'_*root')
os.system('rm OutPutTrees/'+inputfile+'_?.root')
