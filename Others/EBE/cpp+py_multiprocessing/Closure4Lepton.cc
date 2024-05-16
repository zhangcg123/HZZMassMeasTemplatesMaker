#include "RooRealVar.h"                                                                         
#include "TStyle.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "RooBreitWigner.h"
#include "HZZ2L2QRooPdfs.h"
#include "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "TFile.h"
#include "TLine.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TLorentzVector.h"
#include "TLatex.h"
#include "TGraphErrors.h"
using namespace RooFit ;
using namespace std;
void SetAddress(TTree* t);
vector<double*> SplitDataSet(TTree* t);
Double_t* DoClosure(TTree* t, Double_t a1, Double_t a2, TString fs, TString year);
void MakePlot(Double_t* x_corr, Double_t* x_uncorr, Double_t* y, Double_t* yErr, TString fs, TString year);

bool passedFullSelection;
Int_t finalState, ecalDriven1, ecalDriven2, ecalDriven3, ecalDriven4, id1, id2, id4, id3;
Float_t mass4l, mass4lErr, m1, m2, m3, m4, eta1, eta2, eta3, eta4, pt1, pt2, pt3, pt4, pterr1, pterr2, pterr3, pterr4, phi1, phi2, phi3, phi4;
TString path;
int Nbins = 5;
int main(int argc, char* argv[]){
	
	TString year = argv[1];
	path = argv[2];	
	
	TString base = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/Slimmer/OutPutTrees/";
	TFile* f = new TFile(base+"GluGluHToZZTo4L_M125_"+year+"_skimmed.root","READ");
	
	TTree* t = (TTree*)f->Get("passedEvents");
	SetAddress(t);
 	
	vector<double*> cut;
	cut.clear();
	/*	
	double _2016pre4mu[Nbins+1] = {0, 0.007, 0.008, 0.01, 0.012, 1};	
	double _2016pre4e[Nbins+1] = {0, 0.013,  0.017, 0.02, 0.025, 1};
	double _2016pre2e2mu[Nbins+1] = {0, 0.01, 0.012, 0.016, 0.02, 1};
	cut.push_back( _2016pre4mu );
	cut.push_back( _2016pre4e );
	cut.push_back( _2016pre2e2mu );
	*/
	cut = SplitDataSet( t );
	
	/*	
	for ( int i=0; i<cut.size(); i++){
		for( int j=0; j<10; j++){
			cout<<cut.at(i)[j]<<endl;
		}
	}
	*/
	TString fs = "";	
	for ( int j=0; j<cut.size(); j++){
		if ( j == 0 ){
			fs = "4mu";
		}
		if ( j == 1 ){
			fs = "4e";
		}
		if ( j == 2 ){
			fs = "2e2mu";
		}
		
		Double_t* x_corr = new Double_t[Nbins];
		Double_t* x_uncorr = new Double_t[Nbins];
		Double_t* y = new Double_t[Nbins];
		Double_t* yErr = new Double_t[Nbins];
		Double_t* tmp = new Double_t[4];
		
		for(Int_t i=0; i<Nbins; i++){
			tmp = DoClosure(t, cut[j][i], cut[j][i+1], fs, year);
			x_corr[i] = tmp[0];
			x_uncorr[i] = tmp[1];
			y[i] = tmp[2];
			yErr[i] = tmp[3];
		}
		
		MakePlot(x_corr, x_uncorr, y, yErr, fs, year);
		
		delete x_corr;
		delete x_uncorr;
		delete y;
		delete yErr;
		delete tmp;	
	}	
	
}

