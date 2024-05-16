#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "TFile.h"
#include "TLine.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TLorentzVector.h"
#include "TLatex.h"
#include "TGraphErrors.h"
#include "HZZ2L2QRooPdfs.h"
#include <thread>
#include <iostream>
#include <fstream>
using namespace std;
using namespace RooFit ;

Double_t* SplitDataSet(TString fname);
void DoClusure(TString fname, int index, Double_t p1, Double_t p2, TString fs, TString year, bool isData, TString outputpath, double* tmp);
void MakePlot(TString fs, TString year, bool isData, TString outputpath);
const int Nbins = 10;
TString tag = "";
int main(int argc, char* argv[]){
	
	TString year = argv[1];
	TString ismc = argv[2];
	TString fs = "muon";
	TString outputpath = argv[3];
	bool isData;
	if( ismc == "mc"){
		isData = 0;
	}
	if( ismc == "data"){
		isData = 1;
	}	
	TString step = argv[4];
	int index = atoi(argv[5]);
	float highbound = atof(argv[6]);
	float lowbound = atof(argv[7]);
	cout<<year<<"	"<<ismc<<"	"<<fs<<"	"<<outputpath<<"	"<<isData<<"	"<<step<<"	"<<index<<"	"<<highbound<<"	"<<lowbound<<endl;	
	
	TString fname = "";	
	/*
         if(isData==0&&year=="2016pre"&&fs=="muon")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_DYJetsToLL_M50_2016APV_2mu_slimmed.root"; 
	 if(isData==0&&year=="2016post"&&fs=="muon")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_DYJetsToLL_M50_2016_2mu_slimmed.root";
         if(isData==0&&year=="2017"&&fs=="muon")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_DYJetsToLL_M50_2017_new_2mu_slimmed.root";
         if(isData==0&&year=="2018"&&fs=="muon")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_DYJetsToLL_M50_2018_2mu_slimmed.root";
	
	 if(isData==1&&year=="2016pre"&&fs=="muon")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_SingleMuon2016HIPM_2mu_slimmed.root"; 
         if(isData==1&&year=="2016post"&&fs=="muon")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_SingleMuonRun2016_2mu_slimmed.root";
         if(isData==1&&year=="2017"&&fs=="muon")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_SingleMuon_Run17_2mu_slimmed.root";
         if(isData==1&&year=="2018"&&fs=="muon")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_SingleMuon_Run18_2mu_slimmed.root";
	*/
	
	 if(isData==0&&year=="2016pre")fname = "/eos/user/f/ferrico/www/Chenguang/DY_muons_UL_20_s5m0/DY_amcatnlo_2016preVFP.root"; 
         if(isData==0&&year=="2016post")fname = "/eos/user/f/ferrico/www/Chenguang/DY_muons_UL_20_s5m0/DY_amcatnlo_2016postVFP.root";
         if(isData==0&&year=="2017")fname = "/eos/user/f/ferrico/www/Chenguang/DY_muons_UL_20_s5m0/DY_amcatnlo_2017_skimmed.root";
         if(isData==0&&year=="2018")fname = "/eos/user/f/ferrico/www/Chenguang/DY_muons_UL_20_s5m0/DY_amcatnlo_2018_skimmed.root";
            
         if(isData==1&&year=="2016pre")fname = "/eos/user/f/ferrico/www/FullRun2/20160/Data_SingleMuon_Run20160.root"; 
         if(isData==1&&year=="2016post")fname = "/eos/user/f/ferrico/www/FullRun2/20165/Data_SingleMuon_Run20165.root";
         if(isData==1&&year=="2017")fname = "/eos/user/f/ferrico/www/Chenguang/DATA/Data_SingleMuon_Run2017.root";
         if(isData==1&&year=="2018")fname = "/eos/user/f/ferrico/www/Chenguang/DATA/Data_SingleMuon_Run2018.root";
		
	if ( step == "1" ){ 
		Double_t* cut = new Double_t[Nbins+1];
		cut = SplitDataSet(fname);	
		ofstream fo;
		fo.open(outputpath + year + "_" + ismc + "_bounds.json");	
		fo<<"{";
		for(int i=0; i<Nbins; i++){
			fo<<"\""<<to_string(i)<<"\":"<<cut[i]<<",";		
		}
		fo<<"\""<<to_string(Nbins)<<"\":"<<cut[Nbins]<<"}";
		return 0;
	}
	
	/*	
	std::vector<double*> results; results.clear();
	//int ncores = std::thread::hardware_concurrency()-3;     // hardware concurrency in cpp means that cup parallelism
	vector<thread> threads; threads.clear();
	for(Int_t i=0; i<Nbins; i++){
		double tmp[4] = {0,0,0,0};
		results.push_back( tmp );
		threads.push_back( thread( DoClusure, fname, cut[i], cut[i+1], fs, year, isData, outputpath, tmp) );
	}
	//for(thread & th : threads){
	//	th.join();
	//}

	for( int i=0; i < threads.size(); i++){
		threads.at(i).join();
	}
	*/
	
	if ( step == "2" ){
		double tmp[4] = {0,0,0,0};	
		DoClusure( fname, index, lowbound, highbound, fs, year, isData, outputpath, tmp );	
		return 0;
	}
	
	if ( step == "3" ){
		MakePlot( fs, year, isData, outputpath );		
		return 0;
	}

}

