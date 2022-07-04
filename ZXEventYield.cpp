#include "include/Set4LSlimmedTree.h"

void ZXEventYield_(TString plotpath_norm, TString plotpath_param, TString plotpath_kdmap, int fs, TString type, TString tag, TString year, int cates, TString steponedir);

int main(int argc, char* argv[]){
        TString year  = argv[1];
        TString type = argv[2];
        TString tag = argv[3];
        int fs = atoi(argv[4]);
        int cates = atoi(argv[5]);
        TString plotpath_norm = argv[6];
	TString plotpath_param = argv[7];
	TString plotpath_kdmap = argv[8];
        TString steponedir = argv[9];

	ZXEventYield_(plotpath_norm, plotpath_param, plotpath_kdmap, fs, type, tag, year, cates, steponedir);

}
void SetZXTree(TTree* t, TString type){
	t->SetBranchStatus("*",0);
	t->SetBranchStatus("massZ2",1);
	t->SetBranchStatus("mass4l",1);
	t->SetBranchStatus("mass4lREFIT",1);
	t->SetBranchStatus("mass4l_vtxFSR_BS",1);
	t->SetBranchStatus("mass4lREFIT_vtx_BS",1);
	t->SetBranchStatus("mass4lErr",1);
	t->SetBranchStatus("mass4lErrREFIT",1);
	t->SetBranchStatus("mass4lErr_vtx_BS",1);
	t->SetBranchStatus("mass4lErrREFIT_vtx_BS",1);
	t->SetBranchStatus("D_bkg_kin",1);
	t->SetBranchStatus("passedFullSelection",1);
	t->SetBranchStatus("finalState",1);
	t->SetBranchStatus("eventWeight",1);
	t->SetBranchStatus("pileupWeight", 1);
	t->SetBranchStatus("k_ggZZ",1);
	t->SetBranchStatus("is2P2F",1);
	t->SetBranchStatus("is3P1F",1);
	t->SetBranchStatus("isMCzz",1);
	t->SetBranchStatus("eventWeightFR",1);
	t->SetBranchStatus("eventWeightFR_up",1);
	t->SetBranchStatus("eventWeightFR_down",1);
	if(type == "bs"){
		t->SetBranchAddress("mass4l_vtxFSR_BS", &mass4l);
		t->SetBranchAddress("mass4lErr_vtx_BS", &mass4lErr);
	}
	if(type == "bsrefit"){
		t->SetBranchAddress("mass4lREFIT_vtx_BS", &mass4l);
		t->SetBranchAddress("mass4lErrREFIT_vtx_BS", &mass4lErr);
	}
	if(type == "reco"){
		t->SetBranchAddress("mass4l", &mass4l);
		t->SetBranchAddress("mass4lErr", &mass4lErr);
	}
	if(type == "refit"){
		t->SetBranchAddress("mass4lREFIT", &mass4l);
		t->SetBranchAddress("mass4lErrREFIT", &mass4lErr);
	}
	t->SetBranchAddress("massZ2", &massZ2);
	t->SetBranchAddress("D_bkg_kin", &D_bkg_kin);
	t->SetBranchAddress("passedFullSelection", &passedFullSelection);
	t->SetBranchAddress("finalState", &finalState);
	t->SetBranchAddress("eventWeight", &eventWeight);
	t->SetBranchAddress("pileupWeight", &pileupWeight);
	t->SetBranchAddress("k_ggZZ", &k_ggZZ);
	t->SetBranchAddress("is2P2F",&is2P2F);
	t->SetBranchAddress("is3P1F",&is3P1F);
	t->SetBranchAddress("isMCzz",&isMCzz);
	t->SetBranchAddress("eventWeightFR", &eventWeightFR);
	t->SetBranchAddress("eventWeightFR_up", &eventWeightFR_up);
	t->SetBranchAddress("eventWeightFR_down", &eventWeightFR_down);
}
void ZXEventYield_(TString plotpath_norm, TString plotpath_param, TString plotpath_kdmap, int fs, TString type, TString tag, TString year, int cates, TString steponedir){
	
	TString fs_name;
	if(fs==1)fs_name="4mu";
	if(fs==2)fs_name="4e";
	if(fs==3)fs_name="2e2mu";
	if(fs==4)fs_name="2mu2e";
	
	TFile* f = new TFile(steponedir + "/_"+fs_name+"_bounds.root");
	TH1F* h_std = (TH1F*)f->Get("bounds");
	TAxis* h_x = h_std->GetXaxis();

	TFile* f_ = new TFile();
	//if(BASE=="20UL"&&year=="2018")f_ = new TFile("/afs/cern.ch/user/c/chenguan/public/zxsectionfromfilippo/cjlstOSmethodevtsel_2p2plusf_3p1plusf_downupscale_2018_Data_ZZ.root","READ");//first look at UL
	if(BASE=="20UL"&&(year=="2018"||year=="2017"))f_ = new TFile("/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/RedBkg/rootfiles/redbkgest_UL_WZxs5p26pb_"+year+"_Data_ZZ.root","READ");
	if(BASE=="20UL"&&year=="20160")f_ = new TFile("/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/RedBkg/rootfiles/redbkgest_UL_WZxs5p26pb_2016preVFP_Data_ZZ.root","READ");
	if(BASE=="20UL"&&year=="20165")f_ = new TFile("/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/RedBkg/rootfiles/redbkgest_UL_WZxs5p26pb_2016postVFP_Data_ZZ.root","READ");
	

	TTree* t = (TTree*)f_->Get("passedEvents");
	SetZXTree(t, type);
	//for yields
	double yields_1[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yields_2[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	double yields_3[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	/*
	vector<TH1F*> h_yields1; h_yields1.clear();
	vector<TH1F*> h_yields2; h_yields2.clear();
	vector<TH1F*> h_yields3; h_yields3.clear();
	for(int i=0; i<cates; i++){
		TString i_s = tp_string(i);
		TH1D* h_y1 = new TH1D("h_y1_"+i_s,"",20,70,770);
		TH1D* h_y2 = new TH1D("h_y1_"+i_s,"",20,70,770);
		TH1D* h_y3 = new TH1D("h_y1_"+i_s,"",20,70,770);
		h_yields1.push_back(h_y1);
		h_yields2.push_back(h_y2);
		h_yields3.push_back(h_y3);
	}
	*/
	//for fit
	vector<TH1F*> histos_1; histos_1.clear();
	vector<TH1F*> histos_2; histos_2.clear();
	vector<TH1F*> histos_3; histos_3.clear();
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		TH1F* h_1 = new TH1F("h_1_"+i_s,"",50,70,570);
		TH1F* h_2 = new TH1F("h_2_"+i_s,"",50,70,570);
		TH1F* h_3 = new TH1F("h_3_"+i_s,"",50,70,570);
		histos_1.push_back(h_1);
		histos_2.push_back(h_2);
		histos_3.push_back(h_3);
	}
	//for kd map	
	vector<TH2F*> kdmaps_1; kdmaps_1.clear();
	vector<TH2F*> kdmaps_2; kdmaps_2.clear();
	vector<TH2F*> kdmaps_3; kdmaps_3.clear();
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		TH2F* kdmap_1 = new TH2F("kdmap_1_"+i_s,"",35,105,140,20,0,1.0);
		TH2F* kdmap_2 = new TH2F("kdmap_2_"+i_s,"",35,105,140,20,0,1.0);
		TH2F* kdmap_3 = new TH2F("kdmap_3_"+i_s,"",35,105,140,20,0,1.0);
		kdmaps_1.push_back(kdmap_1);
		kdmaps_2.push_back(kdmap_2);
		kdmaps_3.push_back(kdmap_3);
	}

	int tot_ = t->GetEntries();
	for(int i=0; i<tot_; i++){
		t->GetEntry(i);
		if((mass4lErr/mass4l)<0.3&&mass4l>105&&mass4l<140&&finalState==fs/*&&abs(eta1)<ETA1&&abs(eta2)<ETA2&&abs(eta3)<ETA3&&abs(eta4)<ETA4&&pt1>PT1&&pt2>PT2&&pt3>PT3&&pt4>PT4*/){//for kdmap
			if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
				int bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
				if(is2P2F==1){
					yields_1[bin1] = yields_1[bin1]+eventWeightFR;
					//histos_1[bin1]->Fill(mass4l,eventWeightFR);
					kdmaps_1[bin1]->Fill(mass4l,D_bkg_kin,eventWeightFR);
				}
				if(isMCzz==1&&is3P1F==1){
					yields_2[bin1] = yields_2[bin1]+ (eventWeightFR);//*1.943/1.785);the factor is for snowmass
					//histos_2[bin1]->Fill(mass4l,eventWeightFR);//*1.943/1.785);
					kdmaps_2[bin1]->Fill(mass4l,D_bkg_kin,eventWeightFR);//*1.943/1.785);
				}
				if(isMCzz==0&&is3P1F==1){
					yields_3[bin1] = yields_3[bin1] + (eventWeightFR);
					//histos_3[bin1]->Fill(mass4l,eventWeightFR);
					kdmaps_3[bin1]->Fill(mass4l,D_bkg_kin,eventWeightFR);
				}
			}
		}
		if((mass4lErr/mass4l)<0.1&&mass4l<570&&mass4l>70&&finalState==fs/*&&abs(eta1)<ETA1&&abs(eta2)<ETA2&&abs(eta3)<ETA3&&abs(eta4)<ETA4&&pt1>PT1&&pt2>PT2&&pt3>PT3&&pt4>PT4*/){//for fit
			if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
				int bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
				if(is2P2F==1){
					histos_1[bin1]->Fill(mass4l,eventWeightFR);
				}
				if(isMCzz==1&&is3P1F==1){
					histos_2[bin1]->Fill(mass4l,eventWeightFR);//*1.943/1.785);
				}
				if(isMCzz==0&&is3P1F==1){
					histos_3[bin1]->Fill(mass4l,eventWeightFR);
				}
			}
		}
	}
	//pre-process
	//slice normalize 3 kinds of kd maps	
	
	double norm;
	TH1F* tmpProj;
	/*
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		kdmaps_1[i]->Smooth();
		for(int x=1; x<kdmaps_1[i]->GetNbinsX()+1; x++){
			tmpProj = (TH1F*)kdmaps_1.at(i)->ProjectionY("Projec",x,x);
			norm = tmpProj->Integral("width");
			for(int y=1; y<kdmaps_1[i]->GetNbinsY()+1; y++){
				kdmaps_1.at(i)->SetBinContent(x,y,kdmaps_1.at(i)->GetBinContent(x,y)/norm);
			}
		}
	}
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		kdmaps_2[i]->Smooth();
		for(int x=1; x<kdmaps_2[i]->GetNbinsX()+1; x++){
			tmpProj = (TH1F*)kdmaps_2.at(i)->ProjectionY("Projec",x,x);
			norm = tmpProj->Integral("width");
			for(int y=1; y<kdmaps_2[i]->GetNbinsY()+1; y++){
				kdmaps_2.at(i)->SetBinContent(x,y,kdmaps_2.at(i)->GetBinContent(x,y)/norm);
			}
		}
	}
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		kdmaps_3[i]->Smooth();
		for(int x=1; x<kdmaps_3[i]->GetNbinsX()+1; x++){
			tmpProj = (TH1F*)kdmaps_3.at(i)->ProjectionY("Projec",x,x);
			norm = tmpProj->Integral("width");
			for(int y=1; y<kdmaps_3[i]->GetNbinsY()+1; y++){
				kdmaps_3.at(i)->SetBinContent(x,y,kdmaps_3.at(i)->GetBinContent(x,y)/norm);
			}
		}
	}
	*/
	//extract final
	double yields_final[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	vector<TH1F*> histos_final; histos_final.clear();
	vector<TH2F*> kdmaps_final; kdmaps_final.clear();
	for(int i=0; i<cates; i++){
		
		yields_final[i] = yields_3[i] - yields_2[i];
		if(yields_final[i] > 2*yields_1[i]){
			yields_final[i] = yields_final[i] - yields_1[i];
		}else{
			yields_final[i] = yields_1[i];
		}

		TH1F* histo_tmp = (TH1F*)histos_3[i]->Clone();
		histo_tmp->Add(histos_1[i],-1);
		histo_tmp->Add(histos_2[i],-1);
		histos_final.push_back(histo_tmp);

		TH2F* kdmap_tmp = (TH2F*)kdmaps_3[i]->Clone();
		kdmap_tmp->Add(kdmaps_2[i],-1);
		for(int x=1; x<kdmap_tmp->GetNbinsX()+1; x++){
			for(int y=1; y<kdmap_tmp->GetNbinsY()+1; y++){
				if(kdmap_tmp->GetBinContent(x,y)>2*kdmaps_1[i]->GetBinContent(x,y)){
					kdmap_tmp->SetBinContent(x,y,kdmap_tmp->GetBinContent(x,y)-kdmaps_1[i]->GetBinContent(x,y));
				}else{
					kdmap_tmp->SetBinContent(x,y,kdmaps_1[i]->GetBinContent(x,y));
				}
			}
		}
		kdmaps_final.push_back(kdmap_tmp);
	}
		
	//avoid neg
	for(int i=0; i<cates; i++){
		
		if(yields_final[i]<0.0){
			yields_final[i]=0.0000001;
		}
		
		int histo_Nx = histos_final[i]->GetNbinsX();
		for(int j=1; j<histo_Nx+1; j++){
			if(histos_final[i]->GetBinContent(j)<0.0){
				histos_final[i]->SetBinContent(j,0.0000001);
			}
		}
		histos_final[i]->Smooth();
		
		int kdmap_Nx = kdmaps_final[i]->GetNbinsX();
		int kdmap_Ny = kdmaps_final[i]->GetNbinsY();
		for(int j=1; j<kdmap_Nx+1; j++){
			for(int k=1; k<kdmap_Ny+1; k++){
				if(kdmaps_final[i]->GetBinContent(j,k)<0.0){
					kdmaps_final[i]->SetBinContent(j,k,0.0000001);
				}
			}
		}
		kdmaps_final[i]->Smooth();
	}
	
	//slice norm for kd_final
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		for(int x=1; x<kdmaps_final[i]->GetNbinsX()+1; x++){
			tmpProj = (TH1F*)kdmaps_final.at(i)->ProjectionY("Projec",x,x);
			norm = tmpProj->Integral("width");
			if(norm<=0.0)continue;
			for(int y=1; y<kdmaps_final[i]->GetNbinsY()+1; y++){
				kdmaps_final.at(i)->SetBinContent(x,y,kdmaps_final.at(i)->GetBinContent(x,y)/norm);
			}
		}
	}
	
	//make a flat slice normalized kdmap
	TH2F* kd_flat = new TH2F("kdmap_flat","",35,105,140,20,0,1.0);
	for(int x=1; x<kd_flat->GetNbinsX()+1; x++){
		for(int y=1; y<kd_flat->GetNbinsY()+1; y++){
			kd_flat->SetBinContent(x,y,1.0);
		}
	}
	kd_flat->SetStats(0);
	for(int x=1; x<kd_flat->GetNbinsX()+1; x++){
		tmpProj = (TH1F*)kd_flat->ProjectionY("Projec",x,x);
		norm = tmpProj->Integral("width");
		for(int y=1; y<kd_flat->GetNbinsY()+1; y++){
			kd_flat->SetBinContent(x,y,kd_flat->GetBinContent(x,y)/norm);
		}
	}
	
	//make outputs
	//for norm	
	TString plotpath_norm_tmp;
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		plotpath_norm_tmp = plotpath_norm + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_bkgnorm_zjets.py";
		ofstream in;
		in.open(plotpath_norm_tmp);
		in<<"info = {"<<endl;
		in<<"'bkgrate_zjets_" + year + "_"+ fs_name + "_" + i_s + "_" + tag + "':" + to_string(yields_final[i]) + ","<<endl;
		in<<"}"<<endl;
		in.close();
	}
	//for param
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		RooRealVar mass4l("mass4l","",70,570);
		RooRealVar m("m","",130,70,170);
		RooRealVar s("s","",18,8,28);
		RooLandau landau("landau","",mass4l,m,s);
		RooDataHist* dataset = 	new RooDataHist("datahist_"+i_s,"",mass4l,Import(*histos_final.at(i)));
		landau.fitTo(*dataset,SumW2Error(kTRUE),PrintLevel(-1));
		landau.fitTo(*dataset,SumW2Error(kTRUE),PrintLevel(-1));
		TString m_s = to_string(m.getVal());
		TString s_s = to_string(s.getVal());
		TString merr_s = to_string(m.getError());
		TString serr_s = to_string(s.getError());
		RooPlot* frame = mass4l.frame(Bins(100));
		frame->SetTitle("");
		dataset->plotOn(frame);
		landau.plotOn(frame);
		TCanvas c("c","",1400,1000);
		c.cd();
		frame->Draw();
		TLatex* latex = new TLatex();
		latex->SetNDC();
		latex->SetTextSize(0.05);
		latex->SetTextFont(42);
		latex->SetTextAlign(23);
		latex->DrawLatex(0.5,0.9,"mean="+m_s+"+/-"+merr_s);
		latex->DrawLatex(0.5,0.8,"sigma="+s_s+"+/-"+serr_s);
		TString plotpath_param_tmp = plotpath_param + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_bkgshape_zx.png";
		c.SaveAs(plotpath_param_tmp);
		plotpath_param_tmp = plotpath_param + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_bkgshape_zx.py";
		ofstream in;
		in.open(plotpath_param_tmp);
		in<<"info = {"<<endl;
		in<<"'zx_mean_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"' : '"+m_s+"',"<<endl;
		in<<"'zx_sigma_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"' : '"+s_s+"',"<<endl;
		in<<"}"<<endl;
		in.close();
		delete dataset;
		delete frame;
		delete latex;
	}

	//for kdmap
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		kdmaps_final.at(i)->SetStats(0);
		double integral = kdmaps_final[i]->Integral("width");
		TString integral_s = to_string(integral);
		TCanvas c("c","",1400,1000);
		c.cd();
		TString plotpath_kdmap_tmp = plotpath_kdmap + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_kdmap_zx.png";
		kdmaps_final.at(i)->Draw("surf");
		TLatex* latex = new TLatex();
		latex->SetNDC();
		latex->SetTextSize(0.05);
		latex->SetTextFont(42);
		latex->SetTextAlign(23);
		latex->DrawLatex(0.6,0.8,"Integral="+integral_s);
		c.SaveAs(plotpath_kdmap_tmp);
		if(i==0){
			c.Clear();
			double integral_flat = kd_flat->Integral("width");
			TString integral_flat_s = to_string(integral_flat);
			kd_flat->Draw("surf");
			latex->DrawLatex(0.6,0.8,"Integral="+integral_flat_s);
			c.SaveAs(plotpath_kdmap + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_kdmap_zx_flat.png");
		}
		TFile* f = new TFile(plotpath_kdmap + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_kdmap_zx.root","RECREATE");
		f->cd();
		kdmaps_final.at(i)->Write("zx_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"_kd");
		kdmaps_final.at(i)->Write("zx_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"_kdup");
		kdmaps_final.at(i)->Write("zx_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"_kddn");
		kd_flat->Write("zx_"+year+"_"+fs_name+"_"+i_s+"_"+tag+"_flat");
		f->Close();
		delete latex;
	}
}
