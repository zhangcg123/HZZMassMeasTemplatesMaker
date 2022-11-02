#include "RooRealVar.h"                                                                         
#include "TStyle.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "RooBreitWigner.h"
#include "HZZ2L2QRooPdfs.h"
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
using namespace RooFit ;
using namespace std;
void SetAddress(TTree* t);
vector<double*> SplitDataSet(TTree* t);
Double_t* DoClosure(TTree* t, Double_t a1, Double_t a2, TString fs, TString year);
void MakePlot(Double_t* x_corr, Double_t* x_uncorr, Double_t* y, Double_t* yErr, TString fs, TString year);

bool passedFullSelection;
Int_t finalState, ecalDrivenL1, ecalDrivenL2, ecalDrivenL3, ecalDrivenL4;
Float_t mass4l, mass4lErr, mL1, mL2, mL3, mL4, etaL1, etaL2, etaL3, etaL4, pTL1, pTL2, pTL3, pTL4, pTErrL1, pTErrL2, pTErrL3, pTErrL4, phiL1, phiL2, phiL3, phiL4;
TString path;

int main(int argc, char* argv[]){
	
	TString year = argv[1];
	path = argv[2];	
	
	TString base = "/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/"+year+"/";
	TFile* f = new TFile(base+"GluGluHToZZTo4L_M125_"+year+"_skimmed.root","READ");
	
	TTree* t = (TTree*)f->Get("passedEvents");
	SetAddress(t);
 	
	vector<double*> cut;
	cut.clear();
	
	cut = SplitDataSet( t );
	
	/*	
	for ( int i=0; i<cut.size(); i++){
		for( int j=0; j<10; j++){
			cout<<cut.at(i)[j]<<endl;
		}
	}
	*/
	TString fs = "";	
	for ( int j=0; j<cut.size(); j++){
		if ( j == 0 ){
			fs = "4mu";
		}
		if ( j == 1 ){
			fs = "4e";
		}
		if ( j == 2 ){
			fs = "2e2mu";
		}
	cout<<j<<"      pppppppppppp	"<<fs<<endl;  
		Double_t* x_corr = new Double_t[10];
		Double_t* x_uncorr = new Double_t[10];
		Double_t* y = new Double_t[10];
		Double_t* yErr = new Double_t[10];
		Double_t* tmp = new Double_t[4];
		
		//sometimes change the relative mass error bins to get a better plot
//		Double_t bins20164e[11] = {0, 0.01, 0.0114, 0.0135, 0.0165, 0.021, 0.024, 0.027, 1};//2016 4e events	
//		Double_t bins20174e[11] = {0, 0.0131, 0.015, 0.0175, 0.0192, 0.022, 0.025, 0.03, 1};//2017 4e events
//		Double_t bins20174mu[11] = {0, 0.006931, 0.00768, 0.008544, 0.00935, 0.010192, 0.0113, 0.013104, 1};//2017 4mu
//		Double_t bins2018[11] = {0, 0.0125, 0.015, 0.018, 0.0199, 0.023, 0.0255, 0.03, 1};//2018 4e
		
		for(Int_t i=0; i<6; i++){
			tmp = DoClosure(t, cut[j][i], cut[j][i+1], fs, year);
			x_corr[i] = tmp[0];
			x_uncorr[i] = tmp[1];
			y[i] = tmp[2];
			yErr[i] = tmp[3];
		}
		
		MakePlot(x_corr, x_uncorr, y, yErr, fs, year);
		
		delete x_corr;
		delete x_uncorr;
		delete y;
		delete yErr;
		delete tmp;	
	}	
	
}

