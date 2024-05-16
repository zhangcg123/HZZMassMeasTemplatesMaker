#include "include/Set4LSlimmedTree.h"
#include "include/Read4LSlimmedTree.h"
#include "include/do4LFit.h"
#include "include/MakePlot.h"
void BKGEventYieldFromTau_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, double* yields_bkg);
void BKGEventYield_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process, double* yields_bkg);

int main(int argc, char *argv[]){
        TString year  = argv[1];
        TString type = argv[2];
        TString tag = argv[3];
        int fs = atoi(argv[4]);
        int cates = atoi(argv[5]);
        TString plotpath = argv[6];
        TString steponedir = argv[7];
	TString process = argv[8];

	TString fs_name;
	if(fs==1)fs_name = "4mu";
	if(fs==2)fs_name = "4e";
	if(fs==3)fs_name = "2e2mu";
	if(fs==4)fs_name = "2mu2e";
		
	double yields_bkg[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	BKGEventYield_(plotpath, fs, type, tag, year, cates, steponedir, process, yields_bkg);
	if(process=="ggzz")BKGEventYieldFromTau_(plotpath, fs, type, tag, year, cates, steponedir, yields_bkg);

	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		MakeBKGEventYieldPlot(yields_bkg[i], plotpath, i_s, fs_name, type, process, tag, year);
	}
}

