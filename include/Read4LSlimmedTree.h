void Read4LSlimmedTreeFindmass4lErrCategorization(TTree* t, vector<TH1D*>& hists, int fs, vector<float>& bounds, TString Type, TString tag);
void ReadTreeForEventYield(double* yield, double* yielderr, TH1F* h_std, TTree* t, int fs, TString year, TString process, int mass, TString Type, TString tag);
void ReadTreeForQQZZEventYield(double* yield, TH1F* h_std, TTree* t, int fs, TString year, TString process, TString Type, TString tag);

void Read4LSlimmedTree1DCategoryUnbinned(vector<RooDataSet*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag);
void Read4LSlimmedTree1DCategory(vector<TH1F*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag);
void Read4LSlimmedTree1DCategoryForKD(vector<TH2F*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag);
void Read4LSlimmedTree1DCategoryForDm(vector<TH1D*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag);

void Read4LSlimmedTreeFindmass4lErrCategorization(TTree* t, vector<TH1D*>& hists, int fs, vector<float>& bounds, TString Type, TString tag){//used to find mass4Err bounds, it's not very general, type=RECO/REFIT, tag=Notag/Untag/VBFtag
	
	vector<float> mass4lErr_list; mass4lErr_list.clear();					//fill mass4lErr into std::vector in order to sort them
	for(int i=0; i<t->GetEntries(); i++){
		if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
		if(mass4l<140&&mass4l>105&&finalState==fs&&abs(eta1)<2.4&&abs(eta2)<2.4&&abs(eta3)<2.4&&abs(eta4)<2.4){
			if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
				mass4lErr_list.push_back(mass4lErr);
			}		
		}
	}

	sort(mass4lErr_list.begin(), mass4lErr_list.end());					//sort mass4lErr
	
	float nEvent = mass4lErr_list.size();							//boundary value
	int nEvent_perCate = floor(nEvent/hists.size());
	bounds.push_back(0.0);//the first bin low edge
	for(int i=1; i<hists.size(); i++){//1-9
		bounds.push_back(mass4lErr_list.at(i*nEvent_perCate));//
	}
	bounds.push_back(50);//the last bin high edge

	for(int i=0; i<hists.size(); i++){							//fill TH1F with vector elements, time-wasting but humanreadable
		for(int j=0; j<mass4lErr_list.size(); j++){
			if(bounds.at(i)<mass4lErr_list.at(j)&&mass4lErr_list.at(j)<bounds.at(i+1)){
				hists.at(i)->Fill(mass4lErr_list.at(j));
			}
		}
	}	
}