void DoClusure(TString fname, int index, Double_t p1, Double_t p2, TString fs, TString year, bool isData, TString outputpath, double* tmp){
	/*do not used	
	TString name;
	if(isData==false&&year=="2017")name="2017_mc";
	if(isData==true&&year=="2017")name="2017_data";

	if(isData==false&&year=="2016pre")name="2016pre_mc";
	if(isData==true&&year=="2016pre")name="2016pre_data";
	
	if(isData==false&&year=="2016post")name="2016post_mc";
	if(isData==true&&year=="2016post")name="2016post_data";
	
	if(isData==false&&year=="2018")name="2018_mc";
	if(isData==true&&year=="2018")name="2018_data";
	
	TFile* f_e1 = new TFile("LUT/"+name+"_e1.root");
//	TFile* f_e2 = new TFile("LUT/"+name+"_e2.root");
	TFile* f_e3 = new TFile("LUT/"+name+"_e3.root");
//	TFile* f_mu = new TFile("LUT/"+name+"_mu.root");
	
//	TH2D* mu_corr = (TH2D*)f_mu->Get("mu");
	TH2D* e1_corr = (TH2D*)f_e1->Get("e1");
//	TH2D* e2_corr = (TH2D*)f_e2->Get("e2");
	TH2D* e3_corr = (TH2D*)f_e3->Get("e3");
	
//	TAxis* x_mu = mu_corr->GetXaxis();
	TAxis* x_e1 = e1_corr->GetXaxis();
//	TAxis* x_e2 = e2_corr->GetXaxis();
	TAxis* x_e3 = e3_corr->GetXaxis();
//	TAxis* y_mu = mu_corr->GetYaxis();
	TAxis* y_e1 = e1_corr->GetYaxis();
//	TAxis* y_e2 = e2_corr->GetYaxis();
	TAxis* y_e3 = e3_corr->GetYaxis();
	*/	
	TFile* f_lut = new TFile(outputpath + "/lookuptable/ebe_mu.root","READ");
	TH2F* lut = (TH2F*)f_lut->Get("ebe_mu");
	TAxis* xaxis = lut->GetXaxis();
	TAxis* yaxis = lut->GetYaxis();
	int xbin1, ybin1;
	int xbin2, ybin2;
	float corr1, corr2;
	 
	TH1F h_lep1_pt("lep1_pt","",100,0,100);
	TH1F h_lep2_pt("lep2_pt","",100,0,100);
	TH1F h_lep1_eta("lep1_eta","",100,-2.5,2.5);
	TH1F h_lep2_eta("lep2_eta","",100,-2.5,2.5);
	TH1F h_lep1_pterr("lep1_pterr","",100,0,10);
	TH1F h_lep2_pterr("lep2_pterr","",100,0,10);
	TH1F h_lep1_relpterr("lep1_relpterr","",100,0,0.1);
	TH1F h_lep2_relpterr("lep2_relpterr","",100,0,0.1);
		
	double massZ, massZErr, eta1, eta2, pT1, pT2, pterr1, pterr2, pterr1_uncorr, pterr2_uncorr, m1, m2, phi1, phi2, RelIso1, RelIso2;
	Double_t pTlow;
	pTlow=5;
	Double_t etahigh;
	etahigh=2.4;
	
	Double_t pt1err_rl, pt2err_rl;
	Double_t pt1Err_corr, pt2Err_corr;
	Double_t lambda1=1;
        Double_t lambda2=1;
	Double_t Sum_massZErr_uncorr = 0;
	Double_t Sum_massZErr_corr = 0;
	Double_t Ave_massZErr_uncorr = 0;
	Double_t Ave_massZErr_corr = 0;
	Int_t counter = 0;
	RooRealVar massZ_d("massZ","massZ",70,110);
	RooDataSet dataset("dataset","dataset",RooArgSet(massZ_d));
	TFile* f = new TFile( fname, "READ");
	TTree* t = (TTree*)f->Get("passedEvents");		
	t->SetBranchStatus("*",0);
        t->SetBranchStatus("vtx_BS_pT1",1);
        t->SetBranchStatus("vtx_BS_pT2",1);
        t->SetBranchStatus("pterr1_VX_BS",1);
        t->SetBranchStatus("pterr2_VX_BS",1);
        t->SetBranchStatus("vtx_BS_eta1",1);
        t->SetBranchStatus("vtx_BS_eta2",1);
        t->SetBranchStatus("phi1",1);
        t->SetBranchStatus("phi2",1);
        t->SetBranchStatus("m1",1);
        t->SetBranchStatus("m2",1);
        t->SetBranchStatus("massZ_vtx_BS",1);
        t->SetBranchStatus("massZErr_vtx_BS",1);
	t->SetBranchStatus("Iso1",1);
	t->SetBranchStatus("Iso2",1);
                            
        t->SetBranchAddress("vtx_BS_pT1",&pT1);
        t->SetBranchAddress("vtx_BS_pT2",&pT2);
        t->SetBranchAddress("vtx_BS_eta1",&eta1);
        t->SetBranchAddress("vtx_BS_eta2",&eta2);
        t->SetBranchAddress("phi1",&phi1);
        t->SetBranchAddress("phi2",&phi2);
        t->SetBranchAddress("m1",&m1);
        t->SetBranchAddress("m2",&m2);
        t->SetBranchAddress("pterr1_VX_BS",&pterr1);
        t->SetBranchAddress("pterr2_VX_BS",&pterr2);
        t->SetBranchAddress("massZ_vtx_BS",&massZ);	
	t->SetBranchAddress("Iso1",&RelIso1);
	t->SetBranchAddress("Iso2",&RelIso2);
	
	Int_t sum = t->GetEntries();
	for(Int_t i=0; i<sum; i++){
		//if ( i > 1000000) break;
		t->GetEntry(i);
		if(i%1000000==0)std::cout<<i<<"/"<<sum<<std::endl;
		if(massZ<110&&massZ>70&&pT1>pTlow&&pT2>pTlow&&abs(eta1)<etahigh&&abs(eta2)<etahigh&&RelIso1<0.35&&RelIso2<0.35){
			/*
			Int_t xbin1 = x_mu->FindBin(pT1);
			Int_t ybin1 = y_mu->FindBin(abs(eta1));
			Int_t xbin2 = x_mu->FindBin(pT2);
			Int_t ybin2 = y_mu->FindBin(abs(eta2));
			lambda1 = mu_corr->GetBinContent(xbin1,ybin1);
			lambda2 = mu_corr->GetBinContent(xbin2,ybin2);
			pt1Err_corr = pterr1*lambda1;
			pt2Err_corr = pterr2*lambda2;
			*/	
			xbin1 = xaxis->FindBin(abs(eta1));
			xbin2 = xaxis->FindBin(abs(eta2));
			ybin1 = yaxis->FindBin(pterr1/pT1);
			ybin2 = yaxis->FindBin(pterr2/pT2);
			corr1 = lut->GetBinContent(xbin1,ybin1);
			corr2 = lut->GetBinContent(xbin2,ybin2);
			
			pt1Err_corr = pterr1 * corr1;
			pt2Err_corr = pterr2 * corr2;
				
			TLorentzVector lep1, lep2;
			lep1.SetPtEtaPhiM(pT1,eta1,phi1,m1);
			lep2.SetPtEtaPhiM(pT2,eta2,phi2,m2);
			
			TLorentzVector lep1p, lep2p;
			lep1p.SetPtEtaPhiM(pT1+pt1Err_corr,eta1,phi1,m1);
			lep2p.SetPtEtaPhiM(pT2+pt2Err_corr,eta2,phi2,m2);
				
			Double_t dm1corr = (lep1p+lep2).M()-(lep1+lep2).M();
			Double_t dm2corr = (lep1+lep2p).M()-(lep1+lep2).M();
			Double_t massZErr_corr = TMath::Sqrt(dm1corr*dm1corr+dm2corr*dm2corr);
			
			lep1p.SetPtEtaPhiM(pT1 + pterr1, eta1, phi1, m1);
			lep2p.SetPtEtaPhiM(pT2 + pterr2, eta2, phi2, m2);
			double dm1 = (lep1p+lep2).M() - (lep1+lep2).M();
			double dm2 = (lep1+lep2p).M() - (lep1+lep2).M();
			massZErr = TMath::Sqrt(dm1*dm1 + dm2*dm2);

			if ( massZErr/massZ > p2 || massZErr/massZ < p1 ){
				continue;
			}

			Sum_massZErr_corr = Sum_massZErr_corr + massZErr_corr;
			Sum_massZErr_uncorr = Sum_massZErr_uncorr + massZErr;
			counter = counter+1;
				
			massZ_d.setVal(massZ);
			dataset.add(RooArgSet(massZ_d));
			/*		
			h_lep1_pt.Fill(pT1);
			h_lep2_pt.Fill(pT2);
			h_lep1_eta.Fill(eta1);
			h_lep2_eta.Fill(eta2);
			h_lep1_pterr.Fill(pterr1);
			h_lep2_pterr.Fill(pterr2);
			h_lep1_relpterr.Fill(pterr1/pT1);
			h_lep2_relpterr.Fill(pterr2/pT2);
			*/

		}//if
	}
	delete t;
	f->Close();
	massZ_d.setBins(2000,"cache");
	RooDataHist dataset_binned("dataset_binned","dataset_binned",RooArgSet(massZ_d),dataset);

	//make modle
	Double_t Zwidth;
	if(isData==0)Zwidth=2.49;
	if(isData==1)Zwidth=2.49;
	RooRealVar massZ_("massZ","massZ",70,110);
	RooRealVar PDGmZ("PDGmZ","PDGmZ",91.19);
	RooRealVar PDGwZ("PDGwZ","PDGwZ",Zwidth);
	PDGmZ.setConstant(kTRUE);
	PDGwZ.setConstant(kTRUE);
	RooBreitWigner PDGBW("PDGBW","PDGBW",massZ_,PDGmZ,PDGwZ);
	
	RooRealVar mean("mean","mean", 0, -5, 5);                                                        
	RooRealVar alpha("alpha","alpha", 1, 0, 10);
	RooRealVar n("n","n", 2, 0, 30);
	RooRealVar alpha2("alpha2","alpha2",1,0,10);
	RooRealVar n2("n2","n2",2,0,30);
	RooRealVar sigma("sigma", "sigma", 1, 0, 10);
	RooDoubleCB CB("CB","CB", massZ_, mean, sigma, alpha, n, alpha2, n2);
	
	RooFFTConvPdf model("CW","CW",massZ_,PDGBW,CB);
	model.setBufferFraction(0.3);	
	RooRealVar tau("tau","tau",  -1, 1);
	RooExponential bkg("bkg","bkg", massZ_, tau);
	RooRealVar fsig("fsig","signal fraction", 0.7, 0.5, 1);
	//RooAddPdf model("model","model", CW, bkg, fsig);
	
	model.fitTo(dataset_binned,PrintLevel(-1));
	
	RooRealVar mean_("mean_","mean_", mean.getVal(), -5, 5);
	RooRealVar alpha_("alpha_","alpha_", alpha.getVal(), 0, 10);
	RooRealVar n_("n_","n_", n.getVal(), 0, 30);
	RooRealVar alpha2_("alpha2_","alpha2_", alpha2.getVal(), 0,10);
	RooRealVar n2_("n2_","n2_", n2.getVal(), 0,30);
	RooRealVar sigma_("sigma_","sigma_", sigma.getVal(), 0 , 10);
	RooDoubleCB CB_("CB_","CB_", massZ_, mean_, sigma_, alpha_, n_, alpha2_, n2_);
	RooFFTConvPdf model_("CW_","CW_", massZ_, PDGBW, CB_);
	RooRealVar tau_("tau_","tau_", tau.getVal(), -1, 1);
	RooExponential bkg_("bkg_","bkg_", massZ_, tau_);
	RooRealVar fsig_("fsig_","fsig_", fsig.getVal(), 0.5, 1);
	//RooAddPdf model_("model_","model_", CW_, bkg_, fsig_);
	model_.setBufferFraction(0.3);	
	model_.fitTo(dataset_binned,PrintLevel(-1));
	
	RooPlot *frame=massZ_.frame();
	frame->SetTitle("");
	dataset_binned.plotOn(frame);
	model_.plotOn(frame,LineColor(2),LineWidth(1));
	
	Double_t chi2 = frame->chiSquare(6);
	TString mean_s, sigma_s, alpha_s, n_s, tau_s, fsig_s, chi2_s, counter_s;
	mean_s = to_string(mean_.getVal());
	sigma_s = to_string(sigma_.getVal());
	alpha_s = to_string(alpha_.getVal());
	n_s = to_string(n_.getVal());
	tau_s = to_string(tau_.getVal());
	fsig_s = to_string(fsig_.getVal());
	chi2_s = to_string(chi2);
	counter_s = to_string(counter);
	
	TCanvas* c = new TCanvas("c","c",1400,1000);
	c->SetLeftMargin(0.14);
	c->cd();
	frame->Draw();
	
	TLatex *latex=new TLatex();
    latex->SetNDC();
    latex->SetTextSize(0.05);
    latex->SetTextFont(42);
    latex->SetTextAlign(23);
	latex->DrawLatex(0.7,0.9,"#chi2/DOF="+chi2_s);
	latex->DrawLatex(0.7,0.8,"mean="+mean_s);
	latex->DrawLatex(0.7,0.7,"alpha="+alpha_s);
	latex->DrawLatex(0.7,0.6,"sigma="+sigma_s);
	latex->DrawLatex(0.7,0.5,"n="+n_s);
	latex->DrawLatex(0.7,0.4,"tau="+tau_s);
	latex->DrawLatex(0.7,0.3,"fsig="+fsig_s);
	latex->DrawLatex(0.7,0.2,"Entries="+counter_s);

	TLatex * latex1 = new TLatex();
	latex1->SetNDC();
	latex1->SetTextSize(0.05);
	latex1->SetTextFont(42);
	latex1->SetTextAlign(23);
	latex1->DrawLatex(0.13,0.85,year);
	
	TString p1_s, p2_s;
	p1_s = to_string(p1);
	p2_s = to_string(p2);
	TString plotpath;
	if(isData==1)plotpath = outputpath+"/"+fs+"_Relative_massZErr_"+p1_s+"-"+p2_s;
	if(isData==0)plotpath = outputpath+"/"+fs+"_Relative_massZErr_"+p1_s+"-"+p2_s;
	c->SaveAs(plotpath+".pdf");
	c->SaveAs(plotpath+".png");
	/*	
	c->Clear();
	h_lep1_pt.Draw();
	c->SaveAs(outputpath + "/" + to_string(index) + "_lep1_pt.png");
	
	c->Clear();
	h_lep2_pt.Draw();
	c->SaveAs(outputpath + "/" + to_string(index) + "_lep2_pt.png");
	
	c->Clear();
	h_lep1_eta.Draw();
	c->SaveAs(outputpath + "/" + to_string(index) + "_lep1_eta.png");

	c->Clear();
	h_lep2_eta.Draw();
	c->SaveAs(outputpath + "/" + to_string(index) + "_lep2_eta.png");
	
	c->Clear();
	h_lep1_pterr.Draw();
	c->SaveAs(outputpath + "/" + to_string(index) + "_lep1_pterr.png");

	c->Clear();
	h_lep2_pterr.Draw();
	c->SaveAs(outputpath + "/" + to_string(index) + "_lep2_pterr.png");

	c->Clear();
	h_lep1_relpterr.Draw();
	c->SaveAs(outputpath + "/" + to_string(index) + "_lep1_relpterr.png");

	c->Clear();
	h_lep2_relpterr.Draw();
	c->SaveAs(outputpath + "/" + to_string(index) + "_lep2_relpterr.png");
	*/
	//Double_t* results = new Double_t[4];
	tmp[0] = Sum_massZErr_corr/counter;
	tmp[1] = Sum_massZErr_uncorr/counter;
	tmp[2] = sigma_.getVal();
	tmp[3] = sigma_.getError();
	ofstream fo;
	fo.open(outputpath + "/" + to_string(index)+".json");
	fo<<"{\"predicted\":"<<tmp[0]<<",\"predicted_uncorr\":"<<tmp[1]<<",\"sigma\":"<<tmp[2]<<",\"sigmaerr\":"<<tmp[3]<<"}";
	delete c;
	delete latex1;
	delete latex;
	delete frame;

}
			
