RooFitResult* DCB4LFit(RooDataSet* h1, double masspoint);
RooFitResult* VH4LFit(RooDataSet* dataset, TString steptwodir, TString sub_name, TString fs_name, TString type, TString tag, TString year);
void DCB4LSimulFit(vector<RooDataSet*>& datasets, RooFitResult* r, vector<TString>& names, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag_name, TString year);
void FitEventYield(double p1, double p1err, double p2, double p2err, double p3, double p3err, double p4, double p4err, double p5, double p5err, TString plotpath, TString cate_name, TString fs_name, TString process, TString type, TString tag, TString year);
void ZZBKGFit(RooDataHist* dataset, double* params);
RooFitResult* ApproxFit(RooDataSet* h1, double masspoint, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag_name, TString year, TString process_name);
void ApproxSimulFit(vector<RooDataSet*>& datasets, RooFitResult* r, vector<TString>& names, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag_name, TString year);

void ZZBKGFit(RooDataHist* dataset, double* params){
        RooRealVar mass4l("mass4l","",105,140);
        RooRealVar c1("c1","",0,3);
        RooRealVar c2("c2","",0,3);
        RooRealVar c3("c3","",0,3);
        RooBernstein model("model","",mass4l,RooArgList(c1,c2,c3));
        model.fitTo(*dataset,SumW2Error(kTRUE),PrintLevel(-1));
        params[0] = c1.getVal();
        params[1] = c2.getVal();
        params[2] = c3.getVal();
}

