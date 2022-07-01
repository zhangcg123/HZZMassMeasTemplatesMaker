#include "Helper.h"
int main(int argc, char* argv[]){
	
	TString year = argv[1];
	TString isdata = argv[2];
	TString mass_name = argv[3];
	string low = argv[4];
	string high = argv[5];
	TString binned = "binned";
	string init_mean = argv[6];
	string buffer = argv[7];
	TString plotpath = argv[8];//the base dir
	TString fs = argv[9];
	
	//recast the std global histograms for electrons
	if (fs == "2e"){
		eta_low = -2.5;
		eta_high = 2.5;
		h_eta_std->SetBins(etabinN,eta_low,eta_high);
		pt1_low = 7;
		pt2_low = 7;
		separation = false;
	}


	TString subname;
	if(fs == "2e"){
		//if(isdata=="mc" && year == "2017" )subname="crab_DYJetsToLL_M50_2017_new_2e_slimmed.root";	//NLO
		if(isdata=="mc" && year == "2017" )subname="crab_DYJetsToLL_M50_LO_2017_2e_slimmed.root";	//LO
		if(isdata=="data" && year == "2017")subname="crab_SingleElectron_Run17_2e_slimmed.root";

		//if(isdata=="mc" && year == "2018")subname="crab_DYJetsToLL_M50_2018_2e_slimmed.root";		//NLO
		if(isdata=="mc" && year == "2018")subname="crab_DYJetsToLL_M50_LO_2018_2e_slimmed.root";	//LO
		if(isdata=="data" && year == "2018")subname="crab_EGamma_Run18_2e_slimmed.root";
		
		//if(isdata=="mc" && year == "2016pre")subname="crab_DYJetsToLL_M50_2016APV_2e_slimmed.root";	//NLO
		if(isdata=="mc" && year == "2016pre")subname="crab_DYJetsToLL_M50_LO_2016APV_2e_slimmed.root";	//LO
		if(isdata=="data" && year == "2016pre")subname="crab_SingleElectron_Run2016_UL2016HIPM_2e_slimmed.root";
		
		//if(isdata=="mc" && year == "2016post")subname="crab_DYJetsToLL_M50_2016_2e_slimmed.root";	//NLO
		if(isdata=="mc" && year == "2016post")subname="crab_DYJetsToLL_M50_LO_2016_2e_slimmed.root";	//LO
		if(isdata=="data" && year == "2016post")subname="crab_SingleElectron_Run2016_UL2016_2e_slimmed.root";
	}
	if(fs == "2mu"){
		if(isdata=="mc" && year == "2018" )subname="crab_DYJetsToLL_amcatnlo_20UL18_2mu_slimmed.root";
		if(isdata=="data" && year == "2018")subname="crab_SingleMuon_Run2018_UL2018_2mu_slimmed.root";
		if(isdata=="mc" && year == "2017")subname="crab_DYJetsToLL_amcatnlo_20UL17_2mu_slimmed.root";
		if(isdata=="data" && year == "2017")subname="crab_SingleMuon_Run2017_UL2017_2mu_slimmed.root";

	}
	TString inputpath = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/tmp/" + subname;
	TString outputpath = plotpath;

	//double ptbins[7] = {std::0,20,30,40,50,60,pt_max};	//update to be global veriables in Healper.h
	//TH1D* h_eta_std = new TH1D("h_eta_std","",8,eta_low,eta_high);
	//TH1D* h_phi_std = new TH1D("h_phi_std","",8,-M_PI,M_PI);
	//TH1D* h_pt_std = new TH1D("h_pt_std","",6,ptbins);
	
	TH1D* tot_pos_eta = new TH1D("pos_eta","",100,eta_low,eta_high);	//all positive leptons eta distribution
	TH1D* tot_neg_eta = new TH1D("neg_eta","",100,eta_low,eta_high);	//all negative leptons eta distribution
	TH1D* tot_pos_phi = new TH1D("pos_phi","",100,-3.14,3.14);		//all positive leptons phi distribution
	TH1D* tot_neg_phi = new TH1D("neg_phi","",100,-3.14,3.14);		//all negative leptons phi distribution
	TH1D* tot_pos_pt = new TH1D("pos_pt","",100,0,pt_max);			//all positive leptons pt distribution
	TH1D* tot_neg_pt = new TH1D("neg_pt","",100,0,pt_max);			//all negative leptons pt distribution
	TH1D* tot_zpt = new TH1D("zpt","",100,0,100);						//all Z bosons pt distribution
	TH1D* tot_zpz = new TH1D("zpz","",500,-1000,1000);					//all Z bosons pz distribution
		
	//TH2D* pos_eta_vs_mass = new TH2D("pos_eta_vs_mass","",100,-2.4,2.4,100,86,96);
	//TH2D* neg_eta_vs_mass = new TH2D("neg_eta_vs_mass","",100,-2.4,2.4,100,86,96);
	//TH2D* pos_phi_vs_mass = new TH2D("pos_phi_vs_mass","",100,-3.14,3.14,100,86,96);
	//TH2D* neg_phi_vs_mass = new TH2D("neg_phi_vs_mass","",100,-3.14,3.14,100,86,96);

	vector<TH1D*> eta_pos_list; eta_pos_list.clear();		//Z boson mass distributions vs. lepton charge, lepton eta, phi and pt bins
	vector<TH1D*> eta_neg_list; eta_neg_list.clear();
	vector<TH1D*> phi_pos_list; phi_pos_list.clear();
	vector<TH1D*> phi_neg_list; phi_neg_list.clear();
	vector<TH1D*> pt_pos_list; pt_pos_list.clear();
	vector<TH1D*> pt_neg_list; pt_neg_list.clear();

	vector<TH1D*> eta_pos_zpt_list; eta_pos_zpt_list.clear();	//Z boson zpt distributions vs. lepton charge, lepton eta, phi, and pt bins
	vector<TH1D*> eta_neg_zpt_list; eta_neg_zpt_list.clear();
	vector<TH1D*> phi_pos_zpt_list; phi_pos_zpt_list.clear();
	vector<TH1D*> phi_neg_zpt_list; phi_neg_zpt_list.clear();
	vector<TH1D*> pt_pos_zpt_list; pt_pos_zpt_list.clear();
	vector<TH1D*> pt_neg_zpt_list; pt_neg_zpt_list.clear();
		
	vector<TH1D*> eta_pos_zeta_list; eta_pos_zeta_list.clear();	//Z boson zeta distributions vs. lepton charge, lepton eta, phi, and pt bins
	vector<TH1D*> eta_neg_zeta_list; eta_neg_zeta_list.clear();
	vector<TH1D*> phi_pos_zeta_list; phi_pos_zeta_list.clear();
	vector<TH1D*> phi_neg_zeta_list; phi_neg_zeta_list.clear();
	vector<TH1D*> pt_pos_zeta_list; pt_pos_zeta_list.clear();
	vector<TH1D*> pt_neg_zeta_list; pt_neg_zeta_list.clear();

	vector<TH1D*> eta_pos_zphi_list; eta_pos_zphi_list.clear();	//Z boson zphi distributions vs. lepton charge, lepton eta, phi, and pt bins
	vector<TH1D*> eta_neg_zphi_list; eta_neg_zphi_list.clear();
	vector<TH1D*> phi_pos_zphi_list; phi_pos_zphi_list.clear();
	vector<TH1D*> phi_neg_zphi_list; phi_neg_zphi_list.clear();
	vector<TH1D*> pt_pos_zphi_list; pt_pos_zphi_list.clear();
	vector<TH1D*> pt_neg_zphi_list; pt_neg_zphi_list.clear();

	vector<TH1D*> eta_pos_angle_list; eta_pos_angle_list.clear();	//angle between two leptons distributions vs. lepton charge, lepton eta, phi, and pt bins
	vector<TH1D*> eta_neg_angle_list; eta_neg_angle_list.clear();
	vector<TH1D*> phi_pos_angle_list; phi_pos_angle_list.clear();
	vector<TH1D*> phi_neg_angle_list; phi_neg_angle_list.clear();
	vector<TH1D*> pt_pos_angle_list; pt_pos_angle_list.clear();
	vector<TH1D*> pt_neg_angle_list; pt_neg_angle_list.clear();

	vector<TH1D*> eta_pos_zpz_list; eta_pos_zpz_list.clear();	//Z boson zpz distributions vs. lepton charge, lepton eta, phi, and pt bins
	vector<TH1D*> eta_neg_zpz_list; eta_neg_zpz_list.clear();
	vector<TH1D*> phi_pos_zpz_list; phi_pos_zpz_list.clear();
	vector<TH1D*> phi_neg_zpz_list; phi_neg_zpz_list.clear();
	vector<TH1D*> pt_pos_zpz_list; pt_pos_zpz_list.clear();
	vector<TH1D*> pt_neg_zpz_list; pt_neg_zpz_list.clear();

	vector<TH1D*> eta_pos_free_pt_list; eta_pos_free_pt_list.clear();	//pt distribution vs. fix x free x pos. x neg x eta bins
	vector<TH1D*> eta_pos_fix_pt_list; eta_pos_fix_pt_list.clear();
	vector<TH1D*> eta_neg_free_pt_list; eta_neg_free_pt_list.clear();
	vector<TH1D*> eta_neg_fix_pt_list; eta_neg_fix_pt_list.clear();

	vector<TH1D*> eta_pos_free_eta_list; eta_pos_free_eta_list.clear();	//eta distribution vs. fix x free x pos. x neg x eta bins
	vector<TH1D*> eta_pos_fix_eta_list; eta_pos_fix_eta_list.clear();
	vector<TH1D*> eta_neg_free_eta_list; eta_neg_free_eta_list.clear();
	vector<TH1D*> eta_neg_fix_eta_list; eta_neg_fix_eta_list.clear();

	vector<TH1D*> eta_pos_free_phi_list; eta_pos_free_phi_list.clear();	//phi distribution vs. fix x free x pos. x neg x eta bins
	vector<TH1D*> eta_pos_fix_phi_list; eta_pos_fix_phi_list.clear();
	vector<TH1D*> eta_neg_free_phi_list; eta_neg_free_phi_list.clear();
	vector<TH1D*> eta_neg_fix_phi_list; eta_neg_fix_phi_list.clear();

	vector<TH1D*> phi_pos_free_pt_list; phi_pos_free_pt_list.clear();	//pt distribution vs. fix x free x pos. x neg x phi bins
	vector<TH1D*> phi_pos_fix_pt_list; phi_pos_fix_pt_list.clear();
	vector<TH1D*> phi_neg_free_pt_list; phi_neg_free_pt_list.clear();
	vector<TH1D*> phi_neg_fix_pt_list; phi_neg_fix_pt_list.clear();
	
	vector<TH1D*> phi_pos_free_eta_list; phi_pos_free_eta_list.clear();	//eta distribution vs. fix x free x pos. x neg x phi bins
	vector<TH1D*> phi_pos_fix_eta_list; phi_pos_fix_eta_list.clear();
	vector<TH1D*> phi_neg_free_eta_list; phi_neg_free_eta_list.clear();
	vector<TH1D*> phi_neg_fix_eta_list; phi_neg_fix_eta_list.clear();

	vector<TH1D*> phi_pos_free_phi_list; phi_pos_free_phi_list.clear();	//phi distribution vs. fix x free x pos. x neg x phi bins
	vector<TH1D*> phi_pos_fix_phi_list; phi_pos_fix_phi_list.clear();
	vector<TH1D*> phi_neg_free_phi_list; phi_neg_free_phi_list.clear();
	vector<TH1D*> phi_neg_fix_phi_list; phi_neg_fix_phi_list.clear();
	
	vector<TH1D*> pt_pos_free_pt_list; pt_pos_free_pt_list.clear();		//pt distribution vs. fix x free x pos. x neg x pt bins
	vector<TH1D*> pt_pos_fix_pt_list; pt_pos_fix_pt_list.clear();
	vector<TH1D*> pt_neg_free_pt_list; pt_neg_free_pt_list.clear();
	vector<TH1D*> pt_neg_fix_pt_list; pt_neg_fix_pt_list.clear();

	vector<TH1D*> pt_pos_free_eta_list; pt_pos_free_eta_list.clear();	//eta distribution vs. fix x free x pos. x neg x pt bins
	vector<TH1D*> pt_pos_fix_eta_list; pt_pos_fix_eta_list.clear();
	vector<TH1D*> pt_neg_free_eta_list; pt_neg_free_eta_list.clear();
	vector<TH1D*> pt_neg_fix_eta_list; pt_neg_fix_eta_list.clear();
	
	vector<TH1D*> pt_pos_free_phi_list; pt_pos_free_phi_list.clear();	//phi distribution vs. fix x free x pos. x neg x pt bins
	vector<TH1D*> pt_pos_fix_phi_list; pt_pos_fix_phi_list.clear();
	vector<TH1D*> pt_neg_free_phi_list; pt_neg_free_phi_list.clear();
	vector<TH1D*> pt_neg_fix_phi_list; pt_neg_fix_phi_list.clear();
	
	vector<TH1D*> pterr_pos_phi_list; pterr_pos_phi_list.clear();
	vector<TH1D*> pterr_neg_phi_list; pterr_neg_phi_list.clear();
	vector<TH1D*> pterr_pos_eta_list; pterr_pos_eta_list.clear();
	vector<TH1D*> pterr_neg_eta_list; pterr_neg_eta_list.clear();
	vector<TH1D*> pterr_pos_pt_list; pterr_pos_pt_list.clear();
	vector<TH1D*> pterr_neg_pt_list; pterr_neg_pt_list.clear();
	

	vector<TH1D*> pterrold_pos_phi_list; pterrold_pos_phi_list.clear();
	vector<TH1D*> pterrold_neg_phi_list; pterrold_neg_phi_list.clear();
	vector<TH1D*> pterrold_pos_eta_list; pterrold_pos_eta_list.clear();
	vector<TH1D*> pterrold_neg_eta_list; pterrold_neg_eta_list.clear();
	vector<TH1D*> pterrold_pos_pt_list; pterrold_pos_pt_list.clear();
	vector<TH1D*> pterrold_neg_pt_list; pterrold_neg_pt_list.clear();
	
	for(int i=0; i<etabinN; i++){

		TString i_s = to_string(i);
		TH1D* h_eta_pos = new TH1D("eta_pos_"+i_s,"",int(((stof(high)-stof(low))*1000.0)/10.0),stof(low),stof(high));
		TH1D* h_eta_neg = new TH1D("eta_neg_"+i_s,"",int(((stof(high)-stof(low))*1000.0)/10.0),stof(low),stof(high));
		TH1D* h_phi_pos = new TH1D("phi_pos_"+i_s,"",int(((stof(high)-stof(low))*1000.0)/10.0),stof(low),stof(high));
		TH1D* h_phi_neg = new TH1D("phi_neg_"+i_s,"",int(((stof(high)-stof(low))*1000.0)/10.0),stof(low),stof(high));
		h_eta_pos->Sumw2(kTRUE);
		h_eta_neg->Sumw2(kTRUE);
		h_phi_pos->Sumw2(kTRUE);
		h_phi_neg->Sumw2(kTRUE);
				
		TH1D* h_eta_pos_zpt = new TH1D("eta_pos_zpt_"+i_s,"",100,0,100);
		TH1D* h_eta_neg_zpt = new TH1D("eta_neg_zpt_"+i_s,"",100,0,100);
		TH1D* h_phi_pos_zpt = new TH1D("phi_pos_zpt_"+i_s,"",100,0,100);
		TH1D* h_phi_neg_zpt = new TH1D("phi_neg_zpt_"+i_s,"",100,0,100);

		TH1D* h_eta_pos_ang = new TH1D("eta_pos_ang_"+i_s,"",100,0,3.14);
		TH1D* h_eta_neg_ang = new TH1D("eta_neg_ang_"+i_s,"",100,0,3.14);
		TH1D* h_phi_pos_ang = new TH1D("phi_pos_ang_"+i_s,"",100,0,3.14);
		TH1D* h_phi_neg_ang = new TH1D("phi_neg_ang_"+i_s,"",100,0,3.14);
				
		TH1D* h_eta_pos_zphi = new TH1D("eta_pos_zphi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_eta_neg_zphi = new TH1D("eta_neg_zphi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_phi_pos_zphi = new TH1D("phi_pos_zphi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_phi_neg_zphi = new TH1D("phi_neg_zphi_"+i_s,"",100,-3.14,3.14);

		TH1D* h_eta_pos_zeta = new TH1D("eta_pos_zeta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_eta_neg_zeta = new TH1D("eta_neg_zeta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_phi_pos_zeta = new TH1D("phi_pos_zeta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_phi_neg_zeta = new TH1D("phi_neg_zeta_"+i_s,"",100,-2.4,2.4);

		TH1D* h_eta_pos_zpz = new TH1D("eta_pos_zpz_"+i_s,"",500,-1000,1000);
		TH1D* h_eta_neg_zpz = new TH1D("eta_neg_zpz_"+i_s,"",500,-1000,1000);
		TH1D* h_phi_pos_zpz = new TH1D("phi_pos_zpz_"+i_s,"",500,-1000,1000);
		TH1D* h_phi_neg_zpz = new TH1D("phi_neg_zpz_"+i_s,"",500,-1000,1000);

		TH1D* h_eta_pos_free_pt = new TH1D("eta_pos_free_pt_"+i_s,"",100,0,100);
		TH1D* h_eta_pos_fix_pt = new TH1D("eta_pos_fix_pt_"+i_s,"",100,0,100);
		TH1D* h_eta_pos_free_eta = new TH1D("eta_pos_free_eta_"+i_s,"",100,eta_low,eta_high);
		TH1D* h_eta_pos_fix_eta = new TH1D("eta_pos_fix_eta_"+i_s,"",100,eta_low,eta_high);
		TH1D* h_eta_pos_free_phi = new TH1D("eta_pos_free_phi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_eta_pos_fix_phi = new TH1D("eta_pos_fix_phi_"+i_s,"",100,-3.14,3.14);
		
		TH1D* h_phi_pos_free_pt = new TH1D("phi_pos_free_pt_"+i_s,"",100,0,100);
		TH1D* h_phi_pos_fix_pt = new TH1D("phi_pos_fix_pt_"+i_s,"",100,0,100);
		TH1D* h_phi_pos_free_eta = new TH1D("phi_pos_free_eta_"+i_s,"",100,eta_low,eta_high);
		TH1D* h_phi_pos_fix_eta = new TH1D("phi_pos_fix_eta_"+i_s,"",100,eta_low,eta_high);
		TH1D* h_phi_pos_free_phi = new TH1D("phi_pos_free_phi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_phi_pos_fix_phi = new TH1D("phi_pos_fix_phi_"+i_s,"",100,-3.14,3.14);
		

		TH1D* h_eta_neg_free_pt = new TH1D("eta_neg_free_pt_"+i_s,"",100,0,100);
		TH1D* h_eta_neg_fix_pt = new TH1D("eta_neg_fix_pt_"+i_s,"",100,0,100);
		TH1D* h_eta_neg_free_eta = new TH1D("eta_neg_free_eta_"+i_s,"",100,eta_low,eta_high);
		TH1D* h_eta_neg_fix_eta = new TH1D("eta_neg_fix_eta_"+i_s,"",100,eta_low,eta_high);
		TH1D* h_eta_neg_free_phi = new TH1D("eta_neg_free_phi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_eta_neg_fix_phi = new TH1D("eta_neg_fix_phi_"+i_s,"",100,-3.14,3.14);
		
		TH1D* h_phi_neg_free_pt = new TH1D("phi_neg_free_pt_"+i_s,"",100,0,100);
		TH1D* h_phi_neg_fix_pt = new TH1D("phi_neg_fix_pt_"+i_s,"",100,0,100);
		TH1D* h_phi_neg_free_eta = new TH1D("phi_neg_free_eta_"+i_s,"",100,eta_low,eta_high);
		TH1D* h_phi_neg_fix_eta = new TH1D("phi_neg_fix_eta_"+i_s,"",100,eta_low,eta_high);
		TH1D* h_phi_neg_free_phi = new TH1D("phi_neg_free_phi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_phi_neg_fix_phi = new TH1D("phi_neg_fix_phi_"+i_s,"",100,-3.14,3.14);
		
		TH1D* h_pterr_pos_phi = new TH1D("pterr_pos_phi_"+i_s,"",100,0,6);
		TH1D* h_pterr_neg_phi = new TH1D("pterr_neg_phi_"+i_s,"",100,0,6);
		TH1D* h_pterr_pos_eta = new TH1D("pterr_pos_eta_"+i_s,"",100,0,6);
		TH1D* h_pterr_neg_eta = new TH1D("pterr_neg_eta_"+i_s,"",100,0,6);
		TH1D* h_pterr_pos_pt = new TH1D("pterr_pos_pt_"+i_s,"",100,0,6);
		TH1D* h_pterr_neg_pt = new TH1D("pterr_neg_pt_"+i_s,"",100,0,6);

		TH1D* h_pterrold_pos_phi = new TH1D("pterrold_pos_phi_"+i_s,"",100,0,6);
		TH1D* h_pterrold_neg_phi = new TH1D("pterrold_neg_phi_"+i_s,"",100,0,6);
		TH1D* h_pterrold_pos_eta = new TH1D("pterrold_pos_eta_"+i_s,"",100,0,6);
		TH1D* h_pterrold_neg_eta = new TH1D("pterrold_neg_eta_"+i_s,"",100,0,6);
		TH1D* h_pterrold_pos_pt = new TH1D("pterrold_pos_pt_"+i_s,"",100,0,6);
		TH1D* h_pterrold_neg_pt = new TH1D("pterrold_neg_pt_"+i_s,"",100,0,6);

		eta_pos_list.push_back(h_eta_pos);
		eta_neg_list.push_back(h_eta_neg);
		phi_pos_list.push_back(h_phi_pos);
		phi_neg_list.push_back(h_phi_neg);

		eta_pos_zpt_list.push_back(h_eta_pos_zpt);
		eta_neg_zpt_list.push_back(h_eta_neg_zpt);
		phi_pos_zpt_list.push_back(h_phi_pos_zpt);
		phi_neg_zpt_list.push_back(h_phi_neg_zpt);

		eta_pos_angle_list.push_back(h_eta_pos_ang);
		eta_neg_angle_list.push_back(h_eta_neg_ang);
		phi_pos_angle_list.push_back(h_phi_pos_ang);
		phi_neg_angle_list.push_back(h_phi_neg_ang);

		eta_pos_zeta_list.push_back(h_eta_pos_zeta);
		eta_neg_zeta_list.push_back(h_eta_neg_zeta);
		phi_pos_zeta_list.push_back(h_phi_pos_zeta);
		phi_neg_zeta_list.push_back(h_phi_neg_zeta);

		eta_pos_zpz_list.push_back(h_eta_pos_zpz);
		eta_neg_zpz_list.push_back(h_eta_neg_zpz);
		phi_pos_zpz_list.push_back(h_phi_pos_zpz);
		phi_neg_zpz_list.push_back(h_phi_neg_zpz);

		eta_pos_zphi_list.push_back(h_eta_pos_zphi);
		eta_neg_zphi_list.push_back(h_eta_neg_zphi);
		phi_pos_zphi_list.push_back(h_phi_pos_zphi);
		phi_neg_zphi_list.push_back(h_phi_neg_zphi);

		eta_pos_free_eta_list.push_back(h_eta_pos_free_eta);
		eta_pos_fix_eta_list.push_back(h_eta_pos_fix_eta);
		eta_pos_free_phi_list.push_back(h_eta_pos_free_phi);
		eta_pos_fix_phi_list.push_back(h_eta_pos_fix_phi);
		eta_pos_free_pt_list.push_back(h_eta_pos_free_pt);
		eta_pos_fix_pt_list.push_back(h_eta_pos_fix_pt);

		eta_neg_free_eta_list.push_back(h_eta_neg_free_eta);
		eta_neg_fix_eta_list.push_back(h_eta_neg_fix_eta);
		eta_neg_free_phi_list.push_back(h_eta_neg_free_phi);
		eta_neg_fix_phi_list.push_back(h_eta_neg_fix_phi);
		eta_neg_free_pt_list.push_back(h_eta_neg_free_pt);
		eta_neg_fix_pt_list.push_back(h_eta_neg_fix_pt);

		phi_pos_free_eta_list.push_back(h_phi_pos_free_eta);
		phi_pos_fix_eta_list.push_back(h_phi_pos_fix_eta);
		phi_pos_free_phi_list.push_back(h_phi_pos_free_phi);
		phi_pos_fix_phi_list.push_back(h_phi_pos_fix_phi);
		phi_pos_free_pt_list.push_back(h_phi_pos_free_pt);
		phi_pos_fix_pt_list.push_back(h_phi_pos_fix_pt);

		phi_neg_free_eta_list.push_back(h_phi_neg_free_eta);
		phi_neg_fix_eta_list.push_back(h_phi_neg_fix_eta);
		phi_neg_free_phi_list.push_back(h_phi_neg_free_phi);
		phi_neg_fix_phi_list.push_back(h_phi_neg_fix_phi);
		phi_neg_free_pt_list.push_back(h_phi_neg_free_pt);
		phi_neg_fix_pt_list.push_back(h_phi_neg_fix_pt);
		
		pterr_pos_phi_list.push_back(h_pterr_pos_phi);
		pterr_neg_phi_list.push_back(h_pterr_neg_phi);
		pterr_pos_eta_list.push_back(h_pterr_pos_eta);
		pterr_neg_eta_list.push_back(h_pterr_neg_eta);

		pterrold_pos_phi_list.push_back(h_pterrold_pos_phi);
		pterrold_neg_phi_list.push_back(h_pterrold_neg_phi);
		pterrold_pos_eta_list.push_back(h_pterrold_pos_eta);
		pterrold_neg_eta_list.push_back(h_pterrold_neg_eta);

		if(i<ptbinN){
			TH1D* h_pt_pos = new TH1D("pt_pos_"+i_s,"",int((stof(high)-stof(low))*1000.0/10.0),stof(low),stof(high));
			TH1D* h_pt_neg = new TH1D("pt_neg_"+i_s,"",int((stof(high)-stof(low))*1000.0/10.0),stof(low),stof(high));
			TH1D* h_pt_pos_zpt = new TH1D("pt_pos_zpt_"+i_s,"",100,0,100);
			TH1D* h_pt_neg_zpt = new TH1D("pt_neg_zpt_"+i_s,"",100,0,100);
			TH1D* h_pt_pos_ang = new TH1D("pt_pos_ang_"+i_s,"",100,0,3.14);
			TH1D* h_pt_neg_ang = new TH1D("pt_neg_ang_"+i_s,"",100,0,3.14);
			TH1D* h_pt_pos_zphi = new TH1D("pt_pos_zphi_"+i_s,"",100,-3.14,3.14);
			TH1D* h_pt_neg_zphi = new TH1D("pt_neg_zphi_"+i_s,"",100,-3.14,3.14);
			TH1D* h_pt_pos_zeta = new TH1D("pt_pos_zeta_"+i_s,"",100,-2.4,2.4);
			TH1D* h_pt_neg_zeta = new TH1D("pt_neg_zeta_"+i_s,"",100,-2.4,2.4);
			TH1D* h_pt_pos_zpz = new TH1D("pt_pos_zpz_"+i_s,"",500,-1000,1000);
			TH1D* h_pt_neg_zpz = new TH1D("pt_neg_zpz_"+i_s,"",500,-1000,1000);
			TH1D* h_pt_pos_free_pt = new TH1D("pt_pos_free_pt_"+i_s,"",100,0,100);
			TH1D* h_pt_pos_fix_pt = new TH1D("pt_pos_fix_pt_"+i_s,"",100,0,100);
			TH1D* h_pt_pos_free_eta = new TH1D("pt_pos_free_eta_"+i_s,"",100,eta_low,eta_high);
			TH1D* h_pt_pos_fix_eta = new TH1D("pt_pos_fix_eta_"+i_s,"",100,eta_low,eta_high);
			TH1D* h_pt_pos_free_phi = new TH1D("pt_pos_free_phi_"+i_s,"",100,-3.14,3.14);
			TH1D* h_pt_pos_fix_phi = new TH1D("pt_pos_fix_phi_"+i_s,"",100,-3.14,3.14);
			TH1D* h_pt_neg_free_pt = new TH1D("pt_neg_free_pt_"+i_s,"",100,0,100);
			TH1D* h_pt_neg_fix_pt = new TH1D("pt_neg_fix_pt_"+i_s,"",100,0,100);
			TH1D* h_pt_neg_free_eta = new TH1D("pt_neg_free_eta_"+i_s,"",100,eta_low,eta_high);
			TH1D* h_pt_neg_fix_eta = new TH1D("pt_neg_fix_eta_"+i_s,"",100,eta_low,eta_high);
			TH1D* h_pt_neg_free_phi = new TH1D("pt_neg_free_phi_"+i_s,"",100,-3.14,3.14);
			TH1D* h_pt_neg_fix_phi = new TH1D("pt_neg_fix_phi_"+i_s,"",100,-3.14,3.14);

			pt_pos_list.push_back(h_pt_pos);
			pt_neg_list.push_back(h_pt_neg);
			pt_pos_zpt_list.push_back(h_pt_pos_zpt);
			pt_neg_zpt_list.push_back(h_pt_neg_zpt);
			pt_pos_angle_list.push_back(h_pt_pos_ang);
			pt_neg_angle_list.push_back(h_pt_neg_ang);
			pt_pos_zeta_list.push_back(h_pt_pos_zeta);
			pt_neg_zeta_list.push_back(h_pt_neg_zeta);
			pt_pos_zpz_list.push_back(h_pt_pos_zpz);
			pt_neg_zpz_list.push_back(h_pt_neg_zpz);
			pt_pos_zphi_list.push_back(h_pt_pos_zphi);
			pt_neg_zphi_list.push_back(h_pt_neg_zphi);
			pt_pos_free_pt_list.push_back(h_pt_pos_free_pt);
			pt_pos_fix_pt_list.push_back(h_pt_pos_fix_pt);
			pt_pos_free_eta_list.push_back(h_pt_pos_free_eta);
			pt_pos_fix_eta_list.push_back(h_pt_pos_fix_eta);
			pt_pos_free_phi_list.push_back(h_pt_pos_free_phi);
			pt_pos_fix_phi_list.push_back(h_pt_pos_fix_phi);
			pt_neg_free_pt_list.push_back(h_pt_neg_free_pt);
			pt_neg_fix_pt_list.push_back(h_pt_neg_fix_pt);
			pt_neg_free_eta_list.push_back(h_pt_neg_free_eta);
			pt_neg_fix_eta_list.push_back(h_pt_neg_fix_eta);
			pt_neg_free_phi_list.push_back(h_pt_neg_free_phi);
			pt_neg_fix_phi_list.push_back(h_pt_neg_fix_phi);
			pterr_pos_pt_list.push_back(h_pterr_pos_pt);
			pterr_neg_pt_list.push_back(h_pterr_neg_pt);
			pterrold_pos_pt_list.push_back(h_pterrold_pos_pt);
			pterrold_neg_pt_list.push_back(h_pterrold_neg_pt);
		}

	}
	readtree_(year,fs,inputpath,outputpath,isdata,mass_name,binned,low,high,stof(init_mean), stof(buffer),
			
			h_eta_std,h_phi_std,h_pt_std,
			
			eta_pos_list,eta_neg_list,phi_pos_list,phi_neg_list,pt_pos_list,pt_neg_list,						//mass templates holder
			
			eta_pos_free_eta_list, eta_pos_fix_eta_list,										//fix vs free
			eta_pos_free_phi_list, eta_pos_fix_phi_list,
			eta_pos_free_pt_list, eta_pos_fix_pt_list,
			
			phi_pos_free_eta_list, phi_pos_fix_eta_list,
			phi_pos_free_phi_list, phi_pos_fix_phi_list,
			phi_pos_free_pt_list, phi_pos_fix_pt_list,

			pt_pos_free_eta_list, pt_pos_fix_eta_list,
			pt_pos_free_phi_list, pt_pos_fix_phi_list,
			pt_pos_free_pt_list, pt_pos_fix_pt_list,
			
			eta_neg_free_eta_list, eta_neg_fix_eta_list,
			eta_neg_free_phi_list, eta_neg_fix_phi_list,
			eta_neg_free_pt_list, eta_neg_fix_pt_list,
			
			phi_neg_free_eta_list, phi_neg_fix_eta_list,
			phi_neg_free_phi_list, phi_neg_fix_phi_list,
			phi_neg_free_pt_list, phi_neg_fix_pt_list,

			pt_neg_free_eta_list, pt_neg_fix_eta_list,
			pt_neg_free_phi_list, pt_neg_fix_phi_list,
			pt_neg_free_pt_list, pt_neg_fix_pt_list,										//fix vs free end
			

			eta_pos_zeta_list, eta_neg_zeta_list,											//z boson kinematics
			phi_pos_zeta_list, phi_neg_zeta_list,
			pt_pos_zeta_list, pt_neg_zeta_list,

			eta_pos_zphi_list, eta_neg_zphi_list,
			phi_pos_zphi_list, phi_neg_zphi_list,
			pt_pos_zphi_list, pt_neg_zphi_list,

			eta_pos_zpz_list, eta_neg_zpz_list,
			phi_pos_zpz_list, phi_neg_zpz_list,
			pt_pos_zpz_list, pt_neg_zpz_list,

			eta_pos_zpt_list, eta_neg_zpt_list, 
			phi_pos_zpt_list, phi_neg_zpt_list,
			pt_pos_zpt_list, pt_neg_zpt_list,
			
			eta_pos_angle_list, eta_neg_angle_list, 
			phi_pos_angle_list, phi_neg_angle_list,
			pt_pos_angle_list, pt_neg_angle_list,
			
			pterr_pos_phi_list, pterr_neg_phi_list,
			pterr_pos_eta_list, pterr_neg_eta_list,
			pterr_pos_pt_list, pterr_neg_pt_list,

			pterrold_pos_phi_list, pterrold_neg_phi_list,
			pterrold_pos_eta_list, pterrold_neg_phi_list,
			pterrold_pos_pt_list, pterrold_neg_pt_list,
				
			tot_pos_eta, tot_neg_eta,
			tot_pos_phi, tot_neg_phi,
			tot_pos_pt, tot_neg_pt,
			
			tot_zpt, tot_zpz

			//pos_eta_vs_mass, neg_eta_vs_mass,
			//pos_phi_vs_mass, neg_phi_vs_mass,
		);		
			
}

void readtree_(TString year, TString fs, TString inputpath, TString outputpath, TString isdata, TString mass_name, TString binned, string low, string high, float init_mean, float buffer,
	
	TH1D* h_eta_std, TH1D* h_phi_std, TH1D* h_pt_std, 
	
	vector<TH1D*> &eta_pos_list, vector<TH1D*> &eta_neg_list, vector<TH1D*> &phi_pos_list, vector<TH1D*> &phi_neg_list, vector<TH1D*> &pt_pos_list, vector<TH1D*> &pt_neg_list,
	
	vector<TH1D*> &eta_pos_free_eta_list, vector<TH1D*> &eta_pos_fix_eta_list,
	vector<TH1D*> &eta_pos_free_phi_list, vector<TH1D*> &eta_pos_fix_phi_list,
	vector<TH1D*> &eta_pos_free_pt_list, vector<TH1D*> &eta_pos_fix_pt_list,
	
	vector<TH1D*> &phi_pos_free_eta_list, vector<TH1D*> &phi_pos_fix_eta_list,
	vector<TH1D*> &phi_pos_free_phi_list, vector<TH1D*> &phi_pos_fix_phi_list,
	vector<TH1D*> &phi_pos_free_pt_list, vector<TH1D*> &phi_pos_fix_pt_list,
	
	vector<TH1D*> &pt_pos_free_eta_list, vector<TH1D*> &pt_pos_fix_eta_list,
	vector<TH1D*> &pt_pos_free_phi_list, vector<TH1D*> &pt_pos_fix_phi_list,
	vector<TH1D*> &pt_pos_free_pt_list, vector<TH1D*> &pt_pos_fix_pt_list,

	vector<TH1D*> &eta_neg_free_eta_list, vector<TH1D*> &eta_neg_fix_eta_list,
	vector<TH1D*> &eta_neg_free_phi_list, vector<TH1D*> &eta_neg_fix_phi_list,
	vector<TH1D*> &eta_neg_free_pt_list, vector<TH1D*> &eta_neg_fix_pt_list,
	
	vector<TH1D*> &phi_neg_free_eta_list, vector<TH1D*> &phi_neg_fix_eta_list,
	vector<TH1D*> &phi_neg_free_phi_list, vector<TH1D*> &phi_neg_fix_phi_list,
	vector<TH1D*> &phi_neg_free_pt_list, vector<TH1D*> &phi_neg_fix_pt_list,

	vector<TH1D*> &pt_neg_free_eta_list, vector<TH1D*> &pt_neg_fix_eta_list,
	vector<TH1D*> &pt_neg_free_phi_list, vector<TH1D*> &pt_neg_fix_phi_list,
	vector<TH1D*> &pt_neg_free_pt_list, vector<TH1D*> &pt_neg_fix_pt_list,
	
	vector<TH1D*> &eta_pos_zeta_list, vector<TH1D*> &eta_neg_zeta_list,
	vector<TH1D*> &phi_pos_zeta_list, vector<TH1D*> &phi_neg_zeta_list,
	vector<TH1D*> &pt_pos_zeta_list, vector<TH1D*> &pt_neg_zeta_list,

	vector<TH1D*> &eta_pos_zphi_list, vector<TH1D*> &eta_neg_zphi_list,
	vector<TH1D*> &phi_pos_zphi_list, vector<TH1D*> &phi_neg_zphi_list,
	vector<TH1D*> &pt_pos_zphi_list, vector<TH1D*> &pt_neg_zphi_list,
	
	vector<TH1D*> &eta_pos_zpz_list, vector<TH1D*> &eta_neg_zpz_list,
	vector<TH1D*> &phi_pos_zpz_list, vector<TH1D*> &phi_neg_zpz_list,
	vector<TH1D*> &pt_pos_zpz_list, vector<TH1D*> &pt_neg_zpz_list,
	
	vector<TH1D*> &eta_pos_zpt_list, vector<TH1D*> &eta_neg_zpt_list,
	vector<TH1D*> &phi_pos_zpt_list, vector<TH1D*> &phi_neg_zpt_list,
	vector<TH1D*> &pt_pos_zpt_list, vector<TH1D*> &pt_neg_zpt_list,
	
	vector<TH1D*> &eta_pos_angle_list, vector<TH1D*> &eta_neg_angle_list,
	vector<TH1D*> &phi_pos_angle_list, vector<TH1D*> &phi_neg_angle_list,
	vector<TH1D*> &pt_pos_angle_list, vector<TH1D*> &pt_neg_angle_list,
	
	vector<TH1D*> &pterr_pos_phi_list, vector<TH1D*> &pterr_neg_phi_list,
	vector<TH1D*> &pterr_pos_eta_list, vector<TH1D*> &pterr_neg_eta_list,
	vector<TH1D*> &pterr_pos_pt_list, vector<TH1D*> &pterr_neg_pt_list,
	
	vector<TH1D*> &pterrold_pos_phi_list, vector<TH1D*> &pterrold_neg_phi_list,
	vector<TH1D*> &pterrold_pos_eta_list, vector<TH1D*> &pterrold_neg_eta_list,
	vector<TH1D*> &pterrold_pos_pt_list, vector<TH1D*> &pterrold_neg_pt_list,
	
	TH1D* tot_pos_eta, TH1D* tot_neg_eta,
	TH1D* tot_pos_phi, TH1D* tot_neg_phi,
	TH1D* tot_pos_pt, TH1D* tot_neg_pt,
	//TH2D* pos_eta_vs_mass, TH2D* neg_eta_vs_mass,
	//TH2D* pos_phi_vs_mass, TH2D* neg_phi_vs_mass,
	TH1D* tot_zpt, TH1D* tot_zpz
){
	TString pt1name, pt2name, eta1name, eta2name, phi1name, phi2name, q1name, q2name, massname;
	if(GEN==false){
		if(mass_name == "mass2mu_zpt_dt"){
			pt1name = "pt1_zpt";
			pt2name = "pt2_zpt";
		}
		if(mass_name == "mass2mu_ewk_dt"){
			pt1name = "pt1_ewk";
			pt2name = "pt2_ewk";
		}
		if(mass_name == "mass2mu_ewk2_dt"){
			pt1name = "pt1_ewk2";
			pt2name = "pt2_ewk2";
		}
		if(mass_name == "mass2mu_dm_dt"){
			pt1name = "pt1_dm";
			pt2name = "pt2_dm";
		}
		if(mass_name == "mass2mu_ewk2"){
			pt1name = "pt1_ewk2";
			pt2name = "pt2_ewk2";
		}
		if(mass_name == "mass2mu_corr"){
			pt1name = "pt1_corr";
			pt2name = "pt2_corr";
		}
		if(mass_name == "mass2mu_uncorr"){
			pt1name = "pt1_uncorr";
			pt2name = "pt2_uncorr";
		}
		//
		if(mass_name == "mass2e_corr"){
			pt1name = "ele1_ptpostcorr";
			pt2name = "ele2_ptpostcorr";
		}
		if(mass_name == "mass2e_uncorr"){
			pt1name = "pt1";
			pt2name = "pt2";
		}
		if(mass_name == "mass2e_scaleup"){
			pt1name = "ele1_ptpostcorr";
			pt2name = "ele2_ptpostcorr";
		}
		if(mass_name == "mass2e_scaledn"){
			pt1name = "ele1_ptpostcorr";
			pt2name = "ele2_ptpostcorr";
		}
		if(mass_name == "mass2e_smearup"){
			pt1name = "ele1_ptpostcorr";
			pt2name = "ele2_ptpostcorr";
		}
		if(mass_name == "mass2e_smeardn"){
			pt1name = "ele1_ptpostcorr";
			pt2name = "ele2_ptpostcorr";
		}
		eta1name = "eta1"; eta2name = "eta2"; phi1name = "phi1"; phi2name = "phi2"; q1name = "q1"; q2name = "q2";
		//pt1name = "pt1_corr"; pt2name = "pt2_corr";
		massname = mass_name+"_fsr";
	}
	if(GEN==true){
		pt1name = "genlep1_pt"; pt2name = "genlep2_pt"; eta1name = "genlep1_eta"; eta2name = "genlep2_eta"; phi1name = "genlep1_phi"; phi2name = "genlep2_phi";
		q1name = "genlep1_q"; q2name = "genlep2_q";
		massname = "GENmassZ";
	}
	//set tree
	float pt1, pt2, eta1, eta2, phi1, phi2, pterr1, pterr2, pterrold1, pterrold2, energy1, energy2, mass;
	float Sip1, Sip2, RelIso1, RelIso2, weight, weight1, weight2, d01, d02, Zpt, Zy, genmassz;
	float eventweight, pileupweight, genweight, preweight, preweightecal, preweightmuoni, r91, r92;
	bool id1, id2, trigger1, trigger2, genmatching;
	int q1, q2, nFSRPhotons, Event, ele1_ecalDriven, ele2_ecalDriven;

	TFile* f = new TFile(inputpath,"READ");
	TTree* t = (TTree*)f->Get("passedEvents");
	t->SetBranchStatus("*",0);
	//t->SetBranchStatus("Event",1);
	//t->SetBranchAddress("Event",&Event);
	t->SetBranchStatus(pt1name,1);
	t->SetBranchStatus(pt2name,1);
	t->SetBranchStatus(eta1name,1);
	t->SetBranchStatus(eta2name,1);
	t->SetBranchStatus(phi1name,1);
	t->SetBranchStatus(phi2name,1);
	t->SetBranchStatus(q1name,1);
	t->SetBranchStatus(q2name,1);
	t->SetBranchStatus("Sip1",1);
	t->SetBranchStatus("Sip2",1);
	t->SetBranchStatus("RelIso1",1);
	t->SetBranchStatus("RelIso2",1);
	//t->SetBranchStatus("mu1_hzzid",1);
	//t->SetBranchStatus("mu2_hzzid",1);
	if(fs == "2mu"){
		t->SetBranchStatus("trigger24matching1",1);
		t->SetBranchStatus("trigger24matching2",1);
	}
	if(fs == "2e"){
		t->SetBranchStatus("ele_et1",1);
		t->SetBranchStatus("ele_et2",1);
		t->SetBranchStatus("lep1_ecalDriven",1);
		t->SetBranchStatus("lep2_ecalDriven",1);
		t->SetBranchStatus("ele_r91",1);
		t->SetBranchStatus("ele_r92",1);
	}
	
	t->SetBranchStatus("weight1",1);
	t->SetBranchStatus("weight2",1);
	t->SetBranchStatus("weight",1);
	
	//t->SetBranchStatus("eventweight",1);
	t->SetBranchStatus("pileupweight",1);
	//t->SetBranchStatus("genweight",1);
	//t->SetBranchStatus("preweight",1);
	//t->SetBranchStatus("preweightecal",1);
	//t->SetBranchStatus("preweightmuon",1);
	
	//t->SetBranchStatus("lep1_pterr",1);
	//t->SetBranchStatus("lep2_pterr",1);
	//t->SetBranchStatus("lep1_pterrold",1);
	//t->SetBranchStatus("lep2_pterrold",1);
	/*
	t->SetBranchStatus("genlep1_pt",1);
	t->SetBranchStatus("genlep2_pt",1);
	t->SetBranchStatus("genlep1_eta",1);
	t->SetBranchStatus("genlep2_eta",1);
	t->SetBranchStatus("genlep1_phi",1);
	t->SetBranchStatus("genlep2_phi",1);
	*/
	t->SetBranchStatus("nFSRPhotons",1);
	

	t->SetBranchAddress(pt1name,&pt1);
	t->SetBranchAddress(pt2name,&pt2);
	//t->SetBranchAddress("lep1_pterr",&pterr1);
	//t->SetBranchAddress("lep2_pterr",&pterr2);
	//t->SetBranchAddress("lep1_pterrold",&pterrold1);
	//t->SetBranchAddress("lep2_pterrold",&pterrold2);
	t->SetBranchAddress(eta1name,&eta1);
	t->SetBranchAddress(eta2name,&eta2);
	t->SetBranchAddress(phi1name,&phi1);
	t->SetBranchAddress(phi2name,&phi2);
	t->SetBranchAddress(q1name,&q1);
	t->SetBranchAddress(q2name,&q2);
	t->SetBranchAddress("Sip1",&Sip1);
	t->SetBranchAddress("Sip2",&Sip2);
	t->SetBranchAddress("RelIso1",&RelIso1);
	t->SetBranchAddress("RelIso2",&RelIso2);
	//t->SetBranchAddress("mu1_hzzid",&id1);
	//t->SetBranchAddress("mu2_hzzid",&id2);
	if( fs == "2mu" ){
		t->SetBranchAddress("trigger24matching1",&trigger1);
		t->SetBranchAddress("trigger24matching2",&trigger2);
	}
	if( fs == "2e" ){
		t->SetBranchAddress("ele_et1",&energy1);
		t->SetBranchAddress("ele_et2",&energy2);
		t->SetBranchAddress("lep1_ecalDriven",&ele1_ecalDriven);
		t->SetBranchAddress("lep2_ecalDriven",&ele2_ecalDriven);
		t->SetBranchAddress("ele_r91",&r91);
		t->SetBranchAddress("ele_r92",&r92);
	}
	//if( fs == "2mu" && isdata == "mc" && massname == "mass2mu_ewk2_fsr" ){
	//	t->SetBranchStatus("mass2mu_corr_fsr",1);
	//	t->SetBranchAddress("mass2mu_corr_fsr",&mass);
	//}else{
		t->SetBranchStatus(massname,1);
		t->SetBranchAddress(massname,&mass);
	//}
	t->SetBranchAddress("weight1",&weight1);
	t->SetBranchAddress("weight2",&weight2);
	t->SetBranchAddress("weight",&weight);
	
	//t->SetBranchAddress("eventweight",&eventweight);
	//t->SetBranchAddress("genweight",&genweight);
	t->SetBranchAddress("pileupweight",&pileupweight);
	//t->SetBranchAddress("preweight",&preweight);
	//t->SetBranchAddress("preweightecal",&preweightecal);
	//t->SetBranchAddress("preweightmuon",&preweightmuon);
	
	//t->SetBranchAddress("lep1_pterr",&pterr1);
	//t->SetBranchAddress("lep2_pterr",&pterr2);
	//t->SetBranchAddress("lep1_pterrold",&pterrold1);
	//t->SetBranchAddress("lep2_pterrold",&pterrold2);
	/*
	t->SetBranchAddress("genlep1_pt",&genpt1);
	t->SetBranchAddress("genlep2_pt",&genpt2);
	t->SetBranchAddress("genlep1_eta",&geneta1);
	t->SetBranchAddress("genlep2_eta",&geneta2);
	t->SetBranchAddress("genlep1_phi",&genphi1);
	t->SetBranchAddress("genlep2_phi",&genphi2);
	*/
	t->SetBranchAddress("nFSRPhotons",&nFSRPhotons);
	t->SetBranchStatus("lep1_dxy",1);
	t->SetBranchStatus("lep2_dxy",1);
	t->SetBranchAddress("lep1_dxy",&d01);
	t->SetBranchAddress("lep2_dxy",&d02);	
	t->SetBranchStatus("Zpt",1);
	t->SetBranchStatus("Zy",1);
	t->SetBranchAddress("Zpt",&Zpt);
	t->SetBranchAddress("Zy",&Zy);
	if(isdata=="mc"){
		t->SetBranchStatus("GENmassZ",1);
		t->SetBranchStatus("genmatching",1);
		t->SetBranchAddress("genmatching",&genmatching);
		t->SetBranchAddress("GENmassZ",&genmassz);
	}
	TAxis* eta_axis = h_eta_std->GetXaxis();
	TAxis* phi_axis = h_phi_std->GetXaxis();
	TAxis* pt_axis = h_pt_std->GetXaxis();
	int eta_pos_bin, eta_neg_bin, phi_neg_bin, phi_pos_bin, pt_pos_bin, pt_neg_bin;
	TLorentzVector lep1, lep2, Z;
	
	TFile* f_zpty = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Scale/Electron/cpp/reweight/lut/"+year+"_lotodata.root","READ");
	TH1F* lut_zpty = (TH1F*)f_zpty->Get("LUT");
	TAxis* zpt_ax = lut_zpty->GetXaxis();
	TAxis* zy_ax = lut_zpty->GetYaxis();
	double zmw = 1;
	
	float crosssection = 6225.4;
	
	float lumi = 0;
	float sumWeight = 0;
	if(year == "2016pre"){
		sumWeight = 61192713.0;
		lumi = 18.84;
	}
	if(year == "2016post"){
		sumWeight = 49693940.0;
		lumi = 16.81;
	}
	if(year == "2017"){
		sumWeight = 132089877.0;
		lumi = 41.8;
	}
	if(year == "2018"){
		sumWeight = 132322003.0;
		lumi = 59.83;
	}

	for(int i=0; i<t->GetEntries(); i++){
		if(i%1000000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		//if(i>10000)break;
		t->GetEntry(i);
		//if(isdata=="mc"&&!genmatching&&genmassz>120&&genmassz<60)continue;
		if(fs == "2mu"){
			ecalDriven_cut = true;
			trigger_cut = ( trigger1 || trigger2 );
			pt_cut = ( pt1>pt1_low && pt2>pt2_low && pt1<pt_max && pt2<pt_max );
		}
		if(fs == "2e"){
			ecalDriven_cut = ( ele1_ecalDriven && ele2_ecalDriven );
			trigger_cut = true;
			pt_cut = ( pt1>pt1_low && pt2>pt2_low && energy1>energy1_low && energy2>energy2_low && pt1<pt_max && pt2<pt_max );
		}
		if((r91+r92)<2 && RelIso1<0.35 &&RelIso2<0.35 && Sip1<4 && Sip2<4 && trigger_cut && ecalDriven_cut && pt_cut && abs(eta1)<eta_high && abs(eta2)<eta_high && mass<stof(high)&&mass>stof(low) ){
			zmw = pileupweight;
			//if(fs == "2e" && abs(eta1)>1.444 && abs(eta1)<1.566) continue;
			//if(fs == "2e" && abs(eta2)>1.444 && abs(eta2)<1.566) continue;
			if(isdata=="mc"){
				int zxbin = zpt_ax->FindBin(Zpt);
				int zybin = zy_ax->FindBin(Zy);
				zmw = zmw * lut_zpty->GetBinContent(zxbin,zybin);
			}
			//weight = weight * zptyw;
			//if(pt1<15 || pt2<15) continue;
			//if(isdata == "mc")weight = weight * pileupweight * genweight * preweight * crosssection * 1000 * lumi / sumWeight;
			//if(isdata == "data")weight = 1.0;
			if( separation == false){	
				eta_pos_bin = eta_axis->FindBin(eta1)-1;
				eta_neg_bin = eta_axis->FindBin(eta2)-1;
				phi_pos_bin = phi_axis->FindBin((r91+r92))-1;
				phi_neg_bin = phi_axis->FindBin((r92+r91))-1;
				pt_pos_bin = pt_axis->FindBin(pt1)-1;
				pt_neg_bin = pt_axis->FindBin(pt2)-1;			//actually, you don't know positive/negative is positive/negative or not
				
				eta_neg_list.at(eta_pos_bin)->Fill(mass,zmw);
				if(eta_pos_bin != eta_neg_bin){
					eta_neg_list.at(eta_neg_bin)->Fill(mass,zmw);
				}
				phi_neg_list.at(phi_pos_bin)->Fill(mass,zmw);
				if(phi_neg_bin != phi_neg_bin){
					phi_neg_list.at(phi_neg_bin)->Fill(mass,zmw);
				}
				pt_neg_list.at(pt_pos_bin)->Fill(mass,zmw);
				if(pt_neg_bin != pt_pos_bin ){
					pt_neg_list.at(pt_neg_bin)->Fill(mass,zmw);
				}
			}

			if(q1==1 && separation == true){
				
				eta_pos_bin = eta_axis->FindBin(eta1)-1;
				eta_neg_bin = eta_axis->FindBin(eta2)-1;
				phi_pos_bin = phi_axis->FindBin(phi1)-1;
				phi_neg_bin = phi_axis->FindBin(phi2)-1;
				pt_pos_bin = pt_axis->FindBin(pt1)-1;
				pt_neg_bin = pt_axis->FindBin(pt2)-1;
				eta_pos_list.at(eta_pos_bin)->Fill(mass,weight);
				eta_neg_list.at(eta_neg_bin)->Fill(mass,weight);
				phi_pos_list.at(phi_pos_bin)->Fill(mass,weight);
				phi_neg_list.at(phi_neg_bin)->Fill(mass,weight);
				pt_pos_list.at(pt_pos_bin)->Fill(mass,weight);
				pt_neg_list.at(pt_neg_bin)->Fill(mass,weight);
	/*	
				tot_pos_eta->Fill(eta1,weight1);
				tot_neg_eta->Fill(eta2,weight2);
				tot_pos_phi->Fill(phi1,weight1);
				tot_neg_phi->Fill(phi2,weight2);
				tot_pos_pt->Fill(pt1,weight1);
				tot_neg_pt->Fill(pt2,weight2);

				lep1.SetPtEtaPhiM(pt1,eta1,phi1,0.1057);
				lep2.SetPtEtaPhiM(pt2,eta2,phi2,0.1057);
				double zpt = (lep1+lep2).Pt();
				double zeta = (lep1+lep2).Eta();
				double zphi = (lep1+lep2).Phi();
				double zpz = (lep1+lep2).Pz();
				double angle = lep1.Angle(lep2.Vect());
				tot_zpt->Fill(zpt,weight);
				tot_zpz->Fill(zpz,weight);

				eta_pos_zpt_list.at(eta_pos_bin)->Fill(zpt,weight);			//eta pos z 
				eta_pos_angle_list.at(eta_pos_bin)->Fill(angle,weight);
				eta_pos_zeta_list.at(eta_pos_bin)->Fill(zeta,weight);
				eta_pos_zphi_list.at(eta_pos_bin)->Fill(zphi,weight);
				eta_pos_zpz_list.at(eta_pos_bin)->Fill(zpz,weight);

				eta_pos_free_eta_list.at(eta_pos_bin)->Fill(eta2,weight2);		//eta pos free fix
				eta_pos_fix_eta_list.at(eta_pos_bin)->Fill(eta1,weight1);
				eta_pos_free_phi_list.at(eta_pos_bin)->Fill(phi2,weight2);
				eta_pos_fix_phi_list.at(eta_pos_bin)->Fill(phi1,weight1);
				eta_pos_free_pt_list.at(eta_pos_bin)->Fill(pt2,weight2);
				eta_pos_fix_pt_list.at(eta_pos_bin)->Fill(pt1,weight1);
					
				phi_pos_zpt_list.at(phi_pos_bin)->Fill(zpt,weight);			//phi pos z
				phi_pos_angle_list.at(phi_pos_bin)->Fill(angle,weight);
				phi_pos_zeta_list.at(phi_pos_bin)->Fill(zeta,weight);
				phi_pos_zphi_list.at(phi_pos_bin)->Fill(zphi,weight);
				phi_pos_zpz_list.at(phi_pos_bin)->Fill(zpz,weight);

				phi_pos_free_eta_list.at(phi_pos_bin)->Fill(eta2,weight2);		//phi pos free fix
				phi_pos_fix_eta_list.at(phi_pos_bin)->Fill(eta1,weight1);
				phi_pos_free_phi_list.at(phi_pos_bin)->Fill(phi2,weight2);
				phi_pos_fix_phi_list.at(phi_pos_bin)->Fill(phi1,weight1);
				phi_pos_free_pt_list.at(phi_pos_bin)->Fill(pt2,weight2);
				phi_pos_fix_pt_list.at(phi_pos_bin)->Fill(pt1,weight1);
					
				pt_pos_zpt_list.at(pt_pos_bin)->Fill(zpt,weight);			//pt pos z
				pt_pos_angle_list.at(pt_pos_bin)->Fill(angle,weight);
				pt_pos_zeta_list.at(pt_pos_bin)->Fill(zeta,weight);
				pt_pos_zphi_list.at(pt_pos_bin)->Fill(zphi,weight);
				pt_pos_zpz_list.at(pt_pos_bin)->Fill(zpz,weight);

				pt_pos_free_eta_list.at(pt_pos_bin)->Fill(pt2,weight2);			//pt pos free fix
				pt_pos_fix_eta_list.at(pt_pos_bin)->Fill(pt1,weight1);
				pt_pos_free_phi_list.at(pt_pos_bin)->Fill(phi2,weight2);
				pt_pos_fix_phi_list.at(pt_pos_bin)->Fill(phi1,weight1);
				pt_pos_free_pt_list.at(pt_pos_bin)->Fill(pt2,weight2);
				pt_pos_fix_pt_list.at(pt_pos_bin)->Fill(pt1,weight1);

				eta_neg_zpt_list.at(eta_neg_bin)->Fill(zpt,weight);
				eta_neg_angle_list.at(eta_neg_bin)->Fill(angle,weight);
				eta_neg_zeta_list.at(eta_neg_bin)->Fill(zeta,weight);
				eta_neg_zphi_list.at(eta_neg_bin)->Fill(zphi,weight);
				eta_neg_zpz_list.at(eta_neg_bin)->Fill(zpz,weight);

				eta_neg_free_eta_list.at(eta_neg_bin)->Fill(eta1,weight1);
				eta_neg_fix_eta_list.at(eta_neg_bin)->Fill(eta2,weight2);
				eta_neg_free_phi_list.at(eta_neg_bin)->Fill(phi1,weight1);
				eta_neg_fix_phi_list.at(eta_neg_bin)->Fill(phi2,weight2);
				eta_neg_free_pt_list.at(eta_neg_bin)->Fill(pt1,weight1);
				eta_neg_fix_pt_list.at(eta_neg_bin)->Fill(pt2,weight2);
				
				phi_neg_zpt_list.at(phi_neg_bin)->Fill(zpt,weight);
				phi_neg_angle_list.at(phi_neg_bin)->Fill(angle,weight);
				phi_neg_zeta_list.at(phi_neg_bin)->Fill(zeta,weight);
				phi_neg_zphi_list.at(phi_neg_bin)->Fill(zphi,weight);
				phi_neg_zpz_list.at(phi_neg_bin)->Fill(zpz,weight);

				phi_neg_free_eta_list.at(phi_neg_bin)->Fill(eta1,weight1);
				phi_neg_fix_eta_list.at(phi_neg_bin)->Fill(eta2,weight2);
				phi_neg_free_phi_list.at(phi_neg_bin)->Fill(phi1,weight1);
				phi_neg_fix_phi_list.at(phi_neg_bin)->Fill(phi2,weight2);
				phi_neg_free_pt_list.at(phi_neg_bin)->Fill(pt1,weight1);
				phi_neg_fix_pt_list.at(phi_neg_bin)->Fill(pt2,weight2);

				pt_neg_zpt_list.at(pt_neg_bin)->Fill(zpt,weight);
				pt_neg_angle_list.at(pt_neg_bin)->Fill(angle,weight);
				pt_neg_zeta_list.at(pt_neg_bin)->Fill(zeta,weight);
				pt_neg_zphi_list.at(pt_neg_bin)->Fill(zphi,weight);
				pt_neg_zpz_list.at(pt_neg_bin)->Fill(zpz,weight);

				pt_neg_free_eta_list.at(pt_neg_bin)->Fill(pt1,weight1);
				pt_neg_fix_eta_list.at(pt_neg_bin)->Fill(pt2,weight2);
				pt_neg_free_phi_list.at(pt_neg_bin)->Fill(phi1,weight1);
				pt_neg_fix_phi_list.at(pt_neg_bin)->Fill(phi2,weight2);
				pt_neg_free_pt_list.at(pt_neg_bin)->Fill(pt1,weight1);
				pt_neg_fix_pt_list.at(pt_neg_bin)->Fill(pt2,weight2);

				pterr_pos_phi_list.at(phi_pos_bin)->Fill(pterr1,weight);
				pterr_neg_phi_list.at(phi_neg_bin)->Fill(pterr2,weight);
				pterr_pos_eta_list.at(eta_pos_bin)->Fill(pterr1,weight);
				pterr_neg_eta_list.at(eta_neg_bin)->Fill(pterr2,weight);
				pterr_pos_pt_list.at(pt_pos_bin)->Fill(pterr1,weight);
				pterr_neg_pt_list.at(pt_neg_bin)->Fill(pterr2,weight);
			
				pterrold_pos_phi_list.at(phi_pos_bin)->Fill(pterrold1,weight);
				pterrold_neg_phi_list.at(phi_neg_bin)->Fill(pterrold2,weight);
				pterrold_pos_eta_list.at(eta_pos_bin)->Fill(pterrold1,weight);
				pterrold_neg_eta_list.at(eta_neg_bin)->Fill(pterrold2,weight);
				pterrold_pos_pt_list.at(pt_pos_bin)->Fill(pterrold1,weight);
				pterrold_neg_pt_list.at(pt_neg_bin)->Fill(pterrold2,weight);
	*/
			}
			if(q2==1 && separation == true){		

				eta_pos_bin = eta_axis->FindBin(eta2)-1;
				eta_neg_bin = eta_axis->FindBin(eta1)-1;
				phi_pos_bin = phi_axis->FindBin(phi2)-1;
				phi_neg_bin = phi_axis->FindBin(phi1)-1;
				pt_pos_bin = pt_axis->FindBin(pt2)-1;
				pt_neg_bin = pt_axis->FindBin(pt1)-1;	

				eta_pos_list.at(eta_pos_bin)->Fill(mass,weight);
				eta_neg_list.at(eta_neg_bin)->Fill(mass,weight);
				phi_pos_list.at(phi_pos_bin)->Fill(mass,weight);
				phi_neg_list.at(phi_neg_bin)->Fill(mass,weight);
				pt_pos_list.at(pt_pos_bin)->Fill(mass,weight);
				pt_neg_list.at(pt_neg_bin)->Fill(mass,weight);
	/*				
				tot_pos_eta->Fill(eta2,weight2);
				tot_neg_eta->Fill(eta1,weight1);
				tot_pos_phi->Fill(phi2,weight2);
				tot_neg_phi->Fill(phi1,weight1);
				tot_pos_pt->Fill(pt2,weight2);
				tot_neg_pt->Fill(pt1,weight1);
					
				lep1.SetPtEtaPhiM(pt1,eta1,phi1,0.1057);
				lep2.SetPtEtaPhiM(pt2,eta2,phi2,0.1057);
				double zpt = (lep1+lep2).Pt();
				double zpz = (lep1+lep2).Pz();
				double angle = lep1.Angle(lep2.Vect());
				double zeta = (lep1+lep2).Eta();
				double zphi = (lep1+lep2).Phi();
				tot_zpt->Fill(zpt,weight);
				tot_zpz->Fill(zpz,weight);

				eta_pos_zpt_list.at(eta_pos_bin)->Fill(zpt,weight);
				eta_pos_angle_list.at(eta_pos_bin)->Fill(angle,weight);
				eta_pos_zeta_list.at(eta_pos_bin)->Fill(zeta,weight);
				eta_pos_zphi_list.at(eta_pos_bin)->Fill(zphi,weight);
				eta_pos_zpz_list.at(eta_pos_bin)->Fill(zpz,weight);

				eta_pos_free_eta_list.at(eta_pos_bin)->Fill(eta1,weight1);
				eta_pos_fix_eta_list.at(eta_pos_bin)->Fill(eta2,weight2);
				eta_pos_free_phi_list.at(eta_pos_bin)->Fill(phi1,weight1);
				eta_pos_fix_phi_list.at(eta_pos_bin)->Fill(phi2,weight2);
				eta_pos_free_pt_list.at(eta_pos_bin)->Fill(pt1,weight1);
				eta_pos_fix_pt_list.at(eta_pos_bin)->Fill(pt2,weight2);
				
				phi_pos_zpt_list.at(phi_pos_bin)->Fill(zpt,weight);
				phi_pos_angle_list.at(phi_pos_bin)->Fill(angle,weight);
				phi_pos_zeta_list.at(phi_pos_bin)->Fill(zeta,weight);
				phi_pos_zphi_list.at(phi_pos_bin)->Fill(zphi,weight);
				phi_pos_zpz_list.at(phi_pos_bin)->Fill(zpz,weight);

				phi_pos_free_eta_list.at(phi_pos_bin)->Fill(eta1,weight1);
				phi_pos_fix_eta_list.at(phi_pos_bin)->Fill(eta2,weight2);
				phi_pos_free_phi_list.at(phi_pos_bin)->Fill(phi1,weight1);
				phi_pos_fix_phi_list.at(phi_pos_bin)->Fill(phi2,weight2);
				phi_pos_free_pt_list.at(phi_pos_bin)->Fill(pt1,weight1);
				phi_pos_fix_pt_list.at(phi_pos_bin)->Fill(pt2,weight2);
				
				pt_pos_zpt_list.at(pt_pos_bin)->Fill(zpt,weight);
				pt_pos_angle_list.at(pt_pos_bin)->Fill(angle,weight);
				pt_pos_zeta_list.at(pt_pos_bin)->Fill(zeta,weight);
				pt_pos_zphi_list.at(pt_pos_bin)->Fill(zphi,weight);
				pt_pos_zpz_list.at(pt_pos_bin)->Fill(zpz,weight);
								
				pt_pos_free_eta_list.at(pt_pos_bin)->Fill(eta1,weight1);
				pt_pos_fix_eta_list.at(pt_pos_bin)->Fill(eta2,weight2);
				pt_pos_free_phi_list.at(pt_pos_bin)->Fill(phi1,weight1);
				pt_pos_fix_phi_list.at(pt_pos_bin)->Fill(phi2,weight2);
				pt_pos_free_pt_list.at(pt_pos_bin)->Fill(pt1,weight1);
				pt_pos_fix_pt_list.at(pt_pos_bin)->Fill(pt2,weight2);

				eta_neg_zpt_list.at(eta_neg_bin)->Fill(zpt,weight);
				eta_neg_angle_list.at(eta_neg_bin)->Fill(angle,weight);
				eta_neg_zeta_list.at(eta_neg_bin)->Fill(zeta,weight);
				eta_neg_zphi_list.at(eta_neg_bin)->Fill(zphi,weight);
				eta_neg_zpz_list.at(eta_neg_bin)->Fill(zpz,weight);

				eta_neg_free_eta_list.at(eta_neg_bin)->Fill(eta2,weight2);
				eta_neg_fix_eta_list.at(eta_neg_bin)->Fill(eta1,weight1);
				eta_neg_free_phi_list.at(eta_neg_bin)->Fill(phi2,weight2);
				eta_neg_fix_phi_list.at(eta_neg_bin)->Fill(phi1,weight1);
				eta_neg_free_pt_list.at(eta_neg_bin)->Fill(pt2,weight2);
				eta_neg_fix_pt_list.at(eta_neg_bin)->Fill(pt1,weight1);
				
				phi_neg_zpt_list.at(phi_neg_bin)->Fill(zpt,weight);
				phi_neg_angle_list.at(phi_neg_bin)->Fill(angle,weight);
				phi_neg_zeta_list.at(phi_neg_bin)->Fill(zeta,weight);
				phi_neg_zphi_list.at(phi_neg_bin)->Fill(zphi,weight);
				phi_neg_zpz_list.at(phi_neg_bin)->Fill(zpz,weight);

				phi_neg_free_eta_list.at(phi_neg_bin)->Fill(eta2,weight2);
				phi_neg_fix_eta_list.at(phi_neg_bin)->Fill(eta1,weight1);
				phi_neg_free_phi_list.at(phi_neg_bin)->Fill(phi2,weight2);
				phi_neg_fix_phi_list.at(phi_neg_bin)->Fill(phi1,weight1);
				phi_neg_free_pt_list.at(phi_neg_bin)->Fill(pt2,weight2);
				phi_neg_fix_pt_list.at(phi_neg_bin)->Fill(pt1,weight1);

				pt_neg_zpt_list.at(pt_neg_bin)->Fill(zpt,weight);
				pt_neg_angle_list.at(pt_neg_bin)->Fill(angle,weight);
				pt_neg_zeta_list.at(pt_neg_bin)->Fill(zeta,weight);
				pt_neg_zphi_list.at(pt_neg_bin)->Fill(zphi,weight);
				pt_neg_zpz_list.at(pt_neg_bin)->Fill(zpz,weight);
								
				pt_neg_free_eta_list.at(pt_neg_bin)->Fill(eta2,weight2);
				pt_neg_fix_eta_list.at(pt_neg_bin)->Fill(eta1,weight1);
				pt_neg_free_phi_list.at(pt_neg_bin)->Fill(phi2,weight2);
				pt_neg_fix_phi_list.at(pt_neg_bin)->Fill(phi1,weight1);
				pt_neg_free_pt_list.at(pt_neg_bin)->Fill(pt2,weight2);
				pt_neg_fix_pt_list.at(pt_neg_bin)->Fill(pt1,weight1);

				pterr_pos_phi_list.at(phi_pos_bin)->Fill(pterr2,weight);
				pterr_neg_phi_list.at(phi_neg_bin)->Fill(pterr1,weight);
				pterr_pos_eta_list.at(eta_pos_bin)->Fill(pterr2,weight);
				pterr_neg_eta_list.at(eta_neg_bin)->Fill(pterr1,weight);
				pterr_pos_pt_list.at(pt_pos_bin)->Fill(pterr2,weight);
				pterr_neg_pt_list.at(pt_neg_bin)->Fill(pterr1,weight);
			
				pterrold_pos_phi_list.at(phi_pos_bin)->Fill(pterrold2,weight);
				pterrold_neg_phi_list.at(phi_neg_bin)->Fill(pterrold1,weight);
				pterrold_pos_eta_list.at(eta_pos_bin)->Fill(pterrold2,weight);
				pterrold_neg_eta_list.at(eta_neg_bin)->Fill(pterrold1,weight);
				pterrold_pos_pt_list.at(pt_pos_bin)->Fill(pterrold2,weight);
				pterrold_neg_pt_list.at(pt_neg_bin)->Fill(pterrold1,weight);

	*/
			}
		}
	}
	
	mass_distributions(outputpath,eta_pos_list,eta_neg_list,low,high,isdata,"eta","mass_distributions");
	mass_distributions(outputpath,phi_pos_list,phi_neg_list,low,high,isdata,"phi","mass_distributions");
	mass_distributions(outputpath,pt_pos_list,pt_neg_list,low,high,isdata,"pt","mass_distributions");
	
	/*
	mass_distributions(outputpath,eta_pos_angle_list,eta_neg_angle_list,low,high,mass_name,"eta","angle_distributions");
	mass_distributions(outputpath,phi_pos_angle_list,phi_neg_angle_list,low,high,mass_name,"phi","angle_distributions");
	mass_distributions(outputpath,pt_pos_angle_list, pt_neg_angle_list, low,high,mass_name,"pt","angle_distributions");
	
	mass_distributions(outputpath,eta_pos_zeta_list,eta_neg_zeta_list,low,high,mass_name,"eta","zeta_distributions");
	mass_distributions(outputpath,phi_pos_zeta_list,phi_neg_zeta_list,low,high,mass_name,"phi","zeta_distributions");
	mass_distributions(outputpath,pt_pos_zeta_list, pt_neg_zeta_list, low,high,mass_name,"pt","zeta_distributions");

	mass_distributions(outputpath,eta_pos_zphi_list,eta_neg_zphi_list,low,high,mass_name,"eta","zphi_distributions");
	mass_distributions(outputpath,phi_pos_zphi_list,phi_neg_zphi_list,low,high,mass_name,"phi","zphi_distributions");
	mass_distributions(outputpath,pt_pos_zphi_list,pt_neg_zphi_list, low,high,mass_name,"pt","zphi_distributions");

	mass_distributions(outputpath,eta_pos_zpt_list,eta_neg_zpt_list,low,high,mass_name,"eta","zpt_distributions");
	mass_distributions(outputpath,phi_pos_zpt_list,phi_neg_zpt_list,low,high,mass_name,"phi","zpt_distributions");
	mass_distributions(outputpath,pt_pos_zpt_list,pt_neg_zpt_list,low,high,mass_name,"pt","zpt_distributions");

	mass_distributions(outputpath,eta_pos_zpz_list,eta_neg_zpz_list,low,high,mass_name,"eta","zpz_distributions");
	mass_distributions(outputpath,phi_pos_zpz_list,phi_neg_zpz_list,low,high,mass_name,"phi","zpz_distributions");
	mass_distributions(outputpath,pt_pos_zpz_list,pt_neg_zpz_list,low,high,mass_name,"pt","zpz_distributions");


	mass_distributions(outputpath,eta_pos_fix_pt_list,eta_neg_fix_pt_list,low,high,mass_name,"eta","fix_pt_distributions");
	mass_distributions(outputpath,eta_pos_free_pt_list,eta_neg_free_pt_list,low,high,mass_name,"eta","free_pt_distributions");
	
	mass_distributions(outputpath,phi_pos_fix_pt_list,phi_neg_fix_pt_list,low,high,mass_name,"phi","fix_pt_distributions");
	mass_distributions(outputpath,phi_pos_free_pt_list,phi_neg_free_pt_list,low,high,mass_name,"phi","free_pt_distributions");
	
	mass_distributions(outputpath,pt_pos_fix_pt_list,pt_neg_fix_pt_list,low,high,mass_name,"pt","fix_pt_distributions");
	mass_distributions(outputpath,pt_pos_free_pt_list,pt_neg_free_pt_list, low,high,mass_name,"pt","free_pt_distributions");
	
	mass_distributions(outputpath,pterr_pos_phi_list,pterr_neg_phi_list,low,high,mass_name,"phi","pterr_distributions");
	mass_distributions(outputpath,pterr_pos_eta_list,pterr_neg_eta_list,low,high,mass_name,"eta","pterr_distributions");
	mass_distributions(outputpath,pterr_pos_pt_list,pterr_neg_pt_list,low,high,mass_name,"pt","pterr_distributions");

	mass_distributions(outputpath,pterrold_pos_phi_list,pterrold_neg_phi_list,low,high,mass_name,"phi","pterrold_distributions");
	mass_distributions(outputpath,pterrold_pos_eta_list,pterrold_neg_eta_list,low,high,mass_name,"eta","pterrold_distributions");
	mass_distributions(outputpath,pterrold_pos_pt_list,pterrold_neg_pt_list,low,high,mass_name,"pt","pterrold_distributions");
	
	CustomHistoStyle(tot_pos_eta,true,true,"",2,4,1,false,"eta","Events","norm");
	CustomHistoStyle(tot_neg_eta,true,true,"",4,4,1,false,"eta","Events","norm");
	CustomHistoStyle(tot_pos_phi,true,true,"",2,4,1,false,"phi","Events","norm");
	CustomHistoStyle(tot_neg_phi,true,true,"",4,4,1,false,"phi","Events","norm");
	CustomHistoStyle(tot_pos_pt,true,true,"",2,4,1,false,"pt","Events","norm");
	CustomHistoStyle(tot_neg_pt,true,true,"",4,4,1,false,"pt","Events","norm");
	CustomHistoStyle(tot_zpt,true,true,"",4,4,1,false,"zpt","Events","norm");
	CustomHistoStyle(tot_zpz,true,true,"",4,4,1,false,"zpz","Events","norm");

	TCanvas cc("cc","",1400,1000);
	cc.cd();
	tot_pos_eta->SetMaximum(1.1*max(tot_pos_eta->GetMaximum(),tot_neg_eta->GetMaximum()));
	tot_pos_eta->Draw("hist");
	tot_neg_eta->Draw("histsame");
	cc.BuildLegend();
	cc.SaveAs(outputpath + "/lepton_distributions/eta_pos_neg.png");
	cc.Clear();
	tot_pos_phi->SetMaximum(1.1*max(tot_pos_phi->GetMaximum(),tot_neg_phi->GetMaximum()));
	tot_pos_phi->Draw("hist");
	tot_neg_phi->Draw("histsame");
	cc.BuildLegend();
	cc.SaveAs(outputpath + "/lepton_distributions/phi_pos_neg.png");
	cc.Clear();
	tot_pos_pt->SetMaximum(1.1*max(tot_pos_pt->GetMaximum(),tot_neg_pt->GetMaximum()));
	tot_pos_pt->Draw("hist");
	tot_neg_pt->Draw("histsame");
	cc.BuildLegend();
	cc.SaveAs(outputpath + "/lepton_distributions/pt_pos_neg.png");
	cc.Clear();
	tot_zpt->Draw("hist");
	cc.BuildLegend();
	cc.SaveAs(outputpath + "/zpt_distributions/zpt.png");
	cc.Clear();
	tot_zpz->Draw("hist");
	cc.SaveAs(outputpath + "/zpt_distributions/zpz.png");
	cc.Clear();
	
	Custom2DHistoStyle(pos_eta_vs_mass,"","pos_eta","mass",false);
	Custom2DHistoStyle(neg_eta_vs_mass,"","neg_eta","mass",false);
	Custom2DHistoStyle(pos_phi_vs_mass,"","pos_phi","mass",false);
	Custom2DHistoStyle(neg_phi_vs_mass,"","neg_phi","mass",false);
	cc.cd();
	pos_eta_vs_mass->Draw("colz");
	cc.SaveAs(outputpath + "/leptonvsmass/pos_eta_vs_mass.png");
	cc.Clear();
	neg_eta_vs_mass->Draw("colz");
	cc.SaveAs(outputpath + "/leptonvsmass/neg_eta_vs_mass.png");
	cc.Clear();
	pos_phi_vs_mass->Draw("colz");
	cc.SaveAs(outputpath + "/leptonvsmass/pos_phi_vs_mass.png");
	cc.Clear();
	neg_phi_vs_mass->Draw("colz");
	cc.SaveAs(outputpath + "/leptonvsmass/neg_phi_vs_mass.png");
	*/
	//do fit
	if(separation == true)dofit(year,outputpath,eta_pos_list,low,high,isdata,mass_name,init_mean, buffer, "eta");
	dofit(year,outputpath,eta_neg_list,low,high,isdata,mass_name,init_mean, buffer, "eta");
	if(separation == true)dofit(year,outputpath,phi_pos_list,low,high,isdata,mass_name,init_mean, buffer, "phi");
	dofit(year,outputpath,phi_neg_list,low,high,isdata,mass_name,init_mean, buffer, "phi");
	if(separation == true)dofit(year,outputpath,pt_pos_list,low,high,isdata,mass_name,init_mean, buffer, "pt");
	dofit(year,outputpath,pt_neg_list,low,high,isdata,mass_name,init_mean, buffer, "pt");
}

void dofit(TString year,TString outputpath, vector<TH1D*> &histos, string low, string high, TString isdata, TString mass_name,float init_mean, float buffer, TString k){	
	//x position
	int nbins;
	if(k == "eta"){
		nbins = etabinN;
	}
	if(k == "phi"){
		nbins = phibinN;
	}
	if(k == "pt"){
		nbins = ptbinN;
	}
	double xps[nbins] = {};
	double xperrs[nbins] = {};
	if(k == "eta"){
		h_eta_std->GetXaxis()->GetCenter(xps);	//note, when you use this function, the argument have to be specified, 'double* xps;' will throw out errors
		for(int i= 1; i<nbins+1; i++){
			xperrs[i-1] = 0.5*(h_eta_std->GetXaxis()->GetBinWidth(i));
		}
	}
	if(k == "phi"){
		h_phi_std->GetXaxis()->GetCenter(xps);
		for(int i= 1; i<nbins+1; i++){
			xperrs[i-1] = 0.5*(h_phi_std->GetXaxis()->GetBinWidth(i));
		}
	}
	if(k == "pt"){
		h_pt_std->GetXaxis()->GetCenter(xps);
		for(int i= 1; i<nbins+1; i++){
			xperrs[i-1] = 0.5*(h_pt_std->GetXaxis()->GetBinWidth(i));
		}
	}
	

	float width=2.495;
	float peak=91.19;
	if (isdata == "mc"){width=2.495;peak=91.19;}//2.441

	if(GEN==true){
	for(int i=0; i<histos.size(); i++){

		peak = histos.at(i)->GetBinCenter(histos.at(i)->GetMaximumBin());
		RooRealVar x("x","",stof(low),stof(high));
		//x.setBins(int(((stof(high)-stof(low))*1000.0)/10.0),"cache");
		RooRealVar PDGmZ("peak","PDGmZ",peak,0.9*peak,1.1*peak);
		RooRealVar PDGwZ("width","PDGwZ",width,0.9*width,1.1*width);
		RooBreitWigner Model("Model","PDGBW",x,PDGmZ,PDGwZ);

		RooDataHist dataset("dataset","dataset",RooArgSet(x),Import(*histos.at(i)));
		dataset.Print();
		
		x.setRange("peak",89.5,92.5);	
		Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1),Range("peak"));
		Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1),Range("peak"));
		RooChi2Var chi2_var("chi2","chi2",Model,dataset);
		TString ch2_s = to_string(chi2_var.getVal()/dataset.numEntries());
		TString peak_s = to_string(PDGmZ.getVal());
		TString peaktofile_s = to_string(PDGmZ.getVal());
		TString peakerr_s = to_string(PDGmZ.getError());
		TString mean_s = to_string(histos.at(i)->GetMean());
		TString sig_s = to_string(PDGwZ.getVal());
		RooPlot* frame = x.frame(Bins(30));
		frame->SetTitle("");
		TH1D* h_tmp = (TH1D*)histos.at(i)->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/100);
		RooDataHist dataset_toplot("dataset_toplot","dataset_toplot",RooArgSet(x),Import(*h_tmp));
		dataset_toplot.plotOn(frame);
		Model.plotOn(frame,Range("peak"),NormRange("peak"));
		TLatex *latex=new TLatex();
		latex->SetNDC();
		latex->SetTextSize(0.05);
		latex->SetTextFont(42);
		latex->SetTextAlign(23);
		TCanvas c("c","",1400,1000);
		c.cd();
		frame->Draw();
		latex->DrawLatex(0.7,0.99,"#chi^{2}/DOF="+ch2_s);
		latex->DrawLatex(0.7,0.9,"mean="+peak_s+"+/-"+peakerr_s);
		latex->DrawLatex(0.7,0.8,"sigma="+sig_s);
		c.SaveAs(outputpath + "/fitplots/"+k+"/"+year + "_" + isdata + "_" + mass_name + "_" + histos.at(i)->GetName() + "_" + low + "_" + high + ".png");
		ofstream fo;
		fo.open(outputpath + year + "_" + isdata + "_" + mass_name + "_" + histos.at(i)->GetName() + ".py");
		fo<<"info = {"<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_peak':" + peaktofile_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_peakerr':" + peakerr_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_mean':" + mean_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_sigma':" + sig_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_a1':-9999,"<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_a2':-9999,"<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_n1':-9999,"<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_n2':-9999,"<<endl;
		fo<<"}"<<endl;
		fo.close();
		delete latex;
		delete frame;
		delete h_tmp;
	}
	}				

	if(GEN==false){	
	for(int i=0; i<histos.size(); i++){
		double init_sigma=1.0, init_a1=1.0, init_a2=1.0, init_n1=10.0, init_n2=10.0, init_tau=0, init_fsig=1;
		double median=0.0, q=0.5;
		histos.at(i)->GetQuantiles(1, &median, &q);
		TString median_s = to_string(median);
		double sd=histos.at(i)->GetStdDev();
		TString sd_s=to_string(sd);

		RooRealVar x("x","",stof(low),stof(high));
		//x.setBins(int(((stof(high)-stof(low))*1000.0)/10.0),"cache");
		RooRealVar PDGmZ("PDGmZ","PDGmZ",peak,0.9*peak,1.1*peak);
		RooRealVar PDGwZ("PDGwZ","PDGwZ",width,0.9*width,1.1*width);
		RooBreitWigner PDGBW("PDGBW","PDGBW",x,PDGmZ,PDGwZ);
		PDGmZ.setConstant(kTRUE);
		PDGwZ.setConstant(kTRUE);
		RooRealVar m("m","m",init_mean,-5,5);
		RooRealVar s("s","s",init_sigma,0,10);
		RooRealVar a1("a1","a1",init_a1,0,10);
		RooRealVar n1("n1","n1",init_n1,0.0*init_n1,5*init_n1);
		RooRealVar a2("a2","2",init_a2,0,10);
		RooRealVar n2("n2","n2",init_n2,0.0*init_n2,5*init_n2);
		RooRealVar tau("tau","tau",init_tau,-1,1);
		RooRealVar fsig("fsig","fsig",init_fsig,0.5,1);
		RooDoubleCB DCB("DCB","DCB",x,m,s,a1,n1,a2,n2);
		//RooCBShape SCB("SCB","SCB",x,m,s,a1,n1);
		RooFFTConvPdf BW_DCB("BW_DCB","BW_DCB",x,PDGBW,DCB);
		BW_DCB.setBufferFraction(buffer);
		RooExponential bkg("bkg","bkg",x,tau);
		RooAddPdf Model("Model","Model",BW_DCB,bkg,fsig);
		
		RooDataHist dataset("dataset","dataset",RooArgSet(x),Import(*histos.at(i)));
		dataset.Print();
		Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1));		
		RooFitResult* r  = Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1),Save(kTRUE));
		//if(false){
		//	TH2* dmatrix = r->correlationHist();
		//	dmatrix->SetStats(0);
		//	TCanvas c_co("c_co","",1400,1000);
		//	c_co.cd();
		//	dmatrix->Draw("colztext");
		//	c_co.SaveAs(outputpath + "/correlation/" + year + "_" + isdata + "_" + mass_name + "_" + histos.at(i)->GetName() + "_" + low + "_" + high + ".png");
		//}
		double left = s.getVal()*(-a1.getVal())+m.getVal()+peak;
		double right = 	s.getVal()*(a2.getVal())+m.getVal()+peak;
		TLine LL(left,0,left,999999);
		TLine RL(right,0,right,999999);
		
		//x.setRange("core",left,right);	
		//RooFFTConvPdf Model("Model","BW_DCB",x,PDGBW,DCB);
		//Model.setBufferFraction(buffer);
		
		m.setConstant(kFALSE);
		s.setConstant(kTRUE);
		a1.setConstant(kTRUE);
		n1.setConstant(kTRUE);
		a2.setConstant(kTRUE);
		n2.setConstant(kTRUE);
		fsig.setConstant(kTRUE);
		tau.setConstant(kTRUE);
		
		Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1));//Range("core"));
		//Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1));//,Range("core"));
			
		RooChi2Var chi2_var("chi2","chi2",Model,dataset);
		TString ch2_s = to_string(chi2_var.getVal()/dataset.numEntries());
		TString peak_s = to_string(m.getVal());
		TString peaktofile_s = to_string(m.getVal()+peak);
		TString peakerr_s = to_string(m.getError());
		TString mean_s = to_string(histos.at(i)->GetMean());
		TString sig_s = to_string(s.getVal()); TString entries_s = to_string(int(dataset.sumEntries()));
		TString sigerr_s = to_string(s.getError());
		TString n1_s = to_string(n1.getVal()); TString a1_s = to_string(a1.getVal());
		TString n2_s = to_string(n2.getVal()); TString a2_s = to_string(a2.getVal());
		TString fsig_s = to_string(fsig.getVal()); TString tau_s = to_string(tau.getVal());
		RooPlot* frame = x.frame(Bins(30));
		frame->SetTitle("");
		TH1D* h_tmp = (TH1D*)histos.at(i)->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/100);
		RooDataHist dataset_toplot("dataset_toplot","dataset_toplot",RooArgSet(x),Import(*h_tmp));
		dataset_toplot.plotOn(frame);
		Model.plotOn(frame);//,Range("core"),RooFit::NormRange("core"));
		Model.plotOn(frame,Components(bkg));
		TLatex *latex=new TLatex();
		latex->SetNDC();
		latex->SetTextSize(0.03);
		latex->SetTextFont(42);
		latex->SetTextAlign(23);
		TCanvas c("c","",1400,1000);
		c.cd();
		frame->Draw();
		latex->DrawLatex(0.7,0.99,"#chi^{2}/DOF="+ch2_s);
		latex->DrawLatex(0.7,0.95,"peak="+peaktofile_s);
		latex->DrawLatex(0.7,0.9,"mean="+peak_s+"+/-"+peakerr_s);
		latex->DrawLatex(0.7,0.85,"sigma="+sig_s+"+/-"+sigerr_s);
		latex->DrawLatex(0.7,0.8,"a1="+a1_s);
		latex->DrawLatex(0.7,0.75,"n1="+n1_s);
		latex->DrawLatex(0.7,0.7,"a2="+a2_s);
		latex->DrawLatex(0.7,0.65,"n2="+n2_s);
		latex->DrawLatex(0.7,0.6,"entries="+entries_s);
		latex->DrawLatex(0.7,0.55,"fsig="+fsig_s);
		latex->DrawLatex(0.7,0.5,"tau="+tau_s);
		LL.Draw("same");
		RL.Draw("same");
		c.SaveAs(outputpath +"/fitplots/"+k+"/"+ year + "_" + isdata + "_" + mass_name + "_" + histos.at(i)->GetName() + "_" + low + "_" + high + ".png");
		ofstream fo;
		fo.open(outputpath + year + "_" + isdata + "_" + mass_name + "_" + histos.at(i)->GetName() + ".py");
		fo<<"info = {"<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_peak':" + peaktofile_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_peakerr':" + peakerr_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_mean':" + mean_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_median':" + median_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_a1':" + a1_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_a2':" + a2_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_n1':" + n1_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_n2':" + n2_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_sigma':" + sig_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_sigmaerr':" + sigerr_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_sd':" + sd_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_shift':" + peak_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_xposition':" + to_string(xps[i])+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_xpositionerr':" + to_string(xperrs[i])+","<<endl;
		fo<<"}"<<endl;
		fo.close();
		
		delete latex;
		delete frame;
		delete h_tmp;
	}
	}
}