void ReadTreeForEventYield(double* yield, double* yielderr, TH1F* h_std, TTree* t, int fs, TString year, TString process, int mass, TString Type, TString tag){//qqzz isnt here, because cant get ntot automaticlly
	bool Phase2=false;

        Set4LSlimmedTree(t, Type);

        double Lumi;
        //if(year=="2016")Lumi=35.92;
	if(year=="20160")Lumi=19.52;
	if(year=="20165")Lumi=16.81;
        if(year=="2017")Lumi=41.48;
        if(year=="2018")Lumi=59.83;

        //update xs//sometimes the xs value in the ntuples is wrong.
        double xs = 1.0;
        if(process=="ggh"&&BASE=="ReReco"){
                if(mass==120)xs=0.008663/0.00792;
                if(mass==124)xs=0.012327/0.01122;
                if(mass==125)xs=0.013335/0.01212;
                if(mass==126)xs=0.014372/0.01308;
                if(mass==130)xs=0.018686/0.01706;
        }
	if(process=="ggh"&&BASE=="20UL"&&Phase2){
                if(mass==120)xs=1.124*0.008663/1;
                if(mass==124)xs=1.125*0.012327/1;
                if(mass==125)xs=1.125*0.013335/1;
                if(mass==126)xs=1.126*0.014372/1;
                if(mass==130)xs=1.127*0.018686/1;
        }
	if(process=="ggh"&&BASE=="20UL"){
                if(mass==120)xs=0.008663/1;
                if(mass==124)xs=0.012327/1;
                if(mass==125)xs=0.013335/1;
                if(mass==126)xs=0.014372/1;
                if(mass==130)xs=0.018686/1;
        }
	//-------------->ggF
	
	if(process=="vbf"&&BASE=="ReReco"){
                if(mass==120)xs=0.000653/0.0006573;
                if(mass==124)xs=0.000954/0.0009606;
                if(mass==125)xs=0.001038/0.001034;
                if(mass==126)xs=0.001126/0.001133;
                if(mass==130)xs=0.0015/0.001506;
        }
	if(process=="vbf"&&BASE=="20UL"&&Phase2){
                if(mass==120)xs=1.130*0.000653/1;
                if(mass==124)xs=1.131*0.000954/1;
                if(mass==125)xs=1.131*0.001038/1;
                if(mass==126)xs=1.131*0.001126/1;
                if(mass==130)xs=1.132*0.0015/1;
        }
	if(process=="vbf"&&BASE=="20UL"){
                if(mass==120)xs=0.000653/1;
                if(mass==124)xs=0.000954/1;
                if(mass==125)xs=0.001038/1;
                if(mass==126)xs=0.001126/1;
                if(mass==130)xs=0.0015/1;
        }
	//-------------->vbf
	
	if(process=="wmh"&&BASE=="ReReco"){
                if(mass==120)xs=0.000101/0.0001015;
                if(mass==124)xs=0.000137/0.0001379;
                if(mass==125)xs=0.000146/0.0001471;
                if(mass==126)xs=0.000156/0.000157;
                if(mass==130)xs=0.000193/0.0001944;
        }
	if(process=="wmh"&&BASE=="20UL"&&Phase2){
                if(mass==120)xs=1.110*0.000101/1;
                if(mass==124)xs=1.110*0.000137/1;
                if(mass==125)xs=1.112*0.000146/1;
                if(mass==126)xs=1.112*0.000156/1;
                if(mass==130)xs=1.112*0.000193/1;
        }  
	if(process=="wmh"&&BASE=="20UL"){
                if(mass==120)xs=0.000101/1;
                if(mass==124)xs=0.000137/1;
                if(mass==125)xs=0.000146/1;
                if(mass==126)xs=0.000156/1;
                if(mass==130)xs=0.000193/1;
        }
	//-------------->wmh
	//
	if(process=="wph"&&BASE=="ReReco"){
                if(mass==120)xs=0.000159/0.0001606;
                if(mass==124)xs=0.000215/0.000219;
                if(mass==125)xs=0.000231/0.0002339;
                if(mass==126)xs=0.000246/0.0002497;
                if(mass==130)xs=0.000306/0.0003103;
        }
	if(process=="wph"&&BASE=="20UL"&&Phase2){
                if(mass==120)xs=1.097*0.000159/1;
                if(mass==124)xs=1.098*0.000215/1;
                if(mass==125)xs=1.097*0.000231/1;
                if(mass==126)xs=1.098*0.000246/1;
                if(mass==130)xs=1.097*0.000306/1;
        }
	if(process=="wph"&&BASE=="20UL"){
                if(mass==120)xs=0.000159/1;
                if(mass==124)xs=0.000215/1;
                if(mass==125)xs=0.000231/1;
                if(mass==126)xs=0.000246/1;
                if(mass==130)xs=0.000306/1;
        }
	//-------------->wph
	//
	if(process=="zh"&&BASE=="ReReco"){
                if(mass==120)xs=0.000448/0.0004467;
                if(mass==124)xs=0.000615/0.0006145;
                if(mass==125)xs=0.000662/0.0006569;
                if(mass==126)xs=0.000708/0.0007026;
                if(mass==130)xs=0.000892/0.0008778;
        }
	if(process=="zh"&&BASE=="20UL"&&Phase2){
                if(mass==120)xs=1.113*0.000448/1;
                if(mass==124)xs=1.115*0.000615/1;
                if(mass==125)xs=1.116*0.000662/1;
                if(mass==126)xs=1.113*0.000708/1;
                if(mass==130)xs=1.115*0.000892/1;
        }
	if(process=="zh"&&BASE=="20UL"){
                if(mass==120)xs=0.000355/1;
                if(mass==124)xs=0.000494/1;
                if(mass==125)xs=0.000532/1;
                if(mass==126)xs=0.000569/1;
                if(mass==130)xs=0.000719/1;
        }
	//-------------->zh
	//
	if(process=="tth"&&BASE=="ReReco"){
                if(mass==120)xs=0.000262/0.00021383;
                if(mass==124)xs=0.000363/0.00034867;
                if(mass==125)xs=0.00039/0.00038991;
                if(mass==126)xs=0.000418/0.00041842;
                if(mass==130)xs=0.000529/0.00046059;
        }
	if(process=="tth"&&BASE=="20UL"&&Phase2){
                if(mass==120)xs=1.209*0.000262/1;
                if(mass==124)xs=1.209*0.000363/1;
                if(mass==125)xs=1.210*0.00039/1;
                if(mass==126)xs=1.211*0.000418/1;
                if(mass==130)xs=1.209*0.000529/1;
        }
	if(process=="tth"&&BASE=="20UL"){
                if(mass==120)xs=0.000239/1;
                if(mass==124)xs=0.000337/1;
                if(mass==125)xs=0.000364/1;
                if(mass==126)xs=0.000391/1;
                if(mass==130)xs=0.000497/1;
        }
	//-------------->tth
	//
	if(process=="bbh"&&BASE=="20UL"&&Phase2){
		if(mass==120)xs=1.136*0.000152;
		if(mass==124)xs=1.135*0.000138;
		if(mass==125)xs=1.133*0.000134;
		if(mass==126)xs=1.131*0.000131;
		if(mass==130)xs=1.133*0.000119;
	}
	if(process=="bbh"&&BASE=="20UL"){
		if(mass==120)xs=0.000092;
		if(mass==124)xs=0.000125;
		if(mass==125)xs=0.000134;
		if(mass==126)xs=0.000142;
		if(mass==130)xs=0.000177;
	}

	if(process=="ggzz"&&BASE=="ReReco"){
		if(fs==1||fs==2)xs=0.00159/0.00159;
		if(fs==3||fs==4)xs=0.00319/0.00319;
	}
	if(process=="ggzz"&&BASE=="20UL"&&Phase2){
		if(fs==1||fs==2)xs=(2.587 / 2.296)*0.00159;
		if(fs==3||fs==4)xs=(2.587 / 2.296)*0.00319;
	}
	if(process=="ggzz"&&BASE=="20UL"){
		if(fs==1||fs==2)xs=0.00159;
		if(fs==3||fs==4)xs=0.00319;
	}
	//xs=1;
	//end update xs
        
	//retrieve Ntot, only works for signals and ggzz
        TString process_, mass_s = "125";
        if(process=="ggh"){
		if(BASE=="20UL")process_ = "GluGluHToZZTo4L_M";
		if(BASE=="ReReco")process_ = "GGH_";
		mass_s = to_string(mass);
	}
        if(process=="vbf"){
		if(BASE=="20UL")process_ = "VBF_HToZZTo4L_M";
		if(BASE=="ReReco")process_ = "VBF_";
		mass_s = to_string(mass);
	}
        if(process=="wmh"){
		if(BASE=="20UL")process_ = "WminusH_HToZZTo4L_M";
		if(BASE=="ReReco")process_ = "WminusH_";
		mass_s = to_string(mass);
	}
        if(process=="wph"){
		if(BASE=="20UL")process_ = "WplusH_HToZZTo4L_M";
		if(BASE=="ReReco")process_ = "WplusH_";
		mass_s = to_string(mass);
	}
        if(process=="zh"){
		if(BASE=="20UL")process_ = "ZH_HToZZ_4LFilter_M";
		if(BASE=="ReReco")process_ = "ZH_";
		mass_s = to_string(mass);
	}
        if(process=="tth"){
		if(BASE=="20UL")process_ = "ttH_HToZZ_4LFilter_M";
		if(BASE=="ReReco")process_ = "ttH_";
		mass_s = to_string(mass);
	}
	if(process=="bbh"){
		if(BASE=="20UL")process_ = "bbH_HToZZTo4L_M";
		mass_s = to_string(mass);
	}
        if(process=="ggzz"&&fs==1){
		if(BASE=="20UL")process_ = "GluGluToContinToZZTo4mu_M";
		if(BASE=="ReReco")process_ = "ggZZ_4mu"; mass_s = "125";
	}
        if(process=="ggzz"&&fs==2){
		if(BASE=="20UL")process_ = "GluGluToContinToZZTo4e_M";
		if(BASE=="ReReco")process_ = "ggZZ_4e"; mass_s = "125";
	}
        if(process=="ggzz"&&(fs==3||fs==4)){
		if(BASE=="20UL")process_ = "GluGluToContinToZZTo2e2mu_M";
		if(BASE=="ReReco")process_ = "ggZZ_2e2mu"; mass_s = "125";
	}
        TString Ntotpath = "";
	if(BASE=="ReReco")Ntotpath = "/eos/user/c/chenguan/Tuples/AfterSync_200424/" + year + process_ + mass_s + ".root";
        if(BASE=="20UL"){
		Ntotpath = "/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/"+year+"/"+process_ + mass_s + "_"+ year + "_skimmed.root";
		//missing samples
		if(year=="20160"){
			if(process_=="ZH_HToZZ_4LFilter_M"&&mass_s=="120"){
				mass_s = "124";
				Ntotpath = "/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/"+year+"/"+process_ + mass_s + "_"+ year + "_skimmed.root";
			}
			if(process_=="ttH_HToZZ_4LFilter_M"&&(mass_s=="126"||mass_s=="130")){
				mass_s = "125";
				Ntotpath = "/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/"+year+"/"+process_ + mass_s + "_"+ year + "_skimmed.root";
			}
			if(process_=="ttH_HToZZ_4LFilter_M"&&mass_s=="124"){
				mass_s = "120";
				Ntotpath = "/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/"+year+"/"+process_ + mass_s + "_"+ year + "_skimmed.root";
			}
		}
	}
	TFile* f = new TFile(Ntotpath);
        TTree* t_ = (TTree*)f->Get("Ana/passedEvents");
	if(!t_)t_ = (TTree*)f->Get("passedEvents");
        double Ntot = t_->GetEntries();
        TH1F* h_ = (TH1F*)f->Get("sumWeights");
	if(year=="20160")Ntot = h_->Integral();
	delete f;
	//end retrieve Ntot
	
	//for a key factor
	double update_sf = xs*Lumi*1000.0/Ntot;
	//end 
	 
	TAxis* h_x = h_std->GetXaxis();
        int bin1;
        double k=1.0;
        for(int i=0; i<t->GetEntries(); i++){
                //if(i>1000)break;
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
                t->GetEntry(i);
		if(mass4l<140&&mass4l>105&&mass4lErr<50&&finalState==fs&&passedFullSelection==1&&abs(eta1)<2.4&&abs(eta2)<2.4&&abs(eta3)<2.4&&abs(eta4)<2.4){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                if(process=="ggzz")k=k_ggZZ;
                                bin1 = h_x->FindBin(mass4lErr)-1;
                                yield[bin1] = yield[bin1] + eventWeight*k*update_sf;
				yielderr[bin1] = yielderr[bin1] + (eventWeight*k*update_sf)*(eventWeight*k*update_sf);
                        }
                }
        }
        for(int i=0; i<10; i++){//for further update
        	cout<<process<<"	"<<mass<<"	"<<fs<<"	cat"<<to_string(i)<<"	:"<<yield[i]<<"+/-"<<sqrt(yielderr[i])<<endl;
		yield[i] = yield[i];
		yielderr[i] = sqrt(yielderr[i]);
        }
}

