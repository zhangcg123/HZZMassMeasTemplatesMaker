import ROOT
import sys
import os

def readtree(h_std, t, fs, type_, tag, masspoint):
	if type_ == 'refit':
		mass_name = 'mass4lREFIT'
		masserr_name = 'mass4lErrREFIT'
		pt1name = 'pTL1REFIT'
		pt2name = 'pTl2REFIT'
		pt3name = 'pTL3REFIT'
		pt4name = 'pTL4REFIT'
	if type_ == 'reco':
		mass_name = 'mass4l'
		masserr_name = 'mass4lErr'
		pt1name = 'pTL1'
		pt2name = 'pTL2'
		pt3name = 'pTL3'
		pt4name = 'pTL4'
	categs = h_std.GetNbinsX()
	dataset_list = []
	mass4l = ROOT.RooRealVar('mass4l','',105,140)
	for i in range(0,categs):
		dataset = ROOT.RooDataSet('dataset_'+masspoint+'_'+str(i),'',ROOT.RooArgSet(mass4l))
		dataset_list.append(dataset)
	mass4l_ = ROOT.RooRealVar('mass4l','',105,140)
	for i in range(0, t.GetEntries()):
		t.GetEntry(i)
		mass = getattr(t,mass_name)
		masserr = getattr(t,masserr_name)
		pt1 = getattr(t,pt1name)
		pt2 = getattr(t,pt2name)
		pt3 = getattr(t,pt3name)
		pt4 = getattr(t,pt4name)
		if mass>105 and mass<140 and masserr<15 and t.finalState==fs and pt1<100 and pt2<100 and pt3<100 and pt4<100 and abs(t.etaL1)<2.4 and abs(t.etaL2)<2.4 and abs(t.etaL3)<2.4 and abs(t.etaL4)<2.4:
			if tag=='notag' or (tag == 'vbftag' and t.EventCat == 2) or ( tag=='untag' and t.EventCat != 2):
				bin1 = h_std.FindBin(masserr)-1
				mass4l_.setVal(mass)
				dataset_list[bin1].add(ROOT.RooArgSet(mass4l_))
	return dataset_list
	
def ApproxFit(dataset,fs_name,i,plotpath,masspoint):
	mass4l = ROOT.RooRealVar('mass4l','',105,140)
	mass = ROOT.RooRealVar('mass','',masspoint)
	width = ROOT.RooRealVar('width','',0.004)

	shift_ = ROOT.RooRealVar('shift_','',0,-5,5)
	mean_ = ROOT.RooFormulaVar('mean_','@0+'+str(masspoint),ROOT.RooArgList(shift_))
	sigma_ = ROOT.RooRealVar('sigma_','',1,0,10)
	a1_ = ROOT.RooRealVar('a1_','',1,0,10)
	a2_ = ROOT.RooRealVar('a2_','',1,0,10)
	n1_ = ROOT.RooRealVar('n1_','',3)
	n2_ = ROOT.RooRealVar('n2_','',3)
	dcb_ = ROOT.RooDoubleCB('dcb_','',mass4l,mean_,sigma_,a1_,n1_,a2_,n2_)
	dcb_.fitTo(dataset)

	shift = ROOT.RooRealVar('shift','',shift_.getVal(),shift_.getVal()-abs(shift_.getVal())*0.2,shift_.getVal()+abs(shift_.getVal())*0.2)
	sigma = ROOT.RooRealVar('sigma','',1,0,10)
	a1 = ROOT.RooRealVar('a1','',a1_.getVal())
	a2 = ROOT.RooRealVar('a2','',a2_.getVal())
	n1 = 3
	n2 = 3
	theta1 = ROOT.RooRealVar('theta1','',2.3)
	theta2 = ROOT.RooRealVar('theta2','',2.3)
	approx = ROOT.RooaDoubleCBxBW('approx','',mass4l,shift,sigma,a1,a2,mass,width,n1,n2,theta1,theta2,False)
	r = approx.fitTo(dataset,ROOT.RooFit.Save())
	
	frame = mass4l.frame(ROOT.RooFit.Bins(100))
	dataset.plotOn(frame)
	approx.plotOn(frame,ROOT.RooFit.LineColor(4))
	approx.paramOn(frame,ROOT.RooFit.Layout(0.7,0.99,0.99),ROOT.RooFit.Label('dcbxbw'),ROOT.RooFit.ShowConstants(True))
	dcb_.plotOn(frame,ROOT.RooFit.LineColor(2),ROOT.RooFit.LineStyle(2),ROOT.RooFit.LineWidth(4))
	dcb_.paramOn(frame,ROOT.RooFit.Layout(0.7,0.99,0.45),ROOT.RooFit.Label('dcb'),ROOT.RooFit.ShowConstants(True))
	dataset.plotOn(frame)
	approx.plotOn(frame,ROOT.RooFit.LineColor(4))
	dcb_.plotOn(frame,ROOT.RooFit.LineColor(2),ROOT.RooFit.LineStyle(2),ROOT.RooFit.LineWidth(4))
	c = ROOT.TCanvas('c','',1400,1000)
	c.cd()
	frame.Draw()
	lx = ROOT.TLatex()
	lx.SetNDC()
	lx.SetTextSize(0.05)
	lx.SetTextFont(42)
	lx.SetTextAlign(23)
	lx.SetTextColor(2)
	lx.DrawLatex(0.2,0.8,'dcb')
	lx.SetTextColor(4)
	lx.DrawLatex(0.2,0.7,'dcbxbw')
	c.SaveAs(plotpath+'xxxx'+fs_name+'_'+str(i)+'approx.png')
	results = [shift.getVal(),sigma.getVal(),a1.getVal(),a2.getVal()]
	return results