void MakePlot(Double_t* x_corr, Double_t* x_uncorr, Double_t* y, Double_t* yErr, TString fs, TString year){
	gStyle->SetTitleOffset(1.5,"Y");	
	TCanvas* c = new TCanvas("c","c",1000,1000);
	c->cd();
	TGraph* g1 = new TGraphErrors(Nbins,x_corr,y,0,yErr);
	TGraph* g2 = new TGraphErrors(Nbins,x_uncorr,y,0,yErr);
	g1->SetMarkerStyle(20);
	g2->SetMarkerStyle(20);
	g1->SetMarkerColor(2);
	g2->SetMarkerColor(1);
	g1->SetTitle("");
	g1->GetXaxis()->SetTitle("predicted #sigma_{"+fs+"}");
	g1->GetYaxis()->SetTitle("measured #sigma_{"+fs+"}");
	g1->GetXaxis()->SetLimits(0,5);
	g1->GetHistogram()->SetMaximum(5);
	g1->GetHistogram()->SetMinimum(0);
	g1->Draw("AP");
	g2->Draw("p");

	TLine *l1=new TLine(0,0,5,5);
	TLine *l2=new TLine(0,0,5/1.2,5);
	TLine *l3=new TLine(0,0,5,5/1.2);
	
	TLine *l4=new TLine(0,0,5/1.1,5);
	TLine *l5=new TLine(0,0,5,5/1.1);
	l1->SetLineStyle(kDashed);
	l2->SetLineStyle(kDashed);
	l3->SetLineStyle(kDashed);
	l4->SetLineStyle(kDashed);
	l5->SetLineStyle(kDashed);	
	l1->Draw();
	l4->Draw();
	l5->Draw();

	TLegend *legend=new TLegend(0.2,0.75,0.45,0.9);
	legend->AddEntry(g1, "Corr", "P");
	legend->AddEntry(g2, "unCorr", "P");
	legend->SetTextSize(0.03);
	legend->SetLineWidth(2);
	legend->SetFillColor(0);
	legend->SetBorderSize(1);
	//legend->Draw("SAME");
	
	TString plotpath;
	plotpath = path;

	TString filename = fs+"_ClosureTest";
	
	c->SaveAs(path + "/" + filename+".pdf");
	c->SaveAs(path + "/" + filename+".png");
	
}