RooFitResult* VH4LFit(RooDataSet* dataset, TString steptwodir, TString cate_name, TString fs_name, TString type, TString tag, TString year){

        //retrieve ggh dcb params
        TFile* f = new TFile(steptwodir+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_125_sigshape_ggh.root","READ");
        TH1D* h = (TH1D*)f->Get("params");
        double mean_ = h->GetBinContent(1);
	double sigma_ = h->GetBinContent(2);
        double alpha1_ = h->GetBinContent(3);
        double n1_ = h->GetBinContent(4);
        double alpha2_ = h->GetBinContent(5);
        double n2_ = h->GetBinContent(6);
        RooRealVar mass4l("mass4l","",105,140);

        RooRealVar mean("mean","",mean_); //mean.setConstant(kTRUE);
        RooRealVar sigma("sigma","",sigma_); //sigma.setConstant(kTRUE);
        RooRealVar alpha1("alpha1","",alpha1_); //alpha1.setConstant(kTRUE);
        RooRealVar n1("n1","",n1_); //n1.setConstant(kTRUE);
        RooRealVar alpha2("alpha2","",alpha2_); //alpha2.setConstant(kTRUE);
        RooRealVar n2("n2","",n2_); //n2.setConstant(kTRUE);

        RooDoubleCB DCB("DCB","",mass4l,mean,sigma,alpha1,n1,alpha2,n2);

        RooRealVar p0("p0","",130,70,200);
        RooRealVar p1("p1","",10,5,50);
        RooLandau landau("landau","",mass4l,p0,p1);
        RooRealVar frac("frac","",0.7,0.5,1);
        RooAddPdf PDF("PDF","",DCB,landau,frac);
        RooFitResult* r = PDF.fitTo(*dataset,SumW2Error(kTRUE),PrintLevel(-1),Save());
        return r;
}

RooFitResult* DCB4LFit(RooDataSet* h1, double masspoint){
        RooRealVar mass4l("mass4l","",105,140);
        RooRealVar mean("mean","",masspoint,115,135);
        RooRealVar sigma("sigma","",1,0,10);
        RooRealVar alpha1("alpha1","",1,0,10);
        RooRealVar n1("n1","",1,0,10);
        RooRealVar alpha2("alpha2","",1,0,10);
        RooRealVar n2("n2","",1,0,10);
        RooDoubleCB DCB("DCB","",mass4l,mean,sigma,alpha1,n1,alpha2,n2);
	DCB.fitTo(*h1,PrintLevel(-1),SumW2Error(kTRUE));				//first fit to find init params
        RooFitResult* r = DCB.fitTo(*h1,PrintLevel(-1),SumW2Error(kTRUE),Save());	//second fit, return results
        return r;
}

RooFitResult* ApproxFit(RooDataSet* h1, double masspoint, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag_name, TString year, TString process_name){
	TString masspoint_s = to_string(int(masspoint));
	RooRealVar mass4l("mass4l","",105,140);
	RooRealVar mass("mass","",masspoint);
	RooRealVar width("width","",0.004);
	
	RooRealVar shift_("shift_","",0,-3,3);
	RooFormulaVar mean_("mean_","","@0+"+masspoint_s,RooArgList(shift_));
	RooRealVar sigma_("sigma_","",1,0,10);
	RooRealVar a1_("a1_","",1,0,10);
	RooRealVar a2_("a2_","",1,0,10);
	RooRealVar n1_("n1_","",3);
	RooRealVar n2_("n2_","",3);
	RooDoubleCB dcb_("dcb_","",mass4l,mean_,sigma_,a1_,n1_,a2_,n2_);
	dcb_.fitTo(*h1,PrintLevel(-1),SumW2Error(kTRUE));
	dcb_.fitTo(*h1,PrintLevel(-1),SumW2Error(kTRUE));
	//RooChi2Var dcb_chi2("dcb_chi2","",dcb_,*h1->binnedClone());
	//TString dcb_chi2_s = to_string(dcb_chi2.getVal()/float(h1->binnedClone()->numEntries()));
	
	RooRealVar shift("shift","",shift_.getVal(),shift_.getVal()-abs(shift_.getVal())*0.2,shift_.getVal()+abs(shift_.getVal())*0.2);
	RooRealVar sigma("sigma","",1,0,10);
	RooRealVar a1("a1","",a1_.getVal());//,a1_.getVal()*0.8,a1_.getVal()*1.2);
	RooRealVar a2("a2","",a2_.getVal());//,a2_.getVal()*0.8,a2_.getVal()*1.2);
	int n1 = 3;
	int n2 = 3;
	RooRealVar theta1("theta1","",2.3);
	RooRealVar theta2("theta2","",2.3);
	RooaDoubleCBxBW approx("approx","",mass4l,shift,sigma,a1,a2,mass,width,n1,n2,theta1,theta2,false);
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@First Approx Fit@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
	approx.fitTo(*h1,PrintLevel(3),SumW2Error(kTRUE));
	std::cout<<"									"<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@Second Approx Fit@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
	RooFitResult* r_approx = approx.fitTo(*h1,PrintLevel(3),SumW2Error(kTRUE),Save());
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"									 "<<std::endl;
	//RooChi2Var approx_chi2("approx_chi2","",approx,*h1->binnedClone());
	//TString approx_chi2_s = to_string(approx_chi2.getVal()/float(h1->binnedClone()->numEntries()));
	
	RooPlot* frame = mass4l.frame(Bins(100));
	frame->SetTitle("");
	h1->plotOn(frame);
	dcb_.paramOn(frame,Layout(0.7,0.99,0.45),Label("dcb"),ShowConstants(true));
	approx.paramOn(frame,Layout(0.7,0.99,0.99),Label("dcbxbw"),ShowConstants(true));
	approx.plotOn(frame,LineColor(4));
	dcb_.plotOn(frame,LineColor(2),LineStyle(2),LineWidth(4));
	h1->plotOn(frame);
	TCanvas c("c","",1400,1000);
	c.cd();
	frame->Draw();
	TLatex* lx = new TLatex();
	lx->SetNDC();
	lx->SetTextSize(0.05);
	lx->SetTextFont(42);
	lx->SetTextAlign(23);
	lx->SetTextColor(2);
	lx->DrawLatex(0.2,0.8,"dcb");
	lx->SetTextColor(4);
	lx->DrawLatex(0.2,0.7,"dcbxbw");
	c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_"+masspoint_s+"_sigshape_"+process_name+"_approx.png");
	return r_approx;
}

void ApproxSimulFit(vector<RooDataSet*>& datasets, RooFitResult* r, vector<TString>& names, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag_name, TString year){
	
	//retrieve params
	RooRealVar* shift_intercept = (RooRealVar*)r->floatParsFinal().find("shift");
	RooRealVar* sigma_intercept = (RooRealVar*)r->floatParsFinal().find("sigma");
	RooRealVar* a1_intercept = (RooRealVar*)r->constPars().find("a1");
	RooRealVar* a2_intercept = (RooRealVar*)r->constPars().find("a2");
	shift_intercept->setConstant(kTRUE);
	sigma_intercept->setConstant(kTRUE);
	a1_intercept->setConstant(kTRUE);
	a2_intercept->setConstant(kTRUE);
	//define slope
	RooRealVar* shift_slope = new RooRealVar("shift_slope","",0.1,-1,1);
	RooRealVar* sigma_slope = new RooRealVar("sigma_slope","",0.1,-1,1);
	RooRealVar* a1_slope = new RooRealVar("a1_slope","",0);
	RooRealVar* a2_slope = new RooRealVar("a2_slope","",0);
	//
        vector<RooRealVar> mh_s; mh_s.clear();
        RooRealVar mh_120("mh_120","",120); mh_s.push_back(mh_120);
        RooRealVar mh_124("mh_124","",124); mh_s.push_back(mh_124);
        RooRealVar mh_125("mh_125","",125); mh_s.push_back(mh_125);
        RooRealVar mh_126("mh_126","",126); mh_s.push_back(mh_126);
        RooRealVar mh_130("mh_130","",130); mh_s.push_back(mh_130);
	vector<RooRealVar> wh_s; wh_s.clear();
	RooRealVar wh_120("wh_120","",0.00348); wh_s.push_back(wh_120);
	RooRealVar wh_124("wh_124","",0.00391); wh_s.push_back(wh_124);
	RooRealVar wh_125("wh_125","",0.00404); wh_s.push_back(wh_125);
	RooRealVar wh_126("wh_126","",0.00418); wh_s.push_back(wh_126);
	RooRealVar wh_130("wh_130","",0.00488); wh_s.push_back(wh_130);
	
	RooRealVar* mass4l = new RooRealVar("mass4l","",105,140);
	vector<RooaDoubleCBxBW*> dcbxbw; dcbxbw.clear();
	vector<RooFormulaVar*> shifts; shifts.clear();
	vector<RooFormulaVar*> sigmas; sigmas.clear();
	vector<RooFormulaVar*> a1s; a1s.clear();
	vector<RooFormulaVar*> a2s; a2s.clear();
	for(int i=0; i<datasets.size(); i++){
		TString formula_s = "@0+@1*(@2-125)";
		RooFormulaVar* shift = new RooFormulaVar("shift_"+names.at(i), formula_s, RooArgList(*shift_intercept, *shift_slope, mh_s.at(i)));
		RooFormulaVar* sigma = new RooFormulaVar("sigma_"+names.at(i), formula_s, RooArgList(*sigma_intercept, *sigma_slope, mh_s.at(i)));
		RooFormulaVar* a1 = new RooFormulaVar("a1_"+names.at(i), formula_s, RooArgList(*a1_intercept, *a1_slope, mh_s.at(i)));
		RooFormulaVar* a2 = new RooFormulaVar("a2_"+names.at(i), formula_s, RooArgList(*a2_intercept, *a2_slope, mh_s.at(i)));
		int n1 = 3;
		int n2 = 3;
		RooRealVar* theta1 = new RooRealVar("theta1","",2.3);
		RooRealVar* theta2 = new RooRealVar("theta2","",2.3);
		RooaDoubleCBxBW* approx = new RooaDoubleCBxBW("approx_"+names.at(i),"",*mass4l,*shift,*sigma,*a1,*a2,mh_s.at(i),wh_s.at(i),n1,n2,*theta1,*theta2,false);
		shifts.push_back(shift);
		sigmas.push_back(sigma);
		a1s.push_back(a1);
		a2s.push_back(a2);
		dcbxbw.push_back(approx);
	}
        //
        RooCategory samples("samples","samples");
        for(int i=0; i<datasets.size();i++){
                samples.defineType(names.at(i));
        }
        //make comb_pdf
        RooSimultaneous comb_pdf("comb_pdf","comb_pdf",samples);
        for(int i=0; i<datasets.size(); i++){
                comb_pdf.addPdf(*dcbxbw.at(i),names.at(i));
        }
        //make comb_data
        RooDataSet* comb_dataset = new RooDataSet("comb_dataset","",RooArgSet(*mass4l),Index(samples),Import(names.at(0),*datasets.at(0)),Import(names.at(1),*datasets.at(1)),Import(names.at(2),*datasets.at(2)),Import(names.at(3),*datasets.at(3)),Import(names.at(4),*datasets.at(4)));
        std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@First Simult Fit@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	comb_pdf.fitTo(*comb_dataset,SumW2Error(kTRUE),PrintLevel(3));
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
        std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Second Simult Fit@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	std::cout<<"                                                                     "<<std::endl;
	comb_pdf.fitTo(*comb_dataset,SumW2Error(kTRUE),PrintLevel(3));
	//make plot
        TCanvas c("c","",1400,1000);
        for(int i=0; i<names.size(); i++){
                RooPlot* frame = mass4l->frame(Bins(100));
                frame->SetTitle("");
                comb_dataset->plotOn(frame,Cut("samples==samples::"+names.at(i)));
                comb_pdf.plotOn(frame,Slice(samples,names.at(i)),ProjWData(RooArgSet(samples),*comb_dataset));
                c.cd();
                //draw params
                TString shift_sfit_s = to_string(shifts.at(i)->getVal());
                TString sigma_sfit_s = to_string(sigmas.at(i)->getVal());
                TString alpha1_sfit_s = to_string(a1s.at(i)->getVal());
                TString alpha2_sfit_s = to_string(a2s.at(i)->getVal());

                TLatex *latex=new TLatex();
                latex->SetNDC();
                latex->SetTextSize(0.05);
                latex->SetTextFont(42);
                latex->SetTextAlign(23);
                frame->Draw();
                latex->DrawLatex(0.7,0.8,"shift="+shift_sfit_s);
                latex->DrawLatex(0.7,0.7,"sigma="+sigma_sfit_s);
                latex->DrawLatex(0.7,0.6,"a1="+alpha1_sfit_s);
                latex->DrawLatex(0.7,0.5,"a2="+alpha2_sfit_s);
                c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_mass_"+names.at(i)+"_approx.png");
                c.Clear();
        }
        TString mean_int = to_string(shift_intercept->getVal());
        TString sigma_int = to_string(sigma_intercept->getVal());
        TString alpha1_int = to_string(a1_intercept->getVal());
        TString alpha2_int = to_string(a2_intercept->getVal());
        TString mean_slo = to_string(shift_slope->getVal());
        TString sigma_slo = to_string(sigma_slope->getVal());
        TString alpha1_slo = to_string(a1_slope->getVal());
        TString alpha2_slo = to_string(a2_slope->getVal());

        ofstream in;
        in.open(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_sigshape_ggh_approx.py");
	in<<"info = {"<<endl;
        in<<"'mean_approx_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+mean_int+"+("+mean_slo+")*(@0-125)',"<<endl;
        in<<"'sigma_approx_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+sigma_int+"+("+sigma_slo+")*(@0-125)',"<<endl;
        in<<"'alpha_approx_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+alpha1_int+"+("+alpha1_slo+")*(@0-125)',"<<endl;
        in<<"'alpha2_approx_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+alpha2_int+"+("+alpha2_slo+")*(@0-125)',"<<endl;
        in<<"}"<<endl;
	in.close();

}
void DCB4LSimulFit(vector<RooDataSet*>& datasets, RooFitResult* r, vector<TString>& names, TString plotpath, TString cate_name, TString fs_name, TString type, TString tag_name, TString year){
        
	//retrieve params
        RooRealVar* mean_intercept = (RooRealVar*)r->floatParsFinal().find("mean");
        RooRealVar* sigma_intercept = (RooRealVar*)r->floatParsFinal().find("sigma");
        RooRealVar* alpha1_intercept = (RooRealVar*)r->floatParsFinal().find("alpha1");
        RooRealVar* n1_intercept = (RooRealVar*)r->floatParsFinal().find("n1");
        RooRealVar* alpha2_intercept = (RooRealVar*)r->floatParsFinal().find("alpha2");
        RooRealVar* n2_intercept = (RooRealVar*)r->floatParsFinal().find("n2");

        mean_intercept->setConstant(kTRUE);
        sigma_intercept->setConstant(kTRUE);
        alpha1_intercept->setConstant(kTRUE);
        n1_intercept->setConstant(kTRUE);
        alpha2_intercept->setConstant(kTRUE);
        n2_intercept->setConstant(kTRUE);

        //define slope
        RooRealVar* mean_slope = new RooRealVar("mean_slope","",1,-1.5,1.5);			//sometimes, fit cant converge, in this case, remember optimize these params, especially mean and sigma
        RooRealVar* sigma_slope = new RooRealVar("sigma_slope","",0.1,-1,1.5);
        RooRealVar* alpha1_slope = new RooRealVar("alpha1_slope","",0,-1,1);
        RooRealVar* n1_slope = new RooRealVar("n1_slope","",0,-1,1);
        RooRealVar* alpha2_slope = new RooRealVar("alpha2_slope","",0,-1,1);
        RooRealVar* n2_slope = new RooRealVar("n2_slope","",0,-1,1);

        n1_slope->setConstant(kTRUE);
        n2_slope->setConstant(kTRUE);

        //
        vector<RooRealVar> mh_s; mh_s.clear();
        RooRealVar mh_120("mh_120","",120); mh_s.push_back(mh_120);
        RooRealVar mh_124("mh_124","",124); mh_s.push_back(mh_124);
        RooRealVar mh_125("mh_125","",125); mh_s.push_back(mh_125);
        RooRealVar mh_126("mh_126","",126); mh_s.push_back(mh_126);
        RooRealVar mh_130("mh_130","",130); mh_s.push_back(mh_130);

        RooRealVar* mass4l = new RooRealVar("mass4l","",105,140);
        vector<RooDoubleCB*> DCBs; DCBs.clear();
        vector<RooFormulaVar*> means; means.clear();
        vector<RooFormulaVar*> sigmas; sigmas.clear();
        vector<RooFormulaVar*> alpha1s; alpha1s.clear();
        vector<RooFormulaVar*> n1s; n1s.clear();
        vector<RooFormulaVar*> alpha2s; alpha2s.clear();
        vector<RooFormulaVar*> n2s; n2s.clear();
        for(int i=0; i<datasets.size(); i++){
                TString mean_formula_s = "@0+@1*(@2-125)";
                TString formula_s = "@0+@1*(@2-125)";
                RooFormulaVar* mean = new RooFormulaVar("mean_"+names.at(i), mean_formula_s, RooArgList(*mean_intercept, *mean_slope, mh_s[i]));
                RooFormulaVar* sigma = new RooFormulaVar("sigma_"+names.at(i),formula_s, RooArgList(*sigma_intercept, *sigma_slope, mh_s[i]));
                RooFormulaVar* alpha1 = new RooFormulaVar("alpha1_"+names.at(i),formula_s, RooArgList(*alpha1_intercept, *alpha1_slope, mh_s[i]));
                RooFormulaVar* n1 = new RooFormulaVar("n1_"+names.at(i),formula_s, RooArgList(*n1_intercept, *n1_slope, mh_s[i]));
                RooFormulaVar* alpha2 = new RooFormulaVar("alpha2_"+names.at(i), formula_s, RooArgList(*alpha2_intercept, *alpha2_slope, mh_s[i]));
                RooFormulaVar* n2 = new RooFormulaVar("n2_"+names.at(i), formula_s, RooArgList(*n2_intercept, *n2_slope, mh_s[i]));
                RooDoubleCB* DCB = new RooDoubleCB("DCB_"+names.at(i),"",*mass4l,*mean,*sigma,*alpha1,*n1,*alpha2,*n2);
                means.push_back(mean);
                sigmas.push_back(sigma);
                alpha1s.push_back(alpha1);
                alpha2s.push_back(alpha2);
                n1s.push_back(n1);
                n2s.push_back(n2);
                DCBs.push_back(DCB);
        }

        //
        RooCategory samples("samples","samples");
        for(int i=0; i<datasets.size();i++){
                samples.defineType(names.at(i));
        }
        //make comb_pdf
        RooSimultaneous comb_pdf("comb_pdf","comb_pdf",samples);
        for(int i=0; i<datasets.size(); i++){
                comb_pdf.addPdf(*DCBs.at(i),names.at(i));
        }
        //make comb_data
        RooDataSet* comb_dataset = new RooDataSet("comb_dataset","",RooArgSet(*mass4l),Index(samples),Import(names.at(0),*datasets.at(0)),Import(names.at(1),*datasets.at(1)),Import(names.at(2),*datasets.at(2)),Import(names.at(3),*datasets.at(3)),Import(names.at(4),*datasets.at(4)));
        comb_pdf.fitTo(*comb_dataset,SumW2Error(kTRUE),PrintLevel(-1));
        comb_pdf.fitTo(*comb_dataset,SumW2Error(kTRUE),PrintLevel(-1));

        //consider that this function is not very general
        TCanvas c("c","",1400,1000);
        for(int i=0; i<names.size(); i++){
                RooPlot* frame = mass4l->frame(Bins(100));
                frame->SetTitle("");
                comb_dataset->plotOn(frame,Cut("samples==samples::"+names.at(i)));
                comb_pdf.plotOn(frame,Slice(samples,names.at(i)),ProjWData(RooArgSet(samples),*comb_dataset));
                c.cd();

                //draw params
                double tmp = means.at(i)->getVal();
                TString mean_sfit_s = to_string(tmp);
                TString sigma_sfit_s = to_string(sigmas.at(i)->getVal());
                TString alpha1_sfit_s = to_string(alpha1s.at(i)->getVal());
                TString alpha2_sfit_s = to_string(alpha2s.at(i)->getVal());
                TString n1_sfit_s = to_string(n1s.at(i)->getVal());
                TString n2_sfit_s = to_string(n2s.at(i)->getVal());

                TH1D* h_params = new TH1D("params","",6,0,6);
                h_params->SetBinContent(1,tmp);
                h_params->SetBinContent(2,sigmas.at(i)->getVal());
                h_params->SetBinContent(3,alpha1s.at(i)->getVal());
                h_params->SetBinContent(4,n1s.at(i)->getVal());
                h_params->SetBinContent(5,alpha2s.at(i)->getVal());
                h_params->SetBinContent(6,n2s.at(i)->getVal());
                //TFile* f = new TFile(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_mass_"+names.at(i)+".root","RECREATE");
                //f->cd();
                //h_params->Write();
                //f->Close();

                TLatex *latex=new TLatex();
                latex->SetNDC();
                latex->SetTextSize(0.05);
                latex->SetTextFont(42);
                latex->SetTextAlign(23);
                frame->Draw();
                latex->DrawLatex(0.7,0.8,"mean="+mean_sfit_s);
                latex->DrawLatex(0.7,0.7,"sigma="+sigma_sfit_s);
                latex->DrawLatex(0.7,0.6,"alpha1="+alpha1_sfit_s);
                latex->DrawLatex(0.7,0.5,"n1="+n1_sfit_s);
                latex->DrawLatex(0.7,0.4,"alpha2="+alpha2_sfit_s);
                latex->DrawLatex(0.7,0.3,"n2="+n2_sfit_s);

                c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_mass_"+names.at(i)+".png");
                c.Clear();
        }
        TH1F* h_p0 = new TH1F("h_p0","",6,0,6);
        TH1F* h_p1 = new TH1F("h_p1","",6,0,6);

        h_p0->SetBinContent(1,mean_intercept->getVal());
        h_p0->SetBinContent(2,sigma_intercept->getVal());
        h_p0->SetBinContent(3,alpha1_intercept->getVal());
        h_p0->SetBinContent(4,n1_intercept->getVal());
        h_p0->SetBinContent(5,alpha2_intercept->getVal());
        h_p0->SetBinContent(6,n2_intercept->getVal());

        h_p1->SetBinContent(1,mean_slope->getVal());
        h_p1->SetBinContent(2,sigma_slope->getVal());
        h_p1->SetBinContent(3,alpha1_slope->getVal());
        h_p1->SetBinContent(4,n1_slope->getVal());
        h_p1->SetBinContent(5,alpha2_slope->getVal());
        h_p1->SetBinContent(6,n2_slope->getVal());

        //c.cd();
        //h_p0->Draw("histtext");
        //c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_intercept.png");
        //TFile* f_p0 = new TFile(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_intercept.root","RECREATE");
        //f_p0->cd();
        //h_p0->Write();
        //f_p0->Close();

        //c.Clear();
        //c.cd();
        //h_p1->Draw("histtext");
        //c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_slope.png");
        //TFile* f_p1 = new TFile(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_slope.root","RECREATE");
        //f_p1->cd();
        //h_p1->Write();
        //f_p1->Close();

        TString mean_int = to_string(mean_intercept->getVal());
        TString sigma_int = to_string(sigma_intercept->getVal());
        TString alpha1_int = to_string(alpha1_intercept->getVal());
        TString n1_int = to_string(n1_intercept->getVal());
        TString alpha2_int = to_string(alpha2_intercept->getVal());
        TString n2_int = to_string(n2_intercept->getVal());
        TString mean_slo = to_string(mean_slope->getVal());
        TString sigma_slo = to_string(sigma_slope->getVal());
        TString alpha1_slo = to_string(alpha1_slope->getVal());
        TString n1_slo = to_string(n1_slope->getVal());
        TString alpha2_slo = to_string(alpha2_slope->getVal());
        TString n2_slo = to_string(n2_slope->getVal());

        ofstream in;
        in.open(plotpath+year+"_"+fs_name+"_"+type+"_"+tag_name+"_"+cate_name+"_sigshape_ggh.py");
	in<<"info = {"<<endl;
        in<<"'mean_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+mean_int+"+("+mean_slo+")*(@0-125)',"<<endl;
        in<<"'sigma_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+sigma_int+"+("+sigma_slo+")*(@0-125)',"<<endl;
        in<<"'alpha_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+alpha1_int+"+("+alpha1_slo+")*(@0-125)',"<<endl;
        in<<"'n_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+n1_int+"+("+n1_slo+")*(@0-125)',"<<endl;
        in<<"'alpha2_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+alpha2_int+"+("+alpha2_slo+")*(@0-125)',"<<endl;
        in<<"'n2_"+year+"_"+fs_name+"_"+cate_name+"_"+tag_name+"' : '"+n2_int+"+("+n2_slo+")*(@0-125)',"<<endl;
        in<<"}"<<endl;
	in.close();


}

void FitEventYield(double p1, double p1err, double p2, double p2err, double p3, double p3err, double p4, double p4err, double p5, double p5err, TString plotpath, TString cate_name, TString fs_name, TString process, TString type, TString tag, TString year){
	
	double x[5] = {120, 124, 125, 126, 130};
        double xerr[5] = {0,0,0,0,0};
	double y[5] = {p1, p2, p3, p4, p5};
	double yerr[5] = {p1err, p2err, p3err, p4err, p5err};
        TGraphErrors* g = new TGraphErrors(5,x,y,xerr,yerr);
        TF1* f1 = new TF1("f1","[0]+[1]*x+[2]*x*x",115,135);
        TFitResultPtr r1 = g->Fit(f1,"S E");
        f1->SetLineColor(7);
        f1->SetLineWidth(2);
        Double_t p1_0 = r1->Value(0);
        Double_t p1_1 = r1->Value(1);
        Double_t p1_2 = r1->Value(2);

        g->SetTitle("");
        g->GetXaxis()->SetTitle("m_{H}");
        g->GetYaxis()->SetTitle("expected yield");
        g->GetHistogram()->SetMinimum(0);
//retrieve max point
        double tmp_max = y[0]+yerr[0];
        for(int i=1; i<5; i++){
                if(tmp_max < (y[i]+yerr[i]))tmp_max = y[i]+yerr[i];
        }
        g->GetHistogram()->SetMaximum(tmp_max+tmp_max*0.2);
        g->GetXaxis()->SetLimits(115,135);
        g->SetMarkerStyle(22);
        g->SetMarkerSize(3);
        g->SetMarkerColor(4);
        TCanvas c("c","c", 1000, 1000);
        c.cd();
        g->Draw("ap");
        c.SaveAs(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_signorm_"+process+".png");

        TString p0_s, p1_s, p2_s;
        p0_s = to_string(p1_0);
        p1_s = to_string(p1_1);
        p2_s = to_string(p1_2);
        ofstream in;
        in.open(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_signorm_"+process+".py");
	in<<"info = {"<<endl;
	in<<"'sigrate_"+process+"_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' : '("+p0_s+")+("+p1_s+"*@0)+("+p2_s+"*@0*@0)',"<<endl;
        in<<"}"<<endl;
	in.close();

	in.open(plotpath+year+"_"+fs_name+"_"+type+"_"+tag+"_"+cate_name+"_sigabsnorm_"+process+".py");
	in<<"info = {"<<endl;
	in<<"'abs_sigrate_"+process+"_"+year+"_"+fs_name+"_"+cate_name+"_"+tag+"' :"+to_string(y[2])+","<<endl;
	in<<"}"<<endl;
	in.close();

}
