#include "CommonTool.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLatex.h"
#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooFFTConvPdf.h"
#include "RooCBShape.h"
#include "RooNumConvPdf.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooVoigtian.h"
#include "RooFitResult.h"
#include "TH2.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TSystem.h"
#include "HZZ2L2QRooPdfs.h"
#include "RooChi2Var.h"
#include "TLorentzVector.h"
#include "TLegend.h"
#include "TLine.h"
#include "TAxis.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace RooFit;
using namespace std;
void mass_distributions(TString outputpath, vector<TH1D*> &histos_pos, vector<TH1D*> &histos_neg, string low, string high, TString mass_name, TString k);
void dofit(TString year, TString outputpath, vector<TH1D*> &histos, string low, string high, TString isdata, TString mass_name, float init_mean, float buffer, TString k);	
bool GEN=false;

//global variables
//all variables used for std histos must be used for cut otherwise there will be overflow errors
float eta_high = 2.4, eta_low = -2.4, pt1_low = 25, pt2_low = 10; //default for muon
float energy1_low = 32, energy2_low = 20, pt_max = 5000;
bool trigger_cut = true, ecalDriven_cut = true, pt_cut = true;
bool separation = true;
//
int ptbinN = 6;
int etabinN = 8;
int phibinN = 8;
double ptbins[7] = {0,30,40,50,60,100,pt_max};
double phibins[9] = {0,1.6,1.7,1.8,1.84,1.88,1.92,1.96,2};
//double ptbins[2] = {0,pt_max};
TH1D* h_eta_std = new TH1D("h_eta_std","",etabinN,eta_low,eta_high);	//---->adjust itself when electron
TH1D* h_phi_std = new TH1D("h_phi_std","",phibinN,phibins);		//---->consistent
TH1D* h_pt_std = new TH1D("h_pt_std","",ptbinN,ptbins);			//---->check it manually


