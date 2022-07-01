using namespace RooFit;
TString year="2017";
TString lumi_ = "";
TString fs = "";
void makedataset(TString year, vector<RooDataSet*> &h_data);
void dofit(vector<RooDataSet*> &h_data, vector<TString> &names);

void RochUnc(TString year_, TString fs_, TString plotpath_){ 
	
	year = year_;
	fs = fs_;
	TString plotpath = plotpath_;
	vector<RooDataSet*> h_data;
	vector<TString> names;
	if(year == "2016pre"){
		lumi_ = "19.5";
	}
	if(year == "2016post"){
		lumi_ = "16.8";
	}
	if(year == "2017"){
		lumi_ = "41.5";
	}
	if(year == "2018"){
		lumi_ = "59.8";
	}

	RooRealVar mass4l("mass4l","",105,140);
	
	for(int i=0; i<104; i++){
		TString i_s = to_string(i);
		RooDataSet* h = new RooDataSet("h_"+i_s,"",mass4l);
		h_data.push_back(h);
		if(i>=100)continue;
		TString name = plotpath + "Stat_"+i_s;
		names.push_back(name);			//0-99
	}
	names.push_back(plotpath + "EWK");		//100
	names.push_back(plotpath + "ZPT");		//101
	names.push_back(plotpath + "DM");		//102
	names.push_back(plotpath + "DEF");		//103
	names.push_back(plotpath + "RocorrUnc");	//104

	makedataset(year, h_data);//alpha1 alpha2 n1 n2 sigma
	dofit(h_data, names);
}

