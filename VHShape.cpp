#include "include/Set4LSlimmedTree.h"
#include "include/Read4LSlimmedTree.h"
#include "include/do4LFit.h"
#include "include/MakePlot.h"

void WHShape_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString steptwpdir, TString process);

int main(int argc, char *argv[]){
        TString year  = argv[1];
        TString type = argv[2];
        TString tag = argv[3];
        int fs = atoi(argv[4]);
        int cates = atoi(argv[5]);
        TString plotpath = argv[6];
        TString steponedir = argv[7];
	TString steptwodir = argv[8];
	TString process = argv[9];
		
	WHShape_(plotpath, fs, type, tag, year, cates, steponedir, steptwodir, process);	
}

void WHShape_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString steptwodir, TString process){
	
	//get bounds
	TString fs_name;
	if(fs==1)fs_name="4mu";
	if(fs==2)fs_name="4e";
	if(fs==3)fs_name="2e2mu";		
	if(fs==4)fs_name="2mu2e";
	TFile* f = new TFile(steponedir + "/_"+fs_name+"_bounds.root");
	TH1F* h_std = (TH1F*)f->Get("bounds");
	
	TString tuple_name="";
	if(process=="wh"&&BASE=="20UL")tuple_name="WH_HToZZTo4L_M125_"+year+"_skimmed";
	if(process=="zh"&&BASE=="20UL")tuple_name="ZH_HToZZ_4LFilter_M125_"+year+"_skimmed";
	if(process=="wh"&&BASE=="ReReco")tuple_name="WH";
	if(process=="zh"&&BASE=="ReReco")tuple_name="ZH";
	//make datasets
	TFile* f_ = new TFile();
	//if(BASE=="ReReco")f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+tuple_name+"_125.root");
	if(BASE=="20UL")f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+tuple_name+".root");
	TTree* t = (TTree*)f_->Get("passedEvents");
	vector<RooDataSet*> histos; histos.clear();
	RooRealVar mass4l__("mass4l","",105,140);
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		RooDataSet* h = new RooDataSet("h_"+i_s,"",mass4l__);
		histos.push_back(h);
	}
	Read4LSlimmedTree1DCategoryUnbinned(histos, h_std, t, fs, type, tag);
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		RooFitResult* r = VH4LFit(histos.at(i), steptwodir, i_s, fs_name, type, tag, year);
		MakeVH4LFitPlot(r, histos.at(i), plotpath, steptwodir, i_s, fs_name, type, process, tag, year);
	}
}	
