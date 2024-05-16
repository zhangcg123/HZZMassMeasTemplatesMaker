import os
def getpull( inputdir, inputfilename ):
	import ROOT
	nuisname = []
	nuisprefit = []
	nuispremin = []
	nuispremax = []
	digi = 9
	## pre-fit
	f = ROOT.TFile(inputdir+'/'+inputfilename,'r')
	w = f.Get('w')
	a = w.obj('ModelConfig').GetNuisanceParameters()
	iter = a.createIterator()
	var = iter.Next()
	while var:
	    nuisname.append( var.GetName() )
	    nuispremin.append( 0-var.getError() )
	    nuispremax.append( var.getError() )
	    nuisprefit.append( var.getVal() )
	    var = iter.Next()
	f.Close()
	import pandas as pd
	pre = pd.DataFrame( {'name':nuisname,
			'pre-min' : nuispremin,
			'pre-max' : nuispremax,
			'pre-fit' : nuisprefit})
	## post-fit
	with open(inputdir+'/nuisance_summary.txt','r') as fin:
		lines = [ [name.strip().split(':')[0].replace('_nuisance',''),round(float(name.strip().split(':')[-1].split(' ')[0][1:-1]),digi),round(float(name.strip().split(':')[-1].split(' ')[1][:-1]),digi),round(float(name.strip().split(':')[-1].split(' ')[2][:-1]),digi) ] for name in fin ][:-3]
	post = pd.DataFrame( lines, columns=['name','post-fit','post-max','post-min'] )
	result = pd.merge(pre,post,on='name')
	result = result[['name','pre-fit','post-fit','pre-min','post-min','pre-max','post-max']]
	return result


def getimpact( dirr ):
    
    import pandas

    digi = 9
    with open(dirr+'/impact_DN.txt','r') as fin:
        ls = [ l for l in fin ]
    dns = pandas.DataFrame( { name.strip().split(':')[0].replace('_DN','') : [round(float(name.strip().split(':')[-2]),digi)] for name in ls}  ).T
    dns.rename(columns = {0:'dns'}, inplace = True )
    best1 = pandas.DataFrame( { name.strip().split(':')[0].replace('_DN','') : [round(float(name.strip().split(':')[-1]),digi)] for name in ls}  ).T
    best1.rename( columns = {0:'best1'}, inplace = True )

    with open(dirr+'/impact_UP.txt','r') as fin:
        ls = [ l for l in fin ]
    ups = pandas.DataFrame( { name.split(':')[0].replace('_UP','') : [round(float(name.strip().split(':')[-2]),digi)] for name in ls }).T
    best2 = pandas.DataFrame( { name.split(':')[0].replace('_UP','') : [round(float(name.strip().split(':')[-1]),digi)] for name in ls }).T
    ups.rename(columns = {0:'ups'} , inplace = True )
    best2.rename(columns = {0:'best2'}, inplace = True )


    newtb = pandas.concat( [dns, best1, ups, best2], axis=1 )
    newtb['dns-best1'] = newtb['dns']-newtb['best1']
    newtb['ups-best2'] = newtb['ups']-newtb['best2']
    newtb = newtb[['dns','best1','dns-best1','ups','best2','ups-best2']]


    print newtb
'''
for finalstate in ['all']:
	pull = getpull( finalstate )
	impacts = getimpact( finalstate )
	
	import pandas as pd
	finalresult = pd.merge( pull, impacts, on='name' )
	print finalresult
	dict = {'params':[],'POIs':[],"method": "default"}
	for n in range(len(finalresult.index)):
		ele = { 'name': finalresult.loc[n,'name'],
			'prefit': [finalresult.loc[n,'pre-min'],finalresult.loc[n,'pre-fit'],finalresult.loc[n,'pre-max']],
			'fit': [finalresult.loc[n,'post-min'],finalresult.loc[n,'post-fit'],finalresult.loc[n,'post-max']],
			'groups':[],
			'type':'Gaussian',
			'impact_HiggsDecayWidth':finalresult.loc[n,'hi'],
			'HiggsDecayWidth':[finalresult.loc[n,'hi'],0,0]#finalresult.loc[n,'center'],finalresult.loc[n,'center']]
			}
		dict['params'].append(ele)
	poi = {'name':'HiggsDecayWidth',
		'fit':[0,impacts.loc[len(impacts.index)-1,'center'],impacts.loc[len(impacts.index)-1,'hi']] 
		}
	dict['POIs'].append(poi)
	import json
	with open('aaaa.json','w+') as fo:
		json.dump(dict,fo,sort_keys=True,indent=4)
	fo.close()
'''
