#include "TFile.h"
#include "TTree.h"
#include <iostream>
//using namespace RooFit;
using namespace std;

bool passedFullSelection;
int finalState;
float mass4l, mass4lErr; 
float mass4lREFIT, mass4lErrREFIT;
float mass4l_vtx_BS, mass4lErr_vtx_BS;
float mass4lREFIT_vtx_BS, mass4lErrREFIT_vtx_BS;
float D_bkg_kin, D_bkg_kin_vtx_BS;
float FSR_pt1, FSR_pt2, FSR_pt3, FSR_pt4, FSR_eta1, FSR_eta2, FSR_eta3, FSR_eta4, FSR_phi1, FSR_phi2, FSR_phi3, FSR_phi4, FSR_m1, FSR_m2, FSR_m3, FSR_m4;
float pt1, pt2, pt3, pt4, eta1, eta2, eta3, eta4, phi1, phi2, phi3, phi4, m1, m2, m3, m4;

void Slimmer(TTree* oldtree, int start, int end, TTree* newtree);
void SetNewTree(TTree* t);
void SetOldTree(TTree* t);

int main(int argc, char* argv[]){
	TString inputdir = argv[1];
	TString inputfile = argv[2];
	TString outputdir = argv[3];
	string job = argv[4];

	TFile* fin = new TFile(inputdir+"/"+inputfile+".root","READ");
	TTree* tin = (TTree*)fin->Get("passedEvents");
	SetOldTree(tin);
	int tot = tin->GetEntries();
	int perjob = tot/10;
	int start = stoi(job)*perjob;
	int end = (stoi(job)+1)*perjob;

	TFile* fou = new TFile(outputdir+"/"+inputfile+"_"+job+".root","RECREATE");
	TTree* tou = new TTree("passedEvents","passedEvents");
	SetNewTree(tou);

	Slimmer(tin, start, end, tou);

	fou->cd();
	tou->Write("passedEvents",TObject::kOverwrite);
	fou->Write();
	fou->Close();
}

void Slimmer(TTree* oldtree, int start, int end, TTree* newtree){
	for(int i=start; i<end; i++){
		if(i%10000==0)std::cout<<start<<"-"<<i<<"-"<<end<<std::endl;
		oldtree->GetEntry(i);
		if( true && ( (mass4l<140&&mass4l>105) || (mass4lREFIT<140&&mass4lREFIT>105) || (mass4l_vtx_BS<140&&mass4l_vtx_BS>105) || (mass4lREFIT_vtx_BS<140&&mass4lREFIT_vtx_BS>105) ) ){
			newtree->Fill();
		}
	}
}
void SetNewTree(TTree* t){
	
	t->Branch("passedFullSelection",&passedFullSelection,"passedFullSelection/O");
	t->Branch("finalState",&finalState,"finalState/I");

	t->Branch("mass4l",&mass4l,"mass4l/F");
	t->Branch("mass4lErr",&mass4lErr,"mass4lErr/F");
	
	t->Branch("mass4lREFIT",&mass4lREFIT,"mass4lREFIT/F");
	t->Branch("mass4lErrREFIT",&mass4lErrREFIT,"mass4lErrREFIT/F");
	
	t->Branch("mass4l_vtx_BS",&mass4l_vtx_BS,"mass4l_vtx_BS/F");
	t->Branch("mass4lErr_vtx_BS",&mass4lErr_vtx_BS,"mass4lErr_vtx_BS/F");
	
	t->Branch("mass4lREFIT_vtx_BS",&mass4lREFIT_vtx_BS,"mass4lREFIT_vtx_BS/F");
	t->Branch("mass4lErrREFIT_vtx_BS",&mass4lErrREFIT_vtx_BS,"mass4lErrREFIT_vtx_BS/F");
	
	t->Branch("D_bkg_kin",&D_bkg_kin,"D_bkg_kin/F");
	t->Branch("D_bkg_kin_vtx_BS",&D_bkg_kin_vtx_BS,"D_bkg_kin_vtx_BS/F");

}
void SetOldTree(TTree* t){

	t->SetBranchStatus("*",0);
	t->SetBranchStatus("finalState",1);
	t->SetBranchStatus("passedFullSelection",1);
	
	t->SetBranchStatus("mass4l",1);
	t->SetBranchStatus("mass4lErr",1);
	
	t->SetBranchStatus("mass4lREFIT",1);
	t->SetBranchStatus("mass4lErrREFIT",1);
	
	t->SetBranchStatus("mass4l_vtx_BS",1);
	t->SetBranchStatus("mass4lErr_vtx_BS",1);
	
	//t->SetBranchStatus("mass4l_vtxFSR_BS",1);
	//t->SetBranchStatus("mass4lErr_vtxFSR_BS",1);no this variable in Filippo's tree, so why does mass4l_vtxFSR_BS exist?
	
	t->SetBranchStatus("mass4lREFIT_vtx_BS",1);
	t->SetBranchStatus("mass4lErrREFIT_vtx_BS",1);
	
	t->SetBranchStatus("D_bkg_kin",1);
	t->SetBranchStatus("D_bkg_kin_vtx_BS",1);
	
	t->SetBranchAddress("finalState",&finalState);
	t->SetBranchAddress("passedFullSelection",&passedFullSelection);
	
	t->SetBranchAddress("mass4l",&mass4l);
	t->SetBranchAddress("mass4lErr",&mass4lErr);
	
	t->SetBranchAddress("mass4lREFIT",&mass4lREFIT);
	t->SetBranchAddress("mass4lErrREFIT",&mass4lErrREFIT);
	
	t->SetBranchAddress("mass4l_vtx_BS",&mass4l_vtx_BS);
	t->SetBranchAddress("mass4lErr_vtx_BS",&mass4lErr_vtx_BS);
	
	t->SetBranchAddress("mass4lREFIT_vtx_BS",&mass4lREFIT_vtx_BS);
	t->SetBranchAddress("mass4lErrREFIT_vtx_BS",&mass4lErrREFIT_vtx_BS);
	
	t->SetBranchAddress("D_bkg_kin",&D_bkg_kin);
	t->SetBranchAddress("D_bkg_kin_vtx_BS",&D_bkg_kin_vtx_BS);
}
