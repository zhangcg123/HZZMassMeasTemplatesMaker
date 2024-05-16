#include "Helper.h"
int main(int argc, char* argv[]){
	
	//arguments
	TString year = argv[1];
	TString plotpath = argv[2];
	string abseta_low = argv[3];
	string abseta_high = argv[4];
	string relpterr_low = argv[5];
	string relpterr_high = argv[6];
	string isData = argv[7];
	
	//plot name
	TString plotname = plotpath + "/eta_"+abseta_low+"_"+abseta_high+"_pterr_"+relpterr_low+"_"+relpterr_high;
	
	//open and set tree
        TString fname = "";         
	/*
         if(isData=="mc"&&year=="2016pre")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_DYJetsToLL_M50_2016APV_2mu_slimmed.root"; 
         if(isData=="mc"&&year=="2016post")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_DYJetsToLL_M50_2016_2mu_slimmed.root";
         if(isData=="mc"&&year=="2017")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_DYJetsToLL_M50_2017_new_2mu_slimmed.root";
         if(isData=="mc"&&year=="2018")fname = "/eos/user/c/chenguan/Tuples/LeptonScale_20SummerUltraLegacy/newbranch/crab_DYJetsToLL_M50_2018_2mu_slimmed.root";
        */
	    
         if(isData=="mc"&&year=="2016pre")fname = "/eos/user/f/ferrico/www/Chenguang/DY_muons_UL_20_s5m0/DY_amcatnlo_2016preVFP.root"; 
         if(isData=="mc"&&year=="2016post")fname = "/eos/user/f/ferrico/www/Chenguang/DY_muons_UL_20_s5m0/DY_amcatnlo_2016postVFP.root";
         if(isData=="mc"&&year=="2017")fname = "/eos/user/f/ferrico/www/Chenguang/DY_muons_UL_20_s5m0/DY_amcatnlo_2017_skimmed.root";
         if(isData=="mc"&&year=="2018")fname = "/eos/user/f/ferrico/www/Chenguang/DY_muons_UL_20_s5m0/DY_amcatnlo_2018_skimmed.root";

         if(isData=="data"&&year=="2016pre")fname = "/eos/user/f/ferrico/www/FullRun2/20160/Data_SingleMuon_Run20160.root"; 
         if(isData=="data"&&year=="2016post")fname = "/eos/user/f/ferrico/www/FullRun2/20165/Data_SingleMuon_Run20165.root";
         if(isData=="data"&&year=="2017")fname = "/eos/user/f/ferrico/www/Chenguang/DATA/Data_SingleMuon_Run2017.root";
         if(isData=="data"&&year=="2018")fname = "/eos/user/f/ferrico/www/Chenguang/DATA/Data_SingleMuon_Run2018.root";
		
	TFile* f = new TFile(fname,"READ");
	TTree* t = (TTree*)f->Get("passedEvents");
	SetTreeForMuons(t);	  	
	
	//extract lambda
	double lambda = EBECalculator(t, plotname, stof(abseta_low), stof(abseta_high), stof(relpterr_low), stof(relpterr_high) );
	MakeOutput(plotname, lambda);		

}