void ReadTreeForQQZZEventYield(double* yield, TH1F* h_std, TTree* t, int fs, TString year, TString process, TString Type, TString tag){
	
	bool Phase2=false;

        Set4LSlimmedTree(t, Type);

        double Lumi, k; int Ntot;
        if(year=="20160"){
		Lumi=19.52;
		Ntot=49189660;
	}
	if(year=="20165"){
		Lumi=16.81;
		if(BASE=="ReReco"){
			Ntot=77998181+18470796;
		}
		if(BASE=="20UL"){
			Ntot=49878000;
		}
	}
        if(year=="2017"){
		Lumi=41.48;
		//if(BASE=="20UL")Ntot=65298000; first look at 20summerUL, Filippo may not finish the tuple production
		if(BASE=="20UL")Ntot=99388000;
		if(BASE=="ReReco")Ntot=50756359+36007127;
	}
        if(year=="2018"){
		Lumi=59.83;
		//if(BASE=="20UL")Ntot=87840000; first look at 20summerUL, Filippo may not finish the tuple production
		if(BASE=="20UL")Ntot=99191000;
		if(BASE=="ReReco")Ntot=19089600;
	}
	k=1;
        TAxis* h_x = h_std->GetXaxis();
        int bin1;
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);               
		if(mass4l<140&&mass4l>105&&mass4lErr<50&&finalState==fs&&passedFullSelection==1&&abs(eta1)<2.4&&abs(eta2)<2.4&&abs(eta3)<2.4&&abs(eta4)<2.4){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                k=k_qqZZ_ewk*k_qqZZ_qcd_M;
                                bin1 = h_x->FindBin(mass4lErr)-1;
                                yield[bin1] = yield[bin1] + eventWeight*k;
                        }
                }
        }
        double xs = 1;
	if(BASE=="ReReco"){
		xs = 1.0;
	}
	if(BASE=="20UL"&&Phase2){
		xs = (1.943 / 1.785)*1.256;
	}
	if(BASE=="20UL"){
		xs = 1.256;
	}
	for(int i=0; i<10; i++){
                yield[i] = yield[i]*xs*Lumi*1000/Ntot;
        }
}

