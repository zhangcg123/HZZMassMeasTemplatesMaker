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
	TString plotpath = argv[8];
		
	TString subname;
	if(isdata=="mc")subname="LO.root";
	if(isdata=="data")subname="LO.root";
	TString inputpath = "/eos/user/c/chenguan/Tuples/MuonScaleUsed/19UL_EWK2ASDEF_ADDGEN/" + year + "_HZZ_"+subname;
	TString outputpath = plotpath;

	double ptbins[7] = {5,20,30,40,50,60,100};
	TH1D* h_eta_std = new TH1D("h_eta_std","",8,-2.4,2.4);
	TH1D* h_phi_std = new TH1D("h_phi_std","",8,-M_PI,M_PI);
	TH1D* h_pt_std = new TH1D("h_pt_std","",6,ptbins);
	
	TH1D* tot_pos_eta = new TH1D("pos_eta","",100,-2.4,2.4);
	TH1D* tot_neg_eta = new TH1D("neg_eta","",100,-2.4,2.4);
	TH1D* tot_pos_phi = new TH1D("pos_phi","",100,-3.14,3.14);
	TH1D* tot_neg_phi = new TH1D("neg_phi","",100,-3.14,3.14);
	TH1D* tot_pos_pt = new TH1D("pos_pt","",100,5,200);
	TH1D* tot_neg_pt = new TH1D("neg_pt","",100,5,200);
	TH1D* tot_zpt = new TH1D("zpt","",100,0,100);
	TH1D* tot_zpz = new TH1D("zpz","",500,-1000,1000);
		
	//TH2D* pos_eta_vs_mass = new TH2D("pos_eta_vs_mass","",100,-2.4,2.4,100,86,96);
	//TH2D* neg_eta_vs_mass = new TH2D("neg_eta_vs_mass","",100,-2.4,2.4,100,86,96);
	//TH2D* pos_phi_vs_mass = new TH2D("pos_phi_vs_mass","",100,-3.14,3.14,100,86,96);
	//TH2D* neg_phi_vs_mass = new TH2D("neg_phi_vs_mass","",100,-3.14,3.14,100,86,96);

	vector<TH1D*> eta_pos_list; eta_pos_list.clear();	//mass histograms holder list
	vector<TH1D*> eta_neg_list; eta_neg_list.clear();
	vector<TH1D*> phi_pos_list; phi_pos_list.clear();
	vector<TH1D*> phi_neg_list; phi_neg_list.clear();
	vector<TH1D*> pt_pos_list; pt_pos_list.clear();
	vector<TH1D*> pt_neg_list; pt_neg_list.clear();

	vector<TH1D*> eta_pos_zpt_list; eta_pos_zpt_list.clear();//zpt histogram holder list
	vector<TH1D*> eta_neg_zpt_list; eta_neg_zpt_list.clear();
	vector<TH1D*> phi_pos_zpt_list; phi_pos_zpt_list.clear();
	vector<TH1D*> phi_neg_zpt_list; phi_neg_zpt_list.clear();
	vector<TH1D*> pt_pos_zpt_list; pt_pos_zpt_list.clear();
	vector<TH1D*> pt_neg_zpt_list; pt_neg_zpt_list.clear();
		
	vector<TH1D*> eta_pos_zeta_list; eta_pos_zeta_list.clear();//zeta holder list
	vector<TH1D*> eta_neg_zeta_list; eta_neg_zeta_list.clear();
	vector<TH1D*> phi_pos_zeta_list; phi_pos_zeta_list.clear();
	vector<TH1D*> phi_neg_zeta_list; phi_neg_zeta_list.clear();
	vector<TH1D*> pt_pos_zeta_list; pt_pos_zeta_list.clear();
	vector<TH1D*> pt_neg_zeta_list; pt_neg_zeta_list.clear();

	vector<TH1D*> eta_pos_zphi_list; eta_pos_zphi_list.clear();//zphi
	vector<TH1D*> eta_neg_zphi_list; eta_neg_zphi_list.clear();
	vector<TH1D*> phi_pos_zphi_list; phi_pos_zphi_list.clear();
	vector<TH1D*> phi_neg_zphi_list; phi_neg_zphi_list.clear();
	vector<TH1D*> pt_pos_zphi_list; pt_pos_zphi_list.clear();
	vector<TH1D*> pt_neg_zphi_list; pt_neg_zphi_list.clear();

	vector<TH1D*> eta_pos_angle_list; eta_pos_angle_list.clear();//angle
	vector<TH1D*> eta_neg_angle_list; eta_neg_angle_list.clear();
	vector<TH1D*> phi_pos_angle_list; phi_pos_angle_list.clear();
	vector<TH1D*> phi_neg_angle_list; phi_neg_angle_list.clear();
	vector<TH1D*> pt_pos_angle_list; pt_pos_angle_list.clear();
	vector<TH1D*> pt_neg_angle_list; pt_neg_angle_list.clear();

	vector<TH1D*> eta_pos_zpz_list; eta_pos_zpz_list.clear();//zpz
	vector<TH1D*> eta_neg_zpz_list; eta_neg_zpz_list.clear();
	vector<TH1D*> phi_pos_zpz_list; phi_pos_zpz_list.clear();
	vector<TH1D*> phi_neg_zpz_list; phi_neg_zpz_list.clear();
	vector<TH1D*> pt_pos_zpz_list; pt_pos_zpz_list.clear();
	vector<TH1D*> pt_neg_zpz_list; pt_neg_zpz_list.clear();

	vector<TH1D*> eta_pos_free_pt_list; eta_pos_free_pt_list.clear();//eta fix/free pt
	vector<TH1D*> eta_pos_fix_pt_list; eta_pos_fix_pt_list.clear();
	vector<TH1D*> eta_neg_free_pt_list; eta_neg_free_pt_list.clear();
	vector<TH1D*> eta_neg_fix_pt_list; eta_neg_fix_pt_list.clear();

	vector<TH1D*> eta_pos_free_eta_list; eta_pos_free_eta_list.clear();//eta fix/free eta
	vector<TH1D*> eta_pos_fix_eta_list; eta_pos_fix_eta_list.clear();
	vector<TH1D*> eta_neg_free_eta_list; eta_neg_free_eta_list.clear();
	vector<TH1D*> eta_neg_fix_eta_list; eta_neg_fix_eta_list.clear();

	vector<TH1D*> eta_pos_free_phi_list; eta_pos_free_phi_list.clear();//eta fix/free phi
	vector<TH1D*> eta_pos_fix_phi_list; eta_pos_fix_phi_list.clear();
	vector<TH1D*> eta_neg_free_phi_list; eta_neg_free_phi_list.clear();
	vector<TH1D*> eta_neg_fix_phi_list; eta_neg_fix_phi_list.clear();

	vector<TH1D*> phi_pos_free_pt_list; phi_pos_free_pt_list.clear();//phi fix/free pt
	vector<TH1D*> phi_pos_fix_pt_list; phi_pos_fix_pt_list.clear();
	vector<TH1D*> phi_neg_free_pt_list; phi_neg_free_pt_list.clear();
	vector<TH1D*> phi_neg_fix_pt_list; phi_neg_fix_pt_list.clear();
	
	vector<TH1D*> phi_pos_free_eta_list; phi_pos_free_eta_list.clear();//phi fix/free eta
	vector<TH1D*> phi_pos_fix_eta_list; phi_pos_fix_eta_list.clear();
	vector<TH1D*> phi_neg_free_eta_list; phi_neg_free_eta_list.clear();
	vector<TH1D*> phi_neg_fix_eta_list; phi_neg_fix_eta_list.clear();

	vector<TH1D*> phi_pos_free_phi_list; phi_pos_free_phi_list.clear();//phi fix/free phi
	vector<TH1D*> phi_pos_fix_phi_list; phi_pos_fix_phi_list.clear();
	vector<TH1D*> phi_neg_free_phi_list; phi_neg_free_phi_list.clear();
	vector<TH1D*> phi_neg_fix_phi_list; phi_neg_fix_phi_list.clear();
	
	vector<TH1D*> pt_pos_free_pt_list; pt_pos_free_pt_list.clear();//pt fix/free pt
	vector<TH1D*> pt_pos_fix_pt_list; pt_pos_fix_pt_list.clear();
	vector<TH1D*> pt_neg_free_pt_list; pt_neg_free_pt_list.clear();
	vector<TH1D*> pt_neg_fix_pt_list; pt_neg_fix_pt_list.clear();

	vector<TH1D*> pt_pos_free_eta_list; pt_pos_free_eta_list.clear();//pt fix/free eta
	vector<TH1D*> pt_pos_fix_eta_list; pt_pos_fix_eta_list.clear();
	vector<TH1D*> pt_neg_free_eta_list; pt_neg_free_eta_list.clear();
	vector<TH1D*> pt_neg_fix_eta_list; pt_neg_fix_eta_list.clear();
	
	vector<TH1D*> pt_pos_free_phi_list; pt_pos_free_phi_list.clear();//pt fix/free phi
	vector<TH1D*> pt_pos_fix_phi_list; pt_pos_fix_phi_list.clear();
	vector<TH1D*> pt_neg_free_phi_list; pt_neg_free_phi_list.clear();
	vector<TH1D*> pt_neg_fix_phi_list; pt_neg_fix_phi_list.clear();
	
	vector<TH1D*> pterr_pos_phi_list; pterr_pos_phi_list.clear();//pterr
	vector<TH1D*> pterr_neg_phi_list; pterr_neg_phi_list.clear();
	vector<TH1D*> pterr_pos_eta_list; pterr_pos_eta_list.clear();
	vector<TH1D*> pterr_neg_eta_list; pterr_neg_eta_list.clear();
	vector<TH1D*> pterr_pos_pt_list; pterr_pos_pt_list.clear();
	vector<TH1D*> pterr_neg_pt_list; pterr_neg_pt_list.clear();
	
	vector<TH1D*> pterrold_pos_phi_list; pterrold_pos_phi_list.clear();//pterrold
	vector<TH1D*> pterrold_neg_phi_list; pterrold_neg_phi_list.clear();
	vector<TH1D*> pterrold_pos_eta_list; pterrold_pos_eta_list.clear();
	vector<TH1D*> pterrold_neg_eta_list; pterrold_neg_eta_list.clear();
	vector<TH1D*> pterrold_pos_pt_list; pterrold_pos_pt_list.clear();
	vector<TH1D*> pterrold_neg_pt_list; pterrold_neg_pt_list.clear();
	
	vector<TH1D*> phi_pos_cuton_truth_see_reco_list; phi_pos_cuton_truth_see_reco_list.clear(); 
	vector<TH1D*> phi_neg_cuton_truth_see_reco_list; phi_neg_cuton_truth_see_reco_list.clear();
	vector<TH1D*> eta_pos_cuton_truth_see_reco_list; eta_pos_cuton_truth_see_reco_list.clear();
	vector<TH1D*> eta_neg_cuton_truth_see_reco_list; eta_neg_cuton_truth_see_reco_list.clear();
	vector<TH1D*> pt_pos_cuton_truth_see_reco_list; pt_pos_cuton_truth_see_reco_list.clear();
	vector<TH1D*> pt_neg_cuton_truth_see_reco_list; pt_neg_cuton_truth_see_reco_list.clear();

	vector<TH1D*> phi_pos_cuton_reco_see_truth_list; phi_pos_cuton_reco_see_truth_list.clear();
	vector<TH1D*> phi_neg_cuton_reco_see_truth_list; phi_neg_cuton_reco_see_truth_list.clear();
	vector<TH1D*> eta_pos_cuton_reco_see_truth_list; eta_pos_cuton_reco_see_truth_list.clear();
	vector<TH1D*> eta_neg_cuton_reco_see_truth_list; eta_neg_cuton_reco_see_truth_list.clear();
	vector<TH1D*> pt_pos_cuton_reco_see_truth_list; pt_pos_cuton_reco_see_truth_list.clear();
	vector<TH1D*> pt_neg_cuton_reco_see_truth_list; pt_neg_cuton_reco_see_truth_list.clear();

	vector<TH1D*> phi_pos_pull_list; phi_pos_pull_list.clear();
	vector<TH1D*> phi_neg_pull_list; phi_neg_pull_list.clear();
	vector<TH1D*> eta_pos_pull_list; eta_pos_pull_list.clear();
	vector<TH1D*> eta_neg_pull_list; eta_neg_pull_list.clear();
	vector<TH1D*> pt_pos_pull_list; pt_pos_pull_list.clear();
	vector<TH1D*> pt_neg_pull_list; pt_neg_pull_list.clear();
	
	vector<vector<TH1D*>> pt_pos_reco_migration_list; pt_pos_reco_migration_list.clear();
	vector<vector<TH1D*>> pt_neg_reco_migration_list; pt_neg_reco_migration_list.clear();
	vector<vector<TH1D*>> eta_pos_reco_migration_list; eta_pos_reco_migration_list.clear();
	vector<vector<TH1D*>> eta_neg_reco_migration_list; eta_neg_reco_migration_list.clear();
	vector<vector<TH1D*>> phi_pos_reco_migration_list; phi_pos_reco_migration_list.clear();
	vector<vector<TH1D*>> phi_neg_reco_migration_list; phi_neg_reco_migration_list.clear();
	
	TH2D* pt1_corr_vs_gen = new TH2D("pt1_corr_vs_gen","",100,0,100,100,0,100);
	TH2D* pt2_corr_vs_gen = new TH2D("pt2_corr_vs_gen","",100,0,100,100,0,100);
	TH2D* pt1_uncorr_vs_gen = new TH2D("pt1_uncorr_vs_gen","",100,0,100,100,0,100);
	TH2D* pt2_uncorr_vs_gen = new TH2D("pt2_uncorr_vs_gen","",100,0,100,100,0,100);

	for(int i=0; i<8; i++){

		TString i_s = to_string(i);
		TH1D* h_eta_pos = new TH1D("eta_pos_"+i_s,"",int(((stof(high)-stof(low))*1000.0)/10.0),stof(low),stof(high));
		TH1D* h_eta_neg = new TH1D("eta_neg_"+i_s,"",int(((stof(high)-stof(low))*1000.0)/10.0),stof(low),stof(high));
		TH1D* h_phi_pos = new TH1D("phi_pos_"+i_s,"",int(((stof(high)-stof(low))*1000.0)/10.0),stof(low),stof(high));
		TH1D* h_phi_neg = new TH1D("phi_neg_"+i_s,"",int(((stof(high)-stof(low))*1000.0)/10.0),stof(low),stof(high));
				
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

		TH1D* h_eta_pos_free_pt = new TH1D("eta_pos_free_pt_"+i_s,"",100,5,100);
		TH1D* h_eta_pos_fix_pt = new TH1D("eta_pos_fix_pt_"+i_s,"",100,5,100);
		TH1D* h_eta_pos_free_eta = new TH1D("eta_pos_free_eta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_eta_pos_fix_eta = new TH1D("eta_pos_fix_eta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_eta_pos_free_phi = new TH1D("eta_pos_free_phi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_eta_pos_fix_phi = new TH1D("eta_pos_fix_phi_"+i_s,"",100,-3.14,3.14);
		
		TH1D* h_phi_pos_free_pt = new TH1D("phi_pos_free_pt_"+i_s,"",100,5,100);
		TH1D* h_phi_pos_fix_pt = new TH1D("phi_pos_fix_pt_"+i_s,"",100,5,100);
		TH1D* h_phi_pos_free_eta = new TH1D("phi_pos_free_eta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_phi_pos_fix_eta = new TH1D("phi_pos_fix_eta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_phi_pos_free_phi = new TH1D("phi_pos_free_phi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_phi_pos_fix_phi = new TH1D("phi_pos_fix_phi_"+i_s,"",100,-3.14,3.14);
		
		TH1D* h_eta_neg_free_pt = new TH1D("eta_neg_free_pt_"+i_s,"",100,5,100);
		TH1D* h_eta_neg_fix_pt = new TH1D("eta_neg_fix_pt_"+i_s,"",100,5,100);
		TH1D* h_eta_neg_free_eta = new TH1D("eta_neg_free_eta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_eta_neg_fix_eta = new TH1D("eta_neg_fix_eta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_eta_neg_free_phi = new TH1D("eta_neg_free_phi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_eta_neg_fix_phi = new TH1D("eta_neg_fix_phi_"+i_s,"",100,-3.14,3.14);
		
		TH1D* h_phi_neg_free_pt = new TH1D("phi_neg_free_pt_"+i_s,"",100,5,100);
		TH1D* h_phi_neg_fix_pt = new TH1D("phi_neg_fix_pt_"+i_s,"",100,5,100);
		TH1D* h_phi_neg_free_eta = new TH1D("phi_neg_free_eta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_phi_neg_fix_eta = new TH1D("phi_neg_fix_eta_"+i_s,"",100,-2.4,2.4);
		TH1D* h_phi_neg_free_phi = new TH1D("phi_neg_free_phi_"+i_s,"",100,-3.14,3.14);
		TH1D* h_phi_neg_fix_phi = new TH1D("phi_neg_fix_phi_"+i_s,"",100,-3.14,3.14);
		
		TH1D* h_pterr_pos_phi = new TH1D("pterr_pos_phi_"+i_s,"",100,0,6);
		TH1D* h_pterr_neg_phi = new TH1D("pterr_neg_phi_"+i_s,"",100,0,6);
		TH1D* h_pterr_pos_eta = new TH1D("pterr_pos_eta_"+i_s,"",100,0,6);
		TH1D* h_pterr_neg_eta = new TH1D("pterr_neg_eta_"+i_s,"",100,0,6);

		TH1D* h_pterrold_pos_phi = new TH1D("pterrold_pos_phi_"+i_s,"",100,0,6);
		TH1D* h_pterrold_neg_phi = new TH1D("pterrold_neg_phi_"+i_s,"",100,0,6);
		TH1D* h_pterrold_pos_eta = new TH1D("pterrold_pos_eta_"+i_s,"",100,0,6);
		TH1D* h_pterrold_neg_eta = new TH1D("pterrold_neg_eta_"+i_s,"",100,0,6);

		TH1D* h_phi_pos_cuton_truth_see_reco = new TH1D("phi_pos_cuton_truth_see_reco_"+i_s,"",100,0,100);
		TH1D* h_phi_neg_cuton_truth_see_reco = new TH1D("phi_neg_cuton_truth_see_reco_"+i_s,"",100,0,100);
		TH1D* h_eta_pos_cuton_truth_see_reco = new TH1D("eta_pos_cuton_truth_see_reco_"+i_s,"",100,0,100);
		TH1D* h_eta_neg_cuton_truth_see_reco = new TH1D("eta_neg_cuton_truth_see_reco_"+i_s,"",100,0,100);

		TH1D* h_phi_pos_cuton_reco_see_truth = new TH1D("phi_pos_cuton_reco_see_truth_"+i_s,"",100,0,100);
		TH1D* h_phi_neg_cuton_reco_see_truth = new TH1D("phi_neg_cuton_reco_see_truth_"+i_s,"",100,0,100);
		TH1D* h_eta_pos_cuton_reco_see_truth = new TH1D("eta_pos_cuton_reco_see_truth_"+i_s,"",100,0,100);
		TH1D* h_eta_neg_cuton_reco_see_truth = new TH1D("eta_neg_cuton_reco_see_truth_"+i_s,"",100,0,100);
	
		TH1D* h_phi_pos_pull = new TH1D("phi_pos_(reco-gen)_"+i_s,"",100,-6,6);
		TH1D* h_phi_neg_pull = new TH1D("phi_neg_(reco-gen)_"+i_s,"",100,-6,6);
		TH1D* h_eta_pos_pull = new TH1D("eta_pos_(reco-gen)_"+i_s,"",100,-6,6);
		TH1D* h_eta_neg_pull = new TH1D("eta_neg_(reco-gen)_"+i_s,"",100,-6,6);
					
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
		
		eta_pos_cuton_truth_see_reco_list.push_back(h_eta_pos_cuton_truth_see_reco);
		eta_neg_cuton_truth_see_reco_list.push_back(h_eta_neg_cuton_truth_see_reco);
		phi_pos_cuton_truth_see_reco_list.push_back(h_phi_pos_cuton_truth_see_reco);
		phi_neg_cuton_truth_see_reco_list.push_back(h_phi_neg_cuton_truth_see_reco);

		eta_pos_cuton_reco_see_truth_list.push_back(h_eta_pos_cuton_reco_see_truth);
		eta_neg_cuton_reco_see_truth_list.push_back(h_eta_neg_cuton_reco_see_truth);
		phi_pos_cuton_reco_see_truth_list.push_back(h_phi_pos_cuton_reco_see_truth);
		phi_neg_cuton_reco_see_truth_list.push_back(h_phi_neg_cuton_reco_see_truth);
		
		eta_pos_pull_list.push_back(h_eta_pos_pull);
		eta_neg_pull_list.push_back(h_eta_neg_pull);
		phi_pos_pull_list.push_back(h_phi_pos_pull);
		phi_neg_pull_list.push_back(h_phi_neg_pull);
		//for migration
		vector<TH1D*> eta_pos_migration; eta_pos_migration.clear();
		vector<TH1D*> eta_neg_migration; eta_neg_migration.clear();
		vector<TH1D*> phi_pos_migration; phi_pos_migration.clear();
		vector<TH1D*> phi_neg_migration; phi_neg_migration.clear();
		for(int j=0; j<8; j++){
			TString j_s = to_string(j);
			TH1D* h_eta_pos_migration = new TH1D("eta_pos_migration_"+i_s+"_"+j_s,"",100,stof(low),stof(high));
			TH1D* h_eta_neg_migration = new TH1D("eta_neg_migration_"+i_s+"_"+j_s,"",100,stof(low),stof(high));
			TH1D* h_phi_pos_migration = new TH1D("phi_pos_migration_"+i_s+"_"+j_s,"",100,stof(low),stof(high));
			TH1D* h_phi_neg_migration = new TH1D("phi_neg_migration_"+i_s+"_"+j_s,"",100,stof(low),stof(high));
			eta_pos_migration.push_back(h_eta_pos_migration);
			eta_neg_migration.push_back(h_eta_neg_migration);
			phi_pos_migration.push_back(h_phi_pos_migration);
			phi_neg_migration.push_back(h_phi_neg_migration);	
		}
		eta_pos_reco_migration_list.push_back(eta_pos_migration);
		eta_neg_reco_migration_list.push_back(eta_neg_migration);
		phi_pos_reco_migration_list.push_back(phi_pos_migration);
		phi_neg_reco_migration_list.push_back(phi_neg_migration);

		if(i<6){
			
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
			TH1D* h_pt_pos_free_pt = new TH1D("pt_pos_free_pt_"+i_s,"",500,5,100);
			TH1D* h_pt_pos_fix_pt = new TH1D("pt_pos_fix_pt_"+i_s,"",500,5,100);
			TH1D* h_pt_pos_free_eta = new TH1D("pt_pos_free_eta_"+i_s,"",100,-2.4,2.4);
			TH1D* h_pt_pos_fix_eta = new TH1D("pt_pos_fix_eta_"+i_s,"",100,-2.4,2.4);
			TH1D* h_pt_pos_free_phi = new TH1D("pt_pos_free_phi_"+i_s,"",100,-3.14,3.14);
			TH1D* h_pt_pos_fix_phi = new TH1D("pt_pos_fix_phi_"+i_s,"",100,-3.14,3.14);
			TH1D* h_pt_neg_free_pt = new TH1D("pt_neg_free_pt_"+i_s,"",100,5,100);
			TH1D* h_pt_neg_fix_pt = new TH1D("pt_neg_fix_pt_"+i_s,"",100,5,100);
			TH1D* h_pt_neg_free_eta = new TH1D("pt_neg_free_eta_"+i_s,"",100,-2.4,2.4);
			TH1D* h_pt_neg_fix_eta = new TH1D("pt_neg_fix_eta_"+i_s,"",100,-2.4,2.4);
			TH1D* h_pt_neg_free_phi = new TH1D("pt_neg_free_phi_"+i_s,"",100,-3.14,3.14);
			TH1D* h_pt_neg_fix_phi = new TH1D("pt_neg_fix_phi_"+i_s,"",100,-3.14,3.14);
			TH1D* h_pterr_pos_pt = new TH1D("pterr_pos_pt_"+i_s,"",100,0,6);
			TH1D* h_pterr_neg_pt = new TH1D("pterr_neg_pt_"+i_s,"",100,0,6);
			TH1D* h_pterrold_pos_pt = new TH1D("pterrold_pos_pt_"+i_s,"",100,0,6);
			TH1D* h_pterrold_neg_pt = new TH1D("pterrold_neg_pt_"+i_s,"",100,0,6);
			TH1D* h_pt_pos_cuton_truth_see_reco = new TH1D("pt_pos_cuton_truth_see_reco_"+i_s,"",100,0,100);
			TH1D* h_pt_neg_cuton_truth_see_reco = new TH1D("pt_neg_cuton_truth_see_reco_"+i_s,"",100,0,100);
			TH1D* h_pt_pos_cuton_reco_see_truth = new TH1D("pt_pos_cuton_reco_see_truth_"+i_s,"",100,0,100);
			TH1D* h_pt_neg_cuton_reco_see_truth = new TH1D("pt_neg_cuton_reco_see_truth_"+i_s,"",100,0,100);
			TH1D* h_pt_pos_pull = new TH1D("pt_pos_(reco-gen)_"+i_s,"",100,-6,6);
			TH1D* h_pt_neg_pull = new TH1D("pt_neg_(reco-gen)_"+i_s,"",100,-6,6);

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
			pt_pos_cuton_truth_see_reco_list.push_back(h_pt_pos_cuton_truth_see_reco);
			pt_neg_cuton_truth_see_reco_list.push_back(h_pt_neg_cuton_truth_see_reco);
			pt_pos_cuton_reco_see_truth_list.push_back(h_pt_pos_cuton_reco_see_truth);
			pt_neg_cuton_reco_see_truth_list.push_back(h_pt_neg_cuton_reco_see_truth);
			pt_pos_pull_list.push_back(h_pt_pos_pull);
			pt_neg_pull_list.push_back(h_pt_neg_pull);
			//for migration
			vector<TH1D*> pt_pos_migration; pt_pos_migration.clear();
			vector<TH1D*> pt_neg_migration; pt_neg_migration.clear();
			for(int k=0; k<6; k++){
				TString k_s = to_string(k);
				TH1D* h_pt_pos_migration = new TH1D("pt_pos_migration_"+i_s+"_"+k_s,"",100,stof(low),stof(high));
				TH1D* h_pt_neg_migration = new TH1D("pt_neg_migration_"+i_s+"_"+k_s,"",100,stof(low),stof(high));
				pt_pos_migration.push_back(h_pt_pos_migration);
				pt_neg_migration.push_back(h_pt_neg_migration);
			}
			pt_pos_reco_migration_list.push_back(pt_pos_migration);
			pt_neg_reco_migration_list.push_back(pt_neg_migration);

		}

	}
	readtree_(year,inputpath,outputpath,isdata,mass_name,binned,low,high,stof(init_mean), stof(buffer),
			
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
			
			eta_pos_cuton_truth_see_reco_list, eta_neg_cuton_truth_see_reco_list,
			phi_pos_cuton_truth_see_reco_list, phi_neg_cuton_truth_see_reco_list,
			pt_pos_cuton_truth_see_reco_list, pt_neg_cuton_truth_see_reco_list,

			eta_pos_cuton_reco_see_truth_list, eta_neg_cuton_reco_see_truth_list,
			phi_pos_cuton_reco_see_truth_list, phi_neg_cuton_reco_see_truth_list,
			pt_pos_cuton_reco_see_truth_list, pt_neg_cuton_reco_see_truth_list,
			
			eta_pos_pull_list, eta_neg_pull_list,
			phi_pos_pull_list, phi_neg_pull_list,
			pt_pos_pull_list, pt_neg_pull_list,
						
			tot_pos_eta, tot_neg_eta,
			tot_pos_phi, tot_neg_phi,
			tot_pos_pt, tot_neg_pt,
			
			tot_zpt, tot_zpz,
			
			pt_pos_reco_migration_list, pt_neg_reco_migration_list,
			eta_pos_reco_migration_list, eta_neg_reco_migration_list,
			phi_pos_reco_migration_list, phi_neg_reco_migration_list,
			
			pt1_corr_vs_gen, pt2_corr_vs_gen,
			pt1_uncorr_vs_gen, pt2_uncorr_vs_gen

			//pos_eta_vs_mass, neg_eta_vs_mass,
			//pos_phi_vs_mass, neg_phi_vs_mass,
		);		
			
}

void readtree_(TString year, TString inputpath, TString outputpath, TString isdata, TString mass_name, TString binned, string low, string high, float init_mean, float buffer,
	
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
	
	vector<TH1D*> &eta_pos_cuton_truth_see_reco, vector<TH1D*> &eta_neg_cuton_truth_see_reco,
	vector<TH1D*> &phi_pos_cuton_truth_see_reco, vector<TH1D*> &phi_neg_cuton_truth_see_reco,
	vector<TH1D*> &pt_pos_cuton_truth_see_reco, vector<TH1D*> &pt_neg_cuton_truth_see_reco,

	vector<TH1D*> &eta_pos_cuton_reco_see_truth, vector<TH1D*> &eta_neg_cuton_reco_see_truth,
	vector<TH1D*> &phi_pos_cuton_reco_see_truth, vector<TH1D*> &phi_neg_cuton_reco_see_truth,
	vector<TH1D*> &pt_pos_cuton_reco_see_truth, vector<TH1D*> &pt_neg_cuton_reco_see_truth,
	
	vector<TH1D*> &eta_pos_pull_list, vector<TH1D*> &eta_neg_pull_list,
	vector<TH1D*> &phi_pos_pull_list, vector<TH1D*> &phi_neg_pull_list,
	vector<TH1D*> &pt_pos_pull_list, vector<TH1D*> &pt_neg_pull_list,

	TH1D* tot_pos_eta, TH1D* tot_neg_eta,
	TH1D* tot_pos_phi, TH1D* tot_neg_phi,
	TH1D* tot_pos_pt, TH1D* tot_neg_pt,
	//TH2D* pos_eta_vs_mass, TH2D* neg_eta_vs_mass,
	//TH2D* pos_phi_vs_mass, TH2D* neg_phi_vs_mass,
	TH1D* tot_zpt, TH1D* tot_zpz,
	
	vector<vector<TH1D*>> &pt_pos_reco_migration_list, vector<vector<TH1D*>> &pt_neg_reco_migration_list,
	vector<vector<TH1D*>> &eta_pos_reco_migration_list, vector<vector<TH1D*>> &eta_neg_reco_migration_list,
	vector<vector<TH1D*>> &phi_pos_reco_migration_list, vector<vector<TH1D*>> &phi_neg_reco_migration_list,
	
	TH2D* pt1_corr_vs_gen, TH2D* pt2_corr_vs_gen,
	TH2D* pt1_uncorr_vs_gen, TH2D* pt2_uncorr_vs_gen

){

	TString pt1name, pt2name, eta1name, eta2name, phi1name, phi2name, q1name, q2name, massname;
	if(GEN==false){
		if(mass_name == "mass2mu_corr"){
			pt1name = "pt1_corr";
			pt2name = "pt2_corr";
		}
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
		eta1name = "eta1"; eta2name = "eta2"; phi1name = "phi1"; phi2name = "phi2"; q1name = "mu1_q_roc"; q2name = "mu2_q_roc";
		massname = mass_name;
		massname = "GENmassZ";
	}
	if(GEN==true){
		pt1name = "genlep1_pt"; pt2name = "genlep2_pt"; eta1name = "genlep1_eta"; eta2name = "genlep2_eta"; phi1name = "genlep1_phi"; phi2name = "genlep2_phi";
		q1name = "genlep1_q"; q2name = "genlep2_q";
		massname = "GENmassZ";
	}
	//set tree val holders
	double pt1, pt2, pt1_uncorr, pt2_uncorr, eta1, eta2, phi1, phi2, pterr1, pterr2, pterrold1, pterrold2, mass, GENmassZ;
	float genpt1, genpt2, geneta1, geneta2, genphi1, genphi2;
	float Sip1, Sip2, RelIso1, RelIso2, weight, weight1, weight2;
	bool id1, id2, trigger1, trigger2;
	int q1, q2, nFSRPhotons;
	bool genmatching;

	//set binned variables
	float pt1_bin, pt2_bin, eta1_bin, eta2_bin, phi1_bin, phi2_bin, mass_bin;
	//set filled variables
	float pt1_fill, pt2_fill, eta1_fill, eta2_fill, phi1_fill, phi2_fill, mass_fill;

	TFile* f = new TFile(inputpath,"READ");
	TTree* t = (TTree*)f->Get("passedEvents");
	t->SetBranchStatus("*",0);
	t->SetBranchStatus(pt1name,1);
	t->SetBranchStatus(pt2name,1);
	t->SetBranchStatus(eta1name,1);
	t->SetBranchStatus(eta2name,1);
	t->SetBranchStatus(phi1name,1);
	t->SetBranchStatus(phi2name,1);
	t->SetBranchStatus(q1name,1);
	t->SetBranchStatus(q2name,1);
	t->SetBranchStatus("pt1_uncorr",1);
	t->SetBranchStatus("pt2_uncorr",1);
	t->SetBranchStatus("Sip1",1);
	t->SetBranchStatus("Sip2",1);
	t->SetBranchStatus("RelIso1",1);
	t->SetBranchStatus("RelIso2",1);
	t->SetBranchStatus("mu1_hzzid",1);
	t->SetBranchStatus("mu2_hzzid",1);
	t->SetBranchStatus("trigger24matching1",1);
	t->SetBranchStatus("trigger24matching2",1);
	t->SetBranchStatus("genmatching",1);	
	t->SetBranchStatus(massname,1);
	t->SetBranchStatus("weight1",1);
	t->SetBranchStatus("weight2",1);
	t->SetBranchStatus("weight",1);
	t->SetBranchStatus("lep1_pterr",1);
	t->SetBranchStatus("lep2_pterr",1);
	t->SetBranchStatus("lep1_pterrold",1);
	t->SetBranchStatus("lep2_pterrold",1);
	t->SetBranchStatus("genlep1_pt",1);
	t->SetBranchStatus("genlep2_pt",1);
	t->SetBranchStatus("genlep1_eta",1);
	t->SetBranchStatus("genlep2_eta",1);
	t->SetBranchStatus("genlep1_phi",1);
	t->SetBranchStatus("genlep2_phi",1);
	t->SetBranchStatus("nFSRPhotons",1);
	t->SetBranchStatus("GENmassZ",1);

	t->SetBranchAddress(pt1name,&pt1);
	t->SetBranchAddress(pt2name,&pt2);
	t->SetBranchAddress("pt1_uncorr",&pt1_uncorr);
	t->SetBranchAddress("pt2_uncorr",&pt2_uncorr);
	t->SetBranchAddress("lep1_pterr",&pterr1);
	t->SetBranchAddress("lep2_pterr",&pterr2);
	t->SetBranchAddress("lep1_pterrold",&pterrold1);
	t->SetBranchAddress("lep2_pterrold",&pterrold2);
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
	t->SetBranchAddress("mu1_hzzid",&id1);
	t->SetBranchAddress("mu2_hzzid",&id2);
	t->SetBranchAddress("trigger24matching1",&trigger1);
	t->SetBranchAddress("trigger24matching2",&trigger2);
	t->SetBranchAddress("genmatching",&genmatching);
	t->SetBranchAddress(massname,&mass);
	t->SetBranchAddress("weight1",&weight1);
	t->SetBranchAddress("weight2",&weight2);
	t->SetBranchAddress("weight",&weight);
	t->SetBranchAddress("lep1_pterr",&pterr1);
	t->SetBranchAddress("lep2_pterr",&pterr2);
	t->SetBranchAddress("lep1_pterrold",&pterrold1);
	t->SetBranchAddress("lep2_pterrold",&pterrold2);
	t->SetBranchAddress("genlep1_pt",&genpt1);
	t->SetBranchAddress("genlep2_pt",&genpt2);
	t->SetBranchAddress("genlep1_eta",&geneta1);
	t->SetBranchAddress("genlep2_eta",&geneta2);
	t->SetBranchAddress("genlep1_phi",&genphi1);
	t->SetBranchAddress("genlep2_phi",&genphi2);
	t->SetBranchAddress("nFSRPhotons",&nFSRPhotons);
	t->SetBranchAddress("GENmassZ",&GENmassZ);

	TAxis* eta_axis = h_eta_std->GetXaxis();
	TAxis* phi_axis = h_phi_std->GetXaxis();
	TAxis* pt_axis = h_pt_std->GetXaxis();
	int eta_pos_bin, eta_neg_bin, phi_neg_bin, phi_pos_bin, pt_pos_bin, pt_neg_bin;
	TLorentzVector lep1, lep2, Z;
	
	for(int i=0; i<t->GetEntries(); i++){
		//if(i>100000)break;
		if(i%1000000==0)cout<<i<<"/"<<t->GetEntries()<<endl;
		t->GetEntry(i);
		
		weight=1;
		weight1=1;
		weight2=1;
		
		if(isdata=="mc"){
			pt1_bin = float(genpt1); pt2_bin = float(genpt2);
			eta1_bin = float(geneta1); eta2_bin = float(geneta2);
			phi1_bin = float(genphi1); phi2_bin = float(genphi2);
			mass_bin = float(GENmassZ);			
			pt1_fill = float(genpt1); pt2_fill = float(genpt2);
			eta1_fill = float(geneta1); eta2_fill = float(geneta2);
			phi1_fill = float(genphi1); phi2_fill = float(genphi2);
			mass_fill = float(GENmassZ);
		}else{

			pt1_bin = float(pt1); pt2_bin = float(pt2);
			eta1_bin = float(eta1); eta2_bin = float(eta2);
			phi1_bin = float(phi1); phi2_bin = float(phi2);
			mass_bin = float(mass);			
			pt1_fill = float(genpt1); pt2_fill = float(genpt2);
			eta1_fill = float(geneta1); eta2_fill = float(geneta2);
			phi1_fill = float(genphi1); phi2_fill = float(genphi2);
			mass_fill = float(GENmassZ);
		}
		
		if(genmatching && nFSRPhotons==0 && id1 && id2 && RelIso1<0.35 && RelIso2<0.35 && Sip1<4 && Sip2<4 && (trigger1||trigger2) && genpt1>25 && genpt2>5 && genpt1<100 && genpt2<100 && abs(geneta1)<2.4 && abs(geneta2)<2.4 && mass_fill<stof(high)&&mass_fill>stof(low) ){
//			pt1_corr_vs_gen->Fill(pt1,genpt1);
//			pt2_corr_vs_gen->Fill(pt2,genpt2);
//			pt1_uncorr_vs_gen->Fill(pt1_uncorr,genpt1);
//			pt2_uncorr_vs_gen->Fill(pt2_uncorr,genpt2);			
			if(q1==1){
		/*		
				tot_pos_eta->Fill(eta1_fill,weight1);
				tot_neg_eta->Fill(eta2_fill,weight2);
				tot_pos_phi->Fill(phi1_fill,weight1);
				tot_neg_phi->Fill(phi2_fill,weight2);
				tot_pos_pt->Fill(pt1_fill,weight1);
				tot_neg_pt->Fill(pt2_fill,weight2);
				lep1.SetPtEtaPhiM(pt1_fill,eta1_fill,phi1_fill,0.1057);
				lep2.SetPtEtaPhiM(pt2_fill,eta2_fill,phi2_fill,0.1057);
				double zpt = (lep1+lep2).Pt();
				double zeta = (lep1+lep2).Eta();
				double zphi = (lep1+lep2).Phi();
				double zpz = (lep1+lep2).Pz();
				double angle = lep1.Angle(lep2.Vect());
				tot_zpt->Fill(zpt,weight);
				tot_zpz->Fill(zpz,weight);
			
				
				int pt_pos_reco_bin = pt_axis->FindBin(pt1)-1;
				int pt_pos_gen_bin = pt_axis->FindBin(genpt1)-1;
				int eta_pos_reco_bin = eta_axis->FindBin(eta1)-1;
				int eta_pos_gen_bin = eta_axis->FindBin(geneta1)-1;
				int phi_pos_reco_bin = phi_axis->FindBin(phi1)-1;
				int phi_pos_gen_bin = phi_axis->FindBin(genphi1)-1;

				pt_pos_reco_migration_list.at(pt_pos_reco_bin).at(pt_pos_gen_bin)->Fill(mass);
				eta_pos_reco_migration_list.at(eta_pos_reco_bin).at(eta_pos_gen_bin)->Fill(mass);
				phi_pos_reco_migration_list.at(phi_pos_reco_bin).at(phi_pos_gen_bin)->Fill(mass);
		*/		

		//		if(pt_pos_reco_bin == pt_pos_gen_bin){	
		
					eta_pos_bin = eta_axis->FindBin(eta1_bin)-1;
					phi_pos_bin = phi_axis->FindBin(phi1_bin)-1;
					pt_pos_bin = pt_axis->FindBin(pt1_bin)-1;
					
					eta_pos_list.at(eta_pos_bin)->Fill(mass_fill,weight);
					phi_pos_list.at(phi_pos_bin)->Fill(mass_fill,weight);
					pt_pos_list.at(pt_pos_bin)->Fill(mass_fill,weight);
		/*
					eta_pos_zpt_list.at(eta_pos_bin)->Fill(zpt,weight);			//eta pos z 
					eta_pos_angle_list.at(eta_pos_bin)->Fill(angle,weight);
					eta_pos_zeta_list.at(eta_pos_bin)->Fill(zeta,weight);
					eta_pos_zphi_list.at(eta_pos_bin)->Fill(zphi,weight);
					eta_pos_zpz_list.at(eta_pos_bin)->Fill(zpz,weight);
				
					eta_pos_free_eta_list.at(eta_pos_bin)->Fill(eta2_fill,weight2);		//eta pos free fix
					eta_pos_fix_eta_list.at(eta_pos_bin)->Fill(eta1_fill,weight1);
					eta_pos_free_phi_list.at(eta_pos_bin)->Fill(phi2_fill,weight2);
					eta_pos_fix_phi_list.at(eta_pos_bin)->Fill(phi1_fill,weight1);
					eta_pos_free_pt_list.at(eta_pos_bin)->Fill(pt2_fill,weight2);
					eta_pos_fix_pt_list.at(eta_pos_bin)->Fill(pt1_fill,weight1);
					
					phi_pos_zpt_list.at(phi_pos_bin)->Fill(zpt,weight);			//phi pos z
					phi_pos_angle_list.at(phi_pos_bin)->Fill(angle,weight);
					phi_pos_zeta_list.at(phi_pos_bin)->Fill(zeta,weight);
					phi_pos_zphi_list.at(phi_pos_bin)->Fill(zphi,weight);
					phi_pos_zpz_list.at(phi_pos_bin)->Fill(zpz,weight);
				
					phi_pos_free_eta_list.at(phi_pos_bin)->Fill(eta2_fill,weight2);		//phi pos free fix
					phi_pos_fix_eta_list.at(phi_pos_bin)->Fill(eta1_fill,weight1);
					phi_pos_free_phi_list.at(phi_pos_bin)->Fill(phi2_fill,weight2);
					phi_pos_fix_phi_list.at(phi_pos_bin)->Fill(phi1_fill,weight1);
					phi_pos_free_pt_list.at(phi_pos_bin)->Fill(pt2_fill,weight2);
					phi_pos_fix_pt_list.at(phi_pos_bin)->Fill(pt1_fill,weight1);
					
					pt_pos_zpt_list.at(pt_pos_bin)->Fill(zpt,weight);			//pt pos z
					pt_pos_angle_list.at(pt_pos_bin)->Fill(angle,weight);
					pt_pos_zeta_list.at(pt_pos_bin)->Fill(zeta,weight);
					pt_pos_zphi_list.at(pt_pos_bin)->Fill(zphi,weight);
					pt_pos_zpz_list.at(pt_pos_bin)->Fill(zpz,weight);
				
					pt_pos_free_eta_list.at(pt_pos_bin)->Fill(pt2_fill,weight2);			//pt pos free fix
					pt_pos_fix_eta_list.at(pt_pos_bin)->Fill(pt1_fill,weight1);
					pt_pos_free_phi_list.at(pt_pos_bin)->Fill(phi2_fill,weight2);
					pt_pos_fix_phi_list.at(pt_pos_bin)->Fill(phi1_fill,weight1);
		*/
					pt_pos_free_pt_list.at(pt_pos_bin)->Fill(pt2_fill,weight2);
					pt_pos_fix_pt_list.at(pt_pos_bin)->Fill(pt1_fill,weight1);
		/*			
					pterr_pos_phi_list.at(phi_pos_bin)->Fill(pterr1,weight);
					pterr_pos_eta_list.at(eta_pos_bin)->Fill(pterr1,weight);
					pterr_pos_pt_list.at(pt_pos_bin)->Fill(pterr1,weight);

					pterrold_pos_phi_list.at(phi_pos_bin)->Fill(pterrold1,weight);
					pterrold_pos_eta_list.at(eta_pos_bin)->Fill(pterrold1,weight);
					pterrold_pos_pt_list.at(pt_pos_bin)->Fill(pterrold1,weight);

					eta_pos_pull_list.at(eta_pos_bin)->Fill((pt1-genpt1));
					phi_pos_pull_list.at(phi_pos_bin)->Fill((pt1-genpt1));
					pt_pos_pull_list.at(pt_pos_bin)->Fill((pt1-genpt1));
		*/
		//		}
		/*
				int pt_neg_reco_bin = pt_axis->FindBin(pt2)-1;
				int pt_neg_gen_bin = pt_axis->FindBin(genpt2)-1;
				int eta_neg_reco_bin = eta_axis->FindBin(eta2)-1;
				int eta_neg_gen_bin = eta_axis->FindBin(geneta2)-1;
				int phi_neg_reco_bin = phi_axis->FindBin(phi2)-1;
				int phi_neg_gen_bin = phi_axis->FindBin(genphi2)-1;
				
				pt_neg_reco_migration_list.at(pt_neg_reco_bin).at(pt_neg_gen_bin)->Fill(mass);
				eta_neg_reco_migration_list.at(eta_neg_reco_bin).at(eta_neg_gen_bin)->Fill(mass);
				phi_neg_reco_migration_list.at(phi_neg_reco_bin).at(phi_neg_gen_bin)->Fill(mass);
		*/		
		//		if(pt_neg_reco_bin == pt_neg_gen_bin){

					eta_neg_bin = eta_axis->FindBin(eta2_bin)-1;
					phi_neg_bin = phi_axis->FindBin(phi2_bin)-1;
					pt_neg_bin = pt_axis->FindBin(pt2_bin)-1;
					
					eta_neg_list.at(eta_neg_bin)->Fill(mass_fill,weight);
					phi_neg_list.at(phi_neg_bin)->Fill(mass_fill,weight);
					pt_neg_list.at(pt_neg_bin)->Fill(mass_fill,weight);
		/*	
					eta_neg_zpt_list.at(eta_neg_bin)->Fill(zpt,weight);
					eta_neg_angle_list.at(eta_neg_bin)->Fill(angle,weight);
					eta_neg_zeta_list.at(eta_neg_bin)->Fill(zeta,weight);
					eta_neg_zphi_list.at(eta_neg_bin)->Fill(zphi,weight);
					eta_neg_zpz_list.at(eta_neg_bin)->Fill(zpz,weight);
			
					eta_neg_free_eta_list.at(eta_neg_bin)->Fill(eta1_fill,weight1);
					eta_neg_fix_eta_list.at(eta_neg_bin)->Fill(eta2_fill,weight2);
					eta_neg_free_phi_list.at(eta_neg_bin)->Fill(phi1_fill,weight1);
					eta_neg_fix_phi_list.at(eta_neg_bin)->Fill(phi2_fill,weight2);
					eta_neg_free_pt_list.at(eta_neg_bin)->Fill(pt1_fill,weight1);
					eta_neg_fix_pt_list.at(eta_neg_bin)->Fill(pt2_fill,weight2);
				
					phi_neg_zpt_list.at(phi_neg_bin)->Fill(zpt,weight);
					phi_neg_angle_list.at(phi_neg_bin)->Fill(angle,weight);
					phi_neg_zeta_list.at(phi_neg_bin)->Fill(zeta,weight);
					phi_neg_zphi_list.at(phi_neg_bin)->Fill(zphi,weight);
					phi_neg_zpz_list.at(phi_neg_bin)->Fill(zpz,weight);
			
					phi_neg_free_eta_list.at(phi_neg_bin)->Fill(eta1_fill,weight1);
					phi_neg_fix_eta_list.at(phi_neg_bin)->Fill(eta2_fill,weight2);
					phi_neg_free_phi_list.at(phi_neg_bin)->Fill(phi1_fill,weight1);
					phi_neg_fix_phi_list.at(phi_neg_bin)->Fill(phi2_fill,weight2);
					phi_neg_free_pt_list.at(phi_neg_bin)->Fill(pt1_fill,weight1);
					phi_neg_fix_pt_list.at(phi_neg_bin)->Fill(pt2_fill,weight2);
			
					pt_neg_zpt_list.at(pt_neg_bin)->Fill(zpt,weight);
					pt_neg_angle_list.at(pt_neg_bin)->Fill(angle,weight);
					pt_neg_zeta_list.at(pt_neg_bin)->Fill(zeta,weight);
					pt_neg_zphi_list.at(pt_neg_bin)->Fill(zphi,weight);
					pt_neg_zpz_list.at(pt_neg_bin)->Fill(zpz,weight);
			
					pt_neg_free_eta_list.at(pt_neg_bin)->Fill(pt1_fill,weight1);
					pt_neg_fix_eta_list.at(pt_neg_bin)->Fill(pt2_fill,weight2);
					pt_neg_free_phi_list.at(pt_neg_bin)->Fill(phi1_fill,weight1);
					pt_neg_fix_phi_list.at(pt_neg_bin)->Fill(phi2_fill,weight2);
		*/			
					pt_neg_free_pt_list.at(pt_neg_bin)->Fill(pt1_fill,weight1);
					pt_neg_fix_pt_list.at(pt_neg_bin)->Fill(pt2_fill,weight2);
		/*
					pterr_neg_phi_list.at(phi_neg_bin)->Fill(pterr2,weight);
					pterr_neg_eta_list.at(eta_neg_bin)->Fill(pterr2,weight);
					pterr_neg_pt_list.at(pt_neg_bin)->Fill(pterr2,weight);
			
					pterrold_neg_phi_list.at(phi_neg_bin)->Fill(pterrold2,weight);
					pterrold_neg_eta_list.at(eta_neg_bin)->Fill(pterrold2,weight);
					pterrold_neg_pt_list.at(pt_neg_bin)->Fill(pterrold2,weight);

					eta_neg_pull_list.at(eta_neg_bin)->Fill((pt2-genpt2));
					phi_neg_pull_list.at(phi_neg_bin)->Fill((pt2-genpt2));
					pt_neg_pull_list.at(pt_neg_bin)->Fill((pt2-genpt2));
		*/
		
		//		}
			}
			if(q2==1){		
		/*
				tot_pos_eta->Fill(eta2_fill,weight2);
				tot_neg_eta->Fill(eta1_fill,weight1);
				tot_pos_phi->Fill(phi2_fill,weight2);
				tot_neg_phi->Fill(phi1_fill,weight1);
				tot_pos_pt->Fill(pt2_fill,weight2);
				tot_neg_pt->Fill(pt1_fill,weight1);			
				lep1.SetPtEtaPhiM(pt1_fill,eta1_fill,phi1_fill,0.1057);
				lep2.SetPtEtaPhiM(pt2_fill,eta2_fill,phi2_fill,0.1057);
				double zpt = (lep1+lep2).Pt();
				double zpz = (lep1+lep2).Pz();
				double angle = lep1.Angle(lep2.Vect());
				double zeta = (lep1+lep2).Eta();
				double zphi = (lep1+lep2).Phi();
				tot_zpt->Fill(zpt,weight);
				
				int pt_pos_reco_bin = pt_axis->FindBin(pt2)-1;
				int pt_pos_gen_bin = pt_axis->FindBin(genpt2)-1;
				int eta_pos_reco_bin = eta_axis->FindBin(eta2)-1;
				int eta_pos_gen_bin = eta_axis->FindBin(geneta2)-1;
				int phi_pos_reco_bin = phi_axis->FindBin(phi2)-1;
				int phi_pos_gen_bin = phi_axis->FindBin(genphi2)-1;
				
				pt_pos_reco_migration_list.at(pt_pos_reco_bin).at(pt_pos_gen_bin)->Fill(mass);
				eta_pos_reco_migration_list.at(eta_pos_reco_bin).at(eta_pos_gen_bin)->Fill(mass);
				phi_pos_reco_migration_list.at(phi_pos_reco_bin).at(phi_pos_gen_bin)->Fill(mass);
		*/
		//		if(pt_pos_reco_bin == pt_pos_gen_bin){

					eta_pos_bin = eta_axis->FindBin(eta2_bin)-1;
					phi_pos_bin = phi_axis->FindBin(phi2_bin)-1;
					pt_pos_bin = pt_axis->FindBin(pt2_bin)-1;

					eta_pos_list.at(eta_pos_bin)->Fill(mass_fill,weight);
					phi_pos_list.at(phi_pos_bin)->Fill(mass_fill,weight);
					pt_pos_list.at(pt_pos_bin)->Fill(mass_fill,weight);
		/*
					eta_pos_zpt_list.at(eta_pos_bin)->Fill(zpt,weight);
					eta_pos_angle_list.at(eta_pos_bin)->Fill(angle,weight);
					eta_pos_zeta_list.at(eta_pos_bin)->Fill(zeta,weight);
					eta_pos_zphi_list.at(eta_pos_bin)->Fill(zphi,weight);
					eta_pos_zpz_list.at(eta_pos_bin)->Fill(zpz,weight);
			
					eta_pos_free_eta_list.at(eta_pos_bin)->Fill(eta1_fill,weight1);
					eta_pos_fix_eta_list.at(eta_pos_bin)->Fill(eta2_fill,weight2);
					eta_pos_free_phi_list.at(eta_pos_bin)->Fill(phi1_fill,weight1);
					eta_pos_fix_phi_list.at(eta_pos_bin)->Fill(phi2_fill,weight2);
					eta_pos_free_pt_list.at(eta_pos_bin)->Fill(pt1_fill,weight1);
					eta_pos_fix_pt_list.at(eta_pos_bin)->Fill(pt2_fill,weight2);
				
					phi_pos_zpt_list.at(phi_pos_bin)->Fill(zpt,weight);
					phi_pos_angle_list.at(phi_pos_bin)->Fill(angle,weight);
					phi_pos_zeta_list.at(phi_pos_bin)->Fill(zeta,weight);
					phi_pos_zphi_list.at(phi_pos_bin)->Fill(zphi,weight);
					phi_pos_zpz_list.at(phi_pos_bin)->Fill(zpz,weight);
			
					phi_pos_free_eta_list.at(phi_pos_bin)->Fill(eta1_fill,weight1);
					phi_pos_fix_eta_list.at(phi_pos_bin)->Fill(eta2_fill,weight2);
					phi_pos_free_phi_list.at(phi_pos_bin)->Fill(phi1_fill,weight1);
					phi_pos_fix_phi_list.at(phi_pos_bin)->Fill(phi2_fill,weight2);
					phi_pos_free_pt_list.at(phi_pos_bin)->Fill(pt1_fill,weight1);
					phi_pos_fix_pt_list.at(phi_pos_bin)->Fill(pt2_fill,weight2);
				
					pt_pos_zpt_list.at(pt_pos_bin)->Fill(zpt,weight);
					pt_pos_angle_list.at(pt_pos_bin)->Fill(angle,weight);
					pt_pos_zeta_list.at(pt_pos_bin)->Fill(zeta,weight);
					pt_pos_zphi_list.at(pt_pos_bin)->Fill(zphi,weight);
					pt_pos_zpz_list.at(pt_pos_bin)->Fill(zpz,weight);
								
					pt_pos_free_eta_list.at(pt_pos_bin)->Fill(eta1_fill,weight1);
					pt_pos_fix_eta_list.at(pt_pos_bin)->Fill(eta2_fill,weight2);
					pt_pos_free_phi_list.at(pt_pos_bin)->Fill(phi1_fill,weight1);
					pt_pos_fix_phi_list.at(pt_pos_bin)->Fill(phi2_fill,weight2);
		*/
					pt_pos_free_pt_list.at(pt_pos_bin)->Fill(pt1_fill,weight1);
					pt_pos_fix_pt_list.at(pt_pos_bin)->Fill(pt2_fill,weight2);
		/*
					pterr_pos_phi_list.at(phi_pos_bin)->Fill(pterr2,weight);
					pterr_pos_eta_list.at(eta_pos_bin)->Fill(pterr2,weight);
					pterr_pos_pt_list.at(pt_pos_bin)->Fill(pterr2,weight);
					pterrold_pos_phi_list.at(phi_pos_bin)->Fill(pterrold2,weight);
					pterrold_pos_eta_list.at(eta_pos_bin)->Fill(pterrold2,weight);
					pterrold_pos_pt_list.at(pt_pos_bin)->Fill(pterrold2,weight);

					eta_pos_pull_list.at(eta_pos_bin)->Fill((pt2-genpt2));
					phi_pos_pull_list.at(phi_pos_bin)->Fill((pt2-genpt2));
					pt_pos_pull_list.at(pt_pos_bin)->Fill((pt2-genpt2));
		*/
		//		}

		/*		int pt_neg_reco_bin = pt_axis->FindBin(pt1)-1;
				int pt_neg_gen_bin = pt_axis->FindBin(genpt1)-1;
				int eta_neg_reco_bin = eta_axis->FindBin(eta1)-1;
				int eta_neg_gen_bin = eta_axis->FindBin(geneta1)-1;
				int phi_neg_reco_bin = phi_axis->FindBin(phi1)-1;
				int phi_neg_gen_bin = phi_axis->FindBin(genphi1)-1;
		
				pt_neg_reco_migration_list.at(pt_neg_reco_bin).at(pt_neg_gen_bin)->Fill(mass);
				eta_neg_reco_migration_list.at(eta_neg_reco_bin).at(eta_neg_gen_bin)->Fill(mass);
				phi_neg_reco_migration_list.at(phi_neg_reco_bin).at(phi_neg_gen_bin)->Fill(mass);
		*/
		//		if(pt_neg_reco_bin == pt_neg_gen_bin){	
					eta_neg_bin = eta_axis->FindBin(eta1_bin)-1;
					phi_neg_bin = phi_axis->FindBin(phi1_bin)-1;
					pt_neg_bin = pt_axis->FindBin(pt1_bin)-1;
						
					eta_neg_list.at(eta_neg_bin)->Fill(mass_fill,weight);
					phi_neg_list.at(phi_neg_bin)->Fill(mass_fill,weight);
					pt_neg_list.at(pt_neg_bin)->Fill(mass_fill,weight);
		/*
					eta_neg_zpt_list.at(eta_neg_bin)->Fill(zpt,weight);
					eta_neg_angle_list.at(eta_neg_bin)->Fill(angle,weight);
					eta_neg_zeta_list.at(eta_neg_bin)->Fill(zeta,weight);
					eta_neg_zphi_list.at(eta_neg_bin)->Fill(zphi,weight);
					eta_neg_zpz_list.at(eta_neg_bin)->Fill(zpz,weight);
				
					eta_neg_free_eta_list.at(eta_neg_bin)->Fill(eta2_fill,weight2);
					eta_neg_fix_eta_list.at(eta_neg_bin)->Fill(eta1_fill,weight1);
					eta_neg_free_phi_list.at(eta_neg_bin)->Fill(phi2_fill,weight2);
					eta_neg_fix_phi_list.at(eta_neg_bin)->Fill(phi1_fill,weight1);
					eta_neg_free_pt_list.at(eta_neg_bin)->Fill(pt2_fill,weight2);
					eta_neg_fix_pt_list.at(eta_neg_bin)->Fill(pt1_fill,weight1);
				
					phi_neg_zpt_list.at(phi_neg_bin)->Fill(zpt,weight);
					phi_neg_angle_list.at(phi_neg_bin)->Fill(angle,weight);
					phi_neg_zeta_list.at(phi_neg_bin)->Fill(zeta,weight);
					phi_neg_zphi_list.at(phi_neg_bin)->Fill(zphi,weight);
					phi_neg_zpz_list.at(phi_neg_bin)->Fill(zpz,weight);
				
					phi_neg_free_eta_list.at(phi_neg_bin)->Fill(eta2_fill,weight2);
					phi_neg_fix_eta_list.at(phi_neg_bin)->Fill(eta1_fill,weight1);
					phi_neg_free_phi_list.at(phi_neg_bin)->Fill(phi2_fill,weight2);
					phi_neg_fix_phi_list.at(phi_neg_bin)->Fill(phi1_fill,weight1);
					phi_neg_free_pt_list.at(phi_neg_bin)->Fill(pt2_fill,weight2);
					phi_neg_fix_pt_list.at(phi_neg_bin)->Fill(pt1_fill,weight1);

					pt_neg_zpt_list.at(pt_neg_bin)->Fill(zpt,weight);
					pt_neg_angle_list.at(pt_neg_bin)->Fill(angle,weight);
					pt_neg_zeta_list.at(pt_neg_bin)->Fill(zeta,weight);
					pt_neg_zphi_list.at(pt_neg_bin)->Fill(zphi,weight);
					pt_neg_zpz_list.at(pt_neg_bin)->Fill(zpz,weight);
								
					pt_neg_free_eta_list.at(pt_neg_bin)->Fill(eta2_fill,weight2);
					pt_neg_fix_eta_list.at(pt_neg_bin)->Fill(eta1_fill,weight1);
					pt_neg_free_phi_list.at(pt_neg_bin)->Fill(phi2_fill,weight2);
					pt_neg_fix_phi_list.at(pt_neg_bin)->Fill(phi1_fill,weight1);
					pt_neg_free_pt_list.at(pt_neg_bin)->Fill(pt2_fill,weight2);
					pt_neg_fix_pt_list.at(pt_neg_bin)->Fill(pt1_fill,weight1);

					pterr_neg_phi_list.at(phi_neg_bin)->Fill(pterr1,weight);
					pterr_neg_eta_list.at(eta_neg_bin)->Fill(pterr1,weight);
					pterr_neg_pt_list.at(pt_neg_bin)->Fill(pterr1,weight);
				
					pterrold_neg_phi_list.at(phi_neg_bin)->Fill(pterrold1,weight);
					pterrold_neg_eta_list.at(eta_neg_bin)->Fill(pterrold1,weight);
					pterrold_neg_pt_list.at(pt_neg_bin)->Fill(pterrold1,weight);
				
					eta_neg_pull_list.at(eta_neg_bin)->Fill((pt1-genpt1));
					phi_neg_pull_list.at(phi_neg_bin)->Fill((pt1-genpt1));
					pt_neg_pull_list.at(pt_neg_bin)->Fill((pt1-genpt1));
		*/
		//		}//genpt and reco pt at same bin
			
			}//q2=1
			
		}//selection
	}//event loop
	/*
	if(isdata!="mc"){
		migration_summary(outputpath, pt_pos_reco_migration_list, "pt_pos","migration");
		migration_summary(outputpath, pt_neg_reco_migration_list, "pt_neg","migration");
		migration_summary(outputpath, eta_pos_reco_migration_list, "eta_pos","migration");
		migration_summary(outputpath, eta_neg_reco_migration_list, "eta_neg","migration");
		migration_summary(outputpath, phi_pos_reco_migration_list, "phi_pos","migration");
		migration_summary(outputpath, phi_neg_reco_migration_list, "phi_neg","migration");
	}
	*/	
	mass_distributions(outputpath,eta_pos_list,eta_neg_list,low,high,mass_name,"eta","mass_distributions");
	mass_distributions(outputpath,phi_pos_list,phi_neg_list,low,high,mass_name,"phi","mass_distributions");
	mass_distributions(outputpath,pt_pos_list,pt_neg_list,low,high,mass_name,"pt","mass_distributions");
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
	*/
	mass_distributions(outputpath,pt_pos_fix_pt_list,pt_neg_fix_pt_list,low,high,mass_name,"pt","fix_pt_distributions");
	mass_distributions(outputpath,pt_pos_free_pt_list,pt_neg_free_pt_list, low,high,mass_name,"pt","free_pt_distributions");
	/*
	mass_distributions(outputpath,pterr_pos_phi_list,pterr_neg_phi_list,low,high,mass_name,"phi","pterr_distributions");
	mass_distributions(outputpath,pterr_pos_eta_list,pterr_neg_eta_list,low,high,mass_name,"eta","pterr_distributions");
	mass_distributions(outputpath,pterr_pos_pt_list,pterr_neg_pt_list,low,high,mass_name,"pt","pterr_distributions");

	mass_distributions(outputpath,pterrold_pos_phi_list,pterrold_neg_phi_list,low,high,mass_name,"phi","pterrold_distributions");
	mass_distributions(outputpath,pterrold_pos_eta_list,pterrold_neg_eta_list,low,high,mass_name,"eta","pterrold_distributions");
	mass_distributions(outputpath,pterrold_pos_pt_list,pterrold_neg_pt_list,low,high,mass_name,"pt","pterrold_distributions");
	
	mass_distributions(outputpath,eta_pos_pull_list,eta_neg_pull_list,low,high,mass_name,"eta","pull_distributions");
	mass_distributions(outputpath,phi_pos_pull_list,phi_neg_pull_list,low,high,mass_name,"phi","pull_distributions");
	mass_distributions(outputpath,pt_pos_pull_list,pt_neg_pull_list,low,high,mass_name,"pt","pull_distributions");

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
*/	
/*	TCanvas cc("cc","",1400,1000);
	Custom2DHistoStyle(pt1_corr_vs_gen,"","pt1_corr","genpt1",false);
	Custom2DHistoStyle(pt2_corr_vs_gen,"","pt1_corr","genpt1",false);
	Custom2DHistoStyle(pt1_uncorr_vs_gen,"","pt1_uncorr","genpt1",false);
	Custom2DHistoStyle(pt2_uncorr_vs_gen,"","pt2_uncorr","genpt2",false);
	cc.cd();
	pt1_corr_vs_gen->Draw("colz");
	cc.SaveAs(outputpath + "/pt1_corr_vs_genpt1.png");
	cc.Clear();
	pt2_corr_vs_gen->Draw("colz");
	cc.SaveAs(outputpath + "/pt2_corr_vs_genpt2.png");
	cc.Clear();
	pt1_uncorr_vs_gen->Draw("colz");
	cc.SaveAs(outputpath + "/pt1_uncorr_vs_genpt1.png");
	cc.Clear();
	pt2_uncorr_vs_gen->Draw("colz");
	cc.SaveAs(outputpath + "/pt2_uncorr_vs_genpt2.png");
*/	
	//do fit
	dofit(year,outputpath,eta_pos_list,low,high,isdata,mass_name,init_mean, buffer, "eta");
	dofit(year,outputpath,eta_neg_list,low,high,isdata,mass_name,init_mean, buffer, "eta");
	dofit(year,outputpath,phi_pos_list,low,high,isdata,mass_name,init_mean, buffer, "phi");
	dofit(year,outputpath,phi_neg_list,low,high,isdata,mass_name,init_mean, buffer, "phi");
	dofit(year,outputpath,pt_pos_list,low,high,isdata,mass_name,init_mean, buffer, "pt");
	dofit(year,outputpath,pt_neg_list,low,high,isdata,mass_name,init_mean, buffer, "pt");
}

void dofit(TString year,TString outputpath, vector<TH1D*> &histos, string low, string high, TString isdata, TString mass_name,float init_mean, float buffer, TString k){	
	
	float width=2.496;
	float peak=91.19;
	/*if (isdata == "mc")*/
	{width=2.441;peak=91.19;}

	if(GEN==true){
	for(int i=0; i<histos.size(); i++){

		peak = histos.at(i)->GetBinCenter(histos.at(i)->GetMaximumBin());
		RooRealVar x("x","",stof(low),stof(high));
		x.setBins(int(((stof(high)-stof(low))*1000.0)/10.0),"cache");
		RooRealVar PDGmZ("peak","PDGmZ",peak,0.9*peak,1.1*peak);
		RooRealVar PDGwZ("width","PDGwZ",width,0.9*width,1.1*width);
		RooBreitWigner Model("Model","PDGBW",x,PDGmZ,PDGwZ);

		RooDataHist dataset("dataset","dataset",RooArgSet(x),Import(*histos.at(i)));
		dataset.Print();
		
		x.setRange("peak",89.5,92.5);	
		//Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1),Range("peak"));
		//Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1),Range("peak"));
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
		//c.SaveAs(outputpath + "/fitplots/"+k+"/"+year + "_" + isdata + "_" + mass_name + "_" + histos.at(i)->GetName() + "_" + low + "_" + high + ".png");
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
		double init_sigma=1.0, init_a1=1.0, init_a2=1.0, init_n1=10.0, init_n2=10.0, init_tau=0, init_fsig=0.9;
		
		RooRealVar x("x","",stof(low),stof(high));
		x.setBins(int(((stof(high)-stof(low))*1000.0)/10.0),"cache");
		RooRealVar PDGmZ("PDGmZ","PDGmZ",peak,0.9*peak,1.1*peak);
		RooRealVar PDGwZ("PDGwZ","PDGwZ",width,0.9*width,1.1*width);
		RooBreitWigner PDGBW("PDGBW","PDGBW",x,PDGmZ,PDGwZ);
		PDGmZ.setConstant(kTRUE);
		PDGwZ.setConstant(kTRUE);
		RooRealVar m("m","m",init_mean,-5,5);
		RooRealVar s("s","s",init_sigma,0,10);
		RooRealVar a1("a1","a1",init_a1,0,10);
		RooRealVar n1("n1","n1",init_n1,0,70);
		RooRealVar a2("a2","2",init_a2,0,10);
		RooRealVar n2("n2","n2",init_n2,0,70);
		RooRealVar tau("tau","tau",init_tau,-1,1);
		RooRealVar fsig("fsig","fsig",init_fsig,0.5,1);
		RooDoubleCB DCB("DCB","DCB",x,m,s,a1,n1,a2,n2);
		RooFFTConvPdf BW_DCB("BW_DCB","BW_DCB",x,PDGBW,DCB);
		BW_DCB.setBufferFraction(buffer);
		RooExponential bkg("bkg","bkg",x,tau);
		RooAddPdf Model("Model","Model",BW_DCB,bkg,fsig);
		RooDataHist dataset("dataset","dataset",RooArgSet(x),Import(*histos.at(i)));
		dataset.Print();
		//Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1));		
		//RooFitResult* r  = Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1),Save(kTRUE));
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
		//Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1));//,Range("core"));
		//Model.fitTo(dataset,SumW2Error(kTRUE),PrintLevel(-1));//,Range("core"));
			
		RooChi2Var chi2_var("chi2","chi2",Model,dataset);
		TString ch2_s = to_string(chi2_var.getVal()/dataset.numEntries());
		TString peak_s = to_string(m.getVal());
		TString peaktofile_s = to_string(m.getVal()+peak);
		TString peakerr_s = to_string(m.getError());
		TString mean_s = to_string(histos.at(i)->GetMean());
		TString sig_s = to_string(s.getVal()); TString entries_s = to_string(int(dataset.sumEntries()));
		TString n1_s = to_string(n1.getVal()); TString a1_s = to_string(a1.getVal());
		TString n2_s = to_string(n2.getVal()); TString a2_s = to_string(a2.getVal());
		//TString fsig_s = to_string(fsig.getVal()); TString tau_s = to_string(tau.getVal());
		RooPlot* frame = x.frame(Bins(30));
		frame->SetTitle("");
		TH1D* h_tmp = (TH1D*)histos.at(i)->Rebin(int(((stof(high)-stof(low))*1000.0)/10.0)/100);
		RooDataHist dataset_toplot("dataset_toplot","dataset_toplot",RooArgSet(x),Import(*h_tmp));
		dataset_toplot.plotOn(frame);
		Model.plotOn(frame);//,Range("core"),RooFit::NormRange("core"));
		Model.plotOn(frame,Components(bkg));
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
		latex->DrawLatex(0.7,0.7,"a1="+a1_s);
		latex->DrawLatex(0.7,0.6,"n1="+n1_s);
		latex->DrawLatex(0.7,0.5,"a2="+a2_s);
		latex->DrawLatex(0.7,0.4,"n2="+n2_s);
		latex->DrawLatex(0.7,0.3,"entries="+entries_s);
		//latex->DrawLatex(0.7,0.2,"fsig="+fsig_s);
		//latex->DrawLatex(0.7,0.1,"tau="+tau_s);
		LL.Draw("same");
		RL.Draw("same");
		//c.SaveAs(outputpath +"/fitplots/"+k+"/"+ year + "_" + isdata + "_" + mass_name + "_" + histos.at(i)->GetName() + "_" + low + "_" + high + ".png");
		ofstream fo;
		fo.open(outputpath + year + "_" + isdata + "_" + mass_name + "_" + histos.at(i)->GetName() + ".py");
		fo<<"info = {"<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_peak':" + peaktofile_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_peakerr':" + peakerr_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_mean':" + mean_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_a1':" + a1_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_a2':" + a2_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_n1':" + n1_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_n2':" + n2_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_sigma':" + sig_s+","<<endl;
		fo<<"'"+year+"_"+isdata+"_"+mass_name+"_"+histos.at(i)->GetName()+"_"+low+"_"+high+"_shift':" + peak_s+","<<endl;
		fo<<"}"<<endl;
		fo.close();
		
		delete latex;
		delete frame;
		delete h_tmp;
	}
	}
}