double EBECalculator(TTree* t_, TString plotname, 
	double abseta_low, double abseta_high, double relpterr_low, double relpterr_high){
	RooDataHist* Data = MakeDataSet(t_, abseta_low, abseta_high, relpterr_low, relpterr_high);
	Data->Print();
	TString entries = to_string(Data->sumEntries());
	//model building and fit to
	//common variables
	RooRealVar massZ("massZ","",80,100);
	RooRealVar massZErr("massZErr","",0,5);
	massZ.setBins(2000,"cache");
	massZErr.setBins(20,"cache");		
	RooRealVar GENmZ("GENmZ","",91.19);
	RooRealVar GENwZ("GENwZ","",2.49);
	GENmZ.setConstant(kTRUE);
	GENwZ.setConstant(kTRUE);
	RooBreitWigner GENZ("GENZ","",massZ,GENmZ,GENwZ);
	//1D model
	RooRealVar mean("mean","",0,-5,5);
	RooRealVar sigma("sigma","",1,0,10);
	RooRealVar a1("a1","",1,0,10);
	RooRealVar n1("n1","",5,0,60);
	RooRealVar a2("a2","",1,0,10);
	RooRealVar n2("n2","",5,0,60);
	//RooCBShape DCB("DCB","",massZ,mean,sigma,a1,n1);
	RooDoubleCB DCB("DCB","",massZ,mean,sigma,a1,n1,a2,n2);
	RooFFTConvPdf Model("BW_DCB","",massZ,GENZ,DCB);
	Model.setBufferFraction(0.3);
	RooRealVar t("t","",0,-1,1);
	RooRealVar fsig("fsig","",0.9,0.7,1);
	RooExponential bkg("bkg","",massZ,t);
	//RooAddPdf Model("Model","",BW_DCB,bkg,fsig);
	//1D fit to
	auto start = std::chrono::high_resolution_clock::now();	
	Model.fitTo(*Data,SumW2Error(kTRUE),PrintLevel(-9999));
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "################First 1D fit elapsed time: " << elapsed.count() << " s\n";
	start = std::chrono::high_resolution_clock::now();
	Model.fitTo(*Data,SumW2Error(kTRUE),PrintLevel(-9999));
	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "################Second 1D fit elapsed time: " << elapsed.count() << " s\n";
	//2D model
	mean.setConstant(kTRUE);
	RooRealVar SF("SF","",1,0,10);
	RooFormulaVar sigma_2d("sigma_2d","@0*@1",RooArgList(SF,massZErr));
	a1.setConstant(kTRUE);
	n1.setConstant(kTRUE);
	a2.setConstant(kTRUE);
	n2.setConstant(kTRUE);
	TString mean_s = to_string(mean.getVal());
	TString sigma_s = to_string(sigma.getVal());
	TString a1_s = to_string(a1.getVal());
	TString a2_s = to_string(a2.getVal());
	TString n1_s = to_string(n1.getVal());
	TString n2_s = to_string(n2.getVal());
	TString fsig_s = to_string(fsig.getVal());
	TString t_s = to_string(t.getVal()); 
	RooCBShape DCB_2d("DCB_2d","",massZ,mean,sigma_2d,a1,n1);
	//RooDoubleCB DCB_2d("DCB_2d","",massZ,mean,sigma_2d,a1,n1,a2,n2);
	RooFFTConvPdf Model_2d("BW_DCB_2d","",massZ,GENZ,DCB_2d);
	Model_2d.setBufferFraction(0.3);
	t.setConstant(kTRUE);
	fsig.setConstant(kTRUE);
	//RooAddPdf Model_2d("Model_2d","",BW_DCB_2d,bkg,fsig);	
	//2D fit to
	start = std::chrono::high_resolution_clock::now();
	Model_2d.fitTo(*Data,ConditionalObservables(massZErr),SumW2Error(kTRUE),PrintLevel(-999),Timer(kTRUE));
	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "################First 2D fit elapsed time: " << elapsed.count() << " s\n";
	//Model_2d.fitTo(*Data,ConditionalObservables(massZErr),SumW2Error(kTRUE),PrintLevel(-999));
	TString lambda_s = to_string(SF.getVal());	
	std::cout << "################Lambda: "<<lambda_s<<endl;
	//make plot
	RooChi2Var chi2("chi2","",Model_2d,*Data,RooFit::DataError(RooAbsData::Expected));
	TString chi2_s = to_string(chi2.getVal()/Data->numEntries());
	RooPlot *frame = massZ.frame();
	frame->SetTitle("");
	Data->plotOn(frame);
	Model_2d.plotOn(frame,ProjWData(massZErr,*Data),LineColor(4),LineWidth(4));
	TLatex* latex = new TLatex();
	CustomLatex(latex);
	TCanvas c("c","",1400,1000);
	frame->Draw();
	//latex->DrawLatex(0.7,0.99,"#chi^{2}/DOF="+chi2_s);
	latex->DrawLatex(0.7,0.7,"lambda="+lambda_s);
	//latex->DrawLatex(0.7,0.8,"entries="+entries);
	latex->DrawLatex(0.7,0.6,"mean="+mean_s);
	latex->DrawLatex(0.7,0.5,"sigma="+sigma_s);
	/*latex->DrawLatex(0.7,0.5,"a1="+a1_s);
	latex->DrawLatex(0.7,0.4,"a2="+a2_s);
	latex->DrawLatex(0.7,0.3,"n1="+n1_s);
	latex->DrawLatex(0.7,0.2,"n2="+n2_s);
	latex->DrawLatex(0.7,0.1,"fsig="+fsig_s);
	latex->DrawLatex(0.3,0.1,"t="+t_s);
	*/
	c.SaveAs(plotname+".png");
	double running_sf=1.0;
	double final_sf = running_sf * SF.getVal();
	return final_sf;
}
