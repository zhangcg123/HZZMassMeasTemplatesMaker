import os
import sys
import glob
from multiprocessing import Manager

finalstates = {'1':'4mu', '2':'4e', '3':'2e2mu', '4':'2mu2e'}
baseplotpath = '/eos/user/c/chenguan/www/HZZMASSTEMPLATES_'

manager = Manager()
pathlist = manager.dict()
infolist = manager.dict()
rootlist = manager.list()

dic={
	'step1':True,	#mass error categorization
	'step2':True,	#signal shape parametrization
	'step3':True,	#wh shape
	'step4':True,	#zh shape
	'step5':True,	#qqzz shape
	'step6':True,	#ggzz shape
	'step7':True,	#signal norm
	'step8':True,	#qqzz and ggzz norm
	'step9':True,	#kd map
	'step10':True,	#patch for wh
	'step11':True,	#every thing for zx
	'step12':True,	#dm template
	'step13':True,	#summary
}

def dirfunc(stepname, dirname, rename):
        
	baseplotpath = rename

	os.system('mkdir -p ' + dirname)                                                        #every time make sure the dir exists
        dirlist = dirname.split('/')
	del dirlist[0]		#the first element is empty
	
	os.system('rm -rf ' + dirname + '/*')                                               	#every time make sure the dir is empty
       	
	tmpdir = baseplotpath
	os.system('cp /eos/user/c/chenguan/www/index.php ' + tmpdir)
        for i in range(6,len(dirlist)):
                tmpdir = tmpdir + dirlist[i] + '/'
                os.system('cp /eos/user/c/chenguan/www/index.php ' + tmpdir)                    #every time make sure the dir is visible through browser

def collectinfo(path):

	sys.path.append(path + '/')
	file_list = glob.glob(path + '/*py')
	for f in file_list:
		tmp_dict = __import__(f.split('/')[-1].split('.')[0],globals(),locals(),['info'],-1)
		for p in tmp_dict.info:
			infolist[p] = tmp_dict.info[p]

def collectroot(path):

	file_list = glob.glob(path + '/*root')
	for f in file_list:
		rootlist.append(f)