Double_t* SplitDataSet(TString fname){
	
	double massZ, massZErr, eta1, eta2, pT1, pT2, pterr1, pterr2, pterr1_uncorr, pterr2_uncorr, m1, m2, phi1, phi2, RelIso1, RelIso2;
		
	TFile* f = new TFile(fname,"READ");
	TTree* t = (TTree*)f->Get("passedEvents");	
	t->SetBranchAddress("massZ_vtx_BS",&massZ);
	t->SetBranchAddress("eta1",&eta1);
	t->SetBranchAddress("eta2",&eta2);
	t->SetBranchAddress("vtx_BS_pT1",&pT1);
	t->SetBranchAddress("vtx_BS_pT2",&pT2);
	t->SetBranchAddress("pterr1_VX_BS",&pterr1);
	t->SetBranchAddress("pterr2_VX_BS",&pterr2);
	//t->SetBranchAddress("lep1_pterrold",&pterr1_uncorr);//do not used for data split
	//t->SetBranchAddress("lep2_pterrold",&pterr2_uncorr);
	t->SetBranchAddress("m1",&m1);
	t->SetBranchAddress("m2",&m2);
	t->SetBranchAddress("phi1",&phi1);
	t->SetBranchAddress("phi2",&phi2);
	t->SetBranchAddress("Iso1",&RelIso1);
	t->SetBranchAddress("Iso2",&RelIso2);
	
	Int_t sum = t->GetEntries();
	vector<Double_t>massZErr_series;
	massZErr_series.clear();

	Double_t pTlow = 5;
	Double_t etahigh = 2.4;
	for(Int_t i=0; i<sum; i++){
		if (i > 100000 ) break;	
		if(i%10000==0)std::cout<<i<<"/"<<sum<<std::endl;
		t->GetEntry(i);
		if(massZ<110&&massZ>70&&pT1>pTlow&&pT2>pTlow&&abs(eta1)<etahigh&&abs(eta2)<etahigh&&RelIso1<0.35&&RelIso2<0.35){
			Double_t pt1Err_corr = pterr1;
			Double_t pt2Err_corr = pterr2;
			TLorentzVector lep1, lep2;                                                                                                                            
                        lep1.SetPtEtaPhiM(pT1,eta1,phi1,m1);
                        lep2.SetPtEtaPhiM(pT2,eta2,phi2,m2);
                                           
                        TLorentzVector lep1p, lep2p;
                        lep1p.SetPtEtaPhiM(pT1+pt1Err_corr,eta1,phi1,m1);
                        lep2p.SetPtEtaPhiM(pT2+pt2Err_corr,eta2,phi2,m2);
                                           
                        Double_t dm1corr = (lep1p+lep2).M()-(lep1+lep2).M();
                        Double_t dm2corr = (lep1+lep2p).M()-(lep1+lep2).M();
                        Double_t massZErr_corr = TMath::Sqrt(dm1corr*dm1corr+dm2corr*dm2corr);
	
			massZErr_series.push_back(massZErr_corr/massZ);
		}
	}
	Int_t sub_sum = massZErr_series.size();
	sort(massZErr_series.begin(),massZErr_series.end());
	
	const int Nedges = Nbins + 1;	
	Int_t* Quantitle = new Int_t[Nedges];
	Quantitle[0] = 0;	
	for ( int i = 1; i<Nbins; i++){
		Quantitle[i] = floor( sub_sum * i / Nbins );
	}
	Quantitle[Nedges] = sub_sum;
	
	Double_t* cut = new Double_t[Nedges];
	cut[0] = 0;
	for ( int i = 1; i<Nedges-1; i++ ){
		cut[i] = massZErr_series[Quantitle[i]];
	}
	cut[Nbins] = massZErr_series.back();
	delete t;
	f->Close();
	return cut;
	
}


