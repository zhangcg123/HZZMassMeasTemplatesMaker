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
#include <iostream>
#include <string>
#include <fstream>

using namespace RooFit;
using namespace std;
void mass_distributions(TString outputpath, vector<TH1D*> &histos_pos, vector<TH1D*> &histos_neg, string low, string high, TString mass_name, TString k);
void dofit(TString year, TString outputpath, vector<TH1D*> &histos, string low, string high, TString isdata, TString mass_name, float init_mean, float buffer, TString k);	
void migration_summary(TString outputpath, vector<vector<TH1D*>> &histos, TString name, TString dirname);
bool GEN=false;

void readtree_(TString year, TString inputpath, TString outputpath, TString isdata, TString mass_name, TString binned, string low, string high, float init_mean, float buffer,

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
	
	vector<TH1D*> &eta_pos_cuton_truth_see_reco, vector<TH1D*> &eta_neg_cuton_truth_see_reco,
	vector<TH1D*> &phi_pos_cuton_truth_see_reco, vector<TH1D*> &phi_neg_cuton_truth_see_reco,
	vector<TH1D*> &pt_pos_cuton_truth_see_reco, vector<TH1D*> &pt_neg_cuton_truth_see_reco,
	
	vector<TH1D*> &eta_pos_cuton_reco_see_truth, vector<TH1D*> &eta_neg_cuton_reco_see_truth,
	vector<TH1D*> &phi_pos_cuton_reco_see_truth, vector<TH1D*> &phi_neg_cuton_reco_see_truth,
	vector<TH1D*> &pt_pos_cuton_reco_see_truth, vector<TH1D*> &pt_neg_cuton_reco_see_truth,
	
	vector<TH1D*> &eta_pos_pull_list, vector<TH1D*> &eta_neg_pull_list,
	vector<TH1D*> &phi_pos_pull_list, vector<TH1D*> &phi_neg_pull_list,
	vector<TH1D*> &pt_pos_pull_list, vector<TH1D*> &pt_neg_pull_list,
		
	TH1D* tot_pos_eta, TH1D* tot_neg_eta,
        TH1D* tot_pos_phi, TH1D* tot_neg_phi,
        TH1D* tot_pos_pt, TH1D* tot_neg_pt,
        //TH2D* pos_eta_vs_mass, TH2D* neg_eta_vs_mass,
        //TH2D* pos_phi_vs_mass, TH2D* neg_phi_vs_mass,
        TH1D* tot_zpt, TH1D* tot_zpz,
	vector<vector<TH1D*>> &pt_pos_reco_migration_list, vector<vector<TH1D*>> &pt_neg_reco_migration_list,
	vector<vector<TH1D*>> &eta_pos_reco_migration_list, vector<vector<TH1D*>> &eta_neg_reco_migration_list,
	vector<vector<TH1D*>> &phi_pos_reco_migration_list, vector<vector<TH1D*>> &phi_neg_reco_migration_list,
	
	TH2D* pt1_corr_vs_gen, TH2D* pt2_corr_vs_gen,
	TH2D* pt1_uncorr_vs_gen, TH2D* pt2_uncorr_vs_gen

	);

void mass_distributions(TString outputpath, vector<TH1D*> &histos_pos, vector<TH1D*> &histos_neg, string low, string high, TString mass_name, TString k, TString subdirname){

	//if(mass_name=="mass2mu_ewk2"){
		TCanvas c_tmp1("c_tmp1","",1400,1000);
		double max = FindMax(histos_pos);
		for(int i=4; i>=0; i--){//pos 1-5 bin at same canvas
			TString i_s = to_string(i);
			TH1D* test_tmp = (TH1D*)histos_pos[i]->Clone();
			if(subdirname=="mass_distributions")test_tmp->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
			CustomHistoStyle(test_tmp,true,true,"",i+1,2,1,false,subdirname,"Events","norm");
			test_tmp->SetMaximum(max);
			c_tmp1.cd();
			test_tmp->Draw("samehist");
		}
		c_tmp1.BuildLegend();
		c_tmp1.SaveAs(outputpath + "/"+subdirname+"/"+k+"_1-5bin/"+k+"_pos.png");
		
		TCanvas c_tmp2("c_tmp2","",1400,1000);
		max = FindMax(histos_neg);
		for(int i=4; i>=0; i--){//neg 1-5 bin at same canvas
			TString i_s = to_string(i);
			TH1D* test_tmp = (TH1D*)histos_neg[i]->Clone();
			if(subdirname=="mass_distributions")test_tmp->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/200);
			CustomHistoStyle(test_tmp,true,true,"",i+1,2,1,false,subdirname,"Events","norm");
			test_tmp->SetMaximum(max);
			c_tmp2.cd();
			test_tmp->Draw("samehist");
		}
		c_tmp2.BuildLegend();
		c_tmp2.SaveAs(outputpath + "/"+subdirname+"/"+k+"_1-5bin/"+k+"_neg.png");

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
//	}
}

void migration_summary(TString outputpath, vector<vector<TH1D*>> &histos, TString name, TString dirname){

        ofstream fo;
        TString substr = "info = {\n";
	for(int i=0; i<histos.size(); i++){
                TString i_s = to_string(i);
                for(int j=0; j<histos.at(i).size(); j++){
                        TString j_s = to_string(j);

                        double mean = histos.at(i).at(j)->GetMean();
                        double entries = histos.at(i).at(j)->GetEntries();
                        TString mean_s = to_string(mean);
                        TString entries_s = to_string(entries);

                        substr = substr + "'"+name+"_mean_"+i_s+"_"+j_s+"':"+mean_s+",\n";
                        substr = substr + "'"+name+"_entries_"+i_s+"_"+j_s+"':"+entries_s+",\n";
                }
        }
        substr = substr + "}";
	fo.open(outputpath + "/" + dirname + "/" + name + ".py");
        fo<<substr<<endl;
	fo.close();

        TCanvas c("c","",1400,1000);
        for(int i=0; i<histos.size(); i++){
                TString i_s = to_string(i);
                for(int j=0; j<histos.at(i).size(); j++){
                        TString j_s = to_string(j);
			c.cd();
                        histos.at(i).at(j)->SetLineWidth(4);
                        histos.at(i).at(j)->SetStats(0);
                        histos.at(i).at(j)->Draw("hist");
                        c.SaveAs(outputpath + "/" + dirname + "/" + name+"_"+i_s+"_"+j_s+".png");
                }
        }
}