def ApproxSimulFit(dataset_list, r, plotpath, cates, fs_name, type_, tag, year):
	
	#retrieve params
	shift_intercept = ROOT.RooRealVar('shift_intercept','',r[0])
	sigma_intercept = ROOT.RooRealVar('sigma_intercept','',r[1])
	a1_intercept = ROOT.RooRealVar('a1_intercept','',r[2])
	a2_intercept = ROOT.RooRealVar('a2_intercept','',r[3])
	shift_intercept.setConstant(True)
	sigma_intercept.setConstant(True)
	a1_intercept.setConstant(True)
	a2_intercept.setConstant(True)

	#define slope
	shift_slope = ROOT.RooRealVar('shift_slope','',0,-1.5,1.5)
	sigma_slope = ROOT.RooRealVar('sigma_slope','',0.1,-1,1.5)
	a1_slope = ROOT.RooRealVar('a1_slope','',0,-1,1)
	a2_slope = ROOT.RooRealVar('a2_slope','',0,-1,1)
	
	mh_120 = ROOT.RooRealVar('mh_120','',120)
	wh_120 = ROOT.RooRealVar('wh_120','',0.00348)
	mh_124 = ROOT.RooRealVar('mh_124','',124)
	wh_124 = ROOT.RooRealVar('wh_124','',0.00391)
	mh_125 = ROOT.RooRealVar('mh_125','',125)
	wh_125 = ROOT.RooRealVar('wh_125','',0.00404)
	mh_126 = ROOT.RooRealVar('mh_126','',126)
	wh_126 = ROOT.RooRealVar('wh_126','',0.00418)
	mh_130 = ROOT.RooRealVar('mh_130','',130)
	wh_130 = ROOT.RooRealVar('wh_130','',0.00488)
	mh_list = [mh_120,mh_124,mh_125,mh_126,mh_130]
	wh_list = [wh_120,wh_124,wh_125,wh_126,wh_130]
	names = ['120','124','125','126','130']
	shifts = []
	sigmas = []
	a1s = []
	a2s = []
	approxs = []
	mass4l = ROOT.RooRealVar('mass4l','',105,140)
	for i in range(len(dataset_list)):
		shift_formula = ROOT.RooFormulaVar('shift_'+names[i],'@0+@1*(@2-125.0)',ROOT.RooArgList(shift_intercept,shift_slope,mh_list[i]))
		sigma_formula = ROOT.RooFormulaVar('sigma_'+names[i],'@0+@1*(@2-125.0)',ROOT.RooArgList(sigma_intercept,sigma_slope,mh_list[i]))
		a1_formula = ROOT.RooFormulaVar('a1_'+names[i],'@0+@1*(@2-125.0)',ROOT.RooArgList(a1_intercept,a1_slope,mh_list[i]))
		a2_formula = ROOT.RooFormulaVar('a2_'+names[i],'@0+@1*(@2-125.0)',ROOT.RooArgList(a2_intercept,a2_slope,mh_list[i]))
		n1 = 3
		n2 = 3
		theta1 = ROOT.RooRealVar('theta1','',2.3)
		theta2 = ROOT.RooRealVar('theta2','',2.3)
		approx = ROOT.RooaDoubleCBxBW('approx_'+names[i],'',mass4l,shift_formula,sigma_formula,a1_formula,a2_formula,mh_list[i],wh_list[i],n1,n2,theta1,theta2,False)
		shifts.append(shift_formula)
		sigmas.append(sigma_formula)
		a1s.append(a1_formula)
		a2s.append(a2_formula)
		approxs.append(approx)
	
	samples = ROOT.RooCategory('samples','samples')
	for i in range(len(dataset_list)):
		samples.defineType(names[i])

	comb_pdf = ROOT.RooSimultaneous('comb_pdf','comb_pdf',samples)
	for i in range(len(dataset_list)):
		comb_pdf.addPdf(approxs[i],names[i])

	comb_data = ROOT.RooDataSet('comb_data',
					'comb_data',
					ROOT.RooArgSet(mass4l),
					ROOT.RooFit.Index(samples),
					ROOT.RooFit.Import(names[0],dataset_list[0]),
					ROOT.RooFit.Import(names[1],dataset_list[1]),
					ROOT.RooFit.Import(names[2],dataset_list[2]),
					ROOT.RooFit.Import(names[3],dataset_list[3]),
					ROOT.RooFit.Import(names[4],dataset_list[4])
					)
	comb_data.Print()

	comb_pdf.fitTo(comb_data,ROOT.RooFit.SumW2Error(True),ROOT.RooFit.PrintLevel(-1))
