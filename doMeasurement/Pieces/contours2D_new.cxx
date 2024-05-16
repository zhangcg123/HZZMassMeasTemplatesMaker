TH2D* frameTH2D(TH2D *in, double threshold);//so strange, why other functions do not need a declaration?

TGraph* bestFit(TTree *t, TString x, TString y, TCut cut) {
    int nfind = t->Draw(y+":"+x, cut + "deltaNLL == 0");
    if (nfind == 0) {
        TGraph *gr0 = new TGraph(1);
        gr0->SetPoint(0,-999,-999);
        gr0->SetMarkerStyle(34); gr0->SetMarkerSize(2.0);
        return gr0;
    } else {
        TGraph *gr0 = (TGraph*) gROOT->FindObject("Graph")->Clone();
        gr0->SetMarkerStyle(34); gr0->SetMarkerSize(2.0);
        if (gr0->GetN() > 1) gr0->Set(1);
	//gr0->SetPoint(0,125.38,0.007);
        return gr0;
    }
}

TH2 *treeToHist2D(TTree *t, TString x, TString y, TString name, TCut cut, double xmin, double xmax, double ymin, double ymax, int xbins, int ybins) {
    t->Draw(Form("2*deltaNLL:%s:%s>>%s_prof(%d,%10g,%10g,%d,%10g,%10g)", y.Data(), x.Data(), name.Data(), xbins, xmin, xmax, ybins, ymin, ymax), cut + "deltaNLL != 0", "PROF");
    TH2 *prof = (TH2*) gROOT->FindObject(name+"_prof");
    TH2D *h2d = new TH2D(name, name, xbins, xmin, xmax, ybins, ymin, ymax);
    for (int ix = 1; ix <= xbins; ++ix) {
        for (int iy = 1; iy <= ybins; ++iy) {
             double z = prof->GetBinContent(ix,iy);
             if (z != z) z = (name.Contains("bayes") ? 0 : 999); // protect agains NANs
             h2d->SetBinContent(ix, iy, z);
        }
    }
    h2d->GetXaxis()->SetTitle("m_{H} (GeV)");
    h2d->GetYaxis()->SetTitle("#Gamma_{H} (GeV)");
    h2d->GetXaxis()->CenterTitle();
    h2d->GetYaxis()->CenterTitle(); 
    h2d->GetXaxis()->SetTitleSize(0.05);
    h2d->GetYaxis()->SetTitleSize(0.05);
    h2d->SetDirectory(0);
    return h2d;
}

TList* contourFromTH2(TH2 *h2in, double threshold, int minPoints=20) {
    std::cout << "Getting contour at threshold " << threshold << " from " << h2in->GetName() << std::endl;
    //http://root.cern.ch/root/html/tutorials/hist/ContourList.C.html
    Double_t contours[1];
    contours[0] = threshold;
    if (h2in->GetNbinsX() * h2in->GetNbinsY() > 10000) minPoints = 50;
    if (h2in->GetNbinsX() * h2in->GetNbinsY() <= 100) minPoints = 10;

    TH2D *h2 = frameTH2D((TH2D*)h2in,threshold);

    h2->SetContour(1, contours);

    // Draw contours as filled regions, and Save points
    h2->Draw("CONT Z LIST");
    gPad->Update(); // Needed to force the plotting and retrieve the contours in TGraphs


    // Get Contours
    TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
    TList* contLevel = NULL;

    if (conts == NULL || conts->GetSize() == 0){
        printf("*** No Contours Were Extracted!\n");
        return 0;
    }

    TList *ret = new TList();
    for(int i = 0; i < conts->GetSize(); i++){
        contLevel = (TList*)conts->At(i);
        //printf("Contour %d has %d Graphs\n", i, contLevel->GetSize());
        for (int j = 0, n = contLevel->GetSize(); j < n; ++j) {
            TGraph *gr1 = (TGraph*) contLevel->At(j);
            //printf("\t Graph %d has %d points\n", j, gr1->GetN());
            if (gr1->GetN() > minPoints) ret->Add(gr1->Clone());
            //break;
        }
    }
    return ret;
}

