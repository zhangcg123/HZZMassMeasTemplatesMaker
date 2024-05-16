//////////////////////////////////////////////////////////////////////////
//
// make the cpp 
// 
// Making 2/3 dimensional plots of p.d.f.s and datasets
//
//
//
// 07/2008 - Wouter Verkerke 
// 
/////////////////////////////////////////////////////////////////////////
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooWorkspace.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include <chrono>

using namespace RooFit;
using namespace std;

int main(int argc, char *argv[]){
	
	TString year = argv[1];
	TString process = argv[2];
	TString s_fs = argv[3];
	TString categs = argv[4];
	TString type = argv[5];
	float nexp = atof(argv[6]);
	int doSmear = atoi(argv[7]);
	TString outputDIR = argv[8];

	cout<<"launch-->year:"<<year<<"  process:"<<process<<"   finalstate:"<<s_fs<<"   totcategs:"<<categs<<"  type:"<<type<<"	totexp:"<<nexp<<endl;

	int fs;
	if (s_fs=="4mu") fs = 1;
	if (s_fs=="4e") fs = 2;
	if (s_fs=="2e2mu") fs = 3;
	if (s_fs=="2mu2e") fs = 4;
	int cats = atoi(categs);    
    
	TString filename = ""; 
	if (process == "sig") filename = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/ToyStudy/SlimForToyStudy/OutPutTrees/GluGluHToZZTo4L_M125_2018_skimmed.root";
	if (process == "irrbkg") filename = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/ToyStudy/SlimForToyStudy/OutPutTrees/ZZTo4L_M125_2018_skimmed.root";
	if (process == "rbkg") filename = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/ToyStudy/SlimForToyStudy/OutPutTrees/cjlstOSmethodevtsel_2p2plusf_3p1plusf_downupscale_2018_Data_ZZ.root";
	TFile input(filename,"READ");
	TTree* tree;
	tree = (TTree*)input.Get("passedEvents");
	if (!tree) tree = (TTree*)input.Get("Ana/passedEvents");
	
	TString boundsfilename = "/eos/user/c/chenguan/www/HZZMASSTEMPLATES_hllhc3kfb14tevprojection_35x20kdmap_newzx/Categs/"+type+"/"+year+"/Categ_"+s_fs+"_"+categs+"_notag/_"+s_fs+"_bounds.root";

	TFile* f_bounds = new TFile(boundsfilename,"READ");
	TH1F* h_std = (TH1F*)f_bounds->Get("bounds");
	h_std->SetDirectory(0);
	TAxis* h_x = h_std->GetXaxis();
	f_bounds->Close();
	
	vector<TFile*> outfiles; outfiles.clear();
	for(int i=0; i<cats; i++){
		TString i_s = to_string(i);
		TFile* outfile = new TFile(outputDIR + "/hzz4l_"+year+"_"+s_fs+"_cat"+i_s+"_notag."+type+"_"+process+"_toys.root","RECREATE");
		outfiles.push_back(outfile);
	}

	bool passedFullSelection = true;
	int finalState;    
	float mass4l, mass4lErr, D_bkg_kin;
	tree->SetBranchStatus("*",0);
	if( process != "rbkg"){
	    tree->SetBranchStatus("passedFullSelection",1);
	}
	tree->SetBranchStatus("finalState",1);
	tree->SetBranchStatus("mass4l",1);
	tree->SetBranchStatus("mass4lErr",1);
	tree->SetBranchStatus("mass4lREFIT",1);
	tree->SetBranchStatus("mass4lErrREFIT",1);
	tree->SetBranchStatus("mass4l_vtx_BS",1);
	tree->SetBranchStatus("mass4lErr_vtx_BS",1);
	tree->SetBranchStatus("mass4lREFIT_vtx_BS",1);
	tree->SetBranchStatus("mass4lErrREFIT_vtx_BS",1);
	tree->SetBranchStatus("D_bkg_kin",1);
	tree->SetBranchStatus("D_bkg_kin_vtx_BS",1);
	if( process != "rbkg" ){
	    tree->SetBranchAddress("passedFullSelection",&passedFullSelection);
	}	   
	tree->SetBranchAddress("finalState",&finalState);
	if(type == "reco"){
	    tree->SetBranchAddress("mass4l",&mass4l);
	    tree->SetBranchAddress("mass4lErr",&mass4lErr);
	    tree->SetBranchAddress("D_bkg_kin",&D_bkg_kin);
	}
	if(type == "refit"){
	    tree->SetBranchAddress("mass4lREFIT",&mass4l);
	    tree->SetBranchAddress("mass4lErrREFIT",&mass4lErr);
	    tree->SetBranchAddress("D_bkg_kin",&D_bkg_kin);
	}
	if(type == "bs"){
	    tree->SetBranchAddress("mass4l_vtx_BS",&mass4l);
	    tree->SetBranchAddress("mass4lErr_vtx_BS",&mass4lErr);
	    tree->SetBranchAddress("D_bkg_kin_vtx_BS",&D_bkg_kin);
	}
	if(type == "bsrefit"){
	    tree->SetBranchAddress("mass4lREFIT_vtx_BS",&mass4l);
	    tree->SetBranchAddress("mass4lErrREFIT_vtx_BS",&mass4lErr);
	    tree->SetBranchAddress("D_bkg_kin_vtx_BS",&D_bkg_kin);
	}

	TRandom3 rand1;
	rand1.SetSeed(123457);
	TRandom3 rand2;
	rand2.SetSeed(754321);
	TRandom3 rand3;
	rand3.SetSeed(321754);

	int toyn=0;
	Long64_t nevents = tree->GetEntriesFast();

	RooRealVar CMS_zz4l_mass("CMS_zz4l_mass","CMS_zz4l_mass",105.0,140.0);
	CMS_zz4l_mass.setBins(35);
	CMS_zz4l_mass.setBins(20000,"cache");
	RooRealVar CMS_zz4l_massErr("CMS_zz4l_massErr","CMS_zz4l_massErr",0.0,100.0);
	RooRealVar melaLD("melaLD","melaLD",0.0,1.0);
	melaLD.setBins(20);

	RooDataSet* empty_toy = new RooDataSet("empty_toy","empty_toy",RooArgSet(CMS_zz4l_mass,melaLD));

	float npassave = 0;
	int npass=0;
	
	while(1) {
		auto start = std::chrono::high_resolution_clock::now();

		if (toyn==100) break;							//how many toy dataset do you want
			
		vector<RooDataSet*> toys; toys.clear();
		for(int i=0; i<cats; i++){
			TString i_s = to_string(i);
			RooDataSet* toy = (RooDataSet*)empty_toy->Clone("toy_cat"+i_s);
			toys.push_back(toy);
		}
		
		int nexp_poisson = rand1.Poisson(nexp*(3000.0/59.83));
		npass = 0;
				
		float tmp_npassave = 0.0;
		TString s_toy = to_string(toyn);
		if (nexp_poisson==0) { 							//for low lumi cases, maybe it is 0, but for run2 and hllhc....
		    for(int i=0; i<cats; i++){
			    toys.at(i)->SetName("toy_cat"+to_string(i)+"_"+s_toy);
			    toys.at(i)->SetTitle("toy_cat"+to_string(i)+"_"+s_toy);
			    outfiles.at(i)->cd();
			    toys.at(i)->Write();
			    tmp_npassave = tmp_npassave + toys.at(i)->sumEntries();
			    delete toys.at(i);
		    }
		    toyn+=1;
		    npassave = npassave + tmp_npassave;
		    continue;
		} else {

		    while (npass<nexp_poisson) {				//nexp_poisson always is the tot number over masserr categories

			int evt = rand2.Integer(nevents);

			tree->GetEntry(evt);
			if (!passedFullSelection) continue;			//for rbkg, the passedFullSelection has been forced to be true
			if (!(finalState == fs)) continue;	
			float smearmass; float smearmasserr; float masserr_orig;
			if (doSmear) {
			    smearmass = std::max(0.0,mass4l+rand3.Gaus(0.0,0.05*mass4l));
			    float errsmear=0.05;
			    smearmasserr = std::max(0.0,mass4lErr+rand3.Gaus(0.0,errsmear*mass4lErr));
			    mass4l = smearmass;
			    mass4lErr = smearmasserr;
			}
			if (!(mass4l>105.0 && mass4l<140.0) ) continue;
			if (mass4lErr > 50.0) continue;
			

			npass+=1;
			CMS_zz4l_mass=mass4l;
			melaLD=D_bkg_kin;
			int bin = h_x->FindBin(mass4lErr)-1;	 
			toys.at(bin)->add(RooArgSet(CMS_zz4l_mass,melaLD));
		    }
		     
		    for(int i=0; i<cats; i++){
			    toys.at(i)->SetName("toy_cat"+to_string(i)+"_"+s_toy);
			    toys.at(i)->SetTitle("toy_cat"+to_string(i)+"_"+s_toy);
			    outfiles.at(i)->cd();
			    toys.at(i)->Write();
			    tmp_npassave = tmp_npassave + toys.at(i)->sumEntries();
			    delete toys.at(i);
		    }
		    toyn+=1;
		    npassave = npassave + tmp_npassave;
		}//end if nexp_poisson=0
		auto finish = std::chrono::high_resolution_clock::now();	
		std::chrono::duration<double> elapsed = finish - start;
		if(toyn%10==0)cout<<process<<"        "<<s_fs<<"      "<<"toy n"<<toyn<<"	npass:"<<npass<<"	poisson:"<<nexp_poisson<<"	nexp:"<<nexp<<"	:time-taken:"<<elapsed.count()<<endl;
		continue;
	}//end while

	npassave/=toyn;
	cout<<"done<---year:"<<year<<"	process:"<<process<<"	final state:"<<s_fs<<"	totcateg:"<<categs<<"	ntoys: "<<toyn<<" ave: "<<npassave<<"	expect:"<<nexp<<endl;
    
}
