void Read4LSlimmedTreeFindmass4lErrCategorization(TTree* t, vector<TH1D*>& hists, int fs, vector<float>& bounds, TString Type, TString tag);
void ReadTreeForEventYield(double* yield, double* yielderr, TH1F* h_std, TTree* t, int fs, TString year, TString process, int mass, TString Type, TString tag, vector<TH1F*> &histos, vector<TH1F*> &ebehistos, vector<TH1F*> &relebehistos, vector<TH2F*> &ebevsm, vector<TH2F*> &relebevsm);
void ReadTreeForTau(double* yield, TH1F* h_std, TTree* t_2e2tau, TTree* t_2mu2tau, TTree* t_4tau, int fs, TString year, TString type, TString tag);
void Read4LSlimmedTree1DCategoryUnbinned(vector<RooDataSet*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag);
void Read4LSlimmedTree1DCategory(vector<TH1F*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag);
void Read4LSlimmedTree1DCategoryForKD(vector<TH2F*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag);
void Read4LSlimmedTree1DCategoryForDm(vector<TH1D*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag);
void ReadTreeForQQZZEventYield(double* yield, TH1F* h_std, TTree* t, int fs, TString year, TString process, TString Type, TString tag, vector<TH1F*> &histos, vector<TH1F*> &ebehistos, vector<TH1F*> &relebehistos, vector<TH2F*> &ebevsmhistos, vector<TH2F*> &relebevsmhistos );
std::string ReplaceString(std::string subject, const std::string& search,const std::string& replace);

void Read4LSlimmedTreeFindmass4lErrCategorization(TTree* t, vector<TH1D*>& hists, int fs, vector<float>& bounds, TString Type, TString tag){//used to find relative mass4Err bounds. type=RECO/REFIT, tag=Notag/Untag/VBFtag
	
	
	vector<float> rel_mass4lErr_list; rel_mass4lErr_list.clear();					//fill relative mass4lErr into std::vector in order to sort them
	for(int i=0; i<t->GetEntries(); i++){
		if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
		if((mass4lErr/mass4l<MERR)&&mass4l<140&&mass4l>105&&finalState==fs/*&&abs(eta1)<ETA1&&abs(eta2)<ETA2&&abs(eta3)<ETA3&&abs(eta4)<ETA4&&pt1>PT1&&pt2>PT2&&pt3>PT3&&pt4>PT4*/){
			if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
				rel_mass4lErr_list.push_back(mass4lErr/mass4l);
			}		
		}
	}

	sort(rel_mass4lErr_list.begin(), rel_mass4lErr_list.end());
	
	float nEvent = rel_mass4lErr_list.size();
	int nEvent_perCate = floor(nEvent/hists.size());
	bounds.push_back(0.0);//the first bin low edge
	for(int i=1; i<hists.size(); i++){//1-9
		bounds.push_back(rel_mass4lErr_list.at(i*nEvent_perCate));//
	}
	bounds.push_back(MERR);//the last bin high edge

	for(int i=0; i<hists.size(); i++){
		for(int j=0; j<rel_mass4lErr_list.size(); j++){
			if(bounds.at(i)<rel_mass4lErr_list.at(j)&&rel_mass4lErr_list.at(j)<bounds.at(i+1)){
				hists.at(i)->Fill(rel_mass4lErr_list.at(j));
			}
		}
	}	
}


