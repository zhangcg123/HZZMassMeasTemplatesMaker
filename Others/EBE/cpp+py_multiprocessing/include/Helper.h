#include "CommonTool.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooDataHist.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "RooExponential.h"
#include "RooBreitWigner.h"
#include "RooAddPdf.h"
#include "RooFFTConvPdf.h"
#include "HZZ2L2QRooPdfs.h"
#include "RooCBShape.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooChi2Var.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLorentzVector.h"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
using namespace RooFit;


bool recursion = false;

double pt1, pt2, pterr1, pterr2, eta1, eta2, phi1, phi2, m1, m2, mass, Iso1, Iso2, masserr, weight_, RelIso1, RelIso2;
int lep1_ecalDriven, lep2_ecalDriven, Tight1, Tight2;

double l1, l2, l3;

void SetTree(TTree* t);
void SetTreeForMuons( TTree* t );
void MakeOutput(TString plotname, double lambda);
RooDataHist* MakeDataSet(TTree* t, bool isEcal, double abseta_low, double abseta_high, double deltapt_low, double deltapt_high, double lambda);
RooDataHist* MakeDataSet(TTree* t, double abseta_low, double abseta_high, double relpterr_low, double relpterr_high);
double EBECalculator(TTree* t, bool isEcal, TString plotname, double abseta_low, double abseta_high, double deltapt_low, double deltapt_high, double lambda, int inter);
double EBECalculator(TTree* t_, TString plotname, double abseta_low, double abseta_high, double relpterr_low, double relpterr_high);
void SetFirst3Lambdas(TString samples, TString year, TString isData);

void SetFirst3Lambdas(TString samples, TString year, TString isData){
	if(samples == "madgraph"){
		if(year == "2016post"){
			if(isData == "MC"){
				l1 = 1.644;
				l2 = 1.470;
				l3 = 1.272;
			}else{
				l1 = 1.65;
				l2 = 1.53;
				l3 = 1.29;
			}
		}
		if(year == "2016pre"){
			if(isData == "MC"){
				l1 = 1.648;
				l2 = 1.490;
				l3 = 1.229;
			}else{
				l1 = 1.619;
				l2 = 1.506;
				l3 = 1.311;
			}
		}
		if(year == "2017"){
			if(isData == "MC"){
				l1 = 1.773;
				l2 = 1.548;
				l3 = 1.380;
			}else{
				l1 = 1.738;
				l2 = 1.512;
				l3 = 1.404;
			}
		}
		if(year == "2018"){
			if(isData == "MC"){
				l1 = 1.792;
				l2 = 1.531;
				l3 = 1.372;
			}else{
				l1 = 1.754;
				l2 = 1.487;
				l3 = 1.356;
			}
		}
	}
}

void MakeOutput(TString plotname, double lambda){
	string lambda_s = to_string(lambda);
	ofstream fo;
	fo.open(plotname + '_' + lambda_s + ".txt");
	fo<<lambda_s<<endl;
	fo.close();
}

void SetTree(TTree* t){
	t->SetBranchStatus("*",0);
	t->SetBranchStatus("pT1",1);
	t->SetBranchStatus("pT2",1);
	t->SetBranchStatus("pterr1",1);
	t->SetBranchStatus("pterr2",1);
	t->SetBranchStatus("eta1",1);
	t->SetBranchStatus("eta2",1);
	t->SetBranchStatus("phi1",1);
	t->SetBranchStatus("phi2",1);
	t->SetBranchStatus("m1",1);
	t->SetBranchStatus("m2",1);
	t->SetBranchStatus("Iso1",1);
	t->SetBranchStatus("Iso2",1);
	t->SetBranchStatus("massZ",1);
	t->SetBranchStatus("massZErr",1);
	t->SetBranchStatus("weight",1);
	t->SetBranchStatus("lep1_ecalDriven",1);
	t->SetBranchStatus("lep2_ecalDriven",1);
	t->SetBranchStatus("Tight1",1);
	t->SetBranchStatus("Tight2",1);
			
	t->SetBranchAddress("pT1",&pt1);
	t->SetBranchAddress("pT2",&pt2);
	t->SetBranchAddress("eta1",&eta1);
	t->SetBranchAddress("eta2",&eta2);
	t->SetBranchAddress("phi1",&phi1);
	t->SetBranchAddress("phi2",&phi2);
	t->SetBranchAddress("m1",&m1);
	t->SetBranchAddress("m2",&m2);
	t->SetBranchAddress("Iso1",&Iso1);
	t->SetBranchAddress("Iso2",&Iso2);
	t->SetBranchAddress("Tight1",&Tight1);
	t->SetBranchAddress("Tight2",&Tight2);
	t->SetBranchAddress("pterr1",&pterr1);
	t->SetBranchAddress("pterr2",&pterr2);
	t->SetBranchAddress("massZ",&mass);
	t->SetBranchAddress("massZErr",&masserr);
	t->SetBranchAddress("weight",&weight_);
	t->SetBranchAddress("lep1_ecalDriven",&lep1_ecalDriven);
	t->SetBranchAddress("lep2_ecalDriven",&lep2_ecalDriven);
}

