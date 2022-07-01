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
using namespace RooFit;
TString year="2017";
TString path = "";

void doplot(TString fs);

void ScaleSmearUnc( TString year_, TString path_ ){
	        
	year = year_;
	path = path_;
			doplot("4e");
//			doplot("2e2mu", year);
//			doplot("2mu2e", year);
			
}

void doplot(TString fs_){
		
	Int_t fs;
	if(fs_=="4e"){fs=2;}
		

	RooRealVar *mass4lno=new RooRealVar("mass4lno","mass4lno",105,140); 
	RooDataSet *dataset4lno=new RooDataSet("dataset4lno","dataset4lno",RooArgSet(*mass4lno));
	
	RooRealVar *mass4lscup=new RooRealVar("mass4lscup","mass4lscup",105,140); 
	RooDataSet *dataset4lscup=new RooDataSet("dataset4lscup","dataset4lscup",RooArgSet(*mass4lscup));
	
	RooRealVar *mass4lscdn=new RooRealVar("mass4lscdn","mass4lscdn",105,140); 
	RooDataSet *dataset4lscdn=new RooDataSet("dataset4lscdn","dataset4lscdn",RooArgSet(*mass4lscdn));
	
	RooRealVar *mass4lsmup=new RooRealVar("mass4lsmup","mass4lsmup",105,140);
	RooDataSet *dataset4lsmup=new RooDataSet("datasetsmup","datasetsmup",RooArgSet(*mass4lsmup));

	RooRealVar *mass4lsmdn=new RooRealVar("mass4lsmdn","mass4lsmdn",105,140);
	RooDataSet *dataset4lsmdn=new RooDataSet("datasetsmdn","datasetsmdn",RooArgSet(*mass4lsmdn));

	RooRealVar *mass4lptdepup=new RooRealVar("mass4lptdepup","mass4lptdepup",105,140);
	RooDataSet *dataset4lptdepup=new RooDataSet("dataset4lptdepup","dataset4lptdepup",RooArgSet(*mass4lptdepup));
	
	RooRealVar *mass4lptdepdn=new RooRealVar("mass4lptdepdn","mass4lptdepdn",105,140);
	RooDataSet *dataset4lptdepdn=new RooDataSet("dataset4lptdepdn","dataset4lptdepdn",RooArgSet(*mass4lptdepdn));

	vector<RooRealVar*> vars;vars.clear();
	vars.push_back(mass4lno);
	vars.push_back(mass4lscup);
	vars.push_back(mass4lscdn);
	vars.push_back(mass4lsmup);
	vars.push_back(mass4lsmdn);
	vars.push_back(mass4lptdepup);
	vars.push_back(mass4lptdepdn);

	TFile* f=new TFile();
	f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/crab_GGF125_20UL"+year+".root");
	TTree *t=(TTree*)f->Get("passedEvents");
	int sum=t->GetEntries();
	bool passedFullSelection;
	int finalState;
	float mass4l_no, mass4l_scaleup, mass4l_scaledn, mass4l_smearup, mass4l_smeardn, mass4l_ptdepup, mass4l_ptdepdn;	
	t->SetBranchAddress("finalState",&finalState);
	t->SetBranchAddress("passedFullSelection",&passedFullSelection);
	t->SetBranchAddress("mass4e_corr",&mass4l_no);
	t->SetBranchAddress("mass4e_scup",&mass4l_scaleup);
	t->SetBranchAddress("mass4e_scdn",&mass4l_scaledn);
	t->SetBranchAddress("mass4e_smup",&mass4l_smearup);
	t->SetBranchAddress("mass4e_smdn",&mass4l_smeardn);
	t->SetBranchAddress("mass4e_ptdepup",&mass4l_ptdepup);
	t->SetBranchAddress("mass4e_ptdepdn",&mass4l_ptdepdn);

	
	for(Int_t i=0; i<t->GetEntries(); i++){
		t->GetEntry(i);
		if(passedFullSelection==1&&finalState==fs){
			
			if(mass4l_no<140&&mass4l_no>105){//&&pTL1>7&&pTL2>7&&pTL3>7&&pTL4>7){
				*mass4lno=mass4l_no;
				dataset4lno->add(RooArgSet(*mass4lno));
			}
			if(mass4l_scaleup<140&&mass4l_scaleup>105){//&&pTL1_up<100&&pTL2_up<100&&pTL3_up<100&&pTL4_up<100){//&&pTL1_up>7&&pTL2_up>7&&pTL3_up>7&&pTL4_up>7){
				*mass4lscup=mass4l_scaleup;
				dataset4lscup->add(RooArgSet(*mass4lscup));
			}		
			if(mass4l_scaledn<140&&mass4l_scaledn>105){//&&pTL1_dn<100&&pTL2_dn<100&&pTL3_dn<100&&pTL4_dn<100){//&&pTL1_dn<7&&pTL2_dn<7&&pTL3_dn<7&&pTL4_dn<7){
				*mass4lscdn=mass4l_scaledn;
				dataset4lscdn->add(RooArgSet(*mass4lscdn));
			}
			if(mass4l_smearup<140&&mass4l_smearup>105){
				*mass4lsmup=mass4l_smearup;
				dataset4lsmup->add(RooArgSet(*mass4lsmup));
			}
			if(mass4l_smeardn<140&&mass4l_smeardn>105){
				*mass4lsmdn=mass4l_smeardn;
				dataset4lsmdn->add(RooArgSet(*mass4lsmdn));
			}
			if(mass4l_ptdepup<140&&mass4l_ptdepup>105){
				*mass4lptdepup=mass4l_ptdepup;
				dataset4lptdepup->add(RooArgSet(*mass4lptdepup));
			}
			if(mass4l_ptdepdn<140&&mass4l_ptdepdn>105){
				*mass4lptdepdn=mass4l_ptdepdn;
				dataset4lptdepdn->add(RooArgSet(*mass4lptdepdn));
			}
			
		}
		
	}
	vector<RooDataSet*> datasets;datasets.clear();
	datasets.push_back(dataset4lno);
	datasets.push_back(dataset4lscup);
	datasets.push_back(dataset4lscdn);
	datasets.push_back(dataset4lsmup);
	datasets.push_back(dataset4lsmdn);
	datasets.push_back(dataset4lptdepup);
	datasets.push_back(dataset4lptdepdn);
	vector<TString> names;names.clear();
	names.push_back("norm");
	names.push_back("scup");
	names.push_back("scdn");
	names.push_back("smup");
	names.push_back("smdn");
	names.push_back("ptdepup");
	names.push_back("ptdepdn");
	
	vector<double> scale_results; scale_results.clear();
	vector<double> smear_results; smear_results.clear();
	//make model
	for(int k=0; k<7; k++){

		RooRealVar meanDCB1("meanDCB","meanDCB",125,120,130);
		RooRealVar sigmaDCB1("sigmaDCB","sigmaDCB",1,0,10);
		RooRealVar alphaDCB1("alphaDCB","alphaDCB",1,0,10);
		RooRealVar nDCB1("nDCB","nDCB",1,0,10);
		RooRealVar alpha21("alpha2","alpha2",1,0,10);
		RooRealVar n21("n2","n2",1,0,10);
		RooDoubleCB DCBno("DCB","DCB",*vars.at(k),meanDCB1,sigmaDCB1,alphaDCB1,nDCB1,alpha21,n21);		
		DCBno.fitTo(*datasets.at(k),PrintLevel(-1),SumW2Error(kTRUE),Timer(kTRUE));
		RooPlot *frame1=vars.at(k)->frame(Bins(100));
		frame1->SetTitle("");
		datasets.at(k)->plotOn(frame1);
		DCBno.plotOn(frame1);
		DCBno.paramOn(frame1,Layout(0.1,0.4,0.9));
		
		Double_t chisquare1=frame1->chiSquare(6);
		TLatex *latex1=new TLatex();
		latex1->SetNDC();
		latex1->SetTextSize(0.05);
		latex1->SetTextFont(42);
		latex1->SetTextAlign(23);
		char chi21[20];
		sprintf(chi21,"%s%1.4f","#chi^{2}/DOF=",chisquare1);
	   
		TCanvas* c = new TCanvas("c","c",1400,1000);
		c->cd();
		frame1->Draw();
		latex1->DrawLatex(0.7, 0.8, chi21);
		c->SaveAs(path + "/" + names.at(k) + ".png");
		
		scale_results.push_back(meanDCB1.getVal());
		smear_results.push_back(sigmaDCB1.getVal());
	}
	for(int k=1; k<7; k++){
		cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!result!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
		cout<<"                              "<<"Scale"<<endl;
		cout<<names.at(k) + " = "<<((scale_results.at(k)-scale_results.at(0))/125)*100<<" %"<<endl;
		cout<<" "<<endl;
		cout<<names.at(k) + " = "<<((smear_results.at(k)-smear_results.at(0))/125)*100<<" %"<<endl;
		cout<<" "<<endl;
		cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	}



}
