float et1, et2, massZ, pt1, pt2, eta1, eta2, phi1, phi2, m1, m2;
float sip1, sip2, RelIso1, RelIso2, weight, zpt, zy;
int ecal1, ecal2;
void test_(bool isData, TH1D* h_pt, TH1D* h_y, TString year);



void Check_ReWeight(TString year, TString plotpath){
	
	
	TH1D* h_data_pt = new TH1D("h_data_pt","",500,0,500);
	TH1D* h_data_y = new TH1D("h_data_y","",100,-4,4);
	TH1D* h_mc_pt = new TH1D("h_mc_pt","",500,0,500);
	TH1D* h_mc_y = new TH1D("h_mc_y","",100,-4,4);
	
	

	test_(1, h_data_pt, h_data_y, year);
	test_(0, h_mc_pt, h_mc_y, year);
	
	h_data_pt->SetMarkerStyle(34);
	h_data_pt->SetMarkerColor(kBlue);
	h_mc_pt->SetFillColor(kGreen);
	h_mc_pt->SetLineColor(kGreen);
	double mc_max = h_mc_pt->GetMaximum();
	double data_max = h_data_pt->GetMaximum();
	h_mc_pt->SetMaximum((mc_max > data_max ? mc_max : data_max)*1.1);
	TCanvas c("c","",1400,1000);
	c.cd();
	h_mc_pt->Draw("hist");
	h_data_pt->Draw("same");
	c.SaveAs(plotpath+"_Z_Pt_after.png");
	TFile* f = new TFile(plotpath + "_Z_Pt_after.root","RECREATE");
	f->cd();
	h_mc_pt->Write();
	f->Close();

	h_data_y->SetMarkerStyle(34);
	h_data_y->SetMarkerColor(kBlue);
	h_mc_y->SetFillColor(kGreen);
	h_mc_y->SetLineColor(kGreen);
	mc_max = h_mc_y->GetMaximum();
	data_max = h_data_y->GetMaximum();
	h_mc_y->SetMaximum((mc_max > data_max ? mc_max : data_max)*1.1);
	c.Clear();
	c.cd();
	h_mc_y->Draw("hist");
	h_data_y->Draw("same");
	c.SaveAs(plotpath+"_Z_Eta_after.png");




}
void test_(bool isData, TH1D* h_pt, TH1D* h_y, TString year){
	
	TFile* f = new TFile();
	if(isData==0&&year=="2017")f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_amcatnlo_20UL17_2e_slimmed.root","READ");
	if(isData==1&&year=="2017")f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_SingleElectron_Run2017_UL2017_2e_slimmed.root","READ");
	TTree* t = (TTree*)f->Get("passedEvents");
	t->SetBranchStatus("*",0);
	t->SetBranchStatus("mass2e_corr",1);
	t->SetBranchStatus("weight",1);
	t->SetBranchStatus("ele1_ptpostcorr",1);
	t->SetBranchStatus("ele2_ptpostcorr",1);
	t->SetBranchStatus("eta1",1);
	t->SetBranchStatus("eta2",1);
	t->SetBranchStatus("phi1",1);
	t->SetBranchStatus("phi2",1);
	t->SetBranchStatus("RelIso1",1);
	t->SetBranchStatus("RelIso2",1);
	t->SetBranchStatus("Sip1",1);
	t->SetBranchStatus("Sip2",1);
	t->SetBranchStatus("ele_et1",1);
	t->SetBranchStatus("ele_et2",1);
	t->SetBranchStatus("lep1_ecalDriven",1);
	t->SetBranchStatus("lep2_ecalDriven",1);
	t->SetBranchStatus("Zpt",1);
	t->SetBranchStatus("Zy",1);

	t->SetBranchAddress("mass2e_corr",&massZ);	
	t->SetBranchAddress("weight",&weight);
	t->SetBranchAddress("ele1_ptpostcorr",&pt1);
	t->SetBranchAddress("ele2_ptpostcorr",&pt2);
	t->SetBranchAddress("eta1",&eta1);
	t->SetBranchAddress("eta2",&eta2);
	t->SetBranchAddress("phi1",&phi1);
	t->SetBranchAddress("phi2",&phi2);
	t->SetBranchAddress("RelIso1",&RelIso1);
	t->SetBranchAddress("RelIso2",&RelIso2);
	t->SetBranchAddress("Sip1",&sip1);
	t->SetBranchAddress("Sip2",&sip2);
	t->SetBranchAddress("ele_et1",&et1);
	t->SetBranchAddress("ele_et2",&et2);
	t->SetBranchAddress("lep1_ecalDriven",&ecal1);
	t->SetBranchAddress("lep2_ecalDriven",&ecal2);
	t->SetBranchAddress("Zpt",&zpt);
	t->SetBranchAddress("Zy",&zy);

	TFile* f_lut = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Scale/Electron/cpp/reweight/lut/2017.root","READ");
	TH2D* lut = (TH2D*)f_lut->Get("LUT");

	double zptyw;
	int xbin, ybin;
	for(int i=0; i<t->GetEntries(); i++){
		if(i%10000000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
		if(ecal1!=1||ecal2!=1)continue;
		if(massZ<100&&massZ>80&&et1>32&&et2>20&&pt1<5000&&pt2<5000&&pt1>7&&pt2>7&&abs(eta1)<2.5&&abs(eta2)<2.5&&RelIso1<0.35&&RelIso2<0.35&&sip1<4&&sip2<4){
			if(zpt>500||abs(zy)>2.4){
				zptyw=1.0;
			}else{
				xbin = lut->GetXaxis()->FindBin(zpt);
				ybin = lut->GetYaxis()->FindBin(zy);
				zptyw = lut->GetBinContent(xbin,ybin);
			}
			//re-calculate y for Z
			if(isData==0){
				h_pt->Fill(zpt,zptyw*weight);
				h_y->Fill(zy,zptyw*weight);
			}
			else{
				h_pt->Fill(zpt,weight);
				h_y->Fill(zy,weight);
			}

		}
	}

	h_pt->Scale(1/h_pt->Integral());
	h_y->Scale(1/h_y->Integral());
	h_pt->SetStats(0);
	h_y->SetStats(0);
}