void readtree_(TString year, TString fs, TString inputpath, TString outputpath, TString isdata, TString mass_name, TString binned, string low, string high, float init_mean, float buffer,

        TH1D* h_eta_std, TH1D* h_phi_std, TH1D* h_pt_std,

        vector<TH1D*> &eta_pos_list, vector<TH1D*> &eta_neg_list, vector<TH1D*> &phi_pos_list, vector<TH1D*> &phi_neg_list, vector<TH1D*> &pt_pos_list, vector<TH1D*> &pt_neg_list,

        vector<TH1D*> &eta_pos_free_eta_list, vector<TH1D*> &eta_pos_fix_eta_list,
        vector<TH1D*> &eta_pos_free_phi_list, vector<TH1D*> &eta_pos_fix_phi_list,
        vector<TH1D*> &eta_pos_free_pt_list, vector<TH1D*> &eta_pos_fix_pt_list,

        vector<TH1D*> &phi_pos_free_eta_list, vector<TH1D*> &phi_pos_fix_eta_list,
        vector<TH1D*> &phi_pos_free_phi_list, vector<TH1D*> &phi_pos_fix_phi_list,
        vector<TH1D*> &phi_pos_free_pt_list, vector<TH1D*> &phi_pos_fix_pt_list,

        vector<TH1D*> &pt_pos_free_eta_list, vector<TH1D*> &pt_pos_fix_eta_list,
        vector<TH1D*> &pt_pos_free_phi_list, vector<TH1D*> &pt_pos_fix_phi_list,
        vector<TH1D*> &pt_pos_free_pt_list, vector<TH1D*> &pt_pos_fix_pt_list,

        vector<TH1D*> &eta_neg_free_eta_list, vector<TH1D*> &eta_neg_fix_eta_list,
        vector<TH1D*> &eta_neg_free_phi_list, vector<TH1D*> &eta_neg_fix_phi_list,
        vector<TH1D*> &eta_neg_free_pt_list, vector<TH1D*> &eta_neg_fix_pt_list,

        vector<TH1D*> &phi_neg_free_eta_list, vector<TH1D*> &phi_neg_fix_eta_list,
        vector<TH1D*> &phi_neg_free_phi_list, vector<TH1D*> &phi_neg_fix_phi_list,
        vector<TH1D*> &phi_neg_free_pt_list, vector<TH1D*> &phi_neg_fix_pt_list,

        vector<TH1D*> &pt_neg_free_eta_list, vector<TH1D*> &pt_neg_fix_eta_list,
        vector<TH1D*> &pt_neg_free_phi_list, vector<TH1D*> &pt_neg_fix_phi_list,
        vector<TH1D*> &pt_neg_free_pt_list, vector<TH1D*> &pt_neg_fix_pt_list,

        vector<TH1D*> &eta_pos_zeta_list, vector<TH1D*> &eta_neg_zeta_list,
        vector<TH1D*> &phi_pos_zeta_list, vector<TH1D*> &phi_neg_zeta_list,
        vector<TH1D*> &pt_pos_zeta_list, vector<TH1D*> &pt_neg_zeta_list,

        vector<TH1D*> &eta_pos_zphi_list, vector<TH1D*> &eta_neg_zphi_list,
        vector<TH1D*> &phi_pos_zphi_list, vector<TH1D*> &phi_neg_zphi_list,
        vector<TH1D*> &pt_pos_zphi_list, vector<TH1D*> &pt_neg_zphi_list,

        vector<TH1D*> &eta_pos_zpz_list, vector<TH1D*> &eta_neg_zpz_list,
        vector<TH1D*> &phi_pos_zpz_list, vector<TH1D*> &phi_neg_zpz_list,
        vector<TH1D*> &pt_pos_zpz_list, vector<TH1D*> &pt_neg_zpz_list,

	vector<TH1D*> &eta_pos_zpt_list, vector<TH1D*> &eta_neg_zpt_list,
        vector<TH1D*> &phi_pos_zpt_list, vector<TH1D*> &phi_neg_zpt_list,
        vector<TH1D*> &pt_pos_zpt_list, vector<TH1D*> &pt_neg_zpt_list,

        vector<TH1D*> &eta_pos_angle_list, vector<TH1D*> &eta_neg_angle_list,
        vector<TH1D*> &phi_pos_angle_list, vector<TH1D*> &phi_neg_angle_list,
        vector<TH1D*> &pt_pos_angle_list, vector<TH1D*> &pt_neg_angle_list,

        vector<TH1D*> &pterr_pos_phi_list, vector<TH1D*> &pterr_neg_phi_list,
        vector<TH1D*> &pterr_pos_eta_list, vector<TH1D*> &pterr_neg_eta_list,
        vector<TH1D*> &pterr_pos_pt_list, vector<TH1D*> &pterr_neg_pt_list,

        vector<TH1D*> &pterrold_pos_phi_list, vector<TH1D*> &pterrold_neg_phi_list,
        vector<TH1D*> &pterrold_pos_eta_list, vector<TH1D*> &pterrold_neg_eta_list,
        vector<TH1D*> &pterrold_pos_pt_list, vector<TH1D*> &pterrold_neg_pt_list,
	
	TH1D* tot_pos_eta, TH1D* tot_neg_eta,
        TH1D* tot_pos_phi, TH1D* tot_neg_phi,
        TH1D* tot_pos_pt, TH1D* tot_neg_pt,
        //TH2D* pos_eta_vs_mass, TH2D* neg_eta_vs_mass,
        //TH2D* pos_phi_vs_mass, TH2D* neg_phi_vs_mass,
        TH1D* tot_zpt, TH1D* tot_zpz);

