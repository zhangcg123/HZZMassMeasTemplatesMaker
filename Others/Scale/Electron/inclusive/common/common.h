#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLatex.h"
#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooFFTConvPdf.h"
#include "RooCBShape.h"
#include "RooNumConvPdf.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooVoigtian.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TSystem.h"
#include "HZZ2L2QRooPdfs.h"
#include "RooChi2Var.h"
#include "TLorentzVector.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLeaf.h"
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
using namespace std;
using namespace RooFit;
void SortByOIFD(vector<string>& varsname_, vector<string>& varstype_);
void splitbyslash(string str, vector<string>& results);
void multirun(int top, int bottom,TH1F* h );
void multibranchgetentry();
void dcb(TH1F* h, string xname, string plotpath, string filename);

int N=0;
int Nbs=0;
int bins=200;
float low=80;
float high=100;
string fitsetting="";
string histosetting="";
string inputname = "";
string outputdir = "";
string filename = "";
string cuts = "";
string vars = "";
vector<string> varsname;
vector<string> varstype;

int NI=0;
int NF=0;
int ND=0;
int NO=0;
void splitbyslash(string str, vector<string>& results){ 
        while( true ){
                int beginIdx = str.rfind('/');
                if(beginIdx != -1){
                        string tmp = str.substr(beginIdx + 1); 
                        results.push_back(tmp);
                        str = str.substr(0, str.size()-tmp.size()-1);
                }else{
                        results.push_back(str);
                        break;
                }   
        }  
}
void SortByOIFD(vector<string>& varsname_, vector<string>& varstype_){
	int Nbs_tmp = Nbs;
	for(int i; i<Nbs_tmp; i++){
                if(varstype_.at(i)=="Double_t"){
                        varsname_.insert(varsname_.begin(),varsname_.at(i));
                        varstype_.insert(varstype_.begin(),varstype_.at(i));
                	Nbs_tmp++;
			i++;
		}   
        }
	
        for(int i=0; i<Nbs_tmp; i++){
                if(varstype_.at(i)=="Float_t"){
                        varsname_.insert(varsname_.begin(),varsname_.at(i));
                        varstype_.insert(varstype_.begin(),varstype_.at(i));
			Nbs_tmp++;
			i++;
                }
        }
        for(int i=0; i<Nbs_tmp; i++){
                if(varstype_.at(i)=="Int_t"){
                        varsname_.insert(varsname_.begin(),varsname_.at(i));
                        varstype_.insert(varstype_.begin(),varstype_.at(i));
			Nbs_tmp++;
			i++;
                }   
        }  
        for(int i=0; i<Nbs_tmp; i++){
                if(varstype_.at(i)=="Bool_t"){
                        varsname_.insert(varsname_.begin(),varsname_.at(i));
                        varstype_.insert(varstype_.begin(),varstype_.at(i));
			Nbs_tmp++;
			i++;
                }
        }
	for(int i=Nbs; i<Nbs_tmp; i++){
		varsname_.pop_back();
		varstype_.pop_back();
	}
}