Double_t* DoClosure(TTree* t, Double_t b1, Double_t b2, TString fs, TString year){
	
	TString mufname = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/EBE/cpp+py_multiprocessing/LUT/"+year+"/LUT_2mu.root";
	//TString mufname = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/EBE/cpp+py_multiprocessing/NewLUTforV7/"+year+"_mc_mu.root";
	TFile* mu_f = new TFile(mufname,"READ");
	TH2F* mu_lut = (TH2F*)mu_f->Get("2mu");
	TAxis* mu_x = mu_lut->GetXaxis();
	double mu_x_max = mu_x->GetXmax();
	double mu_x_min = mu_x->GetXmin();
	TAxis* mu_y = mu_lut->GetYaxis();
	double mu_y_max = mu_y->GetXmax();
	double mu_y_min = mu_y->GetXmin();
	
	//e1fname = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/EBE/cpp+py_multiprocessing/LUT/"+year+"/LUT_2e_1.root";	
	TString e1fname = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/EBE/cpp+py_multiprocessing/AN19248V6/"+year+"_mc_e1.root";
	TFile* e1_f = new TFile(e1fname,"READ");
	TH2F* e1_lut = (TH2F*)e1_f->Get("e1");
	TAxis* e1_x = e1_lut->GetXaxis();
	double e1_x_max = e1_x->GetXmax();
	double e1_x_min = e1_x->GetXmin();
	TAxis* e1_y = e1_lut->GetYaxis();
	double e1_y_max = e1_y->GetXmax();
	double e1_y_min = e1_y->GetXmin();
	
	//e2fname = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/EBE/cpp+py_multiprocessing/LUT/"+year+"/LUT_2e_3.root";	
	TString e2fname = "/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/Others/EBE/cpp+py_multiprocessing/AN19248V6/"+year+"_mc_e3.root";
	TFile* e2_f = new TFile(e2fname,"READ");
	TH2F* e2_lut = (TH2F*)e2_f->Get("e3");
	TAxis* e2_x = e2_lut->GetXaxis();
	double e2_x_max = e2_x->GetXmax();
	double e2_x_min = e2_x->GetXmin();
	TAxis* e2_y = e2_lut->GetYaxis();
	double e2_y_max = e2_y->GetXmax();
	double e2_y_min = e2_y->GetXmin();
		
	Float_t Sum_mass4lErr_corr = 0;
	Float_t Sum_mass4lErr_uncorr = 0;
	Float_t Ave_mass4lErr_corr = 0;
	Float_t Ave_mass4lErr_uncorr = 0;
	Int_t counter = 0;
	int a1, a2;
	if(fs=="4mu"){a1 = 1; a2 = 1;}
	if(fs=="4e"){a1 = 2; a2 = 2;}
	if(fs=="2e2mu"||fs=="2mu2e"){a1 = 3; a2 = 4;}
	
	RooRealVar mass4l_d("mass4l","mass4l",105,140);
	mass4l_d.setBins(2000,"cache");
	RooDataSet dataset("dataset","dataset",RooArgSet(mass4l_d));
	
	TLorentzVector lep1, lep2, lep3, lep4, lep1p, lep2p, lep3p, lep4p;
		
	Int_t sum = t->GetEntries();
	for(Int_t i=0; i<sum; i++){
		t->GetEntry(i);
		vector<int> ids; ids.clear();
		vector<int> ecals; ecals.clear();
		vector<float> pterrs; pterrs.clear();
		vector<float> etas; etas.clear();
		vector<float> pts; pts.clear();
		if(passedFullSelection==1&&mass4l<140&&mass4l>105&&mass4lErr/mass4l<b2&&mass4lErr/mass4l>b1&&(finalState==a1||finalState==a2)){
			
			lep1.SetPtEtaPhiM(pt1, eta1, phi1, m1);	
			lep2.SetPtEtaPhiM(pt2, eta2, phi2, m2);
			lep3.SetPtEtaPhiM(pt3, eta3, phi3, m3);
			lep4.SetPtEtaPhiM(pt4, eta4, phi4, m4);	
			double MASS = ( lep1 + lep2 + lep3 + lep4 ).M();	

			lep1p.SetPtEtaPhiM(pt1+pterr1, eta1, phi1, m1);
			lep2p.SetPtEtaPhiM(pt2+pterr2, eta2, phi2, m2);
			lep3p.SetPtEtaPhiM(pt3+pterr3, eta3, phi3, m3);
			lep4p.SetPtEtaPhiM(pt4+pterr4, eta4, phi4, m4);
			
			double dm1 = ( lep1p + lep2 + lep3 + lep4 ).M() - MASS;
			double dm2 = ( lep1 + lep2p + lep3 + lep4 ).M() - MASS;
			double dm3 = ( lep1 + lep2 + lep3p + lep4 ).M() - MASS;
			double dm4 = ( lep1 + lep2 + lep3 + lep4p ).M() - MASS;
			
			double mass4lErr_uncorr = sqrt( dm1*dm1 + dm2*dm2 + dm3*dm3 + dm4*dm4 );
			Sum_mass4lErr_uncorr = Sum_mass4lErr_uncorr + mass4lErr_uncorr;
			/*	
			pts.push_back(pt1); pts.push_back(pt2); pts.push_back(pt3); pts.push_back(pt4);	
			etas.push_back(eta1); etas.push_back(eta2); etas.push_back(eta3); etas.push_back(eta4);
			pterrs.push_back(pterr1); pterrs.push_back(pterr2); pterrs.push_back(pterr3); pterrs.push_back(pterr4);	
			ids.push_back(id1); ids.push_back(id2); ids.push_back(id3); ids.push_back(id4);
			ecals.push_back(ecalDriven1); ecals.push_back(ecalDriven2); ecals.push_back(ecalDriven3); ecals.push_back(ecalDriven4);
				
			for( int kkk=0; kkk< 4; kkk++){
				if ( (ids.at(kkk) == 13 || ids.at(kkk) == -13) && abs(etas.at(kkk)) > mu_y_min && abs(etas.at(kkk)) < mu_y_max && pts.at(kkk) < mu_x_max && pts.at(kkk) > mu_x_min ){
					int xbin = mu_x->FindBin( pts.at(kkk) );
					int ybin = mu_y->FindBin( abs(etas.at(kkk)) );
					pterrs.at(kkk) = pterrs.at(kkk) * mu_lut->GetBinContent(xbin, ybin);
				}
				if ( (ids.at(kkk) == 11 || ids.at(kkk) == -11) && pterrs.at(kkk)/pts.at(kkk) < e1_y_max && pterrs.at(kkk)/pts.at(kkk) > e1_y_min && abs(etas.at(kkk)) > e1_x_min && abs(etas.at(kkk)) < e1_x_max && ecals.at(kkk) == 1 ){
					int xbin = e1_x->FindBin( abs(etas.at(kkk) ));
					int ybin = e1_y->FindBin( pterrs.at(kkk)/pts.at(kkk) );
					pterrs.at(kkk) = pterrs.at(kkk) * e1_lut->GetBinContent(xbin, ybin);
				}
				if ( (ids.at(kkk) == 11 || ids.at(kkk) == -11) && ecals.at(kkk) == 0 && pts.at(kkk) > e2_x_min && pts.at(kkk) < e2_x_max && abs(etas.at(kkk)) > e2_y_min && abs(etas.at(kkk)) < e2_y_max ){
					int xbin = e2_x->FindBin( pts.at(kkk) );
					int ybin = e2_y->FindBin( abs(etas.at(kkk)) );
					pterrs.at(kkk) = pterrs.at(kkk) * e2_lut->GetBinContent(xbin, ybin);
				}
			}
			
				
			lep1p.SetPtEtaPhiM(pt1+pterrs.at(0), eta1, phi1, m1);
			lep2p.SetPtEtaPhiM(pt2+pterrs.at(1), eta2, phi2, m2);
			lep3p.SetPtEtaPhiM(pt3+pterrs.at(2), eta3, phi3, m3);
			lep4p.SetPtEtaPhiM(pt4+pterrs.at(3), eta4, phi4, m4);
			
			dm1 = ( lep1p + lep2 + lep3 + lep4 ).M() - MASS;
			dm2 = ( lep1 + lep2p + lep3 + lep4 ).M() - MASS;
			dm3 = ( lep1 + lep2 + lep3p + lep4 ).M() - MASS;
			dm4 = ( lep1 + lep2 + lep3 + lep4p ).M() - MASS;
			
			mass4lErr = sqrt(  dm1*dm1 + dm2*dm2 + dm3*dm3 + dm4*dm4 );	
			*/
				
			Sum_mass4lErr_corr = Sum_mass4lErr_corr + mass4lErr;
			counter = counter + 1;	
			mass4l_d.setVal(MASS);
			dataset.add(RooArgSet(mass4l_d));
		}
	}
			
	//	cout<<"___________________________________"<<counter<<endl;	
		RooDataHist dataset_binned("dataset_binned","dataset_binned",RooArgSet(mass4l_d),dataset);
		RooRealVar mass4l("mass4l","mass4l",105,140);
		mass4l.setBins(2000,"cache");	
		RooRealVar mean("mean","mean",125,120,130);
		RooRealVar sigma("sigma","sigma",2,0,10);
		RooRealVar alpha1("alpha1","alpha1",1,0,10);
		RooRealVar n1("n1","n1",1,0,20);
		RooRealVar alpha2("alpha2","alpha2",1,0,10);
		RooRealVar n2("n2","n2",1,0,60);
		RooDoubleCB DCB("DCB","DCB",mass4l,mean,sigma,alpha1,n1,alpha2,n2);
		DCB.fitTo(dataset_binned,SumW2Error(kTRUE),PrintLevel(-1),Timer(kTRUE));
		
		RooRealVar mean_("mean","mean",mean.getVal(),120,130);
		RooRealVar sigma_("sigma","sigma",sigma.getVal(),0,10);
		RooRealVar alpha1_("alpha1","alpha1",alpha1.getVal(),0,10);
		RooRealVar n1_("n1","n1",n1.getVal(),0,20);
		RooRealVar alpha2_("alpha2","alpha2",alpha2.getVal(),0,10);
		RooRealVar n2_("n2","n2",n2.getVal(),0,60);
		RooDoubleCB DCB_("DCB","DCB",mass4l,mean_,sigma_,alpha1_,n1_,alpha2_,n2_);
		DCB_.fitTo(dataset_binned,SumW2Error(kTRUE),PrintLevel(-1),Timer(kTRUE));
		RooPlot* frame = mass4l.frame();
		frame->SetTitle("");
		dataset_binned.plotOn(frame);
		DCB_.plotOn(frame,LineColor(2),LineWidth(1));
		
		Double_t chi2 = frame->chiSquare(6);
		TString mean_s, sigma_s, alpha1_s, n1_s, alpha2_s, n2_s, chi2_s, counter_s, sigmaErr_s;
		mean_s = to_string(mean_.getVal());
		sigma_s = to_string(sigma_.getVal());
		alpha1_s = to_string(alpha1_.getVal());
		n1_s = to_string(n1_.getVal());
		alpha2_s = to_string(alpha2_.getVal());
		n2_s = to_string(n2_.getVal());
		counter_s = to_string(counter);
		chi2_s = to_string(chi2);
		sigmaErr_s = to_string(sigma_.getError());
		
		TCanvas* c = new TCanvas("c","c",1400,1000);
		c->cd();
		frame->Draw();
		
		TLatex *latex=new TLatex();
		latex->SetNDC();
		latex->SetTextSize(0.05);
		latex->SetTextFont(42);
		latex->SetTextAlign(23);
		latex->DrawLatex(0.7,0.9,"#chi2/DOF="+chi2_s);
		latex->DrawLatex(0.7,0.8,"mean="+mean_s);
		latex->DrawLatex(0.7,0.7,"alpha1="+alpha1_s);
		latex->DrawLatex(0.7,0.6,"sigma="+sigma_s+"+/-"+sigmaErr_s);
		latex->DrawLatex(0.7,0.5,"n1="+n1_s);
		latex->DrawLatex(0.7,0.4,"alpha2="+alpha2_s);
		latex->DrawLatex(0.7,0.3,"n2="+n2_s);
		latex->DrawLatex(0.7,0.2,"Entries="+counter_s);
		
		TString p1_s, p2_s;
		p1_s = to_string(b1);
		p2_s = to_string(b2);
		TString filename = fs+"_Relaticve_mass4lErr_"+p1_s+"-"+p2_s+".png";
		c->SaveAs(path + "/" + filename );
		
		Double_t* results = new Double_t[4];
		results[0] = Sum_mass4lErr_corr/counter;
		results[1] = Sum_mass4lErr_uncorr/counter;
		results[2] = sigma_.getVal();
		results[3] = sigma_.getError();	
		cout<<"prediced_corr:"<<results[0]<<"	predicted_corr:"<<results[1]<<"		mearsured:"<<results[2]<<endl;
		return results;	
}

