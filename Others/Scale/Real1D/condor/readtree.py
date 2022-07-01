import ROOT
import os
import sys
import json
import numpy

def ReadTree(year, isdata, mass, k, charge, bin_, low, high, inputpath, binned):

	etacuts = [str(-2.4),str(-2.4*(3.0/4.0)),str(-2.4*(2.0/4.0)),str(-2.4*(1.0/4.0)),'0.0',str(2.4*(1.0/4.0)),str(2.4*(2.0/4.0)),str(2.4*(3.0/4.0)),str(2.4)]
	phicuts = [str(-numpy.pi),str(-numpy.pi*(3.0/4.0)),str(-numpy.pi*(2.0/4.0)),str(-numpy.pi*(1.0/4.0)),'0.0',str(numpy.pi*(1.0/4.0)),str(numpy.pi*(2.0/4.0)),str(numpy.pi*(3.0/4.0)),str(numpy.pi)]

	if k == 'eta' and charge == 'pos':
		subcut = '((mu1_q_roc== 1 && eta1 > ' + etacuts[int(bin_)] + '&& eta1 < ' + etacuts[int(bin_)+1] + ') ||( mu2_q_roc == 1 && eta2 > ' + etacuts[int(bin_)] + '&& eta2 < ' + etacuts[int(bin_)+1]+'))'
	elif k == 'eta' and charge == 'neg':
		subcut = '((mu1_q_roc==-1 && eta1 > ' + etacuts[int(bin_)] + '&& eta1 < ' + etacuts[int(bin_)+1] + ') ||( mu2_q_roc ==-1 && eta2 > ' + etacuts[int(bin_)] + '&& eta2 < ' + etacuts[int(bin_)+1]+'))'
	elif k == 'phi' and charge == 'pos':
		subcut = '((mu1_q_roc== 1 && phi1 > ' + phicuts[int(bin_)] + '&& phi1 < ' + phicuts[int(bin_)+1] + ') ||( mu2_q_roc == 1 && phi2 > ' + phicuts[int(bin_)] + '&& phi2 < ' + phicuts[int(bin_)+1]+'))'
	elif k == 'phi' and charge == 'neg':
		subcut = '((mu1_q_roc==-1 && phi1 > ' + phicuts[int(bin_)] + '&& phi1 < ' + phicuts[int(bin_)+1] + ') ||( mu2_q_roc ==-1 && phi2 > ' + phicuts[int(bin_)] + '&& phi2 < ' + phicuts[int(bin_)+1]+'))'

	basecut = 'mu1_hzzid && mu2_hzzid && (trigger24matching1 || trigger24matching2) && RelIso1 < 0.35 && RelIso2 < 0.35 && Sip1 < 4.0 && Sip2 < 4.0 && pt1_corr > 25 && pt2_corr > 5 && abs(eta1) < 2.4 && abs(eta2) < 2.4 &&'
	customcut = basecut + mass + ' < ' + str(high) + ' && ' + mass + ' > '+str(low) + ' && ' + subcut
	print customcut

	tmp = ROOT.RDataFrame('passedEvents',inputpath)
	tmp.Filter(customcut).Snapshot('passedEvents','tmp.root',mass)
	f = ROOT.TFile.Open('tmp.root','READ')
	t=f.Get('passedEvents')
	
	#build model
	x = ROOT.RooRealVar(mass,'',low,high)
	x.setBins(int((high-low)*1000),"cache")
	width = 2.496 if isdata == 'data' else 2.441
	peak = 91.19 if isdata == 'data' else 91.19+0.034
	pdgmz = ROOT.RooRealVar('pdgmz','',peak)
	pdgwz = ROOT.RooRealVar('pdgwz','',width)
	pdgmz.setConstant(True)
	pdgwz.setConstant(True)
	bw = ROOT.RooBreitWigner('bw','',x,pdgmz,pdgwz)
	m = ROOT.RooRealVar('m','',0,-5,5)
	s = ROOT.RooRealVar('s','',1,0,10)
	n1 = ROOT.RooRealVar('n1','',10,0,70)
	a1 = ROOT.RooRealVar('a1','',1,0,10)
	n2 = ROOT.RooRealVar('n2','',10,0,70)
	a2 = ROOT.RooRealVar('a2','',1,0,70)
	dcb = ROOT.RooDoubleCB('dcb','',x,m,s,a1,n1,a2,n2)
	model = ROOT.RooFFTConvPdf('dcbxbw','',x,bw,dcb,2)
	model.setBufferFraction(0.2)
	#tau = ROOT.RooRealVar('tau','',0,-1,1)
	#bkg = ROOT.RooExponential('bkg','',x,tau)
	#fsig = ROOT.RooRealVar('fsig','',0.9,0.5,1)
	#model = ROOT.RooAddPdf('model','',dcbxbw,bkg,fsig)
	
	dataset = ROOT.RooDataSet('dataset','',ROOT.RooArgSet(x),ROOT.RooFit.Import(t))
	if binned == 'unbinned':	
		print 'unbinned fit'
		model.fitTo(dataset,ROOT.RooFit.PrintLevel(-1))
		model.fitTo(dataset,ROOT.RooFit.PrintLevel(-1))
	else:
		print 'binned fit'
		datahist = dataset.binnedClone()
		model.fitTo(datahist,ROOT.RooFit.PrintLevel(-1))
		model.fitTo(datahist,ROOT.RooFit.PrintLevel(-1))
	
	chi2_var = ROOT.RooChi2Var('chi2', '', model, dataset.binnedClone(),ROOT.RooFit.DataError(ROOT.RooAbsData.Expected))
	frame = x.frame(ROOT.RooFit.Bins(30))
	frame.SetTitle('')
	dataset.plotOn(frame)
	model.plotOn(frame)
	c = ROOT.TCanvas('c','',1400,1000)
	c.cd()
	frame.Draw()
	lx = ROOT.TLatex()
	lx.SetNDC()
	lx.SetTextSize(0.05)
	lx.SetTextFont(42)
	lx.SetTextAlign(23)
	lx.DrawLatex(0.7,0.9,"#chi^{2}/DOF="+str(chi2_var.getVal()/float(dataset.binnedClone().numEntries())))
	lx.DrawLatex(0.7,0.8,"mean="+str(m.getVal())[0:7]+'+/-'+str(m.getError())[0:7])
	lx.DrawLatex(0.7,0.7,"sigma="+str(s.getVal())[0:7])
	lx.DrawLatex(0.7,0.6,"a1="+str(a1.getVal())[0:7])
	lx.DrawLatex(0.7,0.5,"n1="+str(n1.getVal())[0:7])
	lx.DrawLatex(0.7,0.4,"a2="+str(a2.getVal())[0:7])
	lx.DrawLatex(0.7,0.3,"n2="+str(n2.getVal())[0:7])
	lx.DrawLatex(0.7,0.2,"entries="+str(int(dataset.sumEntries())))
	c.SaveAs(year+'_'+isdata+'_'+mass+'_'+k+'_'+charge+'_'+bin_+'_'+str(int(low))+'_'+str(int(high))+'.png')
	dic={}
	dic[year+'_'+isdata+'_'+mass+'_'+k+'_'+charge+'_'+bin_+'_'+str(int(low))+'_'+str(int(high))+'_peak'] = m.getVal()+peak
	dic[year+'_'+isdata+'_'+mass+'_'+k+'_'+charge+'_'+bin_+'_'+str(int(low))+'_'+str(int(high))+'_peakerr'] = m.getError()
	dic[year+'_'+isdata+'_'+mass+'_'+k+'_'+charge+'_'+bin_+'_'+str(int(low))+'_'+str(int(high))+'_mean'] = dataset.mean(x)
	with open(year+'_'+isdata+'_'+mass+'_'+k+'_'+charge+'_'+bin_+'_'+str(int(low))+'_'+str(int(high))+'.json','w') as fo:
		json.dump(dic,fo)

year = sys.argv[1]
isdata = sys.argv[2]
mass = sys.argv[3]
k = sys.argv[4]
charge = sys.argv[5]
bin_ = sys.argv[6]
low = float(sys.argv[7])
high = float(sys.argv[8])
binned = sys.argv[9]
print year, isdata, mass, k, charge, bin_, low, high, binned
assert isdata in ['mc','data'] and year in ['2016a','2016b','2017','2018'] and charge in ['pos','neg'] and bin_ in ['0','1','2','3','4','5','6','7'] and k in ['phi','eta'] and mass in ['mass2mu_corr','mass2mu_ewk2','mass2mu_ewk_dt','mass2mu_zpt_dt','mass2mu_dm_dt','mass2mu_ewk2_dt'], 'ERRORRRRRRRRRRRRRRRRRR'

if isdata == 'mc': subname = 'LO.root'
if isdata == 'data': subname = 'SingleMu.root'
inputpath = '/eos/user/c/chenguan/Tuples/MuonScaleUsed/19UL_EWK2ASDEF/' + year + '_HZZ_'+subname
ReadTree(year, isdata, mass, k, charge, bin_, low,high, inputpath, binned)