RooDataHist* MakeDataSet(TTree* t, double abseta_low, double abseta_high, double relpterr_low, double relpterr_high){
	TH2D* histo = new TH2D("histo","",100,80,100,20,0,5);
	int totEvt = t->GetEntries();
	TLorentzVector lep1, lep2, lep1p, lep2p;
	for(int i=0; i<totEvt; i++){
		if(i%1000000==0)cout<<i<<"/"<<totEvt<<endl;
		//if(i>1000)break;
		t->GetEntry(i);
		if( /*Tight1 == 1 && Tight2 == 1 && RelIso1 < 0.35 && RelIso2 < 0.35 && */ mass < 100 && mass > 80 && (pterr1/pt1) < relpterr_high && (pterr2/pt2) < relpterr_high && (pterr1/pt1) > relpterr_low && (pterr2/pt2) > relpterr_low && abs(eta1) < abseta_high && abs(eta1) > abseta_low && abs(eta2) > abseta_low && abs(eta2) < abseta_high ){
			lep1.SetPtEtaPhiM(pt1,eta1,phi1,m1);
			lep2.SetPtEtaPhiM(pt2,eta2,phi2,m2);
			lep1p.SetPtEtaPhiM(pt1+pterr1,eta1,phi1,m1);
			lep2p.SetPtEtaPhiM(pt2+pterr2,eta2,phi2,m2);
			double dm1 = (lep1+lep2p).M() - (lep1+lep2).M();
			double dm2 = (lep1p+lep2).M() - (lep1+lep2).M();
			masserr = TMath::Sqrt(dm1*dm1 + dm2*dm2);
			histo->Fill(mass,masserr);
		}
	}
	RooRealVar massZ("massZ","",80,100);
	RooRealVar massZErr("massZErr","",0,5);
	massZ.setBins(2000,"cache");
	massZErr.setBins(20,"cache");
	RooDataHist* Data = new RooDataHist("Data","",RooArgSet(massZ,massZErr),Import(*histo));	
	delete histo;
	return Data;
}