void MakePlot(Double_t* x_corr, Double_t* x_uncorr, Double_t* y, Double_t* yErr, TString fs, TString year){
	gStyle->SetTitleOffset(1.5,"Y");	
	TCanvas* c = new TCanvas("c","c",1000,1000);
	c->cd();
	TGraph* g1 = new TGraphErrors(10,x_corr,y,0,yErr);
	TGraph* g2 = new TGraphErrors(10,x_uncorr,y,0,yErr);
	g1->SetMarkerStyle(20);
	g2->SetMarkerStyle(20);
	g1->SetMarkerColor(2);
	g2->SetMarkerColor(1);
	g1->SetTitle("");
	g1->GetXaxis()->SetTitle("predicted #sigma_{"+fs+"}");
	g1->GetYaxis()->SetTitle("measured #sigma_{"+fs+"}");
	g1->GetXaxis()->SetLimits(0,5);
	g1->GetHistogram()->SetMaximum(5);
	g1->GetHistogram()->SetMinimum(0);
	g1->Draw("AP");
	//g2->Draw("p");

	TLine *l1=new TLine(0,0,5,5);
	TLine *l2=new TLine(0,0,5/1.2,5);
	TLine *l3=new TLine(0,0,5,5/1.2);
	
	TLine *l4=new TLine(0,0,5/1.1,5);
	TLine *l5=new TLine(0,0,5,5/1.1);
	l1->SetLineStyle(kDashed);
	l2->SetLineStyle(kDashed);
	l3->SetLineStyle(kDashed);
	l4->SetLineStyle(kDashed);
	l5->SetLineStyle(kDashed);	
	l1->Draw();
	l4->Draw();
	l5->Draw();

	TLegend *legend=new TLegend(0.2,0.75,0.45,0.9);
	legend->AddEntry(g1, "Corr", "P");
	legend->AddEntry(g2, "unCorr", "P");
	legend->SetTextSize(0.03);
	legend->SetLineWidth(2);
	legend->SetFillColor(0);
	legend->SetBorderSize(1);
	//legend->Draw("SAME");
	
	TString plotpath;
	plotpath = path;

	TString filename = fs+"_ClosureTest";
	
	c->SaveAs(path + "/" + filename+".pdf");
	c->SaveAs(path + "/" + filename+".png");
	
}

Double_t* DoClosure(TTree* t, Double_t b1, Double_t b2, TString fs, TString year){
	
	Float_t Sum_mass4lErr_corr = 0;
	Float_t Sum_mass4lErr_uncorr = 0;
	Float_t Ave_mass4lErr_corr = 0;
	Float_t Ave_mass4lErr_uncorr = 0;
	Int_t counter = 0;
	int a1, a2;
	if(fs=="4mu"){a1 = 1; a2 = 1;}
	if(fs=="4e"){a1 = 2; a2 = 2;}
	if(fs=="2e2mu"||fs=="2mu2e"){a1 = 3; a2 = 4;}
	
	RooRealVar mass4l_d("mass4l","mass4l",105,140);
	RooDataSet dataset("dataset","dataset",RooArgSet(mass4l_d));
		
	Int_t sum = t->GetEntries();
	for(Int_t i=0; i<sum; i++){
		t->GetEntry(i);
		if(passedFullSelection==1&&mass4l<140&&mass4l>105&&mass4lErr/mass4l<b2&&mass4lErr/mass4l>b1&&(finalState==a1||finalState==a2)){
			
			//Sum_mass4lErr_uncorr = Sum_mass4lErr_uncorr + mass4lErr_uncorr;
			Sum_mass4lErr_corr = Sum_mass4lErr_corr + mass4lErr;
			counter = counter + 1;	
			mass4l_d.setVal(mass4l);
			dataset.add(RooArgSet(mass4l_d));
		}
	}
			
	//	cout<<"___________________________________"<<counter<<endl;	
		//mass4l_d.setBin(300);
		RooDataHist dataset_binned("dataset_binned","dataset_binned",RooArgSet(mass4l_d),dataset);
		RooRealVar mass4l("mass4l","mass4l",105,140);
		
		RooRealVar mean("mean","mean",125,120,130);
		RooRealVar sigma("sigma","sigma",2,0,10);
		RooRealVar alpha1("alpha1","alpha1",1,0,10);
		RooRealVar n1("n1","n1",1,0,20);
		RooRealVar alpha2("alpha2","alpha2",1,0,10);
		RooRealVar n2("n2","n2",1,0,60);
		RooDoubleCB DCB("DCB","DCB",mass4l,mean,sigma,alpha1,n1,alpha2,n2);
		DCB.fitTo(dataset_binned,SumW2Error(kTRUE),PrintLevel(-1),Timer(kTRUE));
		
		RooRealVar mean_("mean","mean",mean.getVal(),120,130);
		RooRealVar sigma_("sigma","sigma",sigma.getVal(),0,10);
		RooRealVar alpha1_("alpha1","alpha1",alpha1.getVal(),0,10);
		RooRealVar n1_("n1","n1",n1.getVal(),0,20);
		RooRealVar alpha2_("alpha2","alpha2",alpha2.getVal(),0,10);
		RooRealVar n2_("n2","n2",n2.getVal(),0,60);
		RooDoubleCB DCB_("DCB","DCB",mass4l,mean_,sigma_,alpha1_,n1_,alpha2_,n2_);
		DCB_.fitTo(dataset_binned,SumW2Error(kTRUE),PrintLevel(-1),Timer(kTRUE));
		RooPlot* frame = mass4l.frame();
		frame->SetTitle("");
		dataset_binned.plotOn(frame);
		DCB_.plotOn(frame,LineColor(2),LineWidth(1));
		
		Double_t chi2 = frame->chiSquare(6);
		TString mean_s, sigma_s, alpha1_s, n1_s, alpha2_s, n2_s, chi2_s, counter_s, sigmaErr_s;
		mean_s = to_string(mean_.getVal());
		sigma_s = to_string(sigma_.getVal());
		alpha1_s = to_string(alpha1_.getVal());
		n1_s = to_string(n1_.getVal());
		alpha2_s = to_string(alpha2_.getVal());
		n2_s = to_string(n2_.getVal());
		counter_s = to_string(counter);
		chi2_s = to_string(chi2);
		sigmaErr_s = to_string(sigma_.getError());
		
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
		latex->DrawLatex(0.7,0.6,"sigma="+sigma_s+"+/-"+sigmaErr_s);
		latex->DrawLatex(0.7,0.5,"n1="+n1_s);
		latex->DrawLatex(0.7,0.4,"alpha2="+alpha2_s);
		latex->DrawLatex(0.7,0.3,"n2="+n2_s);
		latex->DrawLatex(0.7,0.2,"Entries="+counter_s);
		
		TString p1_s, p2_s;
		p1_s = to_string(b1);
		p2_s = to_string(b2);
		TString filename = fs+"_Relaticve_mass4lErr_"+p1_s+"-"+p2_s+".png";
		c->SaveAs(path + "/" + filename );
		
		Double_t* results = new Double_t[4];
		results[0] = Sum_mass4lErr_corr/counter;
		results[1] = Sum_mass4lErr_uncorr/counter;
		results[2] = sigma_.getVal();
		results[3] = sigma_.getError();	
		return results;	
}


