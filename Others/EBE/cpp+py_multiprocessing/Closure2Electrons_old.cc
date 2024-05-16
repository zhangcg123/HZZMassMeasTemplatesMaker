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
#include <thread>
#include <iostream>
#include <fstream>
using namespace std;
using namespace RooFit ;

void SetAddress(TTree* t);
Double_t* SplitDataSet(TTree* t);
Double_t* DoClusure(TTree* t, Double_t p1, Double_t p2, TString fs, TString year, bool isData, TString outputpath);
void MakePlot(Double_t* x_corr, Double_t* x_uncorr, Double_t* y, Double_t* yErr, TString fs, TString year, bool isData, TString outputpath);
Double_t massZ, massZErr, eta1, eta2, pT1, pT2, pterr1, pterr2, m1, m2, phi1, phi2, RelIso1, RelIso2;
Int_t lep1_ecalDriven, lep2_ecalDriven;
const int Nbins = 10;
TString tag = "";
int main(int argc, char* argv[]){
	
	TString year = argv[1];
	TString ismc = argv[2];
	TString fs = "electron";
	TString outputpath = argv[3];
	bool isData;
	if( ismc == "mc"){
		isData = 0;
	}
	if( ismc == "data"){
		isData = 1;
	}
	
	TFile* f = new TFile();
         if(isData==0&&year=="2016"&&fs=="muon")f = new TFile("");
         if(isData==0&&year=="2017"&&fs=="muon")f = new TFile("");
         if(isData==0&&year=="2018"&&fs=="muon")f = new TFile("");
         if(isData==0&&year=="2016pre"&&fs=="electron")f = new TFile("/eos/user/c/chenguan/www/Samples/DY_2e_skimmed_A_cancella_madgraph_2016preVFP.root");
         if(isData==0&&year=="2016post"&&fs=="electron")f = new TFile("/eos/user/c/chenguan/www/Samples/DY_2e_skimmed_A_cancella_madgraph_2016postVFP.root");
	 if(isData==0&&year=="2017"&&fs=="electron")f = new TFile("/eos/user/c/chenguan/www/Samples/DY_madgraph_skimmed_2017_2e.root");
         if(isData==0&&year=="2018"&&fs=="electron")f = new TFile("/eos/user/c/chenguan/www/Samples/DY_madgraph_skimmed_2018_2e.root");
 
         if(isData==1&&year=="2016"&&fs=="muon")f = new TFile("");
         if(isData==1&&year=="2017"&&fs=="muon")f = new TFile("");
         if(isData==1&&year=="2018"&&fs=="muon")f = new TFile("");
         if(isData==1&&year=="2016pre"&&fs=="electron")f = new TFile("/eos/user/c/chenguan/www/Samples/Data_SingleElectron_Run2016pre.root","READ");
	 if(isData==1&&year=="2016post"&&fs=="electron")f = new TFile("/eos/user/c/chenguan/www/Samples/Data_SingleElectron_Run2016post.root","READ");
         if(isData==1&&year=="2017"&&fs=="electron")f = new TFile("/eos/user/c/chenguan/www/Samples/Data_SingleElectron_2017.root");
         if(isData==1&&year=="2018"&&fs=="electron")f = new TFile("/eos/user/c/chenguan/www/Samples/Data_EGamma_2018.root");
	
	Double_t* x_corr = new Double_t[10];
	Double_t* x_uncorr = new Double_t[10];
	Double_t* y = new Double_t[10];
	Double_t* yErr = new Double_t[10];
	Double_t* tmp = new Double_t[4];
	
	TTree* t = (TTree*)f->Get("passedEvents");
	SetAddress(t);
	Double_t* cut = new Double_t[11];
	cut = SplitDataSet(t);
	ofstream fo;
	fo.open(outputpath + year + "_" + ismc + "_bounds.json");	
	fo<<"{";
	for(int i=0; i<Nbins; i++){
		fo<<"\""<<to_string(i)<<"\":"<<cut[i]<<",";		
	}
	fo<<"\""<<to_string(Nbins)<<"\":"<<cut[Nbins]<<"}";
	
	for(Int_t i=0; i<10; i++){
		tmp = DoClusure(t, cut[i], cut[i+1], fs, year, isData, outputpath);
		x_corr[i] = tmp[0];
		x_uncorr[i] = tmp[1];
		y[i] = tmp[2];
		yErr[i] = tmp[3];
	}

	for(Int_t i=0; i<11; i++){
		cout<<x_corr[i]<<endl;
		cout<<x_uncorr[i]<<endl;
		cout<<y[i]<<endl;
	}	
	MakePlot(x_corr, x_uncorr, y, yErr, fs, year, isData, outputpath);
	
	
	
	
	
}