TH2D* frameTH2D(TH2D *in, double threshold){
        // NEW LOGIC:
        //   - pretend that the center of the last bin is on the border if the frame
        //   - add one tiny frame with huge values
        double frameValue = 1000;
        if (TString(in->GetName()).Contains("bayes")) frameValue = -1000;

	Double_t xw = in->GetXaxis()->GetBinWidth(1);
	Double_t yw = in->GetYaxis()->GetBinWidth(1);

	Int_t nx = in->GetNbinsX();
	Int_t ny = in->GetNbinsY();

	Double_t x0 = in->GetXaxis()->GetXmin();
	Double_t x1 = in->GetXaxis()->GetXmax();

	Double_t y0 = in->GetYaxis()->GetXmin();
	Double_t y1 = in->GetYaxis()->GetXmax();
        Double_t xbins[999], ybins[999]; 
        double eps = 0.1;

        xbins[0] = x0 - eps*xw - xw; xbins[1] = x0 + eps*xw - xw;
        for (int ix = 2; ix <= nx; ++ix) xbins[ix] = x0 + (ix-1)*xw;
        xbins[nx+1] = x1 - eps*xw + 0.5*xw; xbins[nx+2] = x1 + eps*xw + xw;

        ybins[0] = y0 - eps*yw - yw; ybins[1] = y0 + eps*yw - yw;
        for (int iy = 2; iy <= ny; ++iy) ybins[iy] = y0 + (iy-1)*yw;
        ybins[ny+1] = y1 - eps*yw + yw; ybins[ny+2] = y1 + eps*yw + yw;
        
	TH2D *framed = new TH2D(
			Form("%s framed",in->GetName()),
			Form("%s framed",in->GetTitle()),
			nx + 2, xbins,
			ny + 2, ybins 
			);

	//Copy over the contents
	for(int ix = 1; ix <= nx ; ix++){
		for(int iy = 1; iy <= ny ; iy++){
			framed->SetBinContent(1+ix, 1+iy, in->GetBinContent(ix,iy));
		}
	}
	//Frame with huge values
	nx = framed->GetNbinsX();
	ny = framed->GetNbinsY();
	for(int ix = 1; ix <= nx ; ix++){
		framed->SetBinContent(ix,  1, frameValue);
		framed->SetBinContent(ix, ny, frameValue);
	}
	for(int iy = 2; iy <= ny-1 ; iy++){
		framed->SetBinContent( 1, iy, frameValue);
		framed->SetBinContent(nx, iy, frameValue);
	}

	return framed;
}
void styleMultiGraph(TList *tmg, int lineColor, int lineWidth, int lineStyle) {
    for (int i = 0; i < tmg->GetSize(); ++i) {
        TGraph *g = (TGraph*) tmg->At(i);
        g->SetLineColor(lineColor); g->SetLineWidth(lineWidth); g->SetLineStyle(lineStyle);
    }
}
void styleMultiGraphMarker(TList *tmg, int markerColor, int markerSize, int markerStyle) {
    for (int i = 0; i < tmg->GetSize(); ++i) {
        TGraph *g = (TGraph*) tmg->At(i);
        g->SetMarkerColor(markerColor); g->SetMarkerSize(markerSize); g->SetMarkerStyle(markerStyle);
    }
}