vector<double*> SplitDataSet(TTree* t){
                           
	Int_t sum = t->GetEntries();
	vector<Double_t>mass4eErr_series;
	vector<Double_t>mass4muErr_series;
	vector<Double_t>mass2e2muErr_series;
	mass4eErr_series.clear();
	mass4muErr_series.clear();
	mass2e2muErr_series.clear();
	
	for(Int_t i=0; i<sum; i++){
		t->GetEntry(i);	
		if(passedFullSelection==1&&mass4l<140&&mass4l>105){
			if ( finalState == 1 ){	
				mass4muErr_series.push_back(mass4lErr/mass4l);
			}
			if ( finalState == 2){
				mass4eErr_series.push_back(mass4lErr/mass4l);
			}
			if ( finalState == 3 || finalState == 4 ){
				mass2e2muErr_series.push_back(mass4lErr/mass4l);
			}
		}
	}
	
        vector<vector<double>> ss; ss.clear();
	ss.push_back( mass4muErr_series );
	ss.push_back( mass4eErr_series );
	ss.push_back( mass2e2muErr_series );
	vector<double*> bounds; bounds.clear();
	const int Nedges = Nbins + 1;
	for( auto & mass4lErr_series : ss ){
		int sub_sum = mass4lErr_series.size();
		sort( mass4lErr_series.begin(), mass4lErr_series.end() );
		Int_t* Quantitle = new Int_t[Nedges];
        	for ( int i = 0; i<Nedges-1; i++){
                	Quantitle[i] = floor( sub_sum * i / Nbins );
        	}                  
		Quantitle[Nedges-1] = sub_sum-1;
        	Double_t* cut = new Double_t[Nedges];
        	for ( int i = 0; i<Nedges-1; i++ ){
              	  	cut[i] = mass4lErr_series[Quantitle[i]];
        	}                  
		cut[Nedges-1] = mass4lErr_series[sub_sum-1]; 
		bounds.push_back( cut );
       	} 
	return bounds;        
                           
}                          