vector<double*> SplitDataSet(TTree* t){
	Int_t sum = t->GetEntries();
	vector<Double_t>mass4eErr_series;
	vector<Double_t>mass4muErr_series;
	vector<Double_t>mass2e2muErr_series;
	mass4eErr_series.clear();
	mass4muErr_series.clear();
	mass2e2muErr_series.clear();
	
	for(Int_t i=0; i<sum; i++){
		t->GetEntry(i);	
		if(passedFullSelection==1&&mass4l<140&&mass4l>105){
			if ( finalState == 1 ){	
				mass4muErr_series.push_back(mass4lErr/mass4l);
			}
			if ( finalState == 2){
				mass4eErr_series.push_back(mass4lErr/mass4l);
			}
			if ( finalState == 3 || finalState == 4 ){
				mass2e2muErr_series.push_back(mass4lErr/mass4l);
			}
		}
	}

	vector<vector<Double_t>> ss; ss.clear();
	ss.push_back( mass4muErr_series );
	ss.push_back( mass4eErr_series );
	ss.push_back( mass2e2muErr_series );
	
	vector<double*> bounds; bounds.clear();	
	
	for ( auto & mass4lErr_series : ss ){
				
		Int_t sub_sum = mass4lErr_series.size();
		sort(mass4lErr_series.begin(),mass4lErr_series.end());
		Int_t* Quantitle = new Int_t[7];
		Quantitle[0] = 0;
		Quantitle[1] = floor(sub_sum/6);
		Quantitle[2] = floor(sub_sum*2/6);
		Quantitle[3] = floor(sub_sum*3/6);
		Quantitle[4] = floor(sub_sum*4/6);
		Quantitle[5] = floor(sub_sum*5/6);
		Quantitle[6] = sub_sum;
		
	//	for(Int_t i=0; i<11; i++){
	//		cout<<Quantitle[i]<<endl;
	//	}

		double* cut = new double[7];
		cut[0] = 0.0;
		cut[1] = mass4lErr_series[Quantitle[1]];
		cut[2] = mass4lErr_series[Quantitle[2]];
		cut[3] = mass4lErr_series[Quantitle[3]];

		cut[4] = mass4lErr_series[Quantitle[4]];
		cut[5] = mass4lErr_series[Quantitle[5]];
		cut[6] = 1.0;
		bounds.push_back( cut );
	}
	return bounds;	
}

void SetAddress(TTree* t){
	t->SetBranchAddress("mass4l",&mass4l);
	t->SetBranchAddress("mass4lErr",&mass4lErr);
	
	t->SetBranchAddress("finalState",&finalState);
	t->SetBranchAddress("passedFullSelection",&passedFullSelection);
	/*
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
	*/	
}	
