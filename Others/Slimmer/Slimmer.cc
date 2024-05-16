#include "TFile.h"
#include "TTree.h"
#include <iostream>
//using namespace RooFit;
using namespace std;

std::vector<float>* lep_pt;
std::vector<float>* lep_eta;
std::vector<float>* lep_phi;
std::vector<float>* lep_mass;
std::vector<float>* lepFSR_pt;
std::vector<float>* lepFSR_eta;
std::vector<float>* lepFSR_phi;
std::vector<float>* lepFSR_mass;
std::vector<float>* lep_pterr;
std::vector<int>* lep_ecalDriven;
std::vector<int>* lep_id;
int lep_Hindex[4];
bool passedFullSelection;
int finalState;
int EventCat;
float mass4l, mass4lErr, mass4lREFIT, mass4lErrREFIT, mass4l_vtx_BS, mass4lErr_vtx_BS, mass4l_vtxFSR_BS, mass4lREFIT_vtx_BS, mass4lErrREFIT_vtx_BS;
float D_bkg_kin, D_bkg_kin_vtx_BS, eventWeight, k_ggZZ, k_qqZZ_qcd_M, k_qqZZ_ewk;
float FSR_pt1, FSR_pt2, FSR_pt3, FSR_pt4, FSR_eta1, FSR_eta2, FSR_eta3, FSR_eta4, FSR_phi1, FSR_phi2, FSR_phi3, FSR_phi4, FSR_m1, FSR_m2, FSR_m3, FSR_m4;
float pt1, pt2, pt3, pt4, eta1, eta2, eta3, eta4, phi1, phi2, phi3, phi4, m1, m2, m3, m4;
float pterr1, pterr2, pterr3, pterr4;
int ecaldriven1, ecaldriven2, ecaldriven3, ecaldriven4, id1, id2, id3, id4;
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
		if(passedFullSelection){
			FSR_pt1 = (*lepFSR_pt)[lep_Hindex[0]];
			FSR_pt2 = (*lepFSR_pt)[lep_Hindex[1]];
			FSR_pt3 = (*lepFSR_pt)[lep_Hindex[2]];
			FSR_pt4 = (*lepFSR_pt)[lep_Hindex[3]];
			FSR_eta1 = (*lepFSR_eta)[lep_Hindex[0]];
			FSR_eta2 = (*lepFSR_eta)[lep_Hindex[1]];
			FSR_eta3 = (*lepFSR_eta)[lep_Hindex[2]];
			FSR_eta4 = (*lepFSR_eta)[lep_Hindex[3]];
			FSR_phi1 = (*lepFSR_phi)[lep_Hindex[0]];
			FSR_phi2 = (*lepFSR_phi)[lep_Hindex[1]];
			FSR_phi3 = (*lepFSR_phi)[lep_Hindex[2]];
			FSR_phi4 = (*lepFSR_phi)[lep_Hindex[3]];
			FSR_m1 = (*lepFSR_mass)[lep_Hindex[0]];
			FSR_m2 = (*lepFSR_mass)[lep_Hindex[1]];
			FSR_m3 = (*lepFSR_mass)[lep_Hindex[2]];
			FSR_m4 = (*lepFSR_mass)[lep_Hindex[3]];
			
			pt1 = (*lep_pt)[lep_Hindex[0]];
			pt2 = (*lep_pt)[lep_Hindex[1]];
			pt3 = (*lep_pt)[lep_Hindex[2]];
			pt4 = (*lep_pt)[lep_Hindex[3]];
			eta1 = (*lep_eta)[lep_Hindex[0]];
			eta2 = (*lep_eta)[lep_Hindex[1]];
			eta3 = (*lep_eta)[lep_Hindex[2]];
			eta4 = (*lep_eta)[lep_Hindex[3]];
			phi1 = (*lep_phi)[lep_Hindex[0]];
			phi2 = (*lep_phi)[lep_Hindex[1]];
			phi3 = (*lep_phi)[lep_Hindex[2]];
			phi4 = (*lep_phi)[lep_Hindex[3]];
			m1 = (*lep_mass)[lep_Hindex[0]];
			m2 = (*lep_mass)[lep_Hindex[1]];
			m3 = (*lep_mass)[lep_Hindex[2]];
			m4 = (*lep_mass)[lep_Hindex[3]];
			pterr1 = (*lep_pterr)[lep_Hindex[0]];
			pterr2 = (*lep_pterr)[lep_Hindex[1]];
			pterr3 = (*lep_pterr)[lep_Hindex[2]];
			pterr4 = (*lep_pterr)[lep_Hindex[3]];
			ecaldriven1 = (*lep_ecalDriven)[lep_Hindex[0]];
			ecaldriven2 = (*lep_ecalDriven)[lep_Hindex[1]];
			ecaldriven3 = (*lep_ecalDriven)[lep_Hindex[2]];
			ecaldriven4 = (*lep_ecalDriven)[lep_Hindex[3]];
			id1 = (*lep_id)[lep_Hindex[0]];
			id2 = (*lep_id)[lep_Hindex[1]];
			id3 = (*lep_id)[lep_Hindex[2]];
			id4 = (*lep_id)[lep_Hindex[3]];
			newtree->Fill();
		}
	}
}
void SetNewTree(TTree* t){
	t->Branch("FSR_pt1",&FSR_pt1,"FSR_pt1/F");
	t->Branch("FSR_pt2",&FSR_pt2,"FSR_pt2/F");
	t->Branch("FSR_pt3",&FSR_pt3,"FSR_pt3/F");
	t->Branch("FSR_pt4",&FSR_pt4,"FSR_pt4/F");
	t->Branch("FSR_eta1",&FSR_eta1,"FSR_eta1/F");
	t->Branch("FSR_eta2",&FSR_eta2,"FSR_eta2/F");
	t->Branch("FSR_eta3",&FSR_eta3,"FSR_eta3/F");
	t->Branch("FSR_eta4",&FSR_eta4,"FSR_eta4/F");
	t->Branch("FSR_phi1",&FSR_phi1,"FSR_phi1/F");
	t->Branch("FSR_phi2",&FSR_phi2,"FSR_phi2/F");
	t->Branch("FSR_phi3",&FSR_phi3,"FSR_phi3/F");
	t->Branch("FSR_phi4",&FSR_phi4,"FSR_phi4/F");
	t->Branch("FSR_m1",&FSR_m1,"FSR_m1/F");
	t->Branch("FSR_m2",&FSR_m2,"FSR_m2/F");
	t->Branch("FSR_m3",&FSR_m3,"FSR_m3/F");
	t->Branch("FSR_m4",&FSR_m4,"FSR_m4/F");

	t->Branch("pt1",&pt1,"pt1/F");
	t->Branch("pt2",&pt2,"pt2/F");
	t->Branch("pt3",&pt3,"pt3/F");
	t->Branch("pt4",&pt4,"pt4/F");
	t->Branch("pterr1",&pterr1,"pterr1/F");
	t->Branch("pterr2",&pterr2,"pterr2/F");
	t->Branch("pterr3",&pterr3,"pterr3/F");
	t->Branch("pterr4",&pterr4,"pterr4/F");
	t->Branch("ecaldriven1",&ecaldriven1,"ecaldriven1/I");
	t->Branch("ecaldriven2",&ecaldriven2,"ecaldriven2/I");
	t->Branch("ecaldriven3",&ecaldriven3,"ecaldriven3/I");
	t->Branch("ecaldriven4",&ecaldriven4,"ecaldriven4/I");
	t->Branch("id1",&id1,"id1/I");
	t->Branch("id2",&id2,"id2/I");
	t->Branch("id3",&id3,"id3/I");
	t->Branch("id4",&id4,"id4/I");
	t->Branch("eta1",&eta1,"eta1/F");
	t->Branch("eta2",&eta2,"eta2/F");
	t->Branch("eta3",&eta3,"eta3/F");
	t->Branch("eta4",&eta4,"eta4/F");
	t->Branch("phi1",&phi1,"phi1/F");
	t->Branch("phi2",&phi2,"phi2/F");
	t->Branch("phi3",&phi3,"phi3/F");
	t->Branch("phi4",&phi4,"phi4/F");
	t->Branch("m1",&m1,"m1/F");
	t->Branch("m2",&m2,"m2/F");
	t->Branch("m3",&m3,"m3/F");
	t->Branch("m4",&m4,"m4/F");
	
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
	t->Branch("eventWeight",&eventWeight,"eventWeight/F");
	t->Branch("D_bkg_kin",&D_bkg_kin,"D_bkg_kin/F");
	t->Branch("D_bkg_kin_vtx_BS",&D_bkg_kin_vtx_BS,"D_bkg_kin_vtx_BS/F");
	t->Branch("k_ggZZ",&k_ggZZ,"k_ggZZ/F");
	t->Branch("k_qqZZ_qcd_M",&k_qqZZ_qcd_M,"k_ggZZ_qcd_M/F");
	t->Branch("k_qqZZ_ewk",&k_qqZZ_ewk,"k_qqZZ_ewk/F");

}
void SetOldTree(TTree* t){
	t->SetBranchStatus("*",0);
	t->SetBranchStatus("finalState",1);
	t->SetBranchStatus("passedFullSelection",1);
	t->SetBranchStatus("lepFSR_pt",1);
	t->SetBranchStatus("lepFSR_eta",1);
	t->SetBranchStatus("lepFSR_phi",1);
	t->SetBranchStatus("lepFSR_mass",1);
	t->SetBranchStatus("lep_pt",1);
	t->SetBranchStatus("lep_pterr",1);
	t->SetBranchStatus("lep_ecalDriven",1);
	t->SetBranchStatus("lep_id",1);
	t->SetBranchStatus("lep_eta",1);
	t->SetBranchStatus("lep_phi",1);
	t->SetBranchStatus("lep_mass",1);
	t->SetBranchStatus("lep_Hindex",1);
	t->SetBranchStatus("mass4l",1);
	t->SetBranchStatus("mass4lErr",1);
	t->SetBranchStatus("mass4lREFIT",1);
	t->SetBranchStatus("mass4lErrREFIT",1);
	t->SetBranchStatus("mass4l_vtx_BS",1);
	t->SetBranchStatus("mass4lErr_vtx_BS",1);
	t->SetBranchStatus("mass4l_vtxFSR_BS",1);
	//t->SetBranchStatus("mass4lErr_vtxFSR_BS",1);no this variable in Filippo's tree
	t->SetBranchStatus("mass4lREFIT_vtx_BS",1);
	t->SetBranchStatus("mass4lErrREFIT_vtx_BS",1);
	t->SetBranchStatus("eventWeight",1);
	t->SetBranchStatus("D_bkg_kin",1);
	t->SetBranchStatus("D_bkg_kin_vtx_BS",1);
	t->SetBranchStatus("k_ggZZ",1);
	t->SetBranchStatus("k_qqZZ_qcd_M",1);
	t->SetBranchStatus("k_qqZZ_ewk",1);
	
	t->SetBranchAddress("finalState",&finalState);
	t->SetBranchAddress("passedFullSelection",&passedFullSelection);
	t->SetBranchAddress("lepFSR_pt",&lepFSR_pt);
	t->SetBranchAddress("lepFSR_eta",&lepFSR_eta);
	t->SetBranchAddress("lepFSR_phi",&lepFSR_phi);
	t->SetBranchAddress("lepFSR_mass",&lepFSR_mass);
	t->SetBranchAddress("lep_pt",&lep_pt);
	t->SetBranchAddress("lep_pterr",&lep_pterr);
	t->SetBranchAddress("lep_ecalDriven",&lep_ecalDriven);
	t->SetBranchAddress("lep_id",&lep_id);
	t->SetBranchAddress("lep_eta",&lep_eta);
	t->SetBranchAddress("lep_phi",&lep_phi);
	t->SetBranchAddress("lep_mass",&lep_mass);
	t->SetBranchAddress("lep_Hindex",&lep_Hindex);
		
	t->SetBranchAddress("mass4l",&mass4l);
	t->SetBranchAddress("mass4lErr",&mass4lErr);
	
	t->SetBranchAddress("mass4lREFIT",&mass4lREFIT);
	t->SetBranchAddress("mass4lErrREFIT",&mass4lErrREFIT);
	
	t->SetBranchAddress("mass4l_vtx_BS",&mass4l_vtx_BS);
	t->SetBranchAddress("mass4lErr_vtx_BS",&mass4lErr_vtx_BS);

	t->SetBranchAddress("mass4l_vtxFSR_BS",&mass4l_vtxFSR_BS);
	
	t->SetBranchAddress("mass4lREFIT_vtx_BS",&mass4lREFIT_vtx_BS);
	t->SetBranchAddress("mass4lErrREFIT_vtx_BS",&mass4lErrREFIT_vtx_BS);
	t->SetBranchAddress("eventWeight",&eventWeight);
	t->SetBranchAddress("D_bkg_kin",&D_bkg_kin);
	t->SetBranchAddress("D_bkg_kin_vtx_BS",&D_bkg_kin_vtx_BS);
	t->SetBranchAddress("k_ggZZ",&k_ggZZ);
	t->SetBranchAddress("k_qqZZ_qcd_M",&k_qqZZ_qcd_M);
	t->SetBranchAddress("k_qqZZ_ewk",&k_qqZZ_ewk);

	t->SetBranchStatus("EventCat",1);
	t->SetBranchAddress("EventCat",&EventCat);
}