void makedataset(TString year, vector<RooDataSet*> &h_data){
		
	bool passedFullSelection;
	int finalState;
	double mass4mu_variations[105];
	float pt1, pt2, pt3, pt4;//empty variables
	float eta1, eta2, eta3, eta4;
	float phi1, phi2, phi3, phi4;
	float m1, m2, m3, m4;

	TFile* f=new TFile();
	f = new TFile("/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/elescaleptdependent_lut_withetcut_0-30firstbin-fit_+allfs/crab_GGF125_20UL"+year+".root");
	TTree *t=(TTree*)f->Get("passedEvents");
	int sum=t->GetEntries();
	
	t->SetBranchAddress("finalState",&finalState);
	t->SetBranchAddress("passedFullSelection",&passedFullSelection);
	
	//t->SetBranchAddress("pTL1",&pt1);
	//t->SetBranchAddress("pTL2",&pt2);
	//t->SetBranchAddress("pTL3",&pt3);
	//t->SetBranchAddress("pTL4",&pt4);
		
	t->SetBranchAddress("etaL1",&eta1);
	t->SetBranchAddress("etaL2",&eta2);
	t->SetBranchAddress("etaL3",&eta3);
	t->SetBranchAddress("etaL4",&eta4);

	//t->SetBranchAddress("phiL1",&phi1);
	//t->SetBranchAddress("phiL2",&phi2);
	//t->SetBranchAddress("phiL3",&phi3);
	//t->SetBranchAddress("phiL4",&phi4);
	
	//t->SetBranchAddress("mL1",&m1);
	//t->SetBranchAddress("mL2",&m2);
	//t->SetBranchAddress("mL3",&m3);
	//t->SetBranchAddress("mL4",&m4);
	
	t->SetBranchAddress("mass4mu_variations",&mass4mu_variations);
	
	int fs__;
	if(fs=="4mu"){
		fs__=1;
	}
	if(fs=="2e2mu"){
		fs__=3;
	}
	if(fs=="2mu2e"){
		fs__=4;
	}
	RooRealVar mass4l("mass4l","",105,145);
	for(Int_t i=0; i<t->GetEntries(); i++){
	//	if(i>1000)break;
		t->GetEntry(i);
		if(i%10000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		if(passedFullSelection==1&&finalState==fs__&&abs(eta1)<2.4&&abs(eta2)<2.4&&abs(eta3)<2.4&&abs(eta4)<2.4){
			
			for(int s=0; s<100; s++){//for 100 stat.
				mass4l.setVal(mass4mu_variations[s+5]);
				if(mass4mu_variations[s+5]<140&&mass4mu_variations[s+5]>105)h_data.at(s)->add(mass4l);				//0-99 stat
			}

			mass4l.setVal(mass4mu_variations[2]);
			if(mass4mu_variations[2]<140&&mass4mu_variations[2]>105)h_data.at(100)->add(mass4l);					//100 ewk

			mass4l.setVal(mass4mu_variations[1]);
			if(mass4mu_variations[1]<140&&mass4mu_variations[1]>105)h_data.at(101)->add(mass4l);					//101 zpt

			mass4l.setVal(mass4mu_variations[4]);
			if(mass4mu_variations[4]<140&&mass4mu_variations[4]>105)h_data.at(102)->add(mass4l);					//102 deltaM
	
			mass4l.setVal(mass4mu_variations[0]);
			if(mass4mu_variations[0]<140&&mass4mu_variations[0]>105)h_data.at(103)->add(mass4l);					//103 ewk2 as defualt
		
	//		mass4l.setVal(mass4mu_variations[3]);
	//		if(mass4mu_variations[3]<140&&mass4mu_variations[3]>105)h_data.at(104)->add(mass4l);					//104 ewk2
		
		}//event selection
	}//event loop
}//end function

void dofit(vector<RooDataSet*> &h_data, vector<TString> &names){//dataset, 0-99:stat. 100:ewk, 101:zpt, 102:dm, 103:def
	 
	RooRealVar mass4l("mass4l","mass4l",105,140);
	RooRealVar mean("mean","mean",125,120,130);
	RooRealVar sigma("sigma","sigma",1,0,10);
	RooRealVar alpha1("alpha1","alpha1",1,0,10);
	RooRealVar n1("n1","n1",1,0,10);
        RooRealVar alpha2("alpha2","alpha2",1,0,10);
	RooRealVar n2("n2","n2",1,0,15);
	RooDoubleCB DCB("DCB","DCB",mass4l,mean,sigma,alpha1,n1,alpha2,n2);
	
	double *means = new double[104];//100 stat. + ewk + zpt + dm + def.
	double *sigmas = new double[104];
	for(int s=0; s<104; ++s){
		TString i_s = to_string(s);
		DCB.fitTo(*h_data.at(s),PrintLevel(-1),SumW2Error(kTRUE));
		DCB.fitTo(*h_data.at(s),PrintLevel(-1),SumW2Error(kTRUE));
		means[s] = mean.getVal();													//0-99 stat, 100 ewk, 101 zpt, 102 dm, 103 def 
		sigmas[s] = sigma.getVal();

		//make plot
		RooPlot* frame = mass4l.frame(Bins(100));
		frame->SetTitle("");
		h_data.at(s)->plotOn(frame);
		DCB.plotOn(frame);
		
		double chi2 = frame->chiSquare(6);
		TString chi2_s, mean_s, sigma_s, alpha1_s, n1_s, alpha2_s, n2_s;
		chi2_s = to_string(chi2);
		mean_s = to_string(mean.getVal());
		sigma_s = to_string(sigma.getVal());
		alpha1_s = to_string(alpha1.getVal());
		n1_s = to_string(n1.getVal());
		alpha2_s = to_string(alpha2.getVal());
		n2_s = to_string(n2.getVal());

		TLatex* latex = new TLatex();
		latex->SetNDC();
		latex->SetTextSize(0.05);
		latex->SetTextFont(42);
		latex->SetTextAlign(23);
	
		TCanvas c("c","c",1400,1000);
		c.cd();
		frame->Draw();
		latex->DrawLatex(0.7, 0.8, "chi2/n="+chi2_s);
		latex->DrawLatex(0.7,0.7, "mean="+mean_s);
		latex->DrawLatex(0.7,0.6, "sigma="+sigma_s);
		latex->DrawLatex(0.7,0.5, "alpha1="+alpha1_s);
		latex->DrawLatex(0.7,0.4, "n1="+n1_s);
		latex->DrawLatex(0.7,0.3, "alpha2="+alpha2_s);
		latex->DrawLatex(0.7,0.2, "n2="+n2_s);
		c.SaveAs(names.at(s) + ".png");
		delete frame; //delete dataset;
	}//loop over rochster variations
	
	//summary plot
	double* x_ = new double[104];
	double default_ = means[103];
	double default__ = sigmas[103];
	for(int s=0; s<104; s++){
		if(s<100)x_[s] = s+1;//0-99 100 stat., the last point x position =100
		if(s>=100)x_[s] = 100 + 10*(s-99);
		means[s] = (means[s] - default_)/default_;
		sigmas[s] = (sigmas[s] - default__)/default__;
	}

	//print final result
	TCanvas c("c","",1400,1000);
	TH1D* std_dev = new TH1D("std_dev","",200,-0.00005,0.00005);
	TH1D* sigma_std_dev = new TH1D("sigma_std_dev","",200,-0.00005,0.00005);
	for(int n=0; n<100; n++){
		std_dev->Fill(means[n]);
		sigma_std_dev->Fill(sigmas[n]);
	}
	c.cd();
	std_dev->Draw();
	//c.SaveAs(names.back() + ".png");
	
	TString roch_s = to_string(sqrt(std_dev->GetStdDev()*std_dev->GetStdDev()+means[100]*means[100]+means[101]*means[101]+means[102]*means[102]+means[103]*means[103])*100).substr(0,5)+"%";
	TString sigma_roch_s = to_string(sqrt(sigma_std_dev->GetStdDev()*sigma_std_dev->GetStdDev()+sigmas[100]*sigmas[100]+sigmas[101]*sigmas[101]+sigmas[102]*sigmas[102]+sigmas[103]*sigmas[103])*100).substr(0,5)+"%";
	
	for(int k =0; k<2; k++){		
		c.Clear();c.cd();	
		gStyle->SetTitleOffset(2,"Y");
		c.SetLeftMargin(0.14);
		c.SetFillColor(kGray);
		TGraph* g = new TGraph(105);
		g->SetTitle("");
		g->SetMarkerStyle(1);
		TExec* ex = new TExec("ex","DrawCol();");
		g->GetListOfFunctions()->Add(ex);

		double x, y;
		for(int i=0; i<104; i++){
			x = x_[i];
			if(k==0){
				y = means[i];
			}
			else{
				y = sigmas[i];
			}
			g->SetPoint(i,x,y);
		}
		if(k==0){
			g->GetHistogram()->SetMaximum(0.0002);
			g->GetHistogram()->SetMinimum(-0.0002);
		}else{
			g->GetHistogram()->SetMaximum(0.05);
			g->GetHistogram()->SetMinimum(-0.05);
		}
		g->GetXaxis()->SetLimits(0,200);
		g->GetHistogram()->GetXaxis()->SetLabelSize(0);
		g->GetHistogram()->GetXaxis()->SetTitle("systematics");
		if(k==0){
			g->GetHistogram()->GetYaxis()->SetTitle("m_{4l}^{variations}-m_{4l}^{default}/m_{4l}^{default}");
		}
		if(k==1){
			g->GetHistogram()->GetYaxis()->SetTitle("#sigma_{4l}^{variations}-#sigma_{4l}^{default}/#sigma_{4l}^{default}");
		}
		g->SetMarkerSize(2);
		g->SetTitle("");
		g->Draw("ap");
		TLine* l = new TLine(0,0,160,0);
		l->SetLineStyle(kDashed);
		l->Draw("same");	
		TLatex* latex = new TLatex();
		latex->SetNDC();
		latex->SetTextSize(0.04);
		latex->SetTextFont(42);
		latex->SetTextAlign(23);
		latex->SetTextColor(3);
		latex->DrawLatex(0.85,0.85,"Stat.");
		latex->SetTextColor(0);
		latex->DrawLatex(0.85,0.8,"EWK");
		latex->SetTextColor(4);
		latex->DrawLatex(0.85,0.75,"Zpt");
		latex->SetTextColor(5);
		latex->DrawLatex(0.85,0.7,"deltaM");
		latex->SetTextColor(1);
		latex->DrawLatex(0.85,0.65,"Default");
		latex->SetTextColor(2);
	//	latex->DrawLatex(0.9,0.7,"EWK2");
		latex->SetTextColor(1);
		if(k==0){
			latex->DrawLatex(0.3,0.95,"#font[22]{CMS Run2 Preliminary}");
			latex->DrawLatex(0.8,0.95,"#font[22]{"+lumi_+" fb^{-1} (13TeV)}");
			latex->DrawLatex(0.4,0.3,"scale uncertainty:"+roch_s);
			c.SaveAs(names.at(104) + "scale_unc.png");
			c.SaveAs(names.at(104) + "scale_unc.pdf");
		}
		if(k==1){
			latex->DrawLatex(0.4,0.3,"resolution uncertainty:"+sigma_roch_s);
			latex->DrawLatex(0.3,0.95,"#font[22]{CMS Run2 Preliminary}");
			latex->DrawLatex(0.8,0.95,"#font[22]{"+lumi_+" fb^{-1} (13TeV)}");
			c.SaveAs(names.at(104) + "resol_unc.png");
			c.SaveAs(names.at(104) + "resol_unc.pdf");
		}
	}


}//end function
	
void DrawCol(){
	int i, n;
	double x, y;
	TGraph* g = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph");
	n = g->GetN();
	TMarker* m;
	for(int i=0; i<104; i++){
		g->GetPoint(i,x,y);
		m = new TMarker(x,y,20);
		if(i<100)m->SetMarkerColor(3);
		if(i==100)m->SetMarkerColor(0);
		if(i==101)m->SetMarkerColor(4);
		if(i==102)m->SetMarkerColor(5);
		if(i==103)m->SetMarkerColor(1);
		if(i==104)m->SetMarkerColor(2);
	//	if(i==105)m->SetMarkerColor(4);
		m->Paint();
	}
}
	
	
	
	
	
	
	
	
	
	
	
	
	
