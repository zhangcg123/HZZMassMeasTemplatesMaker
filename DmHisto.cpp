#include "include/Set4LSlimmedTree.h"
#include "include/Read4LSlimmedTree.h"
#include "include/do4LFit.h"
#include "include/MakePlot.h"
void DmHisto_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process);

int main(int argc, char *argv[]){
        TString year  = argv[1];
        TString type = argv[2];
        TString tag = argv[3];
        int fs = atoi(argv[4]);
        int cates = atoi(argv[5]);
        TString plotpath = argv[6];
        TString steponedir = argv[7];
        TString process = argv[8];

	DmHisto_(plotpath, fs, type, tag, year, cates, steponedir, process);	
}


void DmHisto_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process){
	
	//get bounds
	string fs_name;
	TString tuple_subname;
	if(fs==1){fs_name="4mu";tuple_subname="4mu";}
	if(fs==2){fs_name="4e";tuple_subname="4e";}
	if(fs==3){fs_name="2e2mu";tuple_subname="2e2mu";}
	if(fs==4){fs_name="2mu2e";tuple_subname="2e2mu";}

	TFile* f = new TFile(steponedir + "/_"+fs_name+"_bounds.root");
	TH1F* h_std = (TH1F*)f->Get("bounds");

	//make datasets
	//retrieve tuple name
	TString tuple;
	if(process=="ggh"&&BASE=="20UL")tuple = "GluGluHToZZTo4L_M125_"+year+"_skimmed";
	if(process=="ggh"&&BASE=="ReReco")tuple = year+"GGH_125";
	if(process=="ggzz"&&BASE=="20UL")tuple = "GluGluToContinToZZTo"+tuple_subname+"_M125_"+year+"_skimmed";
	if(process=="ggzz"&&BASE=="ReReco")tuple = year+"ggZZ_"+tuple_subname;
	if(process=="qqzz"&&BASE=="20UL")tuple = "ZZTo4L_M125_"+year+"_skimmed";
	if(process=="qqzz"&&BASE=="ReReco")tuple = "slimmed_"+year+"qqZZ";
	TFile* f_ = new TFile();
	if(BASE=="ReReco")f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+tuple+".root");
	if(BASE=="20UL")f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+tuple+".root");
	TTree* t = (TTree*)f_->Get("passedEvents");
	vector<TH1D*> histos; histos.clear();
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		TH1D* h = new TH1D("h_"+i_s,"",100,0,15);
		histos.push_back(h);
	}
	Read4LSlimmedTree1DCategoryForDm(histos, h_std, t, fs, type, tag);
	
	//normalize
	double norm;
	for(int i=0; i<cates; i++){							//loop over categories
		TString i_s = to_string(i);
		histos.at(i)->Scale(1/histos.at(i)->Integral());
	}

	//seva and rename
	TCanvas c("c","",1400,1000);
	c.cd();
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		histos.at(i)->SetStats(0);
		histos.at(i)->Smooth();
		histos.at(i)->Draw("hist");
		c.SaveAs(plotpath + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_dm_" + process + ".png");
		TFile* f = new TFile(plotpath + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_dm_" + process + ".root","RECREATE");
		f->cd();
		histos.at(i)->Write(process+"_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"_dm");
		f->Close();
	}	
}	
