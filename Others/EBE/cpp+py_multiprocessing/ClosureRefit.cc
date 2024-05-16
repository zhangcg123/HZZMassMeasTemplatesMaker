#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
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
using namespace RooFit ;

void SetAddress(TTree* t);
Double_t* SplitDataSet(TTree* t, TString fs);
Double_t* DoClosure(TTree* t, Double_t a1, Double_t a2, TString fs, TString year);
void MakePlot(Double_t* x_refit, Double_t* y, Double_t* yErr, TString fs, TString year);

bool passedFullSelection;
Int_t finalState, ecalDrivenL1, ecalDrivenL2, ecalDrivenL3, ecalDrivenL4;
Float_t mass4lREFIT, mass4lErrREFIT, mL1, mL2, mL3, mL4, etaL1, etaL2, etaL3, etaL4, pTL1, pTL2, pTL3, pTL4, pTErrL1, pTErrL2, pTErrL3, pTErrL4, phiL1, phiL2, phiL3, phiL4;

void ClosureRefit(){
	
	TString year = "2016";
	TString fs = "4e";
	
	
	TFile* f = new TFile();
	if(year=="2016")f = new TFile("/raid/raid9/chenguan/Mass/Z1MassConstraint/liteUFHZZ4LAnalyzer_2016LUT/Ntuples/2016GGH_125.root");
	if(year=="2017")f = new TFile("/raid/raid9/chenguan/Mass/Z1MassConstraint/liteUFHZZ4LAnalyzer_2017LUT/Ntuples/2017GGH_125.root");
	if(year=="2018")f = new TFile("/raid/raid9/chenguan/Mass/Z1MassConstraint/liteUFHZZ4LAnalyzer_2018LUT/Ntuples/2018GGH_125.root");
	
	TTree* t = (TTree*)f->Get("passedEvents");
	SetAddress(t);
 	
	Double_t* cut = new Double_t[11];
	
	cut = SplitDataSet(t, fs);
	
	Double_t* x_refit = new Double_t[10];
	Double_t* y = new Double_t[10];
	Double_t* yErr = new Double_t[10];
	Double_t* tmp = new Double_t[3];
	
	for(Int_t i=0; i<8; i++){
		tmp = DoClosure(t, cut[i], cut[i+1], fs, year);
		x_refit[i] = tmp[0];
		y[i] = tmp[1];
		yErr[i] = tmp[2];
	}
	for(Int_t i=0; i<10; i++){
		cout<<x_refit[i]<<endl;
		cout<<y[i]<<endl;
		cout<<yErr[i]<<endl;
	}
	MakePlot(x_refit, y, yErr, fs, year);
	
	
}

void MakePlot(Double_t* x_reift, Double_t* y, Double_t* yErr, TString fs, TString year){
	gStyle->SetTitleOffset(1.5,"Y");	
	TCanvas* c = new TCanvas("c","c",1000,1000);
	c->cd();
	TGraph* g1 = new TGraphErrors(10,x_reift,y,0,yErr);

	g1->SetMarkerStyle(2);

	g1->SetMarkerColor(1);

	g1->SetTitle("");
	g1->GetXaxis()->SetTitle("predicted #sigma_{"+fs+"}");
	g1->GetYaxis()->SetTitle("measured #sigma_{"+fs+"}");
	g1->GetXaxis()->SetLimits(0,5);
	g1->GetHistogram()->SetMaximum(5);
	g1->GetHistogram()->SetMinimum(0);
	g1->Draw("AP");


	TLine *l1=new TLine(0,0,5,5);
	TLine *l2=new TLine(0,0,5/1.2,5);
	TLine *l3=new TLine(0,0,5,5/1.2);
	l1->SetLineStyle(kDashed);
	l2->SetLineStyle(kDashed);
	l3->SetLineStyle(kDashed);
	l1->Draw();
	l2->Draw();
	l3->Draw();

	TLegend *legend=new TLegend(0.2,0.75,0.45,0.9);
	legend->AddEntry(g1, "REFIT", "P");
	legend->SetTextSize(0.03);
	legend->SetLineWidth(2);
	legend->SetFillColor(0);
	legend->SetBorderSize(1);
	legend->Draw("SAME");
	
	TString plotpath;
	plotpath = "/home/chenguan/public_html/ClosureRefit/";

	TString filename = fs+"_ClosureTest"+".png";
	
	c->SaveAs(plotpath+year+"/"+filename);
	
}

