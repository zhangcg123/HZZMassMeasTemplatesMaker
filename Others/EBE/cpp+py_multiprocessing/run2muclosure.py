import ROOT
import sys
import os
import json
import glob
from multiprocessing import Pool
from array import array
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *
from PlotClass import *
#dirs = DirTree()
#dirs.mkrootdir('test_2017_mc_2muclosure_withoutexp_set3000bins_setbuffer0p3_70_110')
path = sys.argv[1]

def run(year,ismc,path,step,index,low,high):
	print './Closure2Muons.exe '+year+' '+ismc+' '+path+' '+step+' '+str(index)+' '+str(low)+' '+str(high)	
	os.system('./Closure2Muons.exe '+year+' '+ismc+' '+path+' '+step+' '+str(index)+' '+str(low)+' '+str(high))

#year=sys.argv[2]
#ismc=sys.argv[3]
#year='2017'
#ismc='mc'
#os.system('./Closure2Muons.exe '+year+' '+ismc+' '+path + ' 1 0 0 0')# separate events evenly

fin = open(glob.glob( path + '/*bounds.json' )[0], 'r' )
bounds = json.load(fin)
print bounds
argss = []
for i in range(len(bounds)-1):
	argss.append( [ bounds[str(i)],	bounds[str(i+1)] ] )
'''
p=Pool(5)
for i in range(len(argss)):
	p.apply_async(func=run,args=(year,ismc,path,'2',i,argss[i][1],argss[i][0]))
p.close()
p.join()
'''
x,y,xerr,yerr = array('d'),array('d'),array('d'),array('d')
xuncorr = array('d')

for i in range(len(argss)):
	fname = glob.glob(path + '/' + str(i) + '.json')[0]
	with open( fname, 'r' ) as fin:
		dict = json.load( fin )
	x.append( dict['predicted'] )
	y.append( dict['sigma'] )
	yerr.append( dict['sigmaerr'] )
	xerr.append( 0 )
	xuncorr.append( dict['predicted_uncorr'] )

print 'predicted corr',x
print 'measured', y
print 'predicted uncorr', xuncorr

g1 = ROOT.TGraphErrors(10,x,y,xerr,yerr)
g2 = ROOT.TGraphErrors(10,xuncorr,y,xerr,yerr)
g1.GetYaxis().SetTitleOffset(1.5)
g1.SetMarkerStyle(20)
g2.SetMarkerStyle(20)
g1.SetMarkerColor(2)
g1.SetTitle("")
g1.GetXaxis().SetTitle("Predicted #sigma_{m}")
g1.GetYaxis().SetTitle("Measured #sigma_{m}")
g1.GetXaxis().SetLimits(0,3)
g1.GetHistogram().SetMaximum(3)
g1.GetHistogram().SetMinimum(0)
l1 = ROOT.TLine(0,0,3,3)
l2 = ROOT.TLine(0,0,3/1.1,3)
l3 = ROOT.TLine(0,0,3,3/1.1)
# l1 as read, other two as blue
l1.SetLineColor(2)
l2.SetLineColor(4)
l3.SetLineColor(4)
c = ROOT.TCanvas('c','',1000,1000)	
g1.Draw('ap')
g2.Draw('p')
l1.Draw('same')
l2.Draw('same')
l3.Draw('same')
# add legend at the right-bottom coner, g1 name corr_MC, g2 name uncorr_MC
leg = ROOT.TLegend(0.7,0.1,0.9,0.3)
leg.AddEntry(g1,'corr_MC','p')
leg.AddEntry(g2,'uncorr_MC','p')
leg.Draw('same')
c.SaveAs(path + '/closure.png')
c.SaveAs(path + '/closure.pdf')