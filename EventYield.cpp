#include "include/Set4LSlimmedTree.h"
#include "include/Read4LSlimmedTree.h"
#include "include/do4LFit.h"
#include "include/MakePlot.h"

void EventYield_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process);

int main(int argc, char *argv[]){
        TString year  = argv[1];
        TString type = argv[2];
        TString tag = argv[3];
        int fs = atoi(argv[4]);
        int cates = atoi(argv[5]);
        TString plotpath = argv[6];
        TString steponedir = argv[7];
	TString process = argv[8];
	
	EventYield_(plotpath, fs, type, tag, year, cates, steponedir, process);
	
}

void EventYield_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process){
	

	TString fs_name;
	if(fs==1)fs_name = "4mu";
	if(fs==2)fs_name = "4e";
	if(fs==3)fs_name = "2e2mu";
	if(fs==4)fs_name = "2mu2e";
	TFile* f = new TFile(steponedir + "/_" +fs_name+"_bounds.root");
	TH1F* h_std = (TH1F*)f->Get("bounds");
	
	//retrieve name
	TString process_;
	if(process=="ggh"&&BASE=="20UL")process_="GluGluHToZZTo4L_M";
	if(process=="ggh"&&BASE=="ReReco")process_ = "GGH";
	if(process=="vbf"&&BASE=="20UL")process_="VBF_HToZZTo4L_M";
	if(process=="vbf"&&BASE=="ReReco")process_ = "VBF";
	if(process=="wmh"&&BASE=="20UL")process_="WminusH_HToZZTo4L_M";
	if(process=="wmh"&&BASE=="ReReco")process_ = "WminusH";
	if(process=="wph"&&BASE=="20UL")process_="WplusH_HToZZTo4L_M";
	if(process=="wph"&&BASE=="ReReco")process_ = "WplusH";
	if(process=="zh"&&BASE=="20UL")process_="ZH_HToZZ_4LFilter_M";
	if(process=="zh"&&BASE=="ReReco")process_ = "ZH";
	if(process=="tth"&&BASE=="20UL")process_="ttH_HToZZ_4LFilter_M";
	if(process=="tth"&&BASE=="ReReco")process_ = "ttH";
	if(process=="bbh"&&BASE=="20UL")process_="bbH_HToZZTo4L_M";
	if(process=="thq"&&BASE=="20UL")process_="tHq_HToZZTo4L_M";

	TFile* f_120 = new TFile();
	TFile* f_124 = new TFile();
	TFile* f_125 = new TFile();
	TFile* f_126 = new TFile();
	TFile* f_130 = new TFile();
	
	
	if(BASE=="ReReco")f_120 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+process_+"_120.root");
	if(BASE=="20UL")f_120 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/"+year+"/"+process_+"120_"+year+"_skimmed.root");
	TTree* t_120 = (TTree*)f_120->Get("passedEvents");
	if(BASE=="ReReco")f_124 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+process_+"_124.root");
	if(BASE=="20UL")f_124 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/"+year+"/"+process_+"124_"+year+"_skimmed.root");
	TTree* t_124 = (TTree*)f_124->Get("passedEvents");
	if(BASE=="ReReco")f_125 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+process_+"_125.root");
	if(BASE=="20UL")f_125 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/"+year+"/"+process_+"125_"+year+"_skimmed.root");
	TTree* t_125 = (TTree*)f_125->Get("passedEvents");
	if(BASE=="ReReco")f_126 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+process_+"_126.root");
	if(BASE=="20UL")f_126 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/"+year+"/"+process_+"126_"+year+"_skimmed.root");
	TTree* t_126 = (TTree*)f_126->Get("passedEvents");
	if(BASE=="ReReco")f_130 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+process_+"_130.root");
	if(BASE=="20UL")f_130 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/"+year+"/"+process_+"130_"+year+"_skimmed.root");
	TTree* t_130 = (TTree*)f_130->Get("passedEvents");

	double yields_120[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};					//right now, the pkg supports the number of mass error categs until 13
	double yields_124[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yields_125[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yields_126[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yields_130[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	double yieldserr_120[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yieldserr_124[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yieldserr_125[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yieldserr_126[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yieldserr_130[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	ReadTreeForEventYield(yields_120, yieldserr_120, h_std, t_120, fs, year, process, 120, type, tag);
	ReadTreeForEventYield(yields_124, yieldserr_124, h_std, t_124, fs, year, process, 124, type, tag);
	ReadTreeForEventYield(yields_125, yieldserr_125, h_std, t_125, fs, year, process, 125, type, tag);
	ReadTreeForEventYield(yields_126, yieldserr_126, h_std, t_126, fs, year, process, 126, type, tag);
	ReadTreeForEventYield(yields_130, yieldserr_130, h_std, t_130, fs, year, process, 130, type, tag);

	for(int i=0; i<cates; i++){
		TString cate = to_string(i);
		FitEventYield(yields_120[i], sqrt(yieldserr_120[i]), yields_124[i], sqrt(yieldserr_124[i]), yields_125[i], sqrt(yieldserr_125[i]), yields_126[i], sqrt(yieldserr_126[i]), yields_130[i], sqrt(yieldserr_130[i]), plotpath, cate, fs_name, process, type, tag, year);
	}
}
