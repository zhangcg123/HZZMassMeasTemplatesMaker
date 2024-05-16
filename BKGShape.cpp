#include "include/Set4LSlimmedTree.h"
#include "include/Read4LSlimmedTree.h"
#include "include/do4LFit.h"
#include "include/MakePlot.h"

void BKGShape_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process); 

int main(int argc, char *argv[]){
        TString year  = argv[1];
        TString type = argv[2];
        TString tag = argv[3];
        int fs = atoi(argv[4]);
        int cates = atoi(argv[5]);
        TString plotpath = argv[6];
        TString steponedir = argv[7];
	TString process = argv[8];

	BKGShape_(plotpath, fs, type, tag, year, cates, steponedir, process);

}


void BKGShape_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process){
	 
	//get bounds
	TString fs_name;
	if(fs==1)fs_name="4mu";
	if(fs==2)fs_name="4e";
	if(fs==3)fs_name="2e2mu";		
	if(fs==4)fs_name="2mu2e";

	TFile* f = new TFile(steponedir + "/_"+fs_name+"_bounds.root");
	TH1F* h_std = (TH1F*)f->Get("bounds");

	//make datasets
	//retrieve tuple name
	TString tuple;
	if(process=="ggzz"&&fs_name=="4mu"){
		if(BASE=="20UL"){tuple = "GluGluToContinToZZTo4mu_M125_"+year+"_skimmed";
		}else{tuple=year+"ggZZ_4mu";}
	}
	if(process=="ggzz"&&fs_name=="4e"){
		if(BASE=="20UL"){tuple = "GluGluToContinToZZTo4e_M125_"+year+"_skimmed";
		}else{tuple=year+"ggZZ_4e";}
	}
	if(process=="ggzz"&&fs_name=="2e2mu"){
		if(BASE=="20UL"){tuple = "GluGluToContinToZZTo2e2mu_M125_"+year+"_skimmed";
		}else{tuple=year+"ggZZ_2e2mu";}
	}
	if(process=="ggzz"&&fs_name=="2mu2e"){
		if(BASE=="20UL"){tuple = "GluGluToContinToZZTo2e2mu_M125_"+year+"_skimmed";
		}else{tuple=year+"ggZZ_2e2mu";}
	}
	if(process=="qqzz"){
		if(BASE=="20UL"){tuple = "ZZTo4L_M125_"+year+"_skimmed";
		}else{tuple="slimmed_"+year+"qqZZ";}
	}
	TFile* f_ = new TFile();
	if(BASE=="ReReco")f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+tuple+".root");
	if(BASE=="20UL")f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+tuple+".root");
	TTree* t = (TTree*)f_->Get("passedEvents");
	
	vector<TH1F*> histos; histos.clear();
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		TH1F* h = new TH1F("h_"+i_s,"",300,105,140);
		histos.push_back(h);
	}

	Read4LSlimmedTree1DCategory(histos, h_std, t, fs, type, tag);
	RooRealVar mass4l("mass4l","",105,140);
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		RooDataHist* dataset = new RooDataHist("dataset_"+i_s,"",mass4l,Import(*histos.at(i)));
		double params[3] = {0,0,0};
		ZZBKGFit(dataset, params);
		MakeZZBKGFitPlot(params, dataset, plotpath, i_s, fs_name, type, tag, year, process);
	}	
}