#	comb_pdf.fitTo(comb_data)
	
	c = ROOT.TCanvas('c','',1400,1000)
	for i in range(len(dataset_list)):
		frame = mass4l.frame(ROOT.RooFit.Bins(100))
		frame.SetTitle('')
		comb_data.plotOn(frame,ROOT.RooFit.Cut('samples==samples::'+names[i]))
		comb_pdf.plotOn(frame,ROOT.RooFit.Slice(samples,names[i]),ROOT.RooFit.ProjWData(ROOT.RooArgSet(samples),comb_dataset))
		c.cd()
		frame.Draw()
		lx = ROOT.TLatex()
		lx.SetNDC()
		lx.SetTextSize(0.05)
		lx.SetTextFont(42)
		lx.SetTextAlign(23)
		lx.DrawLatex(0.7,0.9,'shift='+str(shifts[i].getVal()))
		lx.DrawLatex(0.7,0.8,'sigma='+str(sigmas[i].getVal()))
		lx.DrawLatex(0.7,0.7,'a1='+str(a1s[i].getVal()))
		lx.DrawLatex(0.7,0.6,'a2='+str(a2s[i].getVal()))
		c.SaveAs(plotpath+'xxxx'+fs_name+'_'+str(cates)+'_'+names[i]+'_approx.png')
		c.Clear()

def do(year, fs, type_, tag, cates, steponedir, plotpath):
	
	if fs==1: fs_name_ = '4mu'
	if fs==2: fs_name_ = '4e'
	if fs==3: fs_name_ = '2e2mu'
	if fs==4: fs_name_ = '2mu2e'
	print steponedir
	f_std = ROOT.TFile.Open(steponedir+'/_'+fs_name_+'_bounds.root','READ')
	h_std = f_std.Get('bounds')
		
	f_120 = ROOT.TFile.Open('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/'+year+'GGH_120.root','READ')
	t_120 = f_120.Get('passedEvents')
	f_124 = ROOT.TFile.Open('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/'+year+'GGH_124.root','READ')
	t_124 = f_124.Get('passedEvents')
	f_125 = ROOT.TFile.Open('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/'+year+'GGH_125.root','READ')
	t_125 = f_125.Get('passedEvents')
	f_126 = ROOT.TFile.Open('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/'+year+'GGH_126.root','READ')
	t_126 = f_126.Get('passedEvents')
	f_130 = ROOT.TFile.Open('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/'+year+'GGH_130.root','READ')
	t_130 = f_130.Get('passedEvents')
	
	dataset_120_list = readtree(h_std, t_120, fs, type_, tag, '120')
	dataset_124_list = readtree(h_std, t_124, fs, type_, tag, '124')
	dataset_125_list = readtree(h_std, t_125, fs, type_, tag, '125')
	dataset_126_list = readtree(h_std, t_126, fs, type_, tag, '126')
	dataset_130_list = readtree(h_std, t_130, fs, type_, tag, '130')
	
	for i in range(0,cates):
		dataset_list = []
		dataset_list.append(dataset_120_list[i])
		dataset_list.append(dataset_124_list[i])
		dataset_list.append(dataset_125_list[i])
		dataset_list.append(dataset_126_list[i])
		dataset_list.append(dataset_130_list[i])		
		r = ApproxFit(dataset_125_list[i],fs_name_,i,plotpath,125)
		#ApproxSimulFit(dataset_list, r, plotpath, cates, fs_name_, type_, tag, year)

year = sys.argv[1]
type_ = sys.argv[2]
tag = sys.argv[3]
fs = int(sys.argv[4])
cates = int(sys.argv[5])
if fs==1: fs_name = '4mu'
if fs==2: fs_name = '4e'
if fs==3: fs_name = '2e2mu'
if fs==4: fs_name = '2mu2e'
steponedir = '/eos/user/c/chenguan/www/HZZMASSTEMPLATES/Categs/'+type_+'/'+year+'/Categ_'+fs_name+'_'+str(cates)+'_'+tag+'/'
plotpath = '/eos/user/c/chenguan/www/TESTPLOTS/'
do(year, fs, type_, tag, cates, steponedir, plotpath)
