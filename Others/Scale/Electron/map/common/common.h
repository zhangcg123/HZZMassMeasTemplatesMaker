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
void multirun(int top, int bottom,TH2F* h );
void multibranchgetentry();

int N=0;
int Nbs=0;
int xbins=200;
float xlow=80;
float xhigh=100;
int ybins=200;
float ylow=80;
float yhigh=100;
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
