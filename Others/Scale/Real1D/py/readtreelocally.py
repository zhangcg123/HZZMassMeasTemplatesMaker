import ROOT
import os
import sys
import json
import numpy

def ReadTree(inputpath, outputpath, isdata, mass_name, binned, low, high, h_eta_std, h_phi_std, dataset_eta_pos_list, dataset_eta_neg_list, dataset_phi_pos_list, dataset_phi_neg_list):
	
	eta_axis = h_eta_std.GetXaxis()
	phi_axis = h_phi_std.GetXaxis()

	fi = ROOT.TFile.Open(inputpath,'READ')
	t = fi.Get('passedEvents')
	for i in range(t.GetEntries()):
		#if i>100:break
		if i%10000 == 0: print i, '/', t.GetEntries()
		t.GetEntry(i)
		mass = getattr(t,mass_name)
		if t.mu1_hzzid and t.mu2_hzzid and (t.trigger24matching1 or t.trigger24matching2) and t.RelIso1 < 0.35 and t.RelIso2 < 0.35 and t.Sip1 < 4.0 and t.Sip2 < 4.0 and t.pt1_corr > 25 and t.pt2_corr > 5 and abs(t.eta1) < 2.4 and abs(t.eta2) < 2.4 and mass < float(high) and mass > float(low):
			if t.mu1_q_roc == 1:
				eta_pos_bin = eta_axis.FindBin(t.eta1)-1
				phi_pos_bin = phi_axis.FindBin(t.phi1)-1
				eta_neg_bin = eta_axis.FindBin(t.eta2)-1
				phi_neg_bin = phi_axis.FindBin(t.phi2)-1
				dataset_eta_pos_list[eta_pos_bin].Fill(mass)
				dataset_eta_neg_list[eta_neg_bin].Fill(mass)
				dataset_phi_pos_list[phi_pos_bin].Fill(mass)
				dataset_phi_neg_list[phi_neg_bin].Fill(mass)
			if t.mu2_q_roc == 1:
				eta_pos_bin = eta_axis.FindBin(t.eta2)-1
				eta_neg_bin = eta_axis.FindBin(t.eta1)-1
				phi_pos_bin = phi_axis.FindBin(t.phi2)-1
				phi_neg_bin = phi_axis.FindBin(t.phi1)-1
				dataset_eta_pos_list[eta_pos_bin].Fill(mass)
				dataset_eta_neg_list[eta_neg_bin].Fill(mass)
				dataset_phi_pos_list[phi_pos_bin].Fill(mass)
				dataset_phi_neg_list[phi_neg_bin].Fill(mass)

	dic = {}
	for hs in [dataset_eta_pos_list, dataset_eta_neg_list, dataset_phi_pos_list, dataset_phi_neg_list]:
		for h in hs:
					
			#build model
			x = ROOT.RooRealVar(mass_name,'',low,high)
			x.setBins(10000,"cache")
			pdgmz = ROOT.RooRealVar('pdgmz','',91.19)
			width = 2.496 if isdata == 'data' else 2.441
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
			dcbxbw = ROOT.RooFFTConvPdf('dcbxbw','',x,bw,dcb,2)
			dcbxbw.setBufferFraction(0.2)
			tau = ROOT.RooRealVar('tau','',0,-1,1)
			bkg = ROOT.RooExponential('bkg','',x,tau)
			fsig = ROOT.RooRealVar('fsig','',0.9,0.5,1)
			model = ROOT.RooAddPdf('model','',dcbxbw,bkg,fsig)
			dataset = ROOT.RooDataHist('dataset','',ROOT.RooArgList(x),ROOT.RooFit.Import(h))
			model.fitTo(dataset,ROOT.RooFit.PrintLevel(-1))
			model.fitTo(dataset,ROOT.RooFit.PrintLevel(-1))
	
			chi2_var = ROOT.RooChi2Var('chi2', '', model, dataset, ROOT.RooFit.DataError(ROOT.RooAbsData.Expected))
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
			lx.DrawLatex(0.7,0.9,"#chi^{2}/DOF="+str(chi2_var.getVal()/float(dataset.numEntries())))
			lx.DrawLatex(0.7,0.8,"mean="+str(m.getVal())[0:7]+'+/-'+str(m.getError())[0:7])
			lx.DrawLatex(0.7,0.7,"sigma="+str(s.getVal())[0:7])
			lx.DrawLatex(0.7,0.6,"a1="+str(a1.getVal())[0:7])
			lx.DrawLatex(0.7,0.5,"n1="+str(n1.getVal())[0:7])
			lx.DrawLatex(0.7,0.4,"a2="+str(a2.getVal())[0:7])
			lx.DrawLatex(0.7,0.3,"n2="+str(n2.getVal())[0:7])
			lx.DrawLatex(0.7,0.2,"entries="+str(int(dataset.sumEntries())))
			c.SaveAs(outputpath + mass_name + '_' + h.GetName()+'.png')
			dic={}
			dic[mass_name+'_'+h.GetName()+'_peak'] = m.getVal()+91.19
			dic[mass_name+'_'+h.GetName()+'_peakerr'] = m.getError()
			dic[mass_name+'_'+h.GetName()+'_mean'] = dataset.mean(x)
			
	with open(outputpath + mass_name + '_' + h.GetName() + '.json','w') as fo:
		json.dump(dic,fo)

year = sys.argv[1]
isdata = sys.argv[2]
mass_name = sys.argv[3]
low = float(sys.argv[4])
high = float(sys.argv[5])
binned = sys.argv[6]
print year, isdata, mass_name, low, high, binned
assert binned in ['binned'] and isdata in ['mc','data'] and year in ['2016a','2016b','2017','2018'] and mass_name in ['mass2mu_corr','mass2mu_ewk2','mass2mu_ewk_dt','mass2mu_zpt_dt','mass2mu_dm_dt','mass2mu_ewk2_dt'], 'ERRORRRRRRRRRRR'

#input path
if isdata == 'mc': subname = 'LO.root'
if isdata == 'data': subname = 'SingleMu.root'
inputpath = '/eos/user/c/chenguan/Tuples/MuonScaleUsed/19UL_EWK2ASDEF/' + year + '_HZZ_'+subname

#output path
outputpath = '/eos/user/c/chenguan/www/vertex/ScaleUnc/Real1D/19UL/local/'+year+'_'+str(low)+'_'+str(high)+'/'

h_eta_std = ROOT.TH1D('h_eta_std','',8,-2.4,2.4)
h_phi_std = ROOT.TH1D('h_phi_std','',8,-numpy.pi,numpy.pi)
dataset_eta_pos_list = []
dataset_phi_pos_list = []
dataset_eta_neg_list = []
dataset_phi_neg_list = []
for i in range(8):
	h_eta_pos = ROOT.TH1D('eta_pos_'+str(i),'',300,float(low),float(high))
	h_eta_neg = ROOT.TH1D('eta_neg_'+str(i),'',300,float(low),float(high))
	h_phi_pos = ROOT.TH1D('phi_pos_'+str(i),'',300,float(low),float(high))
	h_phi_neg = ROOT.TH1D('phi_neg_'+str(i),'',300,float(low),float(high))
	dataset_eta_pos_list.append(h_eta_pos)
	dataset_eta_neg_list.append(h_eta_neg)
	dataset_phi_pos_list.append(h_phi_pos)
	dataset_phi_neg_list.append(h_phi_neg)

ReadTree(inputpath, outputpath, isdata, mass_name, binned, low, high, h_eta_std, h_phi_std, dataset_eta_pos_list, dataset_eta_neg_list, dataset_phi_pos_list, dataset_phi_neg_list)
