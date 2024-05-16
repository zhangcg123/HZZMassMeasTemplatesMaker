#include "include/Set4LSlimmedTree.h"
#include "include/Read4LSlimmedTree.h"
#include "include/do4LFit.h"
#include "include/MakePlot.h"

void SignalShape_(TString plotpath1, TString plotpath2, TString plotpath3, int fs, TString type, TString tag, TString year, int cates, TString steponedir);


int main(int argc, char *argv[]){
	TString year  = argv[1];
	TString type = argv[2];
	TString tag = argv[3];
	int fs = atoi(argv[4]);
	int cates = atoi(argv[5]);
	TString plotpath1 = argv[6];
	TString plotpath2 = argv[7];
	TString plotpath3 = argv[8];
	TString steponedir = argv[9];

	SignalShape_(plotpath1, plotpath2, plotpath3, fs, type, tag, year, cates, steponedir);
}

void SignalShape_(TString plotpath, TString plotpath2, TString plotpath3, int fs, TString type, TString tag, TString year, int cates, TString steponedir){
	
	//get bounds
	TString fs_name;
	if(fs==1)fs_name="4mu";
	if(fs==2)fs_name="4e";
	if(fs==3)fs_name="2e2mu";			
	if(fs==4)fs_name="2mu2e";
	
	TFile* f = new TFile(steponedir + "/_"+fs_name+"_bounds.root");
	TH1F* h_std = (TH1F*)f->Get("bounds");

	//make datasets
	TFile* f_120 = new TFile();
	TFile* f_124 = new TFile();
	TFile* f_125 = new TFile();
	TFile* f_126 = new TFile();
	TFile* f_130 = new TFile();

	//if(BASE=="ReReco")f_120 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+"GGH_120.root");
	if(BASE=="20UL")f_120 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/GluGluHToZZTo4L_M120_"+year+"_skimmed.root");
	TTree* t_120 = (TTree*)f_120->Get("passedEvents");
	//if(BASE=="ReReco")f_124 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+"GGH_124.root");
	if(BASE=="20UL")f_124 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/GluGluHToZZTo4L_M124_"+year+"_skimmed.root");
	TTree* t_124 = (TTree*)f_124->Get("passedEvents");
	//if(BASE=="ReReco")f_125 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+"GGH_125.root");
	if(BASE=="20UL")f_125 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/GluGluHToZZTo4L_M125_"+year+"_skimmed.root");
	TTree* t_125 = (TTree*)f_125->Get("passedEvents");
	//if(BASE=="ReReco")f_126 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+"GGH_126.root");
	if(BASE=="20UL")f_126 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/GluGluHToZZTo4L_M126_"+year+"_skimmed.root");
	TTree* t_126 = (TTree*)f_126->Get("passedEvents");
	//if(BASE=="ReReco")f_130 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+"GGH_130.root");
	if(BASE=="20UL")f_130 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/GluGluHToZZTo4L_M130_"+year+"_skimmed.root");
	TTree* t_130 = (TTree*)f_130->Get("passedEvents");

	vector<RooDataSet*> histos_120; histos_120.clear();
	vector<RooDataSet*> histos_124; histos_124.clear();
	vector<RooDataSet*> histos_125; histos_125.clear();
	vector<RooDataSet*> histos_126; histos_126.clear();
	vector<RooDataSet*> histos_130; histos_130.clear();
	RooRealVar mass4l__("mass4l","",105,140);
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		RooDataSet* h_120 = new RooDataSet("h_120_"+i_s,"",mass4l__); histos_120.push_back(h_120);
		RooDataSet* h_124 = new RooDataSet("h_124_"+i_s,"",mass4l__); histos_124.push_back(h_124);
		RooDataSet* h_125 = new RooDataSet("h_125_"+i_s,"",mass4l__); histos_125.push_back(h_125);
		RooDataSet* h_126 = new RooDataSet("h_126_"+i_s,"",mass4l__); histos_126.push_back(h_126);
		RooDataSet* h_130 = new RooDataSet("h_130_"+i_s,"",mass4l__); histos_130.push_back(h_130);
	}	
	Read4LSlimmedTree1DCategoryUnbinned(histos_120, h_std, t_120, fs, type, tag);
	Read4LSlimmedTree1DCategoryUnbinned(histos_124, h_std, t_124, fs, type, tag);
	Read4LSlimmedTree1DCategoryUnbinned(histos_125, h_std, t_125, fs, type, tag);
	Read4LSlimmedTree1DCategoryUnbinned(histos_126, h_std, t_126, fs, type, tag);
	Read4LSlimmedTree1DCategoryUnbinned(histos_130, h_std, t_130, fs, type, tag);
	
	vector<RooDataSet*> dataset_list;
	vector<RooFitResult*> result_list;
	for(int i=0; i<cates; i++){
		dataset_list.clear();
		result_list.clear();	
		TString cate_s = to_string(i);
		
		dataset_list.push_back(histos_120.at(i));
//		RooFitResult* r_approx_120 = ApproxFit(histos_120.at(i), 120.0, plotpath, cate_s, fs_name, type, tag, year, "ggh");
				
		dataset_list.push_back(histos_124.at(i));
//		RooFitResult* r_approx_124 = ApproxFit(histos_124.at(i), 124.0, plotpath, cate_s, fs_name, type, tag, year, "ggh");

		dataset_list.push_back(histos_125.at(i));
		RooFitResult* r_125 = DCB4LFit(histos_125.at(i),125.0);
		MakeDCB4LFitPlot(r_125, 125.0, histos_125.at(i), plotpath, cate_s, fs_name, type, tag, year, "ggh");
//		RooFitResult* r_approx = ApproxFit(histos_125.at(i), 125.0, plotpath, cate_s, fs_name, type, tag, year, "ggh");

		dataset_list.push_back(histos_126.at(i));
//		RooFitResult* r_approx_126 = ApproxFit(histos_126.at(i), 126.0, plotpath, cate_s, fs_name, type, tag, year, "ggh");

		dataset_list.push_back(histos_130.at(i));
//		RooFitResult* r_approx_130 = ApproxFit(histos_130.at(i), 130.0, plotpath, cate_s, fs_name, type, tag, year, "ggh");
		
		vector<TString> lable_list;lable_list.clear();lable_list.push_back("120");lable_list.push_back("124");lable_list.push_back("125");lable_list.push_back("126");lable_list.push_back("130");
		
		DCB4LSimulFit(dataset_list, r_125, lable_list, plotpath2, cate_s, fs_name, type, tag, year);
//		ApproxSimulFit(dataset_list, r_approx, lable_list, plotpath3, cate_s, fs_name, type, tag, year);
				
//		SeparateFit(plotpath, cate_s, fs_name, type, year);		
	}
}	