void ReadTreeForTau(double* yield, TH1F* h_std, TTree* t_2e2tau, TTree* t_2mu2tau, TTree* t_4tau, int fs, TString year, TString type, TString tag){
	
	double Lumi;
	if(year=="20160")Lumi=19.52;
	if(year=="20165")Lumi=16.81;
	if(year=="2017")Lumi=41.48;
	if(year=="2018")Lumi=59.83;
	if(year=="2019")Lumi=137.64;
	
	double xs_2e2tau = 0.0031942;
	double xs_2mu2tau = 0.0031942;
	double xs_4tau = 0.0015849;
	
	double Ntot_2e2tau=0;
	double Ntot_2mu2tau=0;
	double Ntot_4tau=0;
	
	if(year!="2019"){	
	TString Ntotpath_2e2tau = "/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/"+year+"/GluGluToContinToZZTo2e2tau_M125_"+year+"_skimmed.root";
	TString Ntotpath_2mu2tau = "/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/"+year+"/GluGluToContinToZZTo2mu2tau_M125_"+year+"_skimmed.root";
	TString Ntotpath_4tau = "/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/"+year+"/GluGluToContinToZZTo4tau_M125_"+year+"_skimmed.root";
	
	TFile* f_2e2tau_ntot = new TFile(Ntotpath_2e2tau);
	TFile* f_2mu2tau_ntot = new TFile(Ntotpath_2mu2tau);
	TFile* f_4tau_ntot = new TFile(Ntotpath_4tau);

	TH1F* h_2e2tau = (TH1F*)f_2e2tau_ntot->Get("sumWeights");
	TH1F* h_2mu2tau = (TH1F*)f_2mu2tau_ntot->Get("sumWeights");
	TH1F* h_4tau = (TH1F*)f_4tau_ntot->Get("sumWeights");

	Ntot_2e2tau = h_2e2tau->Integral();
	Ntot_2mu2tau = h_2mu2tau->Integral();
	Ntot_4tau = h_4tau->Integral();
	}else{
		Ntot_2e2tau = 1998000.0;
		Ntot_2mu2tau = 1996000.0;
		Ntot_4tau = 1989213.0;
	}
	
	double update_sf_2e2tau = xs_2e2tau*Lumi*1000.0/Ntot_2e2tau;
	double update_sf_2mu2tau = xs_2mu2tau*Lumi*1000.0/Ntot_2mu2tau;
	double update_sf_4tau = xs_4tau*Lumi*1000.0/Ntot_4tau;
	
	Set4LSlimmedTree(t_2e2tau, type);
	
	TAxis* h_x = h_std->GetXaxis();
	int bin1;
	
	for(int i=0; i<t_2e2tau->GetEntries(); i++){
		if(i%100000==0)cout<<i<<"/"<<t_2e2tau->GetEntries()<<endl;
		t_2e2tau->GetEntry(i);
		if(mass4l<140&&mass4l>105&&(mass4lErr/mass4l)<MERR&&finalState==fs){
			if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
				bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
				yield[bin1] = yield[bin1] + eventWeight*k_ggZZ*update_sf_2e2tau;
			}
		}
	}

	Set4LSlimmedTree(t_2mu2tau, type);
	for(int i=0; i<t_2mu2tau->GetEntries(); i++){
		if(i%100000==0)cout<<i<<"/"<<t_2mu2tau->GetEntries()<<endl;
		t_2mu2tau->GetEntry(i);
		if(mass4l<140&&mass4l>105&&(mass4lErr/mass4l)<MERR&&finalState==fs){
			if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
				bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
				yield[bin1] = yield[bin1] + eventWeight*k_ggZZ*update_sf_2mu2tau;
			}
		}
	}

	Set4LSlimmedTree(t_4tau, type);
	for(int i=0; i<t_4tau->GetEntries(); i++){
		if(i%100000==0)cout<<i<<"/"<<t_4tau->GetEntries()<<endl;
		t_4tau->GetEntry(i);
		if(mass4l<140&&mass4l>105&&(mass4lErr/mass4l)<MERR&&finalState==fs){
			if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
				bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
				yield[bin1] = yield[bin1] + eventWeight*k_ggZZ*update_sf_4tau;
			}
		}
	}

}

