void MakePlotFindmass4lErrCategorization(vector<TH1D*>& hists, TString plotpath, int fs, vector<float>& bounds, TString year, TString type, TString tag);
void MakeDCB4LFitPlot(RooFitResult* r, double mass_point, RooDataSet* h, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag, TString year, TString process_name);
void MakeVH4LFitPlot(RooFitResult* r, RooDataSet* h, TString plotpath, TString steptwodir, TString cate_name, TString fs_name, TString type, TString process, TString tag, TString year);
void MakeZZBKGFitPlot(double* params, RooDataHist* h, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag, TString year, TString process_name);
void MakeBKGEventYieldPlot(double yield, TString plotpath, TString cate_name, TString fs_name, TString type, TString process, TString tag, TString year);
void MakeApproxFitPlot(RooFitResult* r_approx, RooFitResult* r_125, double mass_point, RooDataSet* h, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag, TString year, TString process_name);
void MassDistributionPlot( TString process, TString fs, TString year, TString masspoint, TString type, TString tag, vector<TH1F*> &mass_distributions, TString plotpath );
void ThesisPlot( TCanvas* c );
void MassDistribution2DPlot( TString process, TString fs, TString year, TString masspoint, TString type, TString tag, vector<TH2F*> &mass_distributions, TString plotpath);

