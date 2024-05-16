#include "include/Set4LSlimmedTree.h"

void PatchForWH_(TString plotpath, int fs, TString type, TString tag, int cates, TString dir1, TString dir2, TString year);

int main(int argc, char* argv[]){
	TString year  = argv[1];
        TString type = argv[2];
        TString tag = argv[3];
        int fs = atoi(argv[4]);
        int cates = atoi(argv[5]);
        TString plotpath = argv[6];
        TString stepsevendir1 = argv[7];
 	TString stepsevendir2 = argv[8];

	PatchForWH_(plotpath, fs, type, tag, cates, stepsevendir1, stepsevendir2, year);
}

void PatchForWH_(TString plotpath, int fs, TString type, TString tag, int cates, TString dir1, TString dir2, TString year){
	
	TString fs_name;
	if(fs==1)fs_name="4mu";
	if(fs==2)fs_name="4e";
	if(fs==3)fs_name="2e2mu";
	if(fs==4)fs_name="2mu2e";
	//retrieve WpH and WmH
	TString Cate_s = to_string(cates);
	
	vector<TString> WpH; WpH.clear();
	vector<TString> WmH; WmH.clear();

	for(int i=0; i<cates; i++){
		TString WpHpath = dir1;
		TString WmHpath = dir2;
		TString i_s = to_string(i);
		WmHpath = WmHpath + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_signorm_wmh.py";
		WpHpath = WpHpath + year + "_" + fs_name + "_" + type + "_" + tag + "_" + i_s + "_signorm_wph.py";

		ifstream WpHin, WmHin;
		WpHin.open(WpHpath);
		WmHin.open(WmHpath);
		string WpHstr, WmHstr;
		
		while(!WpHin.eof())
		{
			getline(WpHin,WpHstr);
			if(WpHstr=="info = {")continue;
			if(WpHstr=="}")break;
			std::size_t pos = WpHstr.find(":");
			WpHstr = WpHstr.substr(pos);
			WpHstr = WpHstr.substr(0, WpHstr.length()-2);
			WpHstr = WpHstr.substr(3);
			WpH.push_back(WpHstr);
			cout<<WpH.back()<<endl;
		}
		WpHin.close();
		while(!WmHin.eof())
		{
			getline(WmHin,WmHstr);
			if(WmHstr=="info = {")continue;
			if(WmHstr=="}")break;
			std::size_t pos = WmHstr.find(":");
			WmHstr = WmHstr.substr(pos);
			WmHstr = WmHstr.substr(0,WmHstr.length()-2);
			WmHstr = WmHstr.substr(3);
			WmH.push_back(WmHstr);
			cout<<WmH.back()<<endl;
		}
		WmHin.close();
	}
	
	TString plotpath_tmp;
	for(int i=0; i<cates; i++){
		TString i_s = to_string(i);
		plotpath_tmp = plotpath + year+ "_"+ fs_name + "_" + type + "_" + tag + "_" + i_s + "_signorm_wh.py";
		ofstream in;
		in.open(plotpath_tmp);
		in<<"info = {"<<endl;
		in<<"'sigrate_wh_" + year + "_" + fs_name + "_" + i_s + "_" + tag + "' : '" + WpH.at(i) + "+" + WmH.at(i) + "',"<<endl;
		in<<"}"<<endl;
		in.close();
	}
}