void ReadTreeForEventYield(double* yield, double* yielderr, TH1F* h_std, TTree* t, int fs, TString year, TString process, int mass, TString Type, TString tag, vector<TH1F*> &histos, vector<TH1F*> &ebehistos, vector<TH1F*> &relebehistos, vector<TH2F*> &ebevsm, vector<TH2F*> &relebevsm){//qqzz isnt here, because cant get ntot automaticlly
	bool Phase2=false;
	
	//updatebounds(fs);
        Set4LSlimmedTree(t, Type);

        double Lumi;
        //if(year=="2016")Lumi=35.92;
	if(year=="20160")Lumi=19.52;
	if(year=="20165")Lumi=16.81;
        if(year=="2017")Lumi=41.48;
        if(year=="2018")Lumi=59.83;
	if(year=="2019")Lumi=137.64;

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
                if(mass==120)xs=0.008663298/1;
                if(mass==124)xs=0.012327354/1;
                if(mass==125)xs=0.01333521/1;
                if(mass==126)xs=0.014371789/1;
                if(mass==130)xs=0.018685844/1;
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
                if(mass==120)xs=0.0006528165/1;
                if(mass==124)xs=0.0009537624/1;
                if(mass==125)xs=0.001038159/1;
                if(mass==126)xs=0.0011259752/1;
                if(mass==130)xs=0.0014998988/1;
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
                if(mass==120)xs=0.000101067/1;
                if(mass==124)xs=0.000137242/1;
                if(mass==125)xs=0.0001462348/1;
                if(mass==126)xs=0.000156698761/1;
                if(mass==130)xs=0.0001928346/1;
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
                if(mass==120)xs=0.0001586012/1;
                if(mass==124)xs=0.0002154715/1;
                if(mass==125)xs=0.0002305562/1;
                if(mass==126)xs=0.0002456155435/1;
                if(mass==130)xs=0.0003057235/1;
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
                if(mass==120)xs=0.0003548342/1;
                if(mass==124)xs=0.0004937119/1;
                if(mass==125)xs=0.0005321759/1;
                if(mass==126)xs=0.0005685128/1;
                if(mass==130)xs=0.0007189531/1;
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
                if(mass==120)xs=0.0002393619/1;
                if(mass==124)xs=0.0003373278/1;
                if(mass==125)xs=0.0003639351/1;
                if(mass==126)xs=0.0003913481/1;
                if(mass==130)xs=0.0004969182/1;
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
		if(mass==120)xs=0.00009180991;
		if(mass==124)xs=0.0001250750;
		if(mass==125)xs=0.0001339560;
		if(mass==126)xs=0.0001428475;
		if(mass==130)xs=0.0001774970;
	}
	if(process=="thq"&&BASE=="20UL"){
		if(mass==120)xs=0.0000857830;
		if(mass==124)xs=0.0000857830;
		if(mass==125)xs=0.0000857830;
		if(mass==126)xs=0.0000857830;
		if(mass==130)xs=0.0000857830;
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
		if(fs==1||fs==2)xs=0.00158549;
		if(fs==3||fs==4)xs=0.0031942;
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
	if(process=="thq"){
		if(BASE=="20UL")process_ = "tHq_HToZZTo4L_M";
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
		if(process_=="tHq_HToZZTo4L_M"){
			mass_s = "125";
			Ntotpath = "/eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/"+year+"/"+process_ + mass_s + "_"+ year + "_skimmed.root";
		}
	}
	
	int Ntot = 0;
	if(year!="2019"){	
	TFile* f = new TFile(Ntotpath);
        TH1F* h_ = (TH1F*)f->Get("sumWeights");
	Ntot = h_->Integral();
	delete f;
	}
	else{
		int Ntot1=0;
		int Ntot2=0;
		int Ntot3=0;
		int Ntot4=0;
		std::string in20160path = ReplaceString(Ntotpath.Data(),"/2019/","/20160/");
		std::string in20165path = ReplaceString(Ntotpath.Data(),"/2019/","/20165/");
		std::string in2017path = ReplaceString(Ntotpath.Data(),"/2019/","/2017/");
		std::string in2018path = ReplaceString(Ntotpath.Data(),"/2019/","/2018/");
		std::string newin20160path = ReplaceString(in20160path,"_2019_","_20160_");
		std::string newin20165path = ReplaceString(in20165path,"_2019_","_20165_");
		std::string newin2017path = ReplaceString(in2017path,"_2019_","_2017_");
		std::string newin2018path = ReplaceString(in2018path,"_2019_","_2018_");
		//adjust the path
		std::string newnewpath20160 = newin20160path;
		std::string newnewpath20165 = newin20165path;
		std::string newnewpath2017 = newin2017path;
		std::string newnewpath2018 = newin2018path;
		if(process_=="ZH_HToZZ_4LFilter_M"&&mass_s=="120"){
			newnewpath20160 = ReplaceString(newin20160path,"120","124");
		}
		else if(process_=="ttH_HToZZ_4LFilter_M"&&(mass_s=="126")){
			newnewpath20160 = ReplaceString(newin20160path,"126","125");
		}
		else if(process_=="ttH_HToZZ_4LFilter_M"&&(mass_s=="130")){
			newnewpath20160 = ReplaceString(newin20160path,"130","125");
		}
		else if(process_=="ttH_HToZZ_4LFilter_M"&&mass_s=="124"){
			newnewpath20160 = ReplaceString(newin20160path,"124","120");
		}
		else if(process_=="tHq_HToZZTo4L_M"){
			std::string mass_tmp = mass_s.Data();
			newnewpath20160 = ReplaceString(newin20160path,mass_tmp,"125");
			newnewpath20165 = ReplaceString(newin20165path,mass_tmp,"125");
			newnewpath2017 = ReplaceString(newin2017path,mass_tmp,"125");
			newnewpath2018 = ReplaceString(newin2018path,mass_tmp,"125");
		}

		TString newnewIn20160path = newnewpath20160;
		TString newnewIn20165path = newnewpath20165;
		TString newnewIn2017path = newnewpath2017;
		TString newnewIn2018path = newnewpath2018;

		TFile* f1 = new TFile(newnewIn20160path);
		TH1F* h1 = (TH1F*)f1->Get("sumWeights");
		Ntot1 = h1->Integral();
		delete f1;

		TFile* f2 = new TFile(newnewIn20165path);
		TH1F* h2 = (TH1F*)f2->Get("sumWeights");
		Ntot2 = h2->Integral();
		delete f2;
		
		TFile* f3 = new TFile(newnewIn2017path);
		TH1F* h3 = (TH1F*)f3->Get("sumWeights");
		Ntot3 = h3->Integral();
		delete f3;

		TFile* f4 = new TFile(newnewIn2018path);
		TH1F* h4 = (TH1F*)f4->Get("sumWeights");
		Ntot4 = h4->Integral();
		delete f4;
		Ntot = Ntot1 + Ntot2 + Ntot3 + Ntot4;
	}
	//end retrieve Ntot
	
	double update_sf = xs*Lumi*1000.0/Ntot;
	 
	TAxis* h_x = h_std->GetXaxis();
        int bin1;
        double k=1.0;
        for(int i=0; i<t->GetEntries(); i++){
                //if(i>1000)break;
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
                t->GetEntry(i);
		if(mass4l<140&&mass4l>105&&(mass4lErr/mass4l)<MERR&&finalState==fs/*&&abs(eta1)<ETA1&&abs(eta2)<ETA1&&abs(eta3)<ETA3&&abs(eta4)<ETA4&&pt1>PT1&&pt2>PT2&&pt3>PT3&&pt4>PT4*/){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                if(process=="ggzz")k=k_ggZZ;
                                bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
                                yield[bin1] = yield[bin1] + eventWeight*k*update_sf;
				histos[bin1]->Fill(mass4l,eventWeight*k*update_sf);
				ebehistos[bin1]->Fill(mass4lErr,eventWeight*k*update_sf);
				relebehistos[bin1]->Fill(mass4lErr/mass4l,eventWeight*k*update_sf);
				ebevsm[bin1]->Fill(mass4l,mass4lErr,eventWeight*k*update_sf);
				relebevsm[bin1]->Fill(mass4l,mass4lErr/mass4l,eventWeight*k*update_sf);
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

void ReadTreeForQQZZEventYield(double* yield, TH1F* h_std, TTree* t, int fs, TString year, TString process, TString Type, TString tag, vector<TH1F*> &histos, vector<TH1F*> &ebehistos, vector<TH1F*> &relebehistos, vector<TH2F*> &ebevsmhistos, vector<TH2F*> &relebevsmhistos ){
	
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
			Ntot=49373660;//49878000;
		}
	}
        if(year=="2017"){
		Lumi=41.48;
		//if(BASE=="20UL")Ntot=65298000; first look at 20summerUL
		if(BASE=="20UL")Ntot=98378080;//99388000;
		if(BASE=="ReReco")Ntot=50756359+36007127;
	}
        if(year=="2018"){
		Lumi=59.83;
		//if(BASE=="20UL")Ntot=87840000; first look at 20summerUL
		if(BASE=="20UL")Ntot=98187536;//99191000;
		if(BASE=="ReReco")Ntot=19089600;
	}
	if(year=="2019"){
		Lumi=19.52+16.81+41.48+59.83;
		Ntot=4918966+49373660+98378080+98187536;
	}
	k=1;
        TAxis* h_x = h_std->GetXaxis();
        int bin1;
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
	double update_sf = xs*Lumi*1000/Ntot;
	double yield_err = 0.0;
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);               
		if(mass4l<140&&mass4l>105&&(mass4lErr/mass4l)<MERR&&finalState==fs/*&&abs(eta1)<ETA1&&abs(eta2)<ETA2&&abs(eta3)<ETA3&&abs(eta4)<ETA4&&pt1>PT1&&pt2>PT2&&pt3>PT3&&pt4>PT4*/){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                k=k_qqZZ_ewk*k_qqZZ_qcd_M;
                                bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
                                yield[bin1] = yield[bin1] + eventWeight*k;
				yield_err = yield_err + eventWeight*eventWeight*k*k;
				histos[bin1]->Fill(mass4l,eventWeight*k*update_sf);
				ebehistos[bin1]->Fill(mass4lErr,eventWeight*k*update_sf);
				relebehistos[bin1]->Fill(mass4lErr/mass4l,eventWeight*k*update_sf);
				ebevsmhistos[bin1]->Fill(mass4l,mass4lErr,eventWeight*k*update_sf);
				relebevsmhistos[bin1]->Fill(mass4l,mass4lErr/mass4l,eventWeight*k*update_sf);
                        }
                }
        }
	for(int i=0; i<10; i++){
                yield[i] = yield[i]*xs*Lumi*1000/Ntot;
        }
	std::cout<<"qqzz "<<fs<<" "<<sqrt(yield_err*(xs*Lumi*1000/Ntot)*(xs*Lumi*1000/Ntot))<<endl;
}