void ThesisPlot( TCanvas* cc){
	cc->cd();
	TLatex* latex = new TLatex();
        latex->SetNDC();
        latex->SetTextSize(0.05);
        latex->SetTextFont(42);
        latex->SetTextAlign(23);
	latex->DrawLatex(0.2,0.9, "#scale[1.1]{#bf{CMS}} #scale[0.8]{#it{Working in progress}}");
	delete latex;	
	
}
void MakeZZBKGFitPlot(double* params, RooDataHist* h, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag, TString year, TString process_name){

        RooRealVar* mass4l = new RooRealVar("mass4l","",105,140);
        RooRealVar* c1 = new RooRealVar("c1","",params[0]);
        RooRealVar* c2 = new RooRealVar("c2","",params[1]);
        RooRealVar* c3 = new RooRealVar("c3","",params[2]);
        RooBernstein* model = new RooBernstein("model","",*mass4l,RooArgList(*c1,*c2,*c3));
        RooPlot* frame = mass4l->frame(Bins(100));
        frame->SetTitle("");
        h->plotOn(frame);
        model->plotOn(frame);
        TCanvas c("c","",1400,1000);
        frame->Draw();
        c.cd();
        c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_bkgshape_"+process_name+".png");

        TString c1_in, c2_in, c3_in;
        c1_in = to_string(c1->getVal());
        c2_in = to_string(c2->getVal());
        c3_in = to_string(c3->getVal());
        c1_in="'"+process_name+"_chepol1_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+c1_in+"',";
        c2_in="'"+process_name+"_chepol2_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+c2_in+"',";
        c3_in="'"+process_name+"_chepol3_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+c3_in+"',";
        ofstream in;
        in.open(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_bkgshape_"+process_name+".py");
	in<<"info = {"<<endl;
	in<<c1_in<<endl;
        in<<c2_in<<endl;
        in<<c3_in<<endl;
	in<<"}"<<endl;
        in.close();
}

void MakeVH4LFitPlot(RooFitResult* r, RooDataSet* h, TString plotpath, TString steptwodir, TString cate_name, TString fs_name, TString type, TString process, TString tag, TString year){
	

        //retrieve ggh shape
        TFile* f  = new TFile(steptwodir+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_125_sigshape_ggh.root","READ");
        TH1D* h_ = (TH1D*)f->Get("params");
        double mean_ = h_->GetBinContent(1);
        double sigma_ = h_->GetBinContent(2);
        double alpha1_ = h_->GetBinContent(3);
        double n1_ = h_->GetBinContent(4);
        double alpha2_ = h_->GetBinContent(5);
        double n2_ = h_->GetBinContent(6);

        RooRealVar* mass4l = new RooRealVar("mass4l","",105,140);
        RooRealVar* mean = new RooRealVar("mean","",mean_);
        RooRealVar* sigma = new RooRealVar("sigma","",sigma_);
        RooRealVar* alpha1 = new RooRealVar("alpha1","",alpha1_);
        RooRealVar* n1 = new RooRealVar("n1","",n1_);
        RooRealVar* alpha2 = new RooRealVar("alpha2","",alpha2_);
        RooRealVar* n2 = new RooRealVar("n2","",n2_);
        RooDoubleCB* DCB = new RooDoubleCB("DCB", "", *mass4l, *mean, *sigma, *alpha1, *n1, *alpha2, *n2);


        RooRealVar* p0 = (RooRealVar*)r->floatParsFinal().find("p0");
        RooRealVar* p1 = (RooRealVar*)r->floatParsFinal().find("p1");
        RooLandau* landau = new RooLandau("landau","",*mass4l,*p0,*p1);
        RooRealVar* frac = (RooRealVar*)r->floatParsFinal().find("frac");
        RooAddPdf* PDF = new RooAddPdf("PDF","",*DCB,*landau,*frac);

        TString mean_s = to_string(mean->getVal());
        TString sigma_s = to_string(sigma->getVal());
        TString alpha1_s = to_string(alpha1->getVal());
        TString n1_s = to_string(n1->getVal());
        TString alpha2_s = to_string(alpha2->getVal());
        TString n2_s = to_string(n2->getVal());
        TString p0_s = to_string(p0->getVal());
        TString p1_s = to_string(p1->getVal());
        TString frac_s = to_string(frac->getVal());

        TLatex* latex = new TLatex();
        latex->SetNDC();
        latex->SetTextSize(0.05);
        latex->SetTextFont(42);
        latex->SetTextAlign(23);

        RooPlot* frame = mass4l->frame(Bins(100));
        frame->SetTitle("");
        h->plotOn(frame);
        PDF->plotOn(frame);
        TCanvas c("c","",1400,1000);
        c.cd();
        frame->Draw();
        latex->DrawLatex(0.7,0.9,"mean="+mean_s);
        latex->DrawLatex(0.7,0.8,"sigma="+sigma_s);
        latex->DrawLatex(0.7,0.7,"alpha1="+alpha1_s);
        latex->DrawLatex(0.7,0.6,"n1="+n1_s);
        latex->DrawLatex(0.7,0.5,"alpha2="+alpha2_s);
        latex->DrawLatex(0.7,0.4,"n2="+n2_s);
        latex->DrawLatex(0.7,0.3,"p0="+p0_s);
        latex->DrawLatex(0.7,0.2,"p1="+p1_s);
        latex->DrawLatex(0.7,1,"frac="+frac_s);
        c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_sigshape_"+process+".png");

        TString p0_in, p1_in, frac_in;
        p0_in = "'" + process + "p0_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+p0_s+"',";
        p1_in = "'" + process + "p1_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+p1_s+"',";
        frac_in = "'" + process + "frac_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+frac_s+"',";
        ofstream in;
        in.open(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_sigshape_"+process+".py");
	in<<"info = {"<<endl;
	in<<p0_in<<endl;
        in<<p1_in<<endl;
        in<<frac_in<<endl;
	in<<"}"<<endl;
        in.close();
}

void MakePlotFindmass4lErrCategorization(vector<TH1D*>& hists, TString plotpath, int fs, vector<float>& bounds, TString year, TString type, TString tag){//oly for mass4lErr dounds finding & it is not very general

	TString fs_name;
	if(fs==1)fs_name="4mu";
	if(fs==2)fs_name="4e";
	if(fs==3)fs_name="2e2mu";
	if(fs==4)fs_name="2mu2e";
	
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextSize(0.05);
	latex->SetTextFont(42);
	latex->SetTextAlign(23);

	TCanvas c("c","",1400,1000);
	c.cd();
	for(int i=0; i<hists.size(); i++){
		hists.at(i)->Draw("hist");
		TString cate_name = to_string(i);
		TString low_s = to_string(bounds.at(i));
		TString high_s = to_string(bounds.at(i+1));
		latex->DrawLatex(0.7,0.5,"low="+low_s);
		latex->DrawLatex(0.7,0.6,"high="+high_s);
		c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_dm_ggh.png");
		c.Clear();
		c.cd();
		TFile* f = new TFile(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_dm_ggh.root","RECREATE");
		f->cd();
		hists.at(i)->Write();
		f->Close();
	}
	float* axis_bounds = new float[hists.size()+1];
	for(int i=0; i<=hists.size(); i++){
		axis_bounds[i] = bounds.at(i);
	}
	TH1F* h_bounds = new TH1F("bounds","",hists.size(),axis_bounds);
	for(int i=0; i<hists.size(); i++){
		h_bounds->SetBinContent(i+1,i+1);
	}
	h_bounds->Draw("histtext");
	c.SaveAs(plotpath+"_"+fs_name+"_bounds.png");
	TFile* f = new TFile(plotpath+"_"+fs_name+"_bounds.root","RECREATE");
	f->cd();
	h_bounds->Write();
	f->Close();

	delete h_bounds;
}

void MakeApproxFitPlot(RooFitResult* r_approx, RooFitResult* r_125, double mass_point, RooDataSet* h, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag, TString year, TString process_name){
	RooRealVar* mass4l = new RooRealVar("mass4l","",105,140);
	double _shift = ((RooRealVar*)r_125->floatParsFinal().find("mean"))->getVal()-mass_point;
	RooRealVar* shift = new RooRealVar("shift","",_shift);
	RooRealVar* sigma = (RooRealVar*)r_125->floatParsFinal().find("sigma");
	RooRealVar* alpha1 = (RooRealVar*)r_125->floatParsFinal().find("alpha1");
	RooRealVar* alpha2 = (RooRealVar*)r_125->floatParsFinal().find("alpha2");
	double _n1 = ((RooRealVar*)r_125->floatParsFinal().find("n1"))->getVal();
	double _n2 = ((RooRealVar*)r_125->floatParsFinal().find("n2"))->getVal();
	int n1 = int(round(_n1));
	int n2 = int(round(_n2));
	RooRealVar* theta1 = (RooRealVar*)r_approx->floatParsFinal().find("theta1");
	RooRealVar* theta2 = (RooRealVar*)r_approx->floatParsFinal().find("theta2");
	RooRealVar* mass = (RooRealVar*)r_approx->floatParsFinal().find("mass");
	RooRealVar* width = (RooRealVar*)r_approx->floatParsFinal().find("width");
	RooaDoubleCBxBW* approx = new RooaDoubleCBxBW("approx","",*mass4l,*shift,*sigma,*alpha1,*alpha2,*mass,*width,n1,n2,*theta1,*theta2,false);
	RooPlot* frame = mass4l->frame(Bins(100));
	h->plotOn(frame);
	approx->plotOn(frame);
	TCanvas c("c","",1400,1000);
	c.cd();
	TString mass_name = to_string(int(mass_point));
	c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_"+mass_name+"_sigshape_"+process_name+"_approx.png");
}
		
void MakeDCB4LFitPlot(RooFitResult* r, double mass_point, RooDataSet* h, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag, TString year, TString process_name){

        RooRealVar* mass4l = new RooRealVar("mass4l", "", 105, 140);
	RooRealVar* mean = (RooRealVar*)r->floatParsFinal().find("mean");
	RooRealVar* sigma = (RooRealVar*)r->floatParsFinal().find("sigma");
	RooRealVar* alpha1 = (RooRealVar*)r->floatParsFinal().find("alpha1");
	RooRealVar* n1 = (RooRealVar*)r->floatParsFinal().find("n1");
	RooRealVar* alpha2 = (RooRealVar*)r->floatParsFinal().find("alpha2");
	RooRealVar* n2 = (RooRealVar*)r->floatParsFinal().find("n2");
	RooDoubleCB* DCB = new RooDoubleCB("DCB", "", *mass4l, *mean, *sigma, *alpha1, *n1, *alpha2, *n2);
		
	TString mean_s = to_string(std::round(mean->getVal()*1000/1000));
	TString sigma_s = to_string(std::round(sigma->getVal()*1000/1000));
	TString alpha1_s = to_string(alpha1->getVal());
	TString n1_s = to_string(n1->getVal());
	TString alpha2_s = to_string(alpha2->getVal());
	TString n2_s = to_string(n2->getVal());
	
	TString meanErr_s = to_string(std::round(mean->getError()*1000)/1000);
	TString sigmaErr_s = to_string(std::round(sigma->getError()*1000)/1000);
	TString alpha1Err_s = to_string(alpha1->getError());
	TString n1Err_s = to_string(n1->getError());
	TString alpha2Err_s = to_string(alpha2->getError());
	TString n2Err_s = to_string(n2->getError());
	
	mean_s = Form("%.3f",mean->getVal());
	meanErr_s=Form("%.3f",mean->getError());
	sigma_s = Form("%.3f",sigma->getVal());
	sigmaErr_s=Form("%.3f",sigma->getError());

	
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextSize(0.05);
	latex->SetTextFont(42);
	latex->SetTextAlign(23);
	
	RooPlot* frame = mass4l->frame(Bins(100));
	frame->SetTitle("");
	h->plotOn(frame);
	DCB->plotOn(frame);
	TCanvas c("c","",1400,1000);
	c.cd();
	frame->Draw();
	latex->DrawLatex(0.7,0.9,"mean="+mean_s+"+/-"+meanErr_s);
	latex->DrawLatex(0.7,0.8,"sigma="+sigma_s+"+/-"+sigmaErr_s);
	//latex->DrawLatex(0.7,0.7,"alpha1="+alpha1_s+"+/-"+alpha1Err_s);
	//latex->DrawLatex(0.7,0.6,"n1="+n1_s+"+/-"+n1Err_s);
	//latex->DrawLatex(0.7,0.5,"alpha2="+alpha2_s+"+/-"+alpha2Err_s);
	//latex->DrawLatex(0.7,0.4,"n2="+n2_s+"+/-"+n2Err_s);
	TString mass_name = to_string(int(mass_point));
	c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_"+mass_name+"_sigshape_"+process_name+".png");
	
	TH1D* h_params = new TH1D("params","",6,0,6);
	h_params->SetBinContent(1,mean->getVal());
	h_params->SetBinContent(2,sigma->getVal());
	h_params->SetBinContent(3,alpha1->getVal());
	h_params->SetBinContent(4,n1->getVal());
	h_params->SetBinContent(5,alpha2->getVal());
	h_params->SetBinContent(6,n2->getVal());
	
	h_params->SetBinError(1,mean->getError());
	h_params->SetBinError(2,sigma->getError());
	h_params->SetBinError(3,alpha1->getError());
	h_params->SetBinError(4,n1->getError());
	h_params->SetBinError(5,alpha2->getError());
	h_params->SetBinError(6,n2->getError());
	
	TFile* f = new TFile(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_"+mass_name+"_sigshape_"+process_name+".root", "RECREATE"); 	//make root file
	f->cd();
	h_params->Write();
	f->Close();
	
	TString mean_in, sigma_in, alpha1_in, n1_in, alpha2_in, n2_in;
	mean_in="'mean_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+mean_s+"',";
	sigma_in="'sigma_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+sigma_s+"',";
	alpha1_in="'alpha_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+alpha1_s+"',";
	alpha2_in="'alpha2_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+alpha2_s+"',";
	n1_in="'n_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+n1_s+"',";
	n2_in="'n2_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '"+n2_s+"',";
	std::ofstream in;
	in.open(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_"+mass_name+"_sigshape_"+process_name+".txt");
	in<<mean_in<<endl;
	in<<sigma_in<<endl;
	in<<alpha1_in<<endl;
	in<<alpha2_in<<endl;
	in<<n1_in<<endl;
	in<<n2_in<<endl;
	in.close();                                                             									//make txt file for datacards
}

void MakeBKGEventYieldPlot(double yield, TString plotpath, TString cate_name, TString fs_name, TString type, TString process, TString tag, TString year){

        TString y_s;
        y_s = to_string(yield);

        ofstream in;
        in.open(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_bkgnorm_"+process+".py");
	in<<"info = {"<<endl;
	in<<"'bkgrate_"+process+"_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : "+y_s+","<<endl;
	in<<"}"<<endl;
        in.close();
	std::cout<<fs_name<<" "<<tag<<" "<<process<<" "<<cate_name<<" "<<"file has been created"<<std::endl;

}

void MassDistributionPlot( TString process, TString fs, TString year, TString masspoint, TString type, TString tag, vector<TH1F*> &mass_distributions, TString plotpath ){
	TString sigbkg="signorm";
	if ( process == "ggzz" || process == "qqzz" || process == "zjets" ){
		sigbkg = "bkgnorm";
	}

	for ( int i=0; i<mass_distributions.size(); i++){
		TString plotname = plotpath + "/" + year + "_" + fs + "_" + tag + "_" + to_string(i) + "_"+sigbkg+"_"+process+"_"+masspoint+"_"+mass_distributions.at(i)->GetName();
		TCanvas c("c","",1400,1000);
		mass_distributions.at(i)->Draw("hist");
		c.SaveAs(plotname+".png");
		c.SaveAs(plotname+".pdf");
		TFile fout(plotname+".root","RECREATE");
		mass_distributions.at(i)->Write();
		fout.Close();
	}

}

void MassDistribution2DPlot( TString process, TString fs, TString year, TString masspoint, TString type, TString tag, vector<TH2F*> &mass_distributions, TString plotpath ){
	TString sigbkg="signorm";
	if ( process == "ggzz" || process == "qqzz" || process == "zjets" ){
		sigbkg = "bkgnorm";
	}

	for ( int i=0; i<mass_distributions.size(); i++){
		TString plotname = plotpath + "/" + year + "_" + fs + "_" + tag + "_" + to_string(i) + "_"+sigbkg+"_"+process+"_"+masspoint+"_"+mass_distributions.at(i)->GetName();
		TCanvas c("c","",1400,1000);
		mass_distributions.at(i)->Draw("colz");
		c.SaveAs(plotname+".png");
		c.SaveAs(plotname+".pdf");
		TFile fout(plotname+".root","RECREATE");
		mass_distributions.at(i)->Write();
		fout.Close();
	}

}
