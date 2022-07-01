float massZ, pt1, pt2, eta1, eta2, phi1, phi2, m1, m2;
float weight, Sip1, Sip2, RelIso1, RelIso2, zpt, zy, et1, et2;
int ecal1, ecal2;
void test_(bool isData, TH1D* h_pt, TH1D* h_y, TH2D* h_2d, TString year);

void DoZPtY(TString year, TString plotpath){

	
	TH1D* h_data_pt = new TH1D("h_data_pt","",300,0,150);
	TH1D* h_data_y = new TH1D("h_data_y","",100,-4,4);
	TH1D* h_mc_pt = new TH1D("h_mc_pt","",300,0,150);
	TH1D* h_mc_y = new TH1D("h_mc_y","",100,-4,4);
		
	TH2D* h_2d_data = new TH2D("h_2d_data","",300,0,150,100,-2.4,2.4);
	TH2D* h_2d_mc = new TH2D("h_2d_mc","",300,0,150,100,-2.4,2.4);

	test_(1, h_data_pt, h_data_y, h_2d_data, year);
	test_(0, h_mc_pt, h_mc_y, h_2d_mc, year);

	TLegend* legend1 = new TLegend(.6,.6,.8,.8);
	legend1->SetTextSize(0.08);
	legend1->SetLineWidth(0);
	legend1->SetFillColor(0);
	legend1->SetBorderSize();
	legend1->AddEntry(h_data_pt,"DATA","P");
	legend1->AddEntry(h_mc_pt,"MC","F");

	h_data_pt->SetMarkerStyle(20);
	h_data_pt->SetMarkerColor(kBlack);
	h_mc_pt->SetFillColor(16);
	h_mc_pt->SetLineColor(16);
	
	double mc_max = h_mc_pt->GetMaximum();
	double data_max = h_data_pt->GetMaximum();
	h_mc_pt->SetMaximum((mc_max > data_max ? mc_max : data_max)*1.1);
	TCanvas c("c","",1400,1000);
	c.cd();
	h_mc_pt->Draw("hist");
	h_data_pt->Draw("same");
	legend1->Draw("same");
	c.SaveAs(plotpath+"/_Z_Pt_before.png");

	TLegend* legend2 = new TLegend(.6,.6,.8,.8);
	legend2->SetTextSize(0.08);
	legend2->SetLineWidth(0);
	legend2->SetFillColor(0);
	legend2->SetBorderSize();
	legend2->AddEntry(h_data_y,"DATA","P");
	legend2->AddEntry(h_mc_y,"MC","F");

	h_data_y->SetMarkerStyle(20);
	h_data_y->SetMarkerColor(kBlack);
	h_mc_y->SetFillColor(16);
	h_mc_y->SetLineColor(16);
	mc_max = h_mc_y->GetMaximum();
	data_max = h_data_y->GetMaximum();
	h_mc_y->SetMaximum((mc_max > data_max ? mc_max : data_max)*1.1);
	c.Clear();
	c.cd();
	h_mc_y->Draw("hist");
	h_data_y->Draw("same");
	c.SaveAs(plotpath+"/_Z_Eta_before.png");

	c.Clear();
	c.cd();
	h_2d_data->Draw("COLZTXT");
	c.SaveAs(plotpath+"/_data.png");
	TFile* f_data = new TFile(plotpath+"/_data.root","RECREATE");
	f_data->cd();
	h_2d_data->Write();
	f_data->Close();

	c.Clear();
	c.cd();
	h_2d_mc->Draw("COLZTXT");
	c.SaveAs(plotpath+"/_mc.png");	
	TFile* f_mc = new TFile(plotpath+"/_mc.root","RECREATE");
	f_mc->cd();
	h_2d_mc->Write();
	f_mc->Close();

	TH2D* LUT = new TH2D("LUT","",300,0,150,100,-2.4,2.4);
	LUT->SetStats(0);
	for(int i=1; i<=300; i++){
		for(int j=1; j<=100; j++){
			if(h_2d_data->GetBinContent(i,j)!=0&&h_2d_mc->GetBinContent(i,j)!=0){
				double fill = h_2d_data->GetBinContent(i,j)/h_2d_mc->GetBinContent(i,j);
				LUT->SetBinContent(i,j,fill);
			}
			else{
				LUT->SetBinContent(i,j,1);
			}
		}
	}
	TFile* f_lut = new TFile(plotpath+"/_LUT.root","RECREATE");
	f_lut->cd();
	LUT->Write();
	f_lut->Close();
	c.Clear();
	c.cd();
	LUT->Draw("COLZ");
	c.SaveAs(plotpath+"/_LUT.png");



}
void test_(bool isData, TH1D* h_pt, TH1D* h_y, TH2D* h_2d, TString year){
	
	TFile* f = new TFile();
	//if(year=="2016pre"&&isData==0)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_2016APV_2e_slimmed.root");
	if(year=="2016pre"&&isData==0)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_LO_2016APV_2e_slimmed.root");
	if(year=="2016pre"&&isData==1)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_SingleElectron_Run2016_UL2016HIPM_2e_slimmed.root");
	
	//if(year=="2016post"&&isData==0)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_2016_2e_slimmed.root");
	if(year=="2016post"&&isData==0)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_LO_2016_2e_slimmed.root");
	if(year=="2016post"&&isData==1)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_SingleElectron_Run2016_UL2016_2e_slimmed.root");
	
	//if(year=="2018"&&isData==0)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_2018_2e_slimmed.root");
	if(year=="2018"&&isData==0)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_LO_2018_2e_slimmed.root");
	if(year=="2018"&&isData==1)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_EGamma_Run18_2e_slimmed.root");
	
	//if(year=="2017"&&isData==0)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_2017_new_2e_slimmed.root");
	if(year=="2017"&&isData==1)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_SingleElectron_Run17_2e_slimmed.root");
	if(year=="2017"&&isData==0)f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/crab_DYJetsToLL_M50_LO_2017_2e_slimmed.root");
	
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
	t->SetBranchStatus("RelIso2",2);
	t->SetBranchStatus("Sip1",1);
	t->SetBranchStatus("Sip2",1);
	t->SetBranchStatus("lep1_ecalDriven",1);
	t->SetBranchStatus("lep2_ecalDriven",1);
	t->SetBranchStatus("ele_et1",1);
	t->SetBranchStatus("ele_et2",1);
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
	t->SetBranchAddress("Sip1",&Sip1);
	t->SetBranchAddress("Sip2",&Sip2);
	t->SetBranchAddress("lep1_ecalDriven",&ecal1);
	t->SetBranchAddress("lep2_ecalDriven",&ecal2);
	t->SetBranchAddress("ele_et1",&et1);
	t->SetBranchAddress("ele_et2",&et2);
	t->SetBranchAddress("Zpt",&zpt);
	t->SetBranchAddress("Zy",&zy);
	
	for(int i=0; i<t->GetEntries(); i++){
		if(i%10000000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
		if(ecal1!=1 || ecal2!=1)continue;
		if(massZ<120&&massZ>60&&et1>32&&et2>20&&pt1<5000&&pt2<5000&&pt1>7&&pt2>7&&abs(eta1)<2.5&&abs(eta2)<2.5&&RelIso1<0.35&&RelIso2<0.35&&Sip1<4&&Sip2<4){
			h_pt->Fill(zpt,weight);
			h_y->Fill(zy,weight);
			if(zpt<150&&abs(zy)<2.4){
				h_2d->Fill(zpt,zy,weight);
			}
		}
	}
	h_pt->Scale(1/h_pt->Integral());
	h_y->Scale(1/h_y->Integral());
	h_2d->Scale(1/h_2d->Integral());
	h_pt->SetStats(0);
	h_y->SetStats(0);
	h_2d->SetStats(0);
}