Double_t* DoClosure(TTree* t, Double_t b1, Double_t b2, TString fs, TString year){
	
	Float_t Sum_mass4lErrREFIT = 0;
	Float_t Ave_mass4lErrREFIT = 0;
	
	Int_t counter = 0;
	int a1, a2;
	if(fs=="4mu"){a1 = 1; a2 = 1;}
	if(fs=="4e"){a1 = 2; a2 = 2;}
	if(fs=="2e2mu"){a1 = 3; a2 = 4;}
	
	RooRealVar mass4lREFIT_d("mass4lREFIT","mass4lREFIT",105,140);
	RooDataSet dataset("dataset","dataset",RooArgSet(mass4lREFIT_d));
		
	Int_t sum = t->GetEntries();
	for(Int_t i=0; i<sum; i++){
		t->GetEntry(i);
		if(passedFullSelection==1&&mass4lREFIT<140&&mass4lREFIT>105&&mass4lErrREFIT/mass4lREFIT<b2&&mass4lErrREFIT/mass4lREFIT>b1&&abs(etaL1)<2.4&&abs(etaL2)<2.4&&abs(etaL3)<2.4&&abs(etaL4)<2.4&&(finalState==a1||finalState==a2)){
			
			Sum_mass4lErrREFIT = Sum_mass4lErrREFIT + mass4lErrREFIT;
			
			counter = counter + 1;
			
			mass4lREFIT_d.setVal(mass4lREFIT);
			dataset.add(RooArgSet(mass4lREFIT_d));
		}
	}
			
		cout<<"___________________________________"<<counter<<endl;	
		mass4lREFIT_d.setBin(300);
		RooDataHist dataset_binned("dataset_binned","dataset_binned",RooArgSet(mass4lREFIT_d),dataset);
		RooRealVar mass4lREFIT("mass4lREFIT","mass4lREFIT",105,140);
		
		RooRealVar mean("mean","mean",125,120,130);
		RooRealVar sigma("sigma","sigma",2,0,10);
		RooRealVar alpha1("alpha1","alpha1",1,0,10);
		RooRealVar n1("n1","n1",1,0,10);
		RooRealVar alpha2("alpha2","alpha2",1,0,10);
		RooRealVar n2("n2","n2",1,0,10);
		RooDoubleCB DCB("DCB","DCB",mass4lREFIT,mean,sigma,alpha1,n1,alpha2,n2);
		DCB.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1),Timer(kTRUE));
		
		RooRealVar mean_("mean","mean",mean.getVal(),120,130);
		RooRealVar sigma_("sigma","sigma",sigma.getVal(),0,10);
		RooRealVar alpha1_("alpha1","alpha1",alpha1.getVal(),0,10);
		RooRealVar n1_("n1","n1",n1.getVal(),0,10);
		RooRealVar alpha2_("alpha2","alpha2",alpha2.getVal(),0,10);
		RooRealVar n2_("n2","n2",n2.getVal(),0,10);
		RooDoubleCB DCB_("DCB","DCB",mass4lREFIT,mean_,sigma_,alpha1_,n1_,alpha2_,n2_);
		DCB_.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1),Timer(kTRUE));
		RooPlot* frame = mass4lREFIT.frame();
		frame->SetTitle("");
		dataset_binned.plotOn(frame);
		DCB_.plotOn(frame,LineColor(2),LineWidth(1));
		
		Double_t chi2 = frame->chiSquare(6);
		TString mean_s, sigma_s, alpha1_s, n1_s, alpha2_s, n2_s, chi2_s, counter_s;
		mean_s = to_string(mean_.getVal());
		sigma_s = to_string(sigma_.getVal());
		alpha1_s = to_string(alpha1_.getVal());
		n1_s = to_string(n1_.getVal());
		alpha2_s = to_string(alpha2_.getVal());
		n2_s = to_string(n2_.getVal());
		counter_s = to_string(counter);
		chi2_s = to_string(chi2);
		
		TCanvas* c = new TCanvas("c","c",1400,1000);
		c->cd();
		frame->Draw();
		
		TLatex *latex=new TLatex();
		latex->SetNDC();
		latex->SetTextSize(0.05);
		latex->SetTextFont(42);
		latex->SetTextAlign(23);
		latex->DrawLatex(0.7,0.9,"#chi2/DOF="+chi2_s);
		latex->DrawLatex(0.7,0.8,"mean="+mean_s);
		latex->DrawLatex(0.7,0.7,"alpha1="+alpha1_s);
		latex->DrawLatex(0.7,0.6,"sigma="+sigma_s);
		latex->DrawLatex(0.7,0.5,"n1="+n1_s);
		latex->DrawLatex(0.7,0.4,"alpha2="+alpha2_s);
		latex->DrawLatex(0.7,0.3,"n2="+n2_s);
		latex->DrawLatex(0.7,0.2,"Entries="+counter_s);
		
		TString p1_s, p2_s;
		p1_s = to_string(b1);
		p2_s = to_string(b2);
		TString plotpath = "/home/chenguan/public_html/ClosureRefit/";
		TString filename = fs+"_Relaticve_mass4lErrREFIT_"+p1_s+"-"+p2_s+".png";
		TString finalpath;
		if(year=="2016")finalpath=plotpath+year+"/"+filename;
		if(year=="2017")finalpath=plotpath+year+"/"+filename;
		if(year=="2018")finalpath=plotpath+year+"/"+filename;
		c->SaveAs(finalpath);
		
		Double_t* results = new Double_t[3];
		results[0] = Sum_mass4lErrREFIT/counter;
		results[1] = sigma_.getVal();
		results[2] = sigma_.getError();
	
		return results;
		
	
}