void mass_distributions(TString outputpath, vector<TH1D*> &histos_pos, vector<TH1D*> &histos_neg, string low, string high, TString isdata, TString k, TString subdirname){

	//if(mass_name=="mass2mu_ewk2"){
		/*
		TCanvas c_tmp1("c_tmp1","",1400,1000);
		for(int i=0; i<=histos_pos.size(); i++){//pos all histograms png and root
			TString i_s = to_string(i);
			TH1D* test_tmp = (TH1D*)histos_pos[i]->Clone();
			if(subdirname=="mass_distributions")test_tmp->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
			CustomHistoStyle(test_tmp,true,true,"",i+1,2,1,false,subdirname,"Events","norm");
			c_tmp1.Clear();
			test_tmp->Draw("hist");
			c_tmp1.SaveAs(outputpath + "/" + subdirname + "/" + mass_name + "/" + k + "/" + test_tmp->GetName() + ".png");
			TFile* f_tmp = new TFile(outputpath + "/" + subdirname + "/" + mass_name + "/" + k + "/" + test_tmp->GetName()+".root", "RECREATE");
			f_tmp->cd();
			test_tmp->Write();
			f_tmp->Close();
			delete test_tmp;
		}
		//c_tmp1.BuildLegend();
		//c_tmp1.SaveAs(outputpath + "/"+subdirname+"/"+k+"_1-5bin/"+k+"_pos.png");
		*/
		TCanvas c_tmp2("c_tmp2","",1400,1000);
		for(int i=0; i<histos_neg.size(); i++){//neg all histograms png and root
			TString i_s = to_string(i);
			TH1D* test_tmp = (TH1D*)histos_neg[i]->Clone();
			if(subdirname=="mass_distributions")test_tmp->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
			CustomHistoStyle(test_tmp,true,true,"",1,2,1,false,subdirname,"Fraction","norm");
			c_tmp2.Clear();
			test_tmp->Draw("hist");
			c_tmp2.SaveAs(outputpath + "/" + subdirname + "/" + k + "/" + isdata + "_" + test_tmp->GetName() + ".png");
			TFile* f_tmp = new TFile(outputpath + "/" + subdirname + "/" + k + "/" + isdata + "_" + test_tmp->GetName()+".root", "RECREATE");
			f_tmp->cd();
			test_tmp->Write();
			f_tmp->Close();
			delete test_tmp;
		}
		//c_tmp2.BuildLegend();
		//c_tmp2.SaveAs(outputpath + "/"+subdirname+"/"+k+"_1-5bin/"+k+"_neg.png");
		/*
		for(int i=0; i<histos_pos.size(); i++){//pos edge bins vs middle bin
			TString i_s = to_string(i);
			TH1D* edge = (TH1D*)histos_pos[i]->Clone();
			TH1D* middle = (TH1D*)histos_pos[4]->Clone();
			if(subdirname=="mass_distributions"){edge->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
				middle->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
			}
			TString outputname = outputpath + "/"+subdirname+"/"+k+"_pos_edge_vs_middle/"+k+"_pos_"+i_s+"_vs_4.png";
			ComparisonWithRatio(edge,middle,true,"ratio",subdirname,edge->GetName(),middle->GetName(),outputname);
		}

		for(int i=0; i<histos_neg.size(); i++){//neg edge bins vs middle bin
			TString i_s = to_string(i);
			TH1D* edge = (TH1D*)histos_neg[i]->Clone();
			TH1D* middle = (TH1D*)histos_neg[4]->Clone();
			if(subdirname=="mass_distributions"){edge->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
				middle->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
			}
			TString outputname = outputpath + "/"+subdirname+"/"+k+"_neg_edge_vs_middle/"+k+"_neg_"+i_s+"_vs_4.png";
			ComparisonWithRatio(edge,middle,true,"ratio",subdirname,edge->GetName(),middle->GetName(),outputname);
		}

		for(int i=0; i<histos_pos.size(); i++){//same bin, pos vs neg
			TString i_s = to_string(i);
			TH1D* pos_tmp = (TH1D*)histos_pos[i]->Clone();
			TH1D* neg_tmp = (TH1D*)histos_neg[i]->Clone();
			if(subdirname=="mass_distributions"){pos_tmp->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
				neg_tmp->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
			}
			TString outputname = outputpath + "/"+subdirname+"/"+k+"_pos_vs_neg/"+k+"_"+i_s+"_pos_vs_neg.png";
			TFile* f = new TFile(outputpath + "/"+subdirname+"/"+k+"_pos_vs_neg/"+k+"_"+i_s+"_pos_vs_neg.root","RECREATE");
			f->cd();
			pos_tmp->Write();
			f->Close();
			ComparisonWithRatio(pos_tmp, neg_tmp, true, "ratio", subdirname, pos_tmp->GetName(), neg_tmp->GetName(),outputname);
		}
		*/
//	}
}


