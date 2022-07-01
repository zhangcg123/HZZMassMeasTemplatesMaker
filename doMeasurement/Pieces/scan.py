import sys
import os
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
import glob
from ScanerClass import *
from FileSystemClass import *
from HTMLClass import *
year = sys.argv[1]
fs = sys.argv[2]
poi = sys.argv[3]
seed = sys.argv[4]
syst = sys.argv[5]
#dirs = DirTree()
#dirs.mkrootdir('test_new_scanner')
#outputpath = dirs.root
outputpath = os.getcwd()+'/'+fs+'_'+poi
if not os.path.exists(outputpath):
	os.system('mkdir '+outputpath)
	os.system('cp '+os.getcwd()+'/index.php ' + outputpath)
	
outputname = year + '_' + fs + '_' + poi + '_' + seed + '_' + syst
output = outputpath + '/' +outputname
inputlist = glob.glob(os.getcwd()+'/'+year+'_*_'+fs+'_*_widthmodelfor'+poi+'_*seed'+seed+'*'+syst+'/*haddcombineoutput*.root')
if len(inputlist) != 4:
	sys.exit()
for i in inputlist:
	print i
legends = []
for i in inputlist:
	legends.append(i.split('/')[-1].split('.')[-2].split('_notag')[0].split('8_')[-1])
scanner = OneDScannerClass(inputlist, legends, output, poi)
#scanner.shiftxtozero()
#scanner.debugplot()
scanner.findcross()
scanner.makeplot()
scanner.makejson()
scanner.maketable()