void MakePlot( TString fs, TString year, bool isData, TString outputpath ){
	double x_corr[10] = {};
	double x_uncorr[10] = {};
	double y[10] = {};
	double yErr[10] = {};	
	TCanvas* c = new TCanvas("c","c",1000,1000);
	c->cd();
	TGraph* g1 = new TGraphErrors(10,x_corr,y,0,yErr);
	TGraph* g2 = new TGraphErrors(10,x_uncorr,y,0,yErr);
	g1->GetYaxis()->SetTitleOffset(1.5);
	g2->GetYaxis()->SetTitleOffset(1.5);
	g1->SetMarkerStyle(20);
	g2->SetMarkerStyle(20);
	g1->SetMarkerColor(2);
	g2->SetMarkerColor(1);
	g1->SetTitle("");
	g1->GetXaxis()->SetTitle("Predicted #sigma_{m}");
	g1->GetYaxis()->SetTitle("Measured #sigma_{m}");
	g1->GetXaxis()->SetLimits(0,5);
	g1->GetHistogram()->SetMaximum(5);
	g1->GetHistogram()->SetMinimum(0);
	g1->Draw("AP");
	g2->Draw("p");

	TLine *l1=new TLine(0,0,5,5);
	l1->SetLineColor(kGreen+2);
	TLine *l2=new TLine(0,0,5/1.2,5);
	TLine *l3=new TLine(0,0,5,5/1.2);
	TLine *l4=new TLine(0,0,5/1.1,5);
	l4->SetLineColor(6);
	TLine *l5=new TLine(0,0,5,5/1.1);
	l5->SetLineColor(6);
	//l1->SetLineStyle(kDashed);
	//l2->SetLineStyle(kDashed);
	//l3->SetLineStyle(kDashed);
	//l4->SetLineStyle(kDashed);
	//l5->SetLineStyle(kDashed);
	l1->Draw();
	//l2->Draw();
	//l3->Draw();
	l4->Draw();
	l5->Draw();

	TLegend *legend=new TLegend(0.65,0.1,0.9,0.3);
	legend->AddEntry(g1, "Corr_DATA", "P");
	legend->AddEntry(g2, "unCorr_DATA", "P");
	legend->SetTextSize(0.03);
	legend->SetLineWidth(2);
	legend->SetFillColor(0);
	legend->SetBorderSize(1);
	legend->Draw("SAME");
	
	TString plotpath;
	if(isData==1)plotpath = outputpath;
	if(isData==0)plotpath = outputpath;
	TString filename_png = fs+"_ClosureTest"+"_"+tag+".png";
	TString filename_pdf = fs+"_ClosureTest"+"_"+tag+".pdf";
	c->SaveAs(plotpath+"/"+filename_pdf);
	c->SaveAs(plotpath+"/"+filename_png);
}