void dcb(TH1F* h, string xname, string plotpath, string filename){
	RooRealVar x(TString(xname),"",low,high);
	vector<string> pars;pars.clear();
	splitbyslash(fitsetting,pars);
	float m =  stof(pars[5]);
	float s =  stof(pars[4]);
	float a1 = stof(pars[3]);
	float a2 = stof(pars[2]);
	float n1 = stof(pars[1]);
	float n2 = stof(pars[0]);
	RooRealVar rv_m("m","",m,m-abs(0.4*m),m+abs(0.4*m));
	RooRealVar rv_s("s","",s,s-abs(0.4*s),s+abs(0.4*s));
	RooRealVar rv_a1("a1","",a1,0,a1+abs(10*a1));
	RooRealVar rv_a2("a2","",a2,0,a2+abs(10*a2));
	RooRealVar rv_n1("n1","",n1,0,n1+abs(60*n1));
	RooRealVar rv_n2("n2","",n2,0,n2+abs(60*n2));
	
	RooDoubleCB dcb("dcb","",x,rv_m,rv_s,rv_a1,rv_n1,rv_a2,rv_n2);
	RooDataHist d(TString(xname)+"_d","",RooArgSet(x),Import(*h));

	dcb.fitTo(d,SumW2Error(kTRUE),PrintLevel(-1));
	dcb.fitTo(d,SumW2Error(kTRUE),PrintLevel(-1));

	TString m_s = to_string(rv_m.getVal());
	TString s_s = to_string(rv_s.getVal());
	TString a1_s = to_string(rv_a1.getVal());
	TString a2_s = to_string(rv_a2.getVal());
	TString n1_s = to_string(rv_n1.getVal());
	TString n2_s = to_string(rv_n2.getVal());

	RooPlot* frame = x.frame(Bins(30));
	frame->SetTitle("");
	TH1F* tmp = (TH1F*)h->Rebin(bins/(int(high-low)*1000/100));
	RooDataHist dataset_toplot("dataset_toplot","dataset_toplot",RooArgSet(x),Import(*tmp));
	dataset_toplot.plotOn(frame);
	dcb.plotOn(frame);
	TLatex *latex=new TLatex();
	latex->SetNDC();
	latex->SetTextSize(0.05);
	latex->SetTextFont(42);
	latex->SetTextAlign(23);
	TCanvas c("c","",1400,1000);
	c.cd();
	frame->Draw();
	latex->DrawLatex(0.7,0.9,"m="+m_s);
	latex->DrawLatex(0.7,0.8,"s="+s_s);
	latex->DrawLatex(0.7,0.7,"a1="+a1_s);
	latex->DrawLatex(0.7,0.6,"a1="+a2_s);
	latex->DrawLatex(0.7,0.5,"n1="+n1_s);
	latex->DrawLatex(0.7,0.4,"n2="+n2_s);
	c.SaveAs(TString(plotpath)+TString(filename)+"_dcbfit.png");
}
	
void bw(TH1F* h, string xname, string plotpath, string filename){
	RooRealVar x(TString(xname),"",low,high);
	vector<string> pars;pars.clear();
	splitbyslash(fitsetting,pars);
	float m =  stof(pars[1]);
	float s =  stof(pars[0]);
	RooRealVar rv_m("m","",m,m-abs(0.1*m),m+abs(0.1*m));
	RooRealVar rv_s("s","",s,s-abs(0.1*s),s+abs(0.1*s));
	
	RooBreitWigner dcb("bw","",x,rv_m,rv_s);
	RooDataHist d(TString(xname)+"_d","",RooArgSet(x),Import(*h));

	dcb.fitTo(d,SumW2Error(kTRUE),PrintLevel(-1));
	dcb.fitTo(d,SumW2Error(kTRUE),PrintLevel(-1));

	TString m_s = to_string(rv_m.getVal());
	TString s_s = to_string(rv_s.getVal());

	RooPlot* frame = x.frame(Bins(30));
	frame->SetTitle("");
	TH1F* tmp = (TH1F*)h->Rebin(bins/(int(high-low)*1000/100));
	RooDataHist dataset_toplot("dataset_toplot","dataset_toplot",RooArgSet(x),Import(*tmp));
	dataset_toplot.plotOn(frame);
	dcb.plotOn(frame);
	TLatex *latex=new TLatex();
	latex->SetNDC();
	latex->SetTextSize(0.05);
	latex->SetTextFont(42);
	latex->SetTextAlign(23);
	TCanvas c("c","",1400,1000);
	c.cd();
	frame->Draw();
	latex->DrawLatex(0.7,0.9,"m="+m_s);
	latex->DrawLatex(0.7,0.8,"s="+s_s);
	c.SaveAs(TString(plotpath)+TString(filename)+"_bwfit.png");
}