void Read4LSlimmedTree1DCategoryUnbinned(vector<RooDataSet*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag){
        Set4LSlimmedTree(t,Type);
        TAxis* h_x = h_std->GetXaxis();
        int bin;
        RooRealVar mass4l_("mass4l","",105,140);
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
                if(mass4l>105&&mass4l<140&&mass4lErr<50&&finalState==fs&&abs(eta1)<2.4&&abs(eta2)<2.4&&abs(eta3)<2.4&&abs(eta4)<2.4){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                bin = h_x->FindBin(mass4lErr)-1;
                                mass4l_.setVal(mass4l);
                                histos.at(bin)->add(mass4l_);
                        }
                }
        }
}

void Read4LSlimmedTree1DCategory(vector<TH1F*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag){
        Set4LSlimmedTree(t, Type);
        TAxis* h_x = h_std->GetXaxis();
        int bin1;
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		//if(i<1000)break;
                t->GetEntry(i);
                if(mass4l>105&&mass4l<140&&mass4lErr<50&&finalState==fs&&abs(eta1)<2.4&&abs(eta2)<2.4&&abs(eta3)<2.4&&abs(eta4)<2.4){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                bin1 = h_x->FindBin(mass4lErr)-1;
                                histos.at(bin1)->Fill(mass4l);
                        }
                }
        }
}

void Read4LSlimmedTree1DCategoryForKD(vector<TH2F*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag){
        Set4LSlimmedTree(t,Type);
        TAxis* h_x = h_std->GetXaxis();
        int bin1;
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
                if(mass4l>105&&mass4l<140&&mass4lErr<50&&finalState==fs&&abs(eta1)<2.4&&abs(eta2)<2.4&&abs(eta3)<2.4&&abs(eta4)<2.4){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                bin1 = h_x->FindBin(mass4lErr)-1;
                                histos.at(bin1)->Fill(mass4l,KD);
                        }
                }
        }
}

void Read4LSlimmedTree1DCategoryForDm(vector<TH1D*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag){
        Set4LSlimmedTree(t,Type);
        TAxis* h_x = h_std->GetXaxis();
        int bin1;
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
                if(mass4l>105&&mass4l<140&&mass4lErr<50&&(finalState==fs)&&abs(eta1)<2.4&&abs(eta2)<2.4&&abs(eta3)<2.4&&abs(eta4)<2.4){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                bin1 = h_x->FindBin(mass4lErr)-1;
                                histos.at(bin1)->Fill(mass4lErr/mass4l);
                        }
                }
        }
}