Double_t* SplitDataSet(TTree* t, TString fs){
	Int_t sum = t->GetEntries();
	vector<Double_t>mass4lErr_series;
	mass4lErr_series.clear();
	int a1, a2;
	if(fs=="4mu"){a1 = 1; a2 = 1;}
	if(fs=="4e"){a1 = 2; a2 = 2;}
	if(fs=="2e2mu"){a1 = 3; a2 = 4;}
	
	

	for(Int_t i=0; i<sum; i++){
		t->GetEntry(i);
	
		if(passedFullSelection==1&&mass4lREFIT<140&&mass4lREFIT>105&&abs(etaL1)<2.4&&abs(etaL2)<2.4&&abs(etaL3)<2.4&&abs(etaL4)<2.4&&(finalState==a1||finalState==a2)){
			mass4lErr_series.push_back(mass4lErrREFIT/mass4lREFIT);
			
		}
	}
	
	Int_t sub_sum = mass4lErr_series.size();
	sort(mass4lErr_series.begin(),mass4lErr_series.end());
	Int_t* Quantitle = new Int_t[9];
	Quantitle[0] = 0;
	Quantitle[1] = floor(sub_sum/8);
	Quantitle[2] = floor(sub_sum*2/8);
	Quantitle[3] = floor(sub_sum*3/8);
	Quantitle[4] = floor(sub_sum*4/8);
	Quantitle[5] = floor(sub_sum*5/8);
	Quantitle[6] = floor(sub_sum*6/8);
	Quantitle[7] = floor(sub_sum*7/8);
	Quantitle[8] = floor(sub_sum*8/8);


	
//	for(Int_t i=0; i<11; i++){
//		cout<<Quantitle[i]<<endl;
//	}

	Double_t* cut = new Double_t[9];
	cut[0] = 0;
	cut[1] = mass4lErr_series[Quantitle[1]];
	cut[2] = mass4lErr_series[Quantitle[2]];
	cut[3] = mass4lErr_series[Quantitle[3]];

	cut[4] = mass4lErr_series[Quantitle[4]];
	cut[5] = mass4lErr_series[Quantitle[5]];
	cut[6] = mass4lErr_series[Quantitle[6]];
	cut[7] = mass4lErr_series[Quantitle[7]];
	cut[8] = 1;
	
	


	return cut;
	
	
}

void SetAddress(TTree* t){
	
	t->SetBranchAddress("mass4lREFIT",&mass4lREFIT);
	t->SetBranchAddress("mass4lErrREFIT",&mass4lErrREFIT);
	
	t->SetBranchAddress("finalState",&finalState);
	t->SetBranchAddress("passedFullSelection",&passedFullSelection);

	t->SetBranchAddress("etaL1",&etaL1);
	t->SetBranchAddress("etaL2",&etaL2);
	t->SetBranchAddress("etaL3",&etaL3);
	t->SetBranchAddress("etaL4",&etaL4);
	
	t->SetBranchAddress("pTL1",&pTL1);
	t->SetBranchAddress("pTL2",&pTL2);
	t->SetBranchAddress("pTL3",&pTL3);
	t->SetBranchAddress("pTL4",&pTL4);
	
	t->SetBranchAddress("pTErrL1",&pTErrL1);
	t->SetBranchAddress("pTErrL2",&pTErrL2);
	t->SetBranchAddress("pTErrL3",&pTErrL3);
	t->SetBranchAddress("pTErrL4",&pTErrL4);
	
	t->SetBranchAddress("mL1",&mL1);
	t->SetBranchAddress("mL2",&mL2);
	t->SetBranchAddress("mL3",&mL3);
	t->SetBranchAddress("mL4",&mL4);
	
	t->SetBranchAddress("phiL1",&phiL1);
	t->SetBranchAddress("phiL2",&phiL2);
	t->SetBranchAddress("phiL3",&phiL3);
	t->SetBranchAddress("phiL4",&phiL4);
	
	t->SetBranchAddress("ecalDrivenL1",&ecalDrivenL1);
	t->SetBranchAddress("ecalDrivenL2",&ecalDrivenL2);
	t->SetBranchAddress("ecalDrivenL3",&ecalDrivenL3);
	t->SetBranchAddress("ecalDrivenL4",&ecalDrivenL4);

	
}
