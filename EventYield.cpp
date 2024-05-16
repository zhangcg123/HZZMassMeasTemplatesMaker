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
	if(BASE=="20UL"){
		if(process_=="VBF_HToZZTo4L_M"||process_=="GluGluHToZZTo4L_M"||process_=="WminusH_HToZZTo4L_M"||process_=="WplusH_HToZZTo4L_M"||process_=="ZH_HToZZ_4LFilter_M"){	
			f_120 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"120_"+year+"_skimmed.root");
		}else{
			f_120 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"120_"+year+"_skimmed.root");
		}
	}
	TTree* t_120 = (TTree*)f_120->Get("passedEvents");

	if(BASE=="ReReco")f_124 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+process_+"_124.root");
	if(BASE=="20UL"){
		if(process_=="VBF_HToZZTo4L_M"||process_=="GluGluHToZZTo4L_M"||process_=="WminusH_HToZZTo4L_M"||process_=="WplusH_HToZZTo4L_M"||process_=="ZH_HToZZ_4LFilter_M"){	
			f_124 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"124_"+year+"_skimmed.root");
		}else{
			f_124 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"124_"+year+"_skimmed.root");
		}
	}
	TTree* t_124 = (TTree*)f_124->Get("passedEvents");

	if(BASE=="ReReco")f_125 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+process_+"_125.root");
	if(BASE=="20UL"){//24/Mar/2023, update ebe scale factor
		if(process_=="VBF_HToZZTo4L_M"||process_=="GluGluHToZZTo4L_M"||process_=="WminusH_HToZZTo4L_M"||process_=="WplusH_HToZZTo4L_M"||process_=="ZH_HToZZ_4LFilter_M"){
			f_125 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"125_"+year+"_skimmed.root");
		}else{
			f_125 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"125_"+year+"_skimmed.root");
		}
	}
	TTree* t_125 = (TTree*)f_125->Get("passedEvents");

	if(BASE=="ReReco")f_126 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+process_+"_126.root");
	if(BASE=="20UL"){
		if(process_=="VBF_HToZZTo4L_M"||process_=="GluGluHToZZTo4L_M"||process_=="WminusH_HToZZTo4L_M"||process_=="WplusH_HToZZTo4L_M"||process_=="ZH_HToZZ_4LFilter_M"){
			f_126 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"126_"+year+"_skimmed.root");
		}else{
			f_126 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"126_"+year+"_skimmed.root");
		}
	}
	TTree* t_126 = (TTree*)f_126->Get("passedEvents");

	if(BASE=="ReReco")f_130 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+year+process_+"_130.root");
	if(BASE=="20UL"){
		if(process_=="VBF_HToZZTo4L_M"||process_=="GluGluHToZZTo4L_M"||process_=="WminusH_HToZZTo4L_M"||process_=="WplusH_HToZZTo4L_M"||process_=="ZH_HToZZ_4LFilter_M"){	
			f_130 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"130_"+year+"_skimmed.root");
		}else{
			f_130 = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+process_+"130_"+year+"_skimmed.root");
		}
	}
	TTree* t_130 = (TTree*)f_130->Get("passedEvents");

	double yields_120[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};					//right now, the pkg supports the number of mass error categs until 13
	double yields_124[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yields_125[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yields_126[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yields_130[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	vector<TH1F*> mass120_distributions; mass120_distributions.clear();
	vector<TH1F*> mass124_distributions; mass124_distributions.clear();
	vector<TH1F*> mass125_distributions; mass125_distributions.clear();
	vector<TH1F*> mass126_distributions; mass126_distributions.clear();
	vector<TH1F*> mass130_distributions; mass130_distributions.clear();

	vector<TH1F*> masserr120_distributions; masserr120_distributions.clear();
	vector<TH1F*> masserr124_distributions; masserr124_distributions.clear();
	vector<TH1F*> masserr125_distributions; masserr125_distributions.clear();
	vector<TH1F*> masserr126_distributions; masserr126_distributions.clear();
	vector<TH1F*> masserr130_distributions; masserr130_distributions.clear();

	vector<TH1F*> massrelerr120_distributions; massrelerr120_distributions.clear();
	vector<TH1F*> massrelerr124_distributions; massrelerr124_distributions.clear();
	vector<TH1F*> massrelerr125_distributions; massrelerr125_distributions.clear();
	vector<TH1F*> massrelerr126_distributions; massrelerr126_distributions.clear();
	vector<TH1F*> massrelerr130_distributions; massrelerr130_distributions.clear();

	vector<TH2F*> ebevsm120_distributions; ebevsm120_distributions.clear();
	vector<TH2F*> ebevsm124_distributions; ebevsm124_distributions.clear();
	vector<TH2F*> ebevsm125_distributions; ebevsm125_distributions.clear();
	vector<TH2F*> ebevsm126_distributions; ebevsm126_distributions.clear();
	vector<TH2F*> ebevsm130_distributions; ebevsm130_distributions.clear();

	vector<TH2F*> relebevsm120_distributions; relebevsm120_distributions.clear();
	vector<TH2F*> relebevsm124_distributions; relebevsm124_distributions.clear();
	vector<TH2F*> relebevsm125_distributions; relebevsm125_distributions.clear();
	vector<TH2F*> relebevsm126_distributions; relebevsm126_distributions.clear();
	vector<TH2F*> relebevsm130_distributions; relebevsm130_distributions.clear();
	
	vector<TH1F*> kd120_distributions; kd120_distributions.clear();
	vector<TH1F*> kd124_distributions; kd124_distributions.clear();
	vector<TH1F*> kd125_distributions; kd125_distributions.clear();
	vector<TH1F*> kd126_distributions; kd126_distributions.clear();
	vector<TH1F*> kd130_distributions; kd130_distributions.clear();

	
	for( int kkk=0; kkk<cates; kkk++){
		TH1F* hhh120 = new TH1F(process+"120_mass_"+fs_name+"_"+to_string(kkk),"",100,105,140);
		TH1F* hhh124 = new TH1F(process+"124_mass_"+fs_name+"_"+to_string(kkk),"",100,105,140);
		TH1F* hhh125 = new TH1F(process+"125_mass_"+fs_name+"_"+to_string(kkk),"",100,105,140);
		TH1F* hhh126 = new TH1F(process+"126_mass_"+fs_name+"_"+to_string(kkk),"",100,105,140);
		TH1F* hhh130 = new TH1F(process+"130_mass_"+fs_name+"_"+to_string(kkk),"",100,105,140);
		mass120_distributions.push_back( hhh120 );	
		mass124_distributions.push_back( hhh124 );
		mass125_distributions.push_back( hhh125 );
		mass126_distributions.push_back( hhh126 );
		mass130_distributions.push_back( hhh130 );
		
		TH1F* hhherr120 = new TH1F(process+"120_ebe_"+fs_name+"_"+to_string(kkk),"",100,0,20);
		TH1F* hhherr124 = new TH1F(process+"124_ebe_"+fs_name+"_"+to_string(kkk),"",100,0,20);
		TH1F* hhherr125 = new TH1F(process+"125_ebe_"+fs_name+"_"+to_string(kkk),"",100,0,20);
		TH1F* hhherr126 = new TH1F(process+"126_ebe_"+fs_name+"_"+to_string(kkk),"",100,0,20);
		TH1F* hhherr130 = new TH1F(process+"130_ebe_"+fs_name+"_"+to_string(kkk),"",100,0,20);
		masserr120_distributions.push_back( hhherr120 );	
		masserr124_distributions.push_back( hhherr124 );
		masserr125_distributions.push_back( hhherr125 );
		masserr126_distributions.push_back( hhherr126 );
		masserr130_distributions.push_back( hhherr130 );
	
		TH1F* hhhrelerr120 = new TH1F(process+"120_relebe_"+fs_name+"_"+to_string(kkk),"",100,0,0.15);
		TH1F* hhhrelerr124 = new TH1F(process+"124_relebe_"+fs_name+"_"+to_string(kkk),"",100,0,0.15);
		TH1F* hhhrelerr125 = new TH1F(process+"125_relebe_"+fs_name+"_"+to_string(kkk),"",100,0,0.15);
		TH1F* hhhrelerr126 = new TH1F(process+"126_relebe_"+fs_name+"_"+to_string(kkk),"",100,0,0.15);
		TH1F* hhhrelerr130 = new TH1F(process+"130_relebe_"+fs_name+"_"+to_string(kkk),"",100,0,0.15);
		massrelerr120_distributions.push_back( hhhrelerr120 );	
		massrelerr124_distributions.push_back( hhhrelerr124 );
		massrelerr125_distributions.push_back( hhhrelerr125 );
		massrelerr126_distributions.push_back( hhhrelerr126 );
		massrelerr130_distributions.push_back( hhhrelerr130 );

		TH2F* hhhebevsm120 = new TH2F(process+"120_ebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,20);
		TH2F* hhhebevsm124 = new TH2F(process+"124_ebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,20);
		TH2F* hhhebevsm125 = new TH2F(process+"125_ebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,20);
		TH2F* hhhebevsm126 = new TH2F(process+"126_ebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,20);
		TH2F* hhhebevsm130 = new TH2F(process+"130_ebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,20);
		ebevsm120_distributions.push_back( hhhebevsm120 );	
		ebevsm124_distributions.push_back( hhhebevsm124 );
		ebevsm125_distributions.push_back( hhhebevsm125 );
		ebevsm126_distributions.push_back( hhhebevsm126 );
		ebevsm130_distributions.push_back( hhhebevsm130 );

		TH2F* hhhrelebevsm120 = new TH2F(process+"120_relebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,0.15);
		TH2F* hhhrelebevsm124 = new TH2F(process+"124_relebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,0.15);
		TH2F* hhhrelebevsm125 = new TH2F(process+"125_relebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,0.15);
		TH2F* hhhrelebevsm126 = new TH2F(process+"126_relebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,0.15);
		TH2F* hhhrelebevsm130 = new TH2F(process+"130_relebevsm_"+fs_name+"_"+to_string(kkk),"",100,105,140,100,0,0.15);
		relebevsm120_distributions.push_back( hhhrelebevsm120 );	
		relebevsm124_distributions.push_back( hhhrelebevsm124 );
		relebevsm125_distributions.push_back( hhhrelebevsm125 );
		relebevsm126_distributions.push_back( hhhrelebevsm126 );
		relebevsm130_distributions.push_back( hhhrelebevsm130 );

	}

	double yieldserr_120[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yieldserr_124[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yieldserr_125[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yieldserr_126[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yieldserr_130[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	ReadTreeForEventYield(yields_120, yieldserr_120, h_std, t_120, fs, year, process, 120, type, tag, mass120_distributions, masserr120_distributions, massrelerr120_distributions, ebevsm120_distributions, relebevsm120_distributions);
	
	ReadTreeForEventYield(yields_124, yieldserr_124, h_std, t_124, fs, year, process, 124, type, tag, mass124_distributions, masserr124_distributions, massrelerr124_distributions, ebevsm124_distributions, relebevsm124_distributions);

	ReadTreeForEventYield(yields_125, yieldserr_125, h_std, t_125, fs, year, process, 125, type, tag, mass125_distributions, masserr125_distributions, massrelerr125_distributions, ebevsm125_distributions, relebevsm125_distributions);
	
	ReadTreeForEventYield(yields_126, yieldserr_126, h_std, t_126, fs, year, process, 126, type, tag, mass126_distributions, masserr126_distributions, massrelerr126_distributions, ebevsm126_distributions, relebevsm126_distributions);
	
	ReadTreeForEventYield(yields_130, yieldserr_130, h_std, t_130, fs, year, process, 130, type, tag, mass130_distributions, masserr130_distributions, massrelerr130_distributions, ebevsm130_distributions, relebevsm130_distributions);
	
	//MassDistributionPlot( process, fs_name, year, "120", type, tag, mass120_distributions, plotpath );
	//MassDistributionPlot( process, fs_name, year, "124", type, tag, mass124_distributions, plotpath );
	MassDistributionPlot( process, fs_name, year, "125", type, tag, mass125_distributions, plotpath );
	MassDistributionPlot( process, fs_name, year, "125", type, tag, masserr125_distributions, plotpath );
	MassDistributionPlot( process, fs_name, year, "125", type, tag, massrelerr125_distributions, plotpath );
	MassDistribution2DPlot( process, fs_name, year, "125", type, tag, ebevsm125_distributions, plotpath );
	MassDistribution2DPlot( process, fs_name, year, "125", type, tag, relebevsm125_distributions, plotpath );
	//MassDistributionPlot( process, fs_name, year, "126", type, tag, mass126_distributions, plotpath );
	//MassDistributionPlot( process, fs_name, year, "130", type, tag, mass130_distributions, plotpath );

	for(int i=0; i<cates; i++){
		TString cate = to_string(i);
		FitEventYield(yields_120[i], sqrt(yieldserr_120[i]), yields_124[i], sqrt(yieldserr_124[i]), yields_125[i], sqrt(yieldserr_125[i]), yields_126[i], sqrt(yieldserr_126[i]), yields_130[i], sqrt(yieldserr_130[i]), plotpath, cate, fs_name, process, type, tag, year);
	}

}
