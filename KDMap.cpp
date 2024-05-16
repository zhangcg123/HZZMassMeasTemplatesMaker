#include "include/Set4LSlimmedTree.h"
#include "include/Read4LSlimmedTree.h"
#include "include/do4LFit.h"
#include "include/MakePlot.h"

void KDMap_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process); 

int main(int argc, char *argv[]){
        TString year  = argv[1];
        TString type = argv[2];
        TString tag = argv[3];
        int fs = atoi(argv[4]);
        int cates = atoi(argv[5]);
        TString plotpath = argv[6];
        TString steponedir = argv[7];
	TString process = argv[8];

	KDMap_(plotpath, fs, type, tag, year, cates, steponedir, process);

}


void KDMap_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString steponedir, TString process){
	 
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
	if(process=="ggh"&&BASE=="20UL")tuple = "GluGluHToZZTo4L_M120-130_"+year+"_skimmed";
	if(process=="ggh"&&BASE=="ReReco")tuple = year+"GGH_120-130";
	if(process=="ggzz"&&BASE=="20UL")tuple = "GluGluToContinToZZTo"+tuple_subname+"_M125_"+year+"_skimmed";
	if(process=="ggzz"&&BASE=="ReReco")tuple = year+"ggZZ_"+tuple_subname;
	if(process=="qqzz"&&BASE=="20UL")tuple = "ZZTo4L_M125_"+year+"_skimmed";
	if(process=="qqzz"&&BASE=="ReReco")tuple = "slimmed_"+year+"qqZZ";
	TFile* f_ = new TFile();
	if(BASE=="ReReco")f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/liteUFHZZ4LAnalyzer/Ntuples/ReReco/"+tuple+".root");
	if(BASE=="20UL")f_ = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTreesMarch2023/"+year+"/"+tuple+".root");
	TTree* t = (TTree*)f_->Get("passedEvents");
	vector<TH2F*> histos; histos.clear();
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		TH2F* h = new TH2F("h_"+i_s,"",35,105,140,20,0.0,1.0);
		histos.push_back(h);
	}
	Read4LSlimmedTree1DCategoryForKD(histos, h_std, t, fs, type, tag);
	
	//avoid Nan
	int nXbins = histos.at(0)->GetNbinsX();
	int nYbins = histos.at(0)->GetNbinsY();
	for(int i=0; i<cates; i++){							//loop over categories
		for(int x=1; x<=nXbins; x++){						//loop over x(mass4l) bins
			for(int y=1; y<=nYbins; y++){					//loop over y(KD) bins
				if(histos.at(i)->GetBinContent(x,y)==0)histos.at(i)->SetBinContent(x,y,0.00001);
			}
		}
	}
	
	//normalize slides
	double norm;
	TH1F* tmpProj;
	for(int i=0; i<cates; i++){							//loop over categories
		TString i_s = to_string(i);
		for(int x=1; x<=nXbins; x++){						//x bins mean slides
			tmpProj = (TH1F*)histos.at(i)->ProjectionY("Projec",x,x);
			norm = tmpProj->Integral("width");
			for(int y=1; y<=nYbins; y++){
				histos.at(i)->SetBinContent(x,y,histos.at(i)->GetBinContent(x,y)/norm);
			}
		}
	}

	//seva and rename
	TString plotname;
	if(process=="ggh")plotname = "ggh";
	if(process=="ggzz")plotname = "ggzz";
	if(process=="qqzz")plotname = "qqzz";
	TCanvas c("c","",1400,1000);
	c.cd();
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		histos.at(i)->SetStats(0);
		histos.at(i)->Smooth();
		histos.at(i)->Smooth();
		histos.at(i)->GetXaxis()->SetTitle("mass_{4l}");
		histos.at(i)->GetXaxis()->SetTitleOffset(2);
		histos.at(i)->GetYaxis()->SetTitle("D_{bkg}^{sig}");
		histos.at(i)->GetYaxis()->SetTitleOffset(2);
		double integral = histos.at(i)->Integral();
		TString integral_s = to_string(integral);
		TLatex* latex = new TLatex();
		latex->SetNDC();
		latex->SetTextSize(0.05);
		latex->SetTextFont(42);
		latex->SetTextAlign(23);
		histos.at(i)->Draw("surf");
		//latex->DrawLatex(0.6,0.8,"Integral="+integral_s);
		ThesisPlot( &c );
		c.SaveAs(plotpath + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_kdmap_" + plotname + ".png");
		TFile* f = new TFile(plotpath + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_kdmap_" + plotname + ".root","RECREATE");
		f->cd();
		histos.at(i)->Write(process+"_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"_kd");
		histos.at(i)->Write(process+"_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"_kdup");
		histos.at(i)->Write(process+"_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"_kddn");
		f->Close();
		delete latex;
	}	
}	