void bwxdcb(TH1F* h, string xname, string plotpath, string filename){
	
	RooRealVar x(TString(xname),"",low,high);
	vector<string> pars;pars.clear();
	splitbyslash(fitsetting,pars);
	float zm = stof(pars[7]);
	float zw = stof(pars[6]);
	float m =  stof(pars[5]);
	float s =  stof(pars[4]);
	float a1 = stof(pars[3]);
	float a2 = stof(pars[2]);
	float n1 = stof(pars[1]);
	float n2 = stof(pars[0]);
	RooRealVar rv_zw("zw","",zw);
	RooRealVar rv_zm("zm","",zm);
	RooRealVar rv_m("m","",m,-5.0,5.0);
	RooRealVar rv_s("s","",s,0.0,10.0);
	RooRealVar rv_a1("a1","",a1,0,10);
	RooRealVar rv_a2("a2","",a2,0,10);
	RooRealVar rv_n1("n1","",n1,0,60);
	RooRealVar rv_n2("n2","",n2,0,60);
	RooBreitWigner bw("bw","",x,rv_zm,rv_zw);
	RooDoubleCB dcb("dcb","",x,rv_m,rv_s,rv_a1,rv_n1,rv_a2,rv_n2);
	RooFFTConvPdf bwxdcb("bwxdcb","",x,bw,dcb);
	bwxdcb.setBufferFraction(0.2);
	RooRealVar rv_tau("tau","",0,-1,1);
	RooRealVar rv_fsig("fsig","",0.99,0.5,1);
	RooExponential bkg("bkg","bkg",x,rv_tau);
	RooAddPdf model("model","model",bwxdcb,bkg,rv_fsig);

	RooDataHist d(TString(xname)+"_d","",RooArgSet(x),Import(*h));

	RooFitResult* r = model.fitTo(d,SumW2Error(kTRUE),PrintLevel(-1),Save(kTRUE));
	TH2D* cor = (TH2D*)r->correlationHist();
	rv_s.setConstant(true);
	rv_a1.setConstant(true);
	rv_a2.setConstant(true);
	rv_n1.setConstant(true);
	rv_n2.setConstant(true);
	rv_tau.setConstant(true);
	rv_fsig.setConstant(true);
	model.fitTo(d,SumW2Error(kTRUE),PrintLevel(-1));	
	cor->SetStats(0);
		
	double left = rv_s.getVal()*(-rv_a1.getVal())+rv_m.getVal()+zm;
	double right = rv_s.getVal()*(rv_a2.getVal())+rv_m.getVal()+zm;
	TLine LL(left,0,left,99999999999);
	TLine RL(right,0,right,99999999999);
	TString left_s = to_string(left);
	TString right_s = to_string(right);

	RooChi2Var chi2_var("chi2","chi2",model,d);
	TString ch2_s = to_string(chi2_var.getVal()/d.numEntries());
	TString peak_s = to_string(rv_zm.getVal()+rv_m.getVal());
	TString width_s = to_string(rv_zw.getVal());
	TString m_s = to_string(rv_m.getVal());
	TString s_s = to_string(rv_s.getVal());
	TString a1_s = to_string(rv_a1.getVal());
	TString a2_s = to_string(rv_a2.getVal());
	TString n1_s = to_string(rv_n1.getVal());
	TString n2_s = to_string(rv_n2.getVal());
	TString entries_s = to_string(int(h->GetEntries()));
	TString merr_s = to_string(rv_m.getError());
	TString serr_s = to_string(rv_s.getError());
	TString a1err_s = to_string(rv_a1.getError());
	TString a2err_s = to_string(rv_a2.getError());
	TString n1err_s = to_string(rv_n1.getError());
	TString n2err_s = to_string(rv_n2.getError());
	TString tau_s = to_string(rv_tau.getVal());
	TString tauerr_s = to_string(rv_tau.getError());
	TString fsig_s = to_string(rv_fsig.getVal());
	TString fsigerr_s = to_string(rv_fsig.getError());

	RooPlot* frame = x.frame(Bins(30));
	frame->SetTitle("");
	TH1F* tmp = (TH1F*)h->Rebin(10);
	RooDataHist dataset_toplot("dataset_toplot","dataset_toplot",RooArgSet(x),Import(*tmp));
	dataset_toplot.plotOn(frame);
	model.plotOn(frame);
	model.plotOn(frame,Components(bkg));
	TLatex *latex=new TLatex();
	latex->SetNDC();
	latex->SetTextSize(0.03);
	latex->SetTextFont(42);
	latex->SetTextAlign(23);
	TCanvas c("c","",1400,1000);
	c.cd();
	frame->Draw();
	latex->DrawLatex(0.7,0.95,"chi2/dof="+ch2_s);
	latex->DrawLatex(0.7,0.9,"peak="+peak_s);
	latex->DrawLatex(0.7,0.85,"width="+width_s);
	latex->DrawLatex(0.7,0.8,"m="+m_s+"+/-"+merr_s);
	latex->DrawLatex(0.7,0.75,"s="+s_s+"+/-"+serr_s);
	latex->DrawLatex(0.7,0.7,"a1="+a1_s+"+/-"+a1err_s);
	latex->DrawLatex(0.7,0.65,"a2="+a2_s+"+/-"+a2err_s);
	latex->DrawLatex(0.7,0.6,"n1="+n1_s+"+/-"+n1err_s);
	latex->DrawLatex(0.7,0.55,"n2="+n2_s+"+/-"+n2err_s);
	latex->DrawLatex(0.7,0.5,"fsig="+fsig_s+"+/-"+fsigerr_s);
	latex->DrawLatex(0.7,0.45,"tau="+tau_s+"+/-"+tauerr_s);
	latex->DrawLatex(0.7,0.4,"entries="+entries_s);
	latex->DrawLatex(0.7,0.35,"right="+right_s);
	latex->DrawLatex(0.7,0.3,"left="+left_s);
	LL.Draw("same");
	RL.Draw("same");
	c.SaveAs(TString(plotpath)+TString(filename)+"_bwxdcbfit.png");
	c.Clear();
	cor->Draw("colztext");
	c.SaveAs(TString(plotpath)+TString(filename)+"_bwxdcbfit_correlation.png");
	ofstream fo;
	fo.open(plotpath + filename + "_bwxdcbfit.py");
	fo<<"info = {"<<endl;
	fo<<"'"+filename+"_peak':"+peak_s<<","<<endl;
	fo<<"'"+filename+"_peakerr':"+merr_s<<","<<endl;
	fo<<"'"+filename+"_sigma':"+s_s<<","<<endl;
	fo<<"'"+filename+"_sigmaerr':"+serr_s<<","<<endl;
	fo<<"}"<<endl;
	fo.close();
}

