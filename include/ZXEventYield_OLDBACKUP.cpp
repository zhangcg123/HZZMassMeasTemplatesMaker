#include "include/Set4LSlimmedTree.h"

void ZXEventYield_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString indir1, TString indir2);

int main(int argc, char* argv[]){
        TString year  = argv[1];
        TString type = argv[2];
        TString tag = argv[3];
        int fs = atoi(argv[4]);
        int cates = atoi(argv[5]);
        TString plotpath = argv[6];
        TString indir1 = argv[7];
	TString indir2 = argv[8];

	ZXEventYield_(plotpath, fs, type, tag, year, cates, indir1, indir2);

}

void ZXEventYield_(TString plotpath, int fs, TString type, TString tag, TString year, int cates, TString indir1, TString indir2){
	
	TString fs_name;
	if(fs==1)fs_name="4mu";
	if(fs==2)fs_name="4e";
	if(fs==3)fs_name="2e2mu";
	if(fs==4)fs_name="2mu2e";

	//retrieve qqzz event yields, w/o vbf tag
	TString Cate_s = to_string(cates);
	
	float* n = new float[cates];

	for(int i=0; i<cates; i++){
		TString path = indir1;
		TString i_s = to_string(i);
		path = path + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_bkgnorm_qqzz.py";
		ifstream in;
		in.open(path);
		string str;
		while(!in.eof())
		{
			getline(in,str);
			if(str=="info = {")continue;
			if(str=="}")break;
			std::size_t pos = str.find(":");
			str = str.substr(pos);
			str = str.substr(2);
			str = str.substr(0, str.length()-1);
			n[i] = atof(str.c_str());
		}
		in.close();
	}

	//retrieve qqzz event yields, no tag, used to calculate tot. event
	float* n_ = new float[cates];
	for(int i=0; i<cates; i++){
		TString path = indir2;
		TString i_s = to_string(i);
		path = path + year + "_" + fs_name + "_" + type + "_notag_" + i_s + "_bkgnorm_qqzz.py";
		ifstream in;
		in.open(path);
		string str;
		while(!in.eof())
		{
			getline(in,str);
			if(str=="info = {")continue;
			if(str=="}")break;
			std::size_t pos = str.find(":");
			str = str.substr(pos);
			str = str.substr(2);
			str = str.substr(0, str.length()-1);
			n_[i] = atof(str.c_str());
		}
		in.close();
	}
	float tot = 0;
	for(int i=0; i<cates; i++){
		tot = tot + n_[i];
	}

	//ratio
	float* ratio = new float[cates];
	for(int i=0; i<cates; i++){
		ratio[i] = n[i]/tot;
		cout<<"######qqZZ_"<<year<<"_"<<fs_name<<"_"<<Cate_s<<"_"<<tag<<"	"<<n[i]<<"/"<<tot<<"="<<ratio[i]<<endl;
	}

	float tot_zx;
	float qqzz_2e2m_evt, qqzz_2m2e_evt;
	if(year == "2016"){qqzz_2e2m_evt=16.130039;qqzz_2m2e_evt=10.274862;}
	if(year == "2017"){qqzz_2e2m_evt=17.590396;qqzz_2m2e_evt=10.882449;}
	if(year == "2018"){qqzz_2e2m_evt=26.353169;qqzz_2m2e_evt=15.735331;}

	if(year == "2016" && fs_name == "4mu")tot_zx = 12.2527;				//from hig-16-041 directly
	if(year == "2016" && fs_name == "4e")tot_zx = 5.1283;
	if(year == "2016" && fs_name == "2e2mu")tot_zx = 18.411;
	if(year == "2016" && fs_name == "2mu2e")tot_zx = 18.411;

	if(year == "2017" && fs_name == "4mu")tot_zx = 14.1663;				//scaled with lumi
	if(year == "2017" && fs_name == "4e")tot_zx = 5.9292;
	if(year == "2017" && fs_name == "2e2mu")tot_zx = 21.2864;
	if(year == "2017" && fs_name == "2mu2e")tot_zx = 21.2864;

	if(year == "2018" && fs_name == "4mu")tot_zx = 20.3579;				//scaled with lumi
	if(year == "2018" && fs_name == "4e")tot_zx = 8.5206;
	if(year == "2018" && fs_name == "2e2mu")tot_zx = 30.5893;
	if(year == "2018" && fs_name == "2mu2e")tot_zx = 30.5893;

	float* n_zx = new float[cates];
	for(int i=0; i<cates; i++){
		if(fs_name=="4mu"||fs_name=="4e")n_zx[i] = tot_zx * ratio[i];
		if(fs_name=="2e2mu")n_zx[i] = tot_zx * ratio[i] * (qqzz_2e2m_evt/(qqzz_2e2m_evt+qqzz_2m2e_evt));
		if(fs_name=="2mu2e")n_zx[i] = tot_zx * ratio[i] * (qqzz_2m2e_evt/(qqzz_2e2m_evt+qqzz_2m2e_evt));
	}

	TString plotpath_tmp;
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		plotpath_tmp = plotpath + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_bkgnorm_zjets.py";
		ofstream in;
		in.open(plotpath_tmp);
		in<<"info = {"<<endl;
		in<<"'bkgrate_zjets_" + year + "_"+ fs_name + "_" + i_s + "_" + tag + "':" + to_string(n_zx[i]) + ","<<endl;
		in<<"}"<<endl;
		in.close();
	}
}