void SetAddress(TTree* t){
	t->SetBranchAddress("finalState",&finalState);
	t->SetBranchAddress("passedFullSelection",&passedFullSelection);
	
	t->SetBranchAddress("mass4l",&mass4l);
	t->SetBranchAddress("mass4lErr",&mass4lErr);
		
	t->SetBranchAddress("id1",&id1);
	t->SetBranchAddress("id2",&id2);
	t->SetBranchAddress("id3",&id3);
	t->SetBranchAddress("id4",&id4);
		
	t->SetBranchAddress("eta1",&eta1);
	t->SetBranchAddress("eta2",&eta2);
	t->SetBranchAddress("eta3",&eta3);
	t->SetBranchAddress("eta4",&eta4);
	
	t->SetBranchAddress("pt1",&pt1);
	t->SetBranchAddress("pt2",&pt2);
	t->SetBranchAddress("pt3",&pt3);
	t->SetBranchAddress("pt4",&pt4);
	
	t->SetBranchAddress("pterr1",&pterr1);
	t->SetBranchAddress("pterr2",&pterr2);
	t->SetBranchAddress("pterr3",&pterr3);
	t->SetBranchAddress("pterr4",&pterr4);
	
	t->SetBranchAddress("m1",&m1);
	t->SetBranchAddress("m2",&m2);
	t->SetBranchAddress("m3",&m3);
	t->SetBranchAddress("m4",&m4);
	
	t->SetBranchAddress("phi1",&phi1);
	t->SetBranchAddress("phi2",&phi2);
	t->SetBranchAddress("phi3",&phi3);
	t->SetBranchAddress("phi4",&phi4);
	
	t->SetBranchAddress("ecaldriven1",&ecalDriven1);
	t->SetBranchAddress("ecaldriven2",&ecalDriven2);
	t->SetBranchAddress("ecaldriven3",&ecalDriven3);
	t->SetBranchAddress("ecaldriven4",&ecalDriven4);
}	