void BKGEventYieldFromTau_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, double* yields_bkg){
	
	TString fs_name;
	if(fs==1)fs_name = "4mu";
	if(fs==2)fs_name = "4e";
	if(fs==3)fs_name = "2e2mu";
	if(fs==4)fs_name = "2mu2e";
	TFile* f = new TFile(steponedir + "/_"+fs_name+"_bounds.root");
	TH1F* h_std = (TH1F*)f->Get("bounds");
	
	TFile* f_2e2tau = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/"+year+"/GluGluToContinToZZTo2e2tau_M125_"+year+"_skimmed.root","READ");
	TFile* f_2mu2tau = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/"+year+"/GluGluToContinToZZTo2mu2tau_M125_"+year+"_skimmed.root","READ");
	TFile* f_4tau = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/"+year+"/GluGluToContinToZZTo4tau_M125_"+year+"_skimmed.root","READ");
	TTree* t_2e2tau = (TTree*)f_2e2tau->Get("passedEvents");
	TTree* t_2mu2tau = (TTree*)f_2mu2tau->Get("passedEvents");
	TTree* t_4tau = (TTree*)f_4tau->Get("passedEvents");
	
	ReadTreeForTau(yields_bkg, h_std, t_2e2tau, t_2mu2tau, t_4tau, fs, year, type, tag);
	//for(int i=0; i<cates; i++){
	//	TString i_s = to_string(i);
	//	MakeBKGEventYieldPlot(yields_bkg[i], plotpath, i_s, fs_name, type, "ggzz_tau", tag, year);
	//}
}
	

	
void BKGEventYield_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process, double* yields_bkg){
	
	TString fs_name;
	if(fs==1)fs_name = "4mu";
	if(fs==2)fs_name = "4e";
	if(fs==3)fs_name = "2e2mu";
	if(fs==4)fs_name = "2mu2e";
	TFile* f = new TFile(steponedir + "/_"+fs_name+"_bounds.root");
	TH1F* h_std = (TH1F*)f->Get("bounds");
	
	//retrieve name
	TString process_;
	if(process=="qqzz"&&BASE=="20UL")process_ = "ZZTo4L_M125_"+year+"_skimmed";
	if(process=="qqzz"&&BASE=="ReReco")process_ = "slimmed_"+year+"qqZZ";
	if(fs_name=="4mu"&&process=="ggzz"&&BASE=="20UL")process_ = "GluGluToContinToZZTo4mu_M125_"+year+"_skimmed";
	if(fs_name=="4mu"&&process=="ggzz"&&BASE=="ReReco")process_ = year+"ggZZ_4mu";
	if(fs_name=="4e"&&process=="ggzz"&&BASE=="20UL")process_ = "GluGluToContinToZZTo4e_M125_"+year+"_skimmed";
	if(fs_name=="4e"&&process=="ggzz"&&BASE=="ReReco")process_ = year+"ggZZ_4e";
	if((fs_name=="2mu2e"||fs_name=="2e2mu")&&process=="ggzz"&&BASE=="20UL")process_ = "GluGluToContinToZZTo2e2mu_M125_"+year+"_skimmed";
	if((fs_name=="2mu2e"||fs_name=="2e2mu")&&process=="ggzz"&&BASE=="ReReco")process_ = year+"ggZZ_2e2mu";
	
	TFile* f_ = new TFile();
	if(BASE=="ReReco")f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+process_+".root");
	if(BASE=="20UL"){
			f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+".root");
	}
	TTree* t = (TTree*)f_->Get("passedEvents");

	//double yields_bkg[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yielderr[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	vector<TH1F*> massggzzdistributions; massggzzdistributions.clear();
	vector<TH1F*> massqqzzdistributions; massqqzzdistributions.clear();
	vector<TH1F*> ebeggzzdistributions; ebeggzzdistributions.clear();
	vector<TH1F*> ebeqqzzdistributions; ebeqqzzdistributions.clear();
	vector<TH1F*> relebeggzzdistributions; relebeggzzdistributions.clear();
	vector<TH1F*> relebeqqzzdistributions; relebeqqzzdistributions.clear();

	vector<TH2F*> ebevsmggzzdistributions; ebevsmggzzdistributions.clear();
	vector<TH2F*> ebevsmqqzzdistributions; ebevsmqqzzdistributions.clear();
	vector<TH2F*> relebevsmggzzdistributions; relebevsmggzzdistributions.clear();
	vector<TH2F*> relebevsmqqzzdistributions; relebevsmqqzzdistributions.clear();
	for( int kkk=0; kkk<cates; kkk++){
		TString kkks = to_string(kkk);
		TH1F* ggzzmassdis = new TH1F("ggzz_mass_"+kkks,"",100,105,140);
		TH1F* qqzzmassdis = new TH1F("qqzz_mass_"+kkks,"",100,105,140);
		massggzzdistributions.push_back( ggzzmassdis );
		massqqzzdistributions.push_back( qqzzmassdis );

		TH1F* ggzzebedis = new TH1F("ggzz_ebe_"+kkks,"",100,0,20);
		TH1F* qqzzebedis = new TH1F("qqzz_ebe_"+kkks,"",100,0,20);
		ebeggzzdistributions.push_back( ggzzebedis );
		ebeqqzzdistributions.push_back( qqzzebedis );

		TH1F* ggzzrelebedis = new TH1F("ggzz_relebe_"+kkks,"",100,0,0.15);
		TH1F* qqzzrelebedis = new TH1F("qqzz_relebe_"+kkks,"",100,0,0.15);
		relebeggzzdistributions.push_back( ggzzrelebedis );
		relebeqqzzdistributions.push_back( qqzzrelebedis );

		TH2F* ggzzebevsmdis = new TH2F("ggzz_ebevsm_"+kkks,"",100,105,140,100,0,20);
		TH2F* qqzzebevsmdis = new TH2F("qqzz_ebevsm_"+kkks,"",100,105,140,100,0,20);
		ebevsmggzzdistributions.push_back( ggzzebevsmdis );
		ebevsmqqzzdistributions.push_back( qqzzebevsmdis );

		TH2F* ggzzrelebevsmdis = new TH2F("ggzz_relebevsm_"+kkks,"",100,105,140,100,0,0.15);
		TH2F* qqzzrelebevsmdis = new TH2F("qqzz_relebevsm_"+kkks,"",100,105,140,100,0,0.15);
		relebevsmggzzdistributions.push_back( ggzzrelebevsmdis );
		relebevsmqqzzdistributions.push_back( qqzzrelebevsmdis );
		
	}
	
	if(process=="qqzz"){
		ReadTreeForQQZZEventYield(yields_bkg, h_std, t, fs, year, process, type, tag, massqqzzdistributions, ebeqqzzdistributions, relebeqqzzdistributions, ebevsmqqzzdistributions, relebevsmqqzzdistributions );
		MassDistributionPlot( "qqzz", fs_name, year, "125", type, tag, massqqzzdistributions, plotpath );
		MassDistributionPlot( "qqzz", fs_name, year, "125", type, tag, ebeqqzzdistributions, plotpath );
		MassDistributionPlot( "qqzz", fs_name, year, "125", type, tag, relebeqqzzdistributions, plotpath );
		MassDistribution2DPlot( "qqzz", fs_name, year, "125", type, tag, ebevsmqqzzdistributions, plotpath );
		MassDistribution2DPlot( "qqzz", fs_name, year, "125", type, tag, relebevsmqqzzdistributions, plotpath );
	}
	if(process=="ggzz"){
		ReadTreeForEventYield(yields_bkg, yielderr, h_std, t, fs, year, process, 0, type, tag, massggzzdistributions, ebeggzzdistributions, relebeggzzdistributions, ebevsmggzzdistributions, relebevsmggzzdistributions );

		MassDistributionPlot( "ggzz", fs_name, year, "125", type, tag, massggzzdistributions, plotpath );
		MassDistributionPlot( "ggzz", fs_name, year, "125", type, tag, massggzzdistributions, plotpath );
		MassDistributionPlot( "ggzz", fs_name, year, "125", type, tag, ebeggzzdistributions, plotpath );
		MassDistributionPlot( "ggzz", fs_name, year, "125", type, tag, relebeggzzdistributions, plotpath );
		MassDistribution2DPlot( "ggzz", fs_name, year, "125", type, tag, ebevsmggzzdistributions, plotpath );
		MassDistribution2DPlot( "ggzz", fs_name, year, "125", type, tag, relebevsmggzzdistributions, plotpath );
	}
	

	//for(int i=0; i<cates; i++){
	//	TString i_s = to_string(i);
	//	MakeBKGEventYieldPlot(yields_bkg[i], plotpath, i_s, fs_name, type, process, tag, year);
	//}
}
