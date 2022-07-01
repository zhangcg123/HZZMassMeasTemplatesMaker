#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TF1.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLatex.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooFFTConvPdf.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"
#include "RooPlot.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TSystem.h"
#include "HZZ2L2QRooPdfs.h"
#include "HZZ4LRooPdfs.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooLandau.h"
#include "RooBernstein.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "RooChi2Var.h"
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;
using namespace RooFit;

void Set4LSlimmedTree(TTree* t);
float crossSection;
float mass4l, mass4lErr, mass4lREFIT, mass4lErrREFIT, eventWeight, k_ggZZ, k_qqZZ_ewk, k_qqZZ_qcd_M, KD;
int finalState;
bool passedFullSelection;
int EventCat;
float pt1, pt2, pt3, pt4, eta1, eta2, eta3, eta4;

//for ZX code
int is2P2F, isMCzz, is3P1F;
float massZ2, D_bkg_kin, eventWeightFR, eventWeightFR_up, eventWeightFR_down, pileupWeight;
//

#define UL20
//#define ReReco
#ifdef UL20
string BASE = "20UL";
void Set4LSlimmedTree(TTree* t, TString type){
	//t->SetBranchAddress("EventCat",&EventCat);
	if(type == "reco"){
		t->SetBranchAddress("mass4l", &mass4l);
		t->SetBranchAddress("mass4lErr", &mass4lErr);
		t->SetBranchAddress("D_bkg_kin",&KD);
	}
	if(type == "refit"){
		t->SetBranchAddress("mass4lREFIT", &mass4l);
		t->SetBranchAddress("mass4lErrREFIT", &mass4lErr);
		t->SetBranchAddress("D_bkg_kin",&KD);
	}
	if(type == "bs"){
		t->SetBranchAddress("mass4l_vtx_BS",&mass4l);
		t->SetBranchAddress("mass4lErr_vtx_BS",&mass4lErr);
		t->SetBranchAddress("D_bkg_kin_vtx_BS",&KD);
	}
	if(type == "bsrefit"){
		t->SetBranchAddress("mass4lREFIT_vtx_BS",&mass4l);
		t->SetBranchAddress("mass4lErrREFIT_vtx_BS",&mass4lErr);
		t->SetBranchAddress("D_bkg_kin_vtx_BS",&KD);
	}
	t->SetBranchAddress("passedFullSelection", &passedFullSelection);
	t->SetBranchAddress("finalState", &finalState);
	t->SetBranchAddress("eventWeight",&eventWeight);
	t->SetBranchAddress("k_qqZZ_ewk",&k_qqZZ_ewk);
	t->SetBranchAddress("k_qqZZ_qcd_M",&k_qqZZ_qcd_M);
	t->SetBranchAddress("k_ggZZ",&k_ggZZ);
	//t->SetBranchAddress("crossSection",&crossSection);
	t->SetBranchAddress("pt1",&pt1);
	t->SetBranchAddress("pt2",&pt2);
	t->SetBranchAddress("pt3",&pt3);
	t->SetBranchAddress("pt4",&pt4);
	t->SetBranchAddress("eta1",&eta1);
	t->SetBranchAddress("eta2",&eta2);
	t->SetBranchAddress("eta3",&eta3);
	t->SetBranchAddress("eta4",&eta4);
}
#endif
#ifdef ReReco
string BASE = "ReReco";
void Set4LSlimmedTree(TTree* t, TString type){
	t->SetBranchAddress("EventCat",&EventCat);
	if(type == "reco"){
		t->SetBranchAddress("mass4l", &mass4l);
		t->SetBranchAddress("mass4lErr", &mass4lErr);
		t->SetBranchAddress("D_bkg_kin",&KD);
	}
	if(type == "refit"){
		t->SetBranchAddress("mass4lREFIT", &mass4l);
		t->SetBranchAddress("mass4lErrREFIT", &mass4lErr);
		t->SetBranchAddress("D_bkg_kin",&KD);
	}
	if(type == "bs"){
		t->SetBranchAddress("mass4l_vtx_BS",&mass4l);
		t->SetBranchAddress("mass4lErr_vtx_BS",&mass4lErr);
		t->SetBranchAddress("D_bkg_kin_vtx_BS",&KD);
	}
	if(type == "bsrefit"){
		t->SetBranchAddress("mass4lREFIT_vtx_BS",&mass4l);
		t->SetBranchAddress("mass4lErrREFIT_vtx_BS",&mass4lErr);
		t->SetBranchAddress("D_bkg_kin_vtx_BS",&KD);
	}
	t->SetBranchAddress("passedFullSelection", &passedFullSelection);
	t->SetBranchAddress("finalState", &finalState);
	t->SetBranchAddress("eventWeight",&eventWeight);
	t->SetBranchAddress("k_qqZZ_ewk",&k_qqZZ_ewk);
	t->SetBranchAddress("k_qqZZ_qcd_M",&k_qqZZ_qcd_M);
	t->SetBranchAddress("k_ggZZ",&k_ggZZ);
	//t->SetBranchAddress("crossSection",&crossSection);
	t->SetBranchAddress("pTL1",&pt1);
	t->SetBranchAddress("pTL2",&pt2);
	t->SetBranchAddress("pTL3",&pt3);
	t->SetBranchAddress("pTL4",&pt4);
	t->SetBranchAddress("etaL1",&eta1);
	t->SetBranchAddress("etaL2",&eta2);
	t->SetBranchAddress("etaL3",&eta3);
	t->SetBranchAddress("etaL4",&eta4);
}
#endif