void bwxscb(TH1F* h, string xname, string plotpath, string filename){
	
	RooRealVar x(TString(xname),"",low,high);
	vector<string> pars;pars.clear();
	splitbyslash(fitsetting,pars);
	float zm = stof(pars[7]);
	float zw = stof(pars[6]);
	float m =  stof(pars[5]);
	float s =  stof(pars[4]);
	float a1 = stof(pars[3]);
	float a2 = stof(pars[2]);
	float n1 = stof(pars[1]);
	float n2 = stof(pars[0]);
	RooRealVar rv_zw("zw","",zw);
	RooRealVar rv_zm("zm","",zm);
	RooRealVar rv_m("m","",m,-5.0,5.0);
	RooRealVar rv_s("s","",s,0.0,10.0);
	RooRealVar rv_a1("a1","",a1,0,10);
	RooRealVar rv_a2("a2","",a2,0,10);
	RooRealVar rv_n1("n1","",n1,0,60);
	RooRealVar rv_n2("n2","",n2,0,60);
	RooBreitWigner bw("bw","",x,rv_zm,rv_zw);
	RooCBShape scb("scb","",x,rv_m,rv_s,rv_a1,rv_n1);
	RooFFTConvPdf bwxscb("bwxdcb","",x,bw,scb);
	bwxscb.setBufferFraction(0.2);
	RooRealVar tau("tau","tau",0,-1,1);
	RooRealVar fsig("fsig","fsig",0.95,0.5,1);
	RooExponential bkg("bkg","bkg",x,tau);
	RooAddPdf model("model","model",bwxscb,bkg,fsig);

	RooDataHist d(TString(xname)+"_d","",RooArgSet(x),Import(*h));

	model.fitTo(d,SumW2Error(kTRUE),PrintLevel(-1));
	rv_s.setConstant(true);
	rv_a1.setConstant(true);
	rv_a2.setConstant(true);
	rv_n1.setConstant(true);
	rv_n2.setConstant(true);
	tau.setConstant(true);
	fsig.setConstant(true);
	RooFitResult* r  = model.fitTo(d,SumW2Error(kTRUE),PrintLevel(-1),Save(kTRUE));	
	TH2D* cor = (TH2D*)r->correlationHist();
	cor->SetStats(0);
		
	double left = rv_s.getVal()*(-rv_a1.getVal())+rv_m.getVal()+zm;
	double right = rv_s.getVal()*(rv_a2.getVal())+rv_m.getVal()+zm;
	TLine LL(left,0,left,99999999999);
	TLine RL(right,0,right,99999999999);
	TString left_s = to_string(left);
	TString right_s = to_string(right);

	RooChi2Var chi2_var("chi2","chi2",model,d);
	TString ch2_s = to_string(chi2_var.getVal()/d.numEntries());
	TString peak_s = to_string(rv_zm.getVal()+rv_m.getVal());
	TString width_s = to_string(rv_zw.getVal());
	TString m_s = to_string(rv_m.getVal());
	TString s_s = to_string(rv_s.getVal());
	TString a1_s = to_string(rv_a1.getVal());
	TString a2_s = to_string(rv_a2.getVal());
	TString n1_s = to_string(rv_n1.getVal());
	TString n2_s = to_string(rv_n2.getVal());
	TString entries_s = to_string(int(h->GetEntries()));
	TString merr_s = to_string(rv_m.getError());
	TString serr_s = to_string(rv_s.getError());
	TString a1err_s = to_string(rv_a1.getError());
	TString a2err_s = to_string(rv_a2.getError());
	TString n1err_s = to_string(rv_n1.getError());
	TString n2err_s = to_string(rv_n2.getError());

	RooPlot* frame = x.frame(Bins(30));
	frame->SetTitle("");
	TH1F* tmp = (TH1F*)h->Rebin(10);
	RooDataHist dataset_toplot("dataset_toplot","dataset_toplot",RooArgSet(x),Import(*tmp));
	dataset_toplot.plotOn(frame);
	model.plotOn(frame);
	model.plotOn(frame,Components(bkg));
	TLatex *latex=new TLatex();
	latex->SetNDC();
	latex->SetTextSize(0.03);
	latex->SetTextFont(42);
	latex->SetTextAlign(23);
	TCanvas c("c","",1400,1000);
	c.cd();
	frame->Draw();
	latex->DrawLatex(0.7,0.9,"peak="+peak_s);
	latex->DrawLatex(0.7,0.85,"width="+width_s);
	latex->DrawLatex(0.7,0.8,"m="+m_s+"+/-"+merr_s);
	latex->DrawLatex(0.7,0.75,"s="+s_s+"+/-"+serr_s);
	latex->DrawLatex(0.7,0.7,"a1="+a1_s+"+/-"+a1err_s);
	latex->DrawLatex(0.7,0.65,"a2="+a2_s+"+/-"+a2err_s);
	latex->DrawLatex(0.7,0.6,"n1="+n1_s+"+/-"+n1err_s);
	latex->DrawLatex(0.7,0.55,"n2="+n2_s+"+/-"+n2err_s);	
	latex->DrawLatex(0.7,0.5,"entries="+entries_s);
	latex->DrawLatex(0.7,0.45,"chi2/dof="+ch2_s);
	latex->DrawLatex(0.7,0.4,"right="+right_s);
	latex->DrawLatex(0.7,0.35,"left="+left_s);
	LL.Draw("same");
	RL.Draw("same");
	c.SaveAs(TString(plotpath)+TString(filename)+"_bwxscbfit.png");
	c.Clear();
	cor->Draw("colztext");
	//c.SaveAs(TString(plotpath)+TString(filename)+"_bwxdcbfit_correlation.png");
	ofstream fo;
	fo.open(plotpath + filename + "_bwxdcbfit.py");
	fo<<"info = {"<<endl;
	fo<<"'"+filename+"_peak':"+peak_s<<","<<endl;
	fo<<"'"+filename+"_peakerr':"+merr_s<<","<<endl;
	fo<<"'"+filename+"_sigma':"+s_s<<","<<endl;
	fo<<"'"+filename+"_sigmaerr':"+serr_s<<","<<endl;
	fo<<"}"<<endl;
	fo.close();
}