/** Make a 2D contour plot from the output of MultiDimFit
 *
 * Inputs:
 *  - gFile should point to the TFile containing the 'limit' TTree
 *  - xvar should be the variable to use on the X axis, with xbins bins in the [xmin, xmax] range
 *  - yvar should be the variable to use on the Y axis, with ybins bins in the [ymin, ymax] range
 *  - (smx, smy) are the coordinates at which to put a diamond representing the SM expectation
 *  - if fOut is not null, then the output objects will be saved to fOut:
 *     - the 2D histogram will be saved as a TH2 with name name+"_h2d"
 *     - the 68% CL contour will be saved as a TList of TGraphs with name name+"_c68"
 *     - the 95% CL contour will be saved as a TList of TGraphs with name name+"_c95"
 *     - the 99.7% CL contour will be saved as a TList of TGraphs with name name+"_c997"
 *     - the best fit point will be saved as a TGraph with name name+"_best"
 *
 * Notes:
 *     - it's up to you to make sure that the binning you use for this plot matches the one used
 *       when running MultiDimFit (but you can just plot a subset of the points; e.g. if you had
 *       100x100 points in [-1,1]x[-1,1] you can make a 50x50 plot for [0,1]x[0,1])
 *     - the 99.7 contour is not plotted by default
 *     - the SM marker is not saved
*/
void contour2D(TString xvar, int xbins, float xmin, float xmax, TString yvar, int ybins, float ymin, float ymax, float smx=1.0, float smy=1.0, TFile *fOut=0, TString path="./", TString name="contour2D", TString step="1") {
    TCanvas c("c","",1000,1000);
    c.SetTicks(1,1);
    c.SetLeftMargin(0.13);
    c.SetRightMargin(0.13);
    c.SetTopMargin(0.13);
    c.SetBottomMargin(0.13);
    TFile* fin = new TFile(path + name + ".root","READ");
    TTree *tree = (TTree*) fin->Get("limit") ;
    TH2 *hist2d = treeToHist2D(tree, xvar, yvar, "h2d", "", xmin, xmax, ymin, ymax, xbins, ybins);
    // write the hist2d to a file
    TFile *f = new TFile("hist2d.root","RECREATE");
    hist2d->Write();
    f->Close();
    if( step == "2"){
    TFile *newtree = new TFile("/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/datacardsOutputs_KDTrue_newebeforallmasspoints_new/BSREFIT_2D_9Categ_Scans_WidthModel/Scan2D/OBS/run2_bsrefit_mkd_9_notag_widthpdf_all/hist2d_new.root","READ");
    TH2 *hist2d = (TH2*)newtree->Get("h2_new"); 
    //TH2 *hist2d = treeToHist2D(tree, xvar, yvar, "h2d", "", xmin, xmax, ymin, ymax, xbins, ybins);
    //hist2d->Smooth();
    //hist2d->Smooth();
    hist2d->SetTitle("");
    hist2d->LabelsOption("v");
    hist2d->SetContour(200);
    hist2d->GetZaxis()->SetRangeUser(0,21);
    //hist2d->GetXaxis()->SetNdivisions(505);
    hist2d->GetYaxis()->SetNdivisions(505);
    TGraph *fit = bestFit(tree, xvar, yvar, "");
    TList *c681d = contourFromTH2(hist2d, 1.0);
    TList* c951d = contourFromTH2(hist2d, 4.0);
    TList *c68 = contourFromTH2(hist2d, 2.30);
    TList *c95 = contourFromTH2(hist2d, 5.99);
    TList *c997 = contourFromTH2(hist2d, 11.83);
    //cout<<c95->First()<<endl;
    //for(int i=0; i<c95->GetSize(); i++){
	//	    cout<<c95->At(i)<<endl;
    //}
    styleMultiGraph(c681d, 2, 3, 1);
    styleMultiGraph(c951d, 2, 3, 1);
    styleMultiGraph(c68,  /*color=*/1, /*width=*/3, /*style=*/1);
    styleMultiGraph(c95,  /*color=*/1, /*width=*/3, /*style=*/2);
    styleMultiGraph(c997, /*color=*/1, /*width=*/3, /*style=*/2);
    hist2d->Draw("COLZ"); gStyle->SetOptStat(0);
    c68->Draw("L");
    c95->Draw("L SAME");
    //c681d->Draw("L");
    //c951d->Draw("L SAME");
    fit->Draw("P SAME");
    TMarker m;
    m.SetMarkerSize(3.0); m.SetMarkerColor(97); m.SetMarkerStyle(33); 
    m.DrawMarker(smx,smy);
    m.SetMarkerSize(1.8); m.SetMarkerColor(89); m.SetMarkerStyle(33); 
    m.DrawMarker(smx,smy);
    //if (fOut != 0) {
    //    hist2d->SetName(name+"_h2d");  fOut->WriteTObject(hist2d,0);
    //    fit->SetName(name+"_best");    fOut->WriteTObject(fit,0);
    //    c68->SetName(name+"_c68");     fOut->WriteTObject(c68,0,"SingleKey");
    //    c95->SetName(name+"_c95");     fOut->WriteTObject(c95,0,"SingleKey");
    //    c997->SetName(name+"_c997");   fOut->WriteTObject(c997,0,"SingleKey");
    //}
    TLegend* leg = new TLegend(.65,.75,.8,.83);
    leg->SetTextSize(0.04);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    TLine* L1 = new TLine();
    L1->SetLineColor(2);
    L1->SetLineStyle(1);
    L1->SetLineWidth(3);
    //leg->AddEntry(L1,"-2 #Delta NLL=1.0");
    TLine* L2 = new TLine();
    L2->SetLineColor(2);
    L2->SetLineStyle(2);
    L2->SetLineWidth(3);
    //leg->AddEntry(L2,"-2 #Delta NLL=4.0");
    TH1F* L3 = new TH1F();
    L3->SetLineColor(1);
    L3->SetLineStyle(1);
    L3->SetLineWidth(3);
    leg->AddEntry(L3,"68\% CL","l");
    TH1F* L4 = new TH1F();
    L4->SetLineColor(1);
    L4->SetLineStyle(2);
    L4->SetLineWidth(3);
    leg->AddEntry(L4,"95\% CL","l");
    leg->Draw("SAME");
    TLatex *latex=new TLatex();latex->SetNDC();latex->SetTextSize(0.05);latex->SetTextFont(42);
    //TLatex* CMS_status_Snowmass_Phase2 = new TLatex(0.1,0.93, "#scale[0.7]{CMS Phase-2 Projection Preliminary}");
    //TLatex* Lumi_latex_Snowmass = new TLatex(0.7,0.93, "#scale[0.7]{3000 fb^{-1} (14 TeV)}");
    //CMS_status_Snowmass_Phase2->DrawLatex();
    //Lumi_latex_Snowmass->DrawLatex(); 
    latex->DrawLatex(0.129,0.9, "#scale[1.1]{#bf{CMS}} #scale[0.8]{#it{Preliminary}}");
    latex->DrawLatex(0.6,0.9, "#scale[0.9]{138 fb^{-1} (13 TeV)}");
    //TPaveText* decayChannel = new TPaveText(0.20,0.75,0.35,0.825, "NDC");
    //decayChannel->SetTextSize(0.04);
    //decayChannel->SetFillColor(0); //white background
    //decayChannel->AddText("H #rightarrow ZZ^{*} #rightarrow 4I");
    //latex->DrawLatex(0.13,0.84,"H#rightarrowZZ#rightarrow4l");
    //decayChannel->Draw("SAME");
    c.SaveAs(path + name + "_2D_contours.pdf");
    c.SaveAs(path + name + "_2D_contours.png");
    }

}

void contours2D_new(TString path, TString name, TString step) {
	
	contour2D("MH",20,124.71,125.39,"HiggsDecayWidth",20,0,.39,1.0,1.0,0,path,name,step);	
	
}