void MakePlot(Double_t* x_corr, Double_t* x_uncorr, Double_t* y, Double_t* yErr, TString fs, TString year, bool isData, TString outputpath){
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


Double_t* DoClusure(TTree* t, Double_t p1, Double_t p2, TString fs, TString year, bool isData, TString outputpath){
	
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
	
	
	Double_t pTlow;
	if(fs=="electron")pTlow=7;
	if(fs=="muon")pTlow=5;
	Double_t etahigh;
	if(fs=="electron")etahigh=2.5;
	if(fs=="muon")etahigh=2.4;
	
	Double_t pt1err_rl, pt2err_rl;
	Double_t pt1Err_corr, pt2Err_corr;
	Double_t lambda1=1;
    Double_t lambda2=1;
	Double_t Sum_massZErr_uncorr = 0;
	Double_t Sum_massZErr_corr = 0;
	Double_t Ave_massZErr_uncorr = 0;
	Double_t Ave_massZErr_corr = 0;
	
	Int_t counter = 0;
	
	RooRealVar massZ_d("massZ","massZ",60,120);
	RooDataSet dataset("dataset","dataset",RooArgSet(massZ_d));
	
	Int_t sum = t->GetEntries();
	
	for(Int_t i=0; i<sum; i++){
		t->GetEntry(i);
		//if(i>1000)break;
		pt1err_rl = pterr1/pT1;
		pt2err_rl = pterr2/pT2;
		if(massZErr<7.2&&massZErr>0.2&&massZ<120&&massZ>60&&massZErr/massZ<p2&&massZErr/massZ>p1&&pT1<100&&pT1>pTlow&&pT2>pTlow&&pT2<100&&abs(eta1)<etahigh&&abs(eta2)<etahigh&&RelIso1<0.35&&RelIso2<0.35){
		/*	if(fs=="muon"){
				Int_t xbin1 = x_mu->FindBin(pT1);
				Int_t ybin1 = y_mu->FindBin(abs(eta1));
				Int_t xbin2 = x_mu->FindBin(pT2);
				Int_t ybin2 = y_mu->FindBin(abs(eta2));
				lambda1 = mu_corr->GetBinContent(xbin1,ybin1);
				lambda2 = mu_corr->GetBinContent(xbin2,ybin2);
			}
		*/
			if(fs=="electron"){

				if(lep1_ecalDriven==1||lep2_ecalDriven==1){

					Int_t xbin1 = x_e1->FindBin(abs(eta1));
					Int_t ybin1 = y_e1->FindBin(pt1err_rl);
					Int_t xbin2 = x_e1->FindBin(abs(eta2));
					Int_t ybin2 = y_e1->FindBin(pt2err_rl);
					lambda1 = e1_corr->GetBinContent(xbin1,ybin1);
					lambda2 = e1_corr->GetBinContent(xbin2,ybin2);

				}
				
				if(lep1_ecalDriven==0||lep2_ecalDriven==0){
					Int_t xbin1 = x_e3->FindBin(pT1);
					Int_t ybin1 = y_e3->FindBin(abs(eta1));
					Int_t xbin2 = x_e3->FindBin(pT2);
					Int_t ybin2 = x_e3->FindBin(abs(eta2));
					lambda1 = e3_corr->GetBinContent(xbin1,ybin1);
					lambda2 = e3_corr->GetBinContent(xbin2,ybin2);
				}
			
			}//if 2e
			
			pt1Err_corr = pterr1*lambda1;
			pt2Err_corr = pterr2*lambda2;
		
				
			TLorentzVector lep1, lep2;
			lep1.SetPtEtaPhiM(pT1,eta1,phi1,m1);
			lep2.SetPtEtaPhiM(pT2,eta2,phi2,m2);
			
			TLorentzVector lep1p, lep2p;
			lep1p.SetPtEtaPhiM(pT1+pt1Err_corr,eta1,phi1,m1);
			lep2p.SetPtEtaPhiM(pT2+pt2Err_corr,eta2,phi2,m2);
				
			Double_t dm1corr = (lep1p+lep2).M()-(lep1+lep2).M();
			Double_t dm2corr = (lep1+lep2p).M()-(lep1+lep2).M();
			Double_t massZErr_corr = TMath::Sqrt(dm1corr*dm1corr+dm2corr*dm2corr);
				
			Sum_massZErr_corr = Sum_massZErr_corr+massZErr_corr;
			Sum_massZErr_uncorr = Sum_massZErr_uncorr+massZErr;
			counter = counter+1;
				
			massZ_d.setVal(massZ);
			dataset.add(RooArgSet(massZ_d));
			
		}//if
	}
	
	
	massZ_d.setBins(300);
	RooDataHist dataset_binned("dataset_binned","dataset_binned",RooArgSet(massZ_d),dataset);

	//make modle
	Double_t Zwidth;
	if(isData==0)Zwidth=2.44;
	if(isData==1)Zwidth=2.49;
	RooRealVar massZ("massZ","massZ",60,120);
	RooRealVar PDGmZ("PDGmZ","PDGmZ",91.19);
	RooRealVar PDGwZ("PDGwZ","PDGwZ",Zwidth);
	PDGmZ.setConstant(kTRUE);
	PDGwZ.setConstant(kTRUE);
	RooBreitWigner PDGBW("PDGBW","PDGBW",massZ,PDGmZ,PDGwZ);
	
	RooRealVar mean("mean","mean", 0, -5, 5);                                                        
	RooRealVar alpha("alpha","alpha", 1, 0, 10);
	RooRealVar n("n","n", 2, 0, 30);
	RooRealVar sigma("sigma", "sigma", 1, 0, 10);
	RooCBShape CB("CB","CB", massZ, mean, sigma, alpha, n);
	
	RooFFTConvPdf CW("CW","CW",massZ,PDGBW,CB);
	RooRealVar tau("tau","tau",  -1, 1);
	RooExponential bkg("bkg","bkg", massZ, tau);
	RooRealVar fsig("fsig","signal fraction", 0.7, 0.5, 1);
	RooAddPdf model("model","model", CW, bkg, fsig);
	
	model.fitTo(dataset_binned,PrintLevel(-1));
	
	RooRealVar mean_("mean_","mean_", mean.getVal(), -5, 5);
	RooRealVar alpha_("alpha_","alpha_", alpha.getVal(), 0, 10);
	RooRealVar n_("n_","n_", n.getVal(), 0, 30);
	RooRealVar sigma_("sigma_","sigma_", sigma.getVal(), 0 , 10);
	RooCBShape CB_("CB_","CB_", massZ, mean_, sigma_, alpha_, n_);
	RooFFTConvPdf CW_("CW_","CW_", massZ, PDGBW, CB_);
	RooRealVar tau_("tau_","tau_", tau.getVal(), -1, 1);
	RooExponential bkg_("bkg_","bkg_", massZ, tau_);
	RooRealVar fsig_("fsig_","fsig_", fsig.getVal(), 0.5, 1);
	RooAddPdf model_("model_","model_", CW_, bkg_, fsig_);
	
	model_.fitTo(dataset_binned,PrintLevel(-1));
	
	RooPlot *frame=massZ.frame();
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
	Double_t* results = new Double_t[4];

	results[0] = Sum_massZErr_corr/counter;
	results[1] = Sum_massZErr_uncorr/counter;
	results[2] = sigma_.getVal();
	results[3] = sigma_.getError();
	ofstream fo;
	fo.open(outputpath + "/" + to_string(index)+".json");
	fo<<"{\"predicted\":"<<results[0]<<",\"predicted_uncorr\":"<<results[1]<<",\"sigma\":"<<results[2]<<",\"sigmaerr\":"<<results[3]<<"}";

/*	
	delete f_e1; delete f_e2; delete f_e3; delete f_mu;
	delete mu_corr; delete e1_corr; delete e2_corr; delete e3_corr;
	delete x_e1; delete x_e2; delete x_e3; delete x_mu;
	delete y_e1; delete y_e2; delete y_e3; delete y_mu;
	delete latex; delete c; delete frame; delete t;
*/	
	return results;

}
			
Double_t* SplitDataSet(TTree* t){
	Int_t sum = t->GetEntries();
	vector<Double_t>massZErr_series;
	massZErr_series.clear();
	
	for(Int_t i=0; i<sum; i++){
		t->GetEntry(i);
		if(massZ<120&&massZ>60&&RelIso1<0.35&&RelIso2<0.35){
//			if((abs(eta1)<0.8)||(abs(eta2)<0.8))continue;
//			if(abs(eta1)>1.44||abs(eta2)>1.44)continue;
			massZErr_series.push_back(massZErr/massZ);
		}
	}
	Int_t sub_sum = massZErr_series.size();
	sort(massZErr_series.begin(),massZErr_series.end());
	
	Int_t* Quantitle = new Int_t[11];
	Quantitle[0] = 0;
	Quantitle[1] = floor(sub_sum/10);
	Quantitle[2] = floor(sub_sum*2/10);
	Quantitle[3] = floor(sub_sum*3/10);
	Quantitle[4] = floor(sub_sum*4/10);
	Quantitle[5] = floor(sub_sum*5/10);
	Quantitle[6] = floor(sub_sum*6/10);
	Quantitle[7] = floor(sub_sum*7/10);
	Quantitle[8] = floor(sub_sum*8/10);
	Quantitle[9] = floor(sub_sum*9/10);
	Quantitle[10] = sub_sum;
	
	Double_t* cut = new Double_t[11];
	cut[0] = 0;
	cut[1] = massZErr_series[Quantitle[1]];
	cut[2] = massZErr_series[Quantitle[2]];
	cut[3] = massZErr_series[Quantitle[3]];
	cut[4] = massZErr_series[Quantitle[4]];
	cut[5] = massZErr_series[Quantitle[5]];
	cut[6] = massZErr_series[Quantitle[6]];
	cut[7] = massZErr_series[Quantitle[7]];
	cut[8] = massZErr_series[Quantitle[8]];
	cut[9] = massZErr_series[Quantitle[9]];
	cut[10] = 1;
/*	
	cut[0] = 0;
	cut[1] = 0.0136;
	cut[2] = 0.0142;
	cut[3] = 0.0152;
	cut[4] = 0.0169;
	cut[5] = 0.0195;
	cut[6] = 0.0221;
	cut[7] = 0.0247;
	cut[8] = 0.0274;
	cut[9] = 0.035;
	cut[10] = 1;
*/	
	return cut;
	
}
	
	
	
	
	

void SetAddress(TTree* t){
	t->SetBranchAddress("massZ",&massZ);
	t->SetBranchAddress("massZErr",&massZErr);
	t->SetBranchAddress("eta1",&eta1);
	t->SetBranchAddress("eta2",&eta2);
	t->SetBranchAddress("pT1",&pT1);
	t->SetBranchAddress("pT2",&pT2);
	t->SetBranchAddress("pterr1",&pterr1);
	t->SetBranchAddress("pterr2",&pterr2);
	t->SetBranchAddress("m1",&m1);
	t->SetBranchAddress("m2",&m2);
	t->SetBranchAddress("phi1",&phi1);
	t->SetBranchAddress("phi2",&phi2);
	t->SetBranchAddress("lep1_ecalDriven",&lep1_ecalDriven);
	t->SetBranchAddress("lep2_ecalDriven",&lep2_ecalDriven);
	t->SetBranchAddress("Iso1",&RelIso1);
	t->SetBranchAddress("Iso2",&RelIso2);
}
