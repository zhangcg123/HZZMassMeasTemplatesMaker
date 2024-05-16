#include "Helper.h"
int main(int argc, char* argv[]){
	
	//arguments
	TString year = argv[1];
	TString plotpath = argv[2];
	string abseta_low = argv[3];
	string abseta_high = argv[4];
	string deltapt_low = argv[5];
	string deltapt_high = argv[6];
	string trackElectron = argv[7];
	string isData = argv[8];
	
	//electron type
	bool isEcal=true;
	if(trackElectron=="trk")isEcal=false;
	//plot name
	TString plotname = plotpath + trackElectron + "_eta_"+abseta_low+"_"+abseta_high+"_deltapt_"+deltapt_low+"_"+deltapt_high;
	
	//open and set tree
	TFile* f = new TFile();
	if(isData == "mc" && year == "2016post")f = new TFile("/eos/user/c/chenguan/www/Samples/DY_2e_skimmed_A_cancella_madgraph_2016postVFP.root","READ");
	if(isData == "mc" && year == "2016pre") f = new TFile("/eos/user/c/chenguan/www/Samples/DY_2e_skimmed_A_cancella_madgraph_2016preVFP.root","READ");
	if(isData == "mc" && year == "2017") f = new TFile("/eos/user/c/chenguan/www/Samples/DY_madgraph_skimmed_2017_2e.root","READ");
	if(isData == "mc" && year == "2018") f = new TFile("/eos/user/c/chenguan/www/Samples/DY_madgraph_skimmed_2018_2e.root","READ");
	if(isData == "data" && year == "2016post") f = new TFile("/eos/user/c/chenguan/www/Samples/Data_SingleElectron_Run2016post.root","READ");
	if(isData == "data" && year == "2016pre") f = new TFile("/eos/user/c/chenguan/www/Samples/Data_SingleElectron_Run2016pre.root","READ");
	if(isData == "data" && year == "2017") f = new TFile("/eos/user/c/chenguan/www/Samples/Data_SingleElectron_2017.root","READ");
	if(isData == "data" && year == "2018") f = new TFile("/eos/user/c/chenguan/www/Samples/Data_EGamma_2018.root","READ");
	TTree* t = (TTree*)f->Get("passedEvents");
	SetTree(t);	  	
	
	//extract lambda
	SetFirst3Lambdas("madgraph",year,isData);
	double lambda = EBECalculator(t, isEcal, plotname, stof(abseta_low), stof(abseta_high), stof(deltapt_low), stof(deltapt_high), 1, 0);
	MakeOutput(plotname, lambda);		
}
double EBECalculator(TTree* t_, bool isEcal, TString plotname, 
	double abseta_low, double abseta_high, double deltapt_low, double deltapt_high, 
	double running_sf, int inter){
	
	RooDataHist* Data = MakeDataSet(t_, isEcal, abseta_low, abseta_high, deltapt_low, deltapt_high, running_sf);
	Data->Print();
	TString entries = to_string(Data->sumEntries());
	//model building and fit to
	//common variables
	RooRealVar massZ("massZ","",80,100);
	RooRealVar massZErr("massZErr","",0,10);
	massZ.setBins(1000,"cache");
	massZErr.setBins(20,"cache");		
	RooRealVar GENmZ("GENmZ","",91.19);
	RooRealVar GENwZ("GENwZ","",2.44);
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
	RooCBShape DCB("DCB","",massZ,mean,sigma,a1,n1);
	//RooDoubleCB DCB("DCB","",massZ,mean,sigma,a1,n1,a2,n2);
	RooFFTConvPdf BW_DCB("BW_DCB","",massZ,GENZ,DCB);
	BW_DCB.setBufferFraction(0.45);
	RooRealVar t("t","",0,-1,1);
	RooRealVar fsig("fsig","",0.9,0.7,1);
	RooExponential bkg("bkg","",massZ,t);
	RooAddPdf Model("Model","",BW_DCB,bkg,fsig);
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
	RooCBShape DCB_2d("DCB_2d","",massZ,mean,sigma_2d,a1,n1);
	//RooDoubleCB DCB_2d("DCB_2d","",massZ,mean,sigma_2d,a1,n1,a2,n2);
	RooFFTConvPdf BW_DCB_2d("BW_DCB_2d","",massZ,GENZ,DCB_2d);
	BW_DCB_2d.setBufferFraction(0.45);
	t.setConstant(kTRUE);
	fsig.setConstant(kTRUE);
	RooAddPdf Model_2d("Model_2d","",BW_DCB_2d,bkg,fsig);	
	//2D fit to
	start = std::chrono::high_resolution_clock::now();
	Model_2d.fitTo(*Data,ConditionalObservables(massZErr),SumW2Error(kTRUE),PrintLevel(-999),Timer(kTRUE));
	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "################First 2D fit elapsed time: " << elapsed.count() << " s\n";
	//Model_2d.fitTo(*Data,ConditionalObservables(massZErr),SumW2Error(kTRUE),PrintLevel(-999));
	TString lambda_s = to_string(SF.getVal());	
	TString mean_s = to_string(mean.getVal());
	TString sigma_s = to_string(sigma.getVal());
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
	latex->DrawLatex(0.7,0.9,"lambda="+lambda_s);
	latex->DrawLatex(0.7,0.8,"mean="+mean_s);
	latex->DrawLatex(0.7,0.7,"sigma="+sigma_s);
	//latex->DrawLatex(0.7,0.8,"entries="+entries);
	c.SaveAs(plotname+"_v"+to_string(inter)+".png");
	double final_sf = running_sf * SF.getVal();
	if(recursion==true && abs(SF.getVal()-1)>0.001 && inter < 6){
		inter = inter + 1;
		final_sf = EBECalculator(t_, isEcal, plotname, abseta_low, abseta_high, deltapt_low, deltapt_high, final_sf, inter);
	}
	return final_sf;
}
