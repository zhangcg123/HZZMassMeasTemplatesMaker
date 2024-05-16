#include "include/Set4LSlimmedTree.h"
#include "include/Read4LSlimmedTree.h"
#include "include/MakePlot.h"

void GetBounds_(TString plotpath, int fs, TString type, TString tag, TString year, int cates);


int main(int argc, char *argv[]){
		
	TString year = argv[1];			//2016 to 2018
	TString type = argv[2];			//reco/refit/bs/bsrefit
	TString tag = argv[3];			//notag/untag/vbftag
	int fs = atoi(argv[4]);			//1 to 4
	int cates = atoi(argv[5]);		//1 to 9+
	TString plotpath = argv[6];		//
	
	TString Cate_s = to_string(cates);
	
	GetBounds_(plotpath, fs, type, tag, year, cates);

}

void GetBounds_(TString plotpath, int fs, TString type, TString tag, TString year, int cates){
	
	TFile* f = new TFile();	
	//if(BASE=="ReReco")f = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+"GGH_125.root");
	if(BASE=="20UL")f = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/GluGluHToZZTo4L_M125_"+year+"_skimmed.root");
	TTree* t = (TTree*)f->Get("passedEvents");
	Set4LSlimmedTree(t,type);

	vector<TH1D*> hists; hists.clear();
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		TH1D* h = new TH1D("h"+i_s,"",100,0,MERR);
		hists.push_back(h);
	}

	vector<float> bounds; bounds.clear();
	
	Read4LSlimmedTreeFindmass4lErrCategorization(t, hists, fs, bounds, type, tag);

	MakePlotFindmass4lErrCategorization(hists, plotpath, fs, bounds, year, type, tag);
	
	for(int i=0; i<cates; i++){
		delete hists.at(i);
	}
}