void Read4LSlimmedTree1DCategoryUnbinned(vector<RooDataSet*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag){
        //updatebounds(fs);
	Set4LSlimmedTree(t,Type);
        TAxis* h_x = h_std->GetXaxis();
        int bin;
        RooRealVar mass4l_("mass4l","",105,140);
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
                if(mass4l>105&&mass4l<140&&(mass4lErr/mass4l)<MERR&&finalState==fs/*&&abs(eta1)<ETA1&&abs(eta2)<ETA2&&abs(eta3)<ETA3&&abs(eta4)<ETA4&&pt1>PT1&&pt2>PT2&&pt3>PT3&&pt4>PT4*/){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                bin = h_x->FindBin(mass4lErr/mass4l)-1;
                                mass4l_.setVal(mass4l);
                                histos.at(bin)->add(mass4l_);
                        }
                }
        }
}

void Read4LSlimmedTree1DCategory(vector<TH1F*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag){
        //updatebounds(fs);
	Set4LSlimmedTree(t, Type);
        TAxis* h_x = h_std->GetXaxis();
        int bin1;
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		//if(i<1000)break;
                t->GetEntry(i);
                if((mass4lErr/mass4l)<MERR&&mass4l>105&&mass4l<140&&finalState==fs/*&&abs(eta1)<ETA1&&abs(eta2)<ETA2&&abs(eta3)<ETA3&&abs(eta4)<ETA4&&pt1>PT1&&pt2>PT2&&pt3>PT3&&pt4>PT4*/){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
                                histos.at(bin1)->Fill(mass4l);
                        }
                }
        }
}