RooDataHist* MakeDataSet(TTree* t, bool isEcal, double abseta_low, double abseta_high, double deltapt_low, double deltapt_high, double lambda){
	//ecal cut
	int ecal1, ecal2;
	if(isEcal == true){
		ecal1=1; ecal2=1;
	}
	if(isEcal == false && recursion == false){
		ecal1=0; ecal2=0;
	}
	if(isEcal == false && recursion == true){
		ecal1=1; ecal2=0;
	}
	//read tree
	int totEvents = t->GetEntries();
	TH2D* histo = new TH2D("histo","",100,80,100,20,0,10);
	if(recursion==false){
		for(int i=0; i<totEvents; i++){
			if(i%1000000==0)cout<<i<<"/"<<totEvents<<endl;
			t->GetEntry(i);
			//if(i>100000)break;
			if(mass<100 && mass>80 && lep1_ecalDriven==ecal1 && lep2_ecalDriven==ecal2 && Tight1==1 && Tight2==1 && Iso1 < 0.35 && Iso2 < 0.35){
				if( (pterr1/pt1) < deltapt_high && (pterr1/pt1) > deltapt_low && (pterr2/pt2) < deltapt_high && (pterr2/pt2) > deltapt_low &&
					abs(eta1) < abseta_high && abs(eta1)>abseta_low && abs(eta2) < abseta_high && abs(eta2) > abseta_low){
					histo->Fill(mass,masserr);
				}
			}
		}
	}
	else{
		double L1=1, L2=1, dm1=0, dm2=0, dmz=0;
		TLorentzVector lep1 = TLorentzVector(0,0,0,0);
		TLorentzVector lep2 = TLorentzVector(0,0,0,0);
		TLorentzVector lep1p = TLorentzVector(0,0,0,0);
		TLorentzVector lep2p = TLorentzVector(0,0,0,0);
		for(int i=0; i<totEvents; i++){
			if(i%1000000==0)cout<<i<<"/"<<totEvents<<endl;
			t->GetEntry(i);
			//if(i>100000)break;
			if(mass<100 && mass>80 && Tight1==1 && Tight2==1 && Iso1 < 0.35 && Iso2 < 0.35 ){
				if( (pterr1/pt1) < 0.025 && abs(eta1) < 0.8 &&  lep1_ecalDriven==ecal1 && lep2_ecalDriven==ecal2 &&
					abs(eta2) < abseta_high && abs(eta2) > abseta_low && (pterr2/pt2) > deltapt_low && abs(pterr2/pt2) < deltapt_high){
					L2=lambda;
					if( (pterr1/pt1) < 0.01)L1=l1;
					if( (pterr1/pt1) < 0.015 && (pterr1/pt1) > 0.01)L1=l2;
					if( (pterr1/pt1) < 0.025 && (pterr1/pt1) > 0.015)L1=l3;
					lep1.SetPtEtaPhiM(pt1,eta1,phi1,m1);
					lep2.SetPtEtaPhiM(pt2,eta2,phi2,m2);
					lep1p.SetPtEtaPhiM(pt1+(pterr1)*L1,eta1,phi1,m1);
					lep2p.SetPtEtaPhiM(pt2+(pterr2)*L2,eta2,phi2,m2);
					dm1 = (lep1p+lep2).M()-(lep1+lep2).M();
					dm2 = (lep1+lep2p).M()-(lep1+lep2).M();
					dmz = TMath::Sqrt(dm1*dm1+dm2*dm2);
					histo->Fill(mass,dmz);
				}
				if( (pterr2/pt2) < 0.025 && abs(eta2) < 0.8 &&  lep1_ecalDriven==ecal2 && lep2_ecalDriven==ecal1 && 
					abs(eta1) < abseta_high && abs(eta1) > abseta_low && (pterr1/pt1) > deltapt_low && abs(pterr1/pt1) < deltapt_high){
					L1=lambda;
					if( (pterr2/pt2) < 0.01)L2=l1;
					if( (pterr2/pt2) < 0.015 && (pterr2/pt2) > 0.01)L2=l2;
					if( (pterr2/pt2) < 0.025 && (pterr2/pt2) > 0.015)L2=l3;
					lep1.SetPtEtaPhiM(pt1,eta1,phi1,m1);
					lep2.SetPtEtaPhiM(pt2,eta2,phi2,m2);
					lep1p.SetPtEtaPhiM(pt1+(pterr1)*L1,eta1,phi1,m1);
					lep2p.SetPtEtaPhiM(pt2+(pterr2)*L2,eta2,phi2,m2);
					dm1 = (lep1p+lep2).M()-(lep1+lep2).M();
					dm2 = (lep1+lep2p).M()-(lep1+lep2).M();
					dmz = TMath::Sqrt(dm1*dm1+dm2*dm2);
					histo->Fill(mass,dmz);
				}
			}
		}
	}
	//recursion
	if(histo->GetEntries()<5000 && recursion==false){
		delete histo;
		recursion = true;
		RooDataHist* Data = MakeDataSet(t, isEcal, abseta_low, abseta_high, deltapt_low, deltapt_high, lambda);
		return Data;
	}
	//make dataset
	RooRealVar massZ("massZ","",80,100);
	RooRealVar massZErr("massZErr","",0,10);
	massZ.setBins(1000,"cache");
	massZErr.setBins(10,"cache");
	RooDataHist* Data = new RooDataHist("Data","",RooArgSet(massZ,massZErr),Import(*histo));	
	delete histo;
	return Data;
}

void SetTreeForMuons( TTree* t ){
        
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
	t->SetBranchStatus("weight",1);
	t->SetBranchStatus("Iso1",1);
	t->SetBranchStatus("Iso2",1);
	t->SetBranchStatus("Tight1",1);
	t->SetBranchStatus("Tight2",1);
			
	t->SetBranchAddress("vtx_BS_pT1",&pt1);
	t->SetBranchAddress("vtx_BS_pT2",&pt2);
	t->SetBranchAddress("vtx_BS_eta1",&eta1);
	t->SetBranchAddress("vtx_BS_eta2",&eta2);
	t->SetBranchAddress("phi1",&phi1);
	t->SetBranchAddress("phi2",&phi2);
	t->SetBranchAddress("m1",&m1);
	t->SetBranchAddress("m2",&m2);
	t->SetBranchAddress("pterr1_VX_BS",&pterr1);
	t->SetBranchAddress("pterr2_VX_BS",&pterr2);
	t->SetBranchAddress("massZ_vtx_BS",&mass);
	t->SetBranchAddress("massZErr_vtx_BS",&masserr);
	t->SetBranchAddress("weight",&weight_);
	t->SetBranchAddress("Iso1",&RelIso1);
	t->SetBranchAddress("Iso2",&RelIso2);
	t->SetBranchAddress("Tight1",&Tight1);
	t->SetBranchAddress("Tight2",&Tight2);
}
