import os
import sys
import json
import glob
import ROOT
from multiprocessing import Pool
sys.path.append(os.getcwd() + '/include/')
from patches import *

def stepbystep(year, fs, fs_name, tag, type_, cat, dic):
	#########step I##############mass4l error categorization
	stepone_plotpath = baseplotpath + 'Categs/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag
	if dic['step1']:
		dirfunc('make categs', stepone_plotpath, baseplotpath)
		os.system('./GetBounds.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + stepone_plotpath + '/' )

	#########step II##############GGF signal line shape
	steptwo_plotpath1 = baseplotpath + 'GGHShape/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag
	steptwo_plotpath2 = baseplotpath + 'GGHShape/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag + '/SimFit'
	steptwo_plotpath3 = baseplotpath + 'GGHShape/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag + '/SimApproxFit'
	pathlist['ggh_sigshape_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = steptwo_plotpath2 + '/'
	pathlist['ggh_approx_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = steptwo_plotpath3 + '/'
	if dic['step2']:
		dirfunc('ggf shapes path1', steptwo_plotpath1, baseplotpath)
		dirfunc('ggf shapes path2', steptwo_plotpath2, baseplotpath)
		dirfunc('ggf shapes path3', steptwo_plotpath3, baseplotpath)
		os.system('./SignalShape.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + steptwo_plotpath1 + '/ ' + steptwo_plotpath2 +'/ ' + steptwo_plotpath3 + '/ ' + stepone_plotpath + '/' )
		collectinfo(steptwo_plotpath2)
		collectinfo(steptwo_plotpath3)
	
	#########step III#############WH sig line shape
	steptree_plotpath = baseplotpath + 'WHShape/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag
	pathlist['wh_sigshape_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = steptree_plotpath + '/'
	if dic['step3']:
		dirfunc('wh shapes', steptree_plotpath, baseplotpath)
		os.system('./VHShape.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + steptree_plotpath + '/ ' + stepone_plotpath + '/ ' + steptwo_plotpath1 + '/ wh') 
		collectinfo(steptree_plotpath)
			
	#########step IV##############ZH sig line shape
	stepfour_plotpath = baseplotpath + 'ZHShape/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag
	pathlist['zh_sigshape_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepfour_plotpath + '/'
	if dic['step4']:
		dirfunc('zh shapes', stepfour_plotpath, baseplotpath)
		os.system('./VHShape.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + stepfour_plotpath + '/ ' + stepone_plotpath + '/ ' + steptwo_plotpath1 + '/ zh')
		collectinfo(stepfour_plotpath)

	#########step V##############QQZZ bkg line shape
	stepfive_plotpath = baseplotpath + 'QQZZShape/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag
	pathlist['qqzz_bkgshape_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepfive_plotpath + '/'
	if dic['step5']:
		dirfunc('qqzz shape', stepfive_plotpath, baseplotpath)
		os.system('./BKGShape.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + stepfive_plotpath + '/ ' + stepone_plotpath + '/ qqzz')
		collectinfo(stepfive_plotpath)

	#########step VI#############GGZZ bkg line shape
	stepsix_plotpath = baseplotpath + 'GGZZShape/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag
	pathlist['ggzz_bkgshape_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepsix_plotpath + '/'
	if dic['step6']:
		dirfunc('ggzz shape', stepsix_plotpath, baseplotpath)
		os.system('./BKGShape.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + stepsix_plotpath + '/ ' + stepone_plotpath + '/ ggzz')
		collectinfo(stepsix_plotpath)

	#########step VII############sig event yields
	siglist = ['ggh', 'vbf', 'wph', 'wmh', 'zh', 'tth','bbh','thq']
	for proc in siglist:
		stepseven_plotpath = baseplotpath + 'SIGNorm/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag +'/' + proc
		pathlist[proc + '_signorm_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepseven_plotpath + '/'
		if dic['step7']:
			dirfunc('norm events', stepseven_plotpath, baseplotpath)
			os.system('./EventYield.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + stepseven_plotpath + '/ ' + stepone_plotpath + '/ ' + proc) 
			collectinfo(stepseven_plotpath)

	#########step VIII###########bkg event yields
	bkglist = ['ggzz','qqzz']
	for proc in bkglist:
		stepeight_plotpath = baseplotpath + 'BKGNorm/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag + '/' + proc
		pathlist[proc + '_bkgnorm_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepeight_plotpath + '/'
		if dic['step8']:
			dirfunc('bkgnorm events', stepeight_plotpath, baseplotpath)
			os.system('./BKGEventYield.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + stepeight_plotpath + '/ ' + stepone_plotpath + '/ ' + proc)	
			collectinfo(stepeight_plotpath)

	#########step IX#############KD map
	proclist = ['ggh','ggzz','qqzz']
	for proc in proclist:
		stepnine_plotpath = baseplotpath + 'KDMap/' + type_ + '/' + year + '/Categ_' + fs_name + '_' +str(cat) + '_' + tag + '/' + proc
		pathlist[proc + '_kdmap_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepnine_plotpath + '/'
		if dic['step9']:
			dirfunc('kdmap', stepnine_plotpath, baseplotpath)
			os.system('./KDMap.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + stepnine_plotpath + '/ ' + stepone_plotpath + '/ ' + proc )
			collectroot(stepnine_plotpath)
						
	#########step X##############patch for WH
	stepten_plotpath = baseplotpath + 'SIGNorm/' + type_ + '/' + year + '/Categ_' + fs_name + '_' +str(cat) + '_' + tag + '/wh'
	pathlist['WH_signorm_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepten_plotpath + '/'
	indir1 = baseplotpath + 'SIGNorm/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag +'/wph/'
	indir2 = baseplotpath + 'SIGNorm/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag +'/wmh/'
	if dic['step10']:
		dirfunc('patch for WH', stepten_plotpath, baseplotpath)
		os.system('./PatchForWH.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' +stepten_plotpath + '/ ' + indir1 + ' ' + indir2 )
		collectinfo(stepten_plotpath)

	#########step XI#############ZX yields
	stepeleven_plotpath_norm = baseplotpath + 'BKGNorm/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag + '/zx'
	stepeleven_plotpath_para = baseplotpath + 'ZXShape/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag
	stepeleven_plotpath_kdmp = baseplotpath + 'KDMap/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag + '/zx'
	pathlist['zjets_bkgnorm_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepeleven_plotpath_norm + '/'
	pathlist['zjets_shape_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepeleven_plotpath_para + '/'
	pathlist['zjets_kdmap_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = stepeleven_plotpath_kdmp + '/'
	if dic['step11']:
		dirfunc('zx events', stepeleven_plotpath_norm, baseplotpath)
		dirfunc('zx events', stepeleven_plotpath_para, baseplotpath)
		dirfunc('zx events', stepeleven_plotpath_kdmp, baseplotpath)
		os.system('./ZXEventYield.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + stepeleven_plotpath_norm + '/ ' + stepeleven_plotpath_para + '/ ' + stepeleven_plotpath_kdmp + '/ ' + stepone_plotpath + '/')
		collectinfo(stepeleven_plotpath_norm)
		collectinfo(stepeleven_plotpath_para)
		collectroot(stepeleven_plotpath_kdmp)

	#########step XII############Dm template
	histolist = ['ggh','ggzz','qqzz']
	for proc in histolist:
		steptwelve_plotpath = baseplotpath + 'DmHisto/' + type_ + '/' + year + '/Categ_' + fs_name + '_' + str(cat) + '_' + tag + '/' + proc
		pathlist[proc + '_dm_' + year + '_' + fs_name + '_' + type_ + '_' + tag + '_' + cat] = steptwelve_plotpath + '/'
		if dic['step12']:
			dirfunc('dm template', steptwelve_plotpath, baseplotpath)
			os.system('./DmHisto.exe ' + year + ' ' + type_ + ' ' + tag + ' ' + fs + ' ' + cat + ' ' + steptwelve_plotpath + '/ ' + stepone_plotpath + '/ ' + proc )
			collectroot(steptwelve_plotpath)

year = sys.argv[1]
cat = sys.argv[2]
type_ = sys.argv[3]
dirsuffix = sys.argv[4]
tag_ = 'notag'
assert year in ['20160','20165','2017','2018','2019'] and type_ in ['reco','refit','bs','bsrefit'] and int(cat) in [1,2,3,4,5,6,7,8,9,10,11,12,13], 'ERRORRRRRRRRRRRRRRRRRR'

baseplotpath = baseplotpath + dirsuffix+'/'
if not os.path.exists(baseplotpath):
	#mk
	os.system('mkdir '+baseplotpath)


p=Pool(4)	#four final states parallel
for i in finalstates:
	p.apply_async(func=stepbystep,args=(year,i,finalstates[i],tag_,type_,str(cat),dic))
p.close()
p.join()

#######step XIII############Summary in json for data cards mechinery, and in html for you.
if dic['step13']:
	
	stepthirteen_plotpath = baseplotpath + '/JSON/' + year + '_' + str(cat) + 'categs_'+type_+'_'+tag_
	dirfunc('make json',stepthirteen_plotpath, baseplotpath)

	pathdicttojson={}
	for i in pathlist.keys():
		pathdicttojson[i] = pathlist[i]
	with open(stepthirteen_plotpath + '/' + year + '_' + str(cat) + 'categs_'+type_+'_'+tag_+'_abspath.json', 'w') as fp:
		json.dump(pathdicttojson,fp)
	
	infodicttojson={}	
	for i in infolist.keys():
		infodicttojson[i] = infolist[i]
	with open(stepthirteen_plotpath + '/' + year + '_' + str(cat) + 'categs_'+type_+'_'+tag_+'_params.json','w') as fp:
		json.dump(infodicttojson,fp)

	fo = ROOT.TFile.Open(stepthirteen_plotpath + '/' + year + '_' + str(cat) + 'categs_'+type_+'_'+tag_+'_templates.root','RECREATE')
	histolist = []
	for name in rootlist:		
		fi = ROOT.TFile.Open(name,'READ')
		keylist = fi.GetListOfKeys()
		for key in keylist:
			h = key.ReadObj()
			h.SetDirectory(0)
			h.SetName(key.GetName())				#why the obj name changed?
			histolist.append(h)
		fi.Close()
	fo = ROOT.TFile.Open(stepthirteen_plotpath + '/' + year + '_' + str(cat) + 'categs_'+type_+'_'+tag_+'_templates.root','RECREATE')
	fo.cd()
	for h in histolist:
		h.Write()
	fo.Close()
	
	#for i in infodicttojson:
	#	print i, infodicttojson	