void Read4LSlimmedTree1DCategoryForKD(vector<TH2F*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag){
        //updatebounds(fs);
	Set4LSlimmedTree(t,Type);
        TAxis* h_x = h_std->GetXaxis();
        int bin1;
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
                if((mass4lErr/mass4l)<MERR&&mass4l>MERR&&mass4l<140&&finalState==fs/*&&abs(eta1)<ETA1&&abs(eta2)<ETA2&&abs(eta3)<ETA3&&abs(eta4)<ETA4&&pt1>PT1&&pt2>PT2&&pt3>PT3&&pt4>PT4*/){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
                                histos.at(bin1)->Fill(mass4l,KD);
                        }
                }
        }
}

void Read4LSlimmedTree1DCategoryForDm(vector<TH1D*>& histos, TH1F* h_std, TTree* t, int fs, TString Type, TString tag){
        //updatebounds(fs);
	Set4LSlimmedTree(t,Type);
        TAxis* h_x = h_std->GetXaxis();
        int bin1;
        for(int i=0; i<t->GetEntries(); i++){
                if(i%100000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
                if((mass4lErr/mass4l)<MERR&&mass4l>105&&mass4l<140&&(finalState==fs)/*&&abs(eta1)<ETA1&&abs(eta2)<ETA2&&abs(eta3)<ETA3&&abs(eta4)<ETA4&&pt1>PT1&&pt2>PT2&&pt3>PT3&&pt4>PT4*/){
                        if((tag=="notag")||(tag=="vbftag"&&EventCat==2)||(tag=="untag"&&EventCat!=2)){
                                bin1 = h_x->FindBin(mass4lErr/mass4l)-1;
                                histos.at(bin1)->Fill(mass4lErr);
                        }
                }
        }
}

std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}
