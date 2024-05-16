import os
import sys
import glob
outdir = os.getcwd().split('/')[-1]
indir = sys.argv[1]
syst = sys.argv[2]
poi = sys.argv[3]
finalstate = sys.argv[4]
ranges = {'MH':'124p9_125p1','HiggsDecayWidth':'0p0_0p5'}

if finalstate == '4e' or finalstate == '2e2mu' or finalstate == '2mu2e':
        if syst == 'withoutsyst':
                ranges['MH'] = '124p7_125p3'
                ranges['HiggsDecayWidth'] = '0p0_0p5'
                if finalstate == '4e':
                        ranges['HiggsDecayWidth'] = '0p0_0p8'
        if syst == 'withsyst':
                ranges['MH'] = '124p3_125p7'
                ranges['HiggsDecayWidth'] = '0p0_1p2'
                if finalstate == '4e':
                        ranges['HiggsDecayWidth'] = '0p0_1p8'

if finalstate == '4mu' or finalstate == 'allfinalstates':
        if syst == 'withoutsyst':
                ranges['MH'] = '124p9_125p1'
                ranges['HiggsDecayWidth'] = '0p0_0p25'
        if syst == 'withsyst':
                ranges['MH'] = '124p9_125p1'
                ranges['HiggsDecayWidth'] = '0p0_0p8'
if poi == 'MassWidth':
	if syst == 'withsyst':
		ranges['MH'] = '124p95_125p05'
		ranges['HiggsDecayWidth'] = '0p0_0p3'

substr = '2018_'+finalstate+'_allcategs_widthmodelfor'+poi+'_floatothers1_width_'+ranges['HiggsDecayWidth']+'_MH_'+ranges['MH']+'_'

seedlist = ['toy_total_w']

if not os.path.exists('/eos/user/c/chenguan/www/'+outdir):
	os.system('mkdir /eos/user/c/chenguan/www/'+outdir)
	os.system('cp /eos/user/c/chenguan/www/index.php /eos/user/c/chenguan/www/'+outdir+'/')

for seed in seedlist:
	if not os.path.exists('/eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst):
		os.system('mkdir /eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst)
		os.system('cp /eos/user/c/chenguan/www/index.php /eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst)
	else:
		print 'warnning!!!'
		sys.exit()	
		#for seed in seedlist:
		#	os.system('rm /eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'seed'+seed+'_'+syst+'/*')
		#	os.system('cp /eos/user/c/chenguan/www/index.php /eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'seed'+seed+'_'+syst)
for seed in seedlist:
	outputlist = glob.glob(indir+'/higgs*'+substr+'*fullsimtoys_'+seed+'*'+syst+'*POINT*root')
	print outputlist
	for f in outputlist:
		print f
	if len(outputlist) < 10: 
		print '######## no '+substr+' seed'+seed+' in '+indir 
		continue
	os.system('hadd -f /eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'_haddcombineoutput.root '+indir+'/higgs*'+substr+'*fullsimtoys_'+seed+'*'+syst+'*POINT*root')
	os.system('mv '+indir+'/higgs*'+substr+'*fullsimtoys_'+seed+'_'+syst+'*POINT*root '+'/eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'/')
	os.system('mv '+indir+'/*'+substr+'*fullsimtoys_'+seed+'_'+syst+'*err '+'/eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'/')
	os.system('mv '+indir+'/*'+substr+'*fullsimtoys_'+seed+'_'+syst+'*out '+'/eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'/')
	os.system('mv '+indir+'/*'+substr+'*fullsimtoys_'+seed+'_'+syst+'*log '+'/eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'/')
	os.system('mv '+indir+'/*'+substr+'*fullsimtoys_'+seed+'_'+syst+'*condor.sub '+'/eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'/')
	os.system('mv '+indir+'/*'+substr+'*fullsimtoys_'+seed+'_'+syst+'*condor.sh '+'/eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'/')
	if poi == 'MH' or poi == 'HiggsDecayWidth':
		os.system('python /afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/Pieces/plot1DScan.py /eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'_haddcombineoutput.root --POI '+poi+' -o ' + indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'_1Dscan')
		os.system('mv *png /eos/user/c/chenguan/www/'+outdir+'/'+indir+'_'+substr+'fullsimtoys_'+seed+'_'+syst+'/')

