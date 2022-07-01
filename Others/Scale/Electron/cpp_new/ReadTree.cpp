#include "common.h"

void multirun(int top, int bottom,TH1F* h ){			//this function should be self-contained
	TFile* f = new TFile(TString(inputname),"READ");
	TTree* t = (TTree*)f->Get("passedEvents");
	
	vector<TBranch*> branches;branches.clear();
	for(int i=0; i<Nbs; i++){
		TBranch* b = t->GetBranch(TString(varsname.at(i)));
		branches.push_back(b);
	}

	bool* vars_o = new bool[NO];
	int* vars_i = new int[NI];	
	float* vars_f = new float[NF];
	double* vars_d = new double[ND];
	for(int i=0; i<NO; i++){
		branches[i]->SetAddress(&vars_o[i]);
	}
	for(int i=0; i<NI; i++){
		branches[i+NO]->SetAddress(&vars_i[i]);
	}
	for(int i=0; i<NF; i++){
		branches[i+NO+NI]->SetAddress(&vars_f[i]);
	}
	for(int i=0; i<ND; i++){
		branches[i+NO+NI+NF]->SetAddress(&vars_d[i]);
	}
	for(int i=top; i<bottom; i++){
		for(int j=0; j<Nbs; j++){branches[j]->GetEntry(i);}	
		if(vars_i[0]==0 && vars_i[1]==0 && vars_i[2]==1 && vars_i[3]==1 && vars_i[4]==1 && vars_i[5]==1 && vars_f[0]<0.35 && vars_f[1]<0.35 && vars_f[2]<4 && vars_f[3]<4 && 70<vars_f[4] && vars_f[4]<110 && vars_f[5]<5000 && vars_f[6]<5000 && vars_f[7]<0.96 && vars_f[8]<0.96 && vars_f[9]>32 && vars_f[10]>20 && -2.5<vars_f[11] && vars_f[11]<2.5 && -2.5<vars_f[12] && vars_f[12]<2.5){
			h->Fill(vars_f[4]);
		}
	}
}
void multibranchgetentry(){
	int ncores = std::thread::hardware_concurrency()-3;	// hardware concurrency in cpp means that cup parallelism
	vector<thread> threads; threads.clear();
	vector<TH1F*> histos; histos.clear();
	for(int i=0; i<ncores; i++){
		TString i_s = to_string(i);
		TH1F* h = new TH1F("h_"+i_s,"",bins,low,high);
		histos.push_back(h);
		threads.push_back(thread(multirun, i*floor(N/ncores), (i+1)*floor(N/ncores), h));
	}
	for(thread & th : threads){
		th.join();
	}
	for(int i=1; i<histos.size(); i++){
		histos.at(0)->Add(histos.at(i));
	}
	
	if(fitsetting!="")bwxscb(histos.at(0),varsname.at(0),outputdir,filename);
	
	TFile* fout = new TFile(TString(outputdir) + TString(filename) + ".root","RECREATE");
	fout->cd();
	histos.at(0)->SetName(TString(filename));
	histos.at(0)->Write();
	fout->Close();
}

int main(int argc, char* argv[]){
	
	inputname = argv[1];			//full path if the input file is not here.
	outputdir = argv[2];			//full path for output file
	filename = argv[3];			//outputname
	vars = argv[4];				//string connected by slash, the code will find the variable type automatically 

	histosetting = argv[5];
	fitsetting = argv[6];
			
	vector<string> aa; aa.clear();			//aa is a tmp vector
	splitbyslash(histosetting,aa);		
	bins = stoi(aa[2]);
	low = stof(aa[1]);
	high = stof(aa[0]);
		
	varsname.clear();
	splitbyslash(vars,varsname);
	Nbs = varsname.size();
	
	//warm the cache, in addition to all common settings should be here in order that multiran only need to read tree
	TFile* f = new TFile(TString(inputname),"READ");
	TTree* t = (TTree*)f->Get("passedEvents");
	if(!t) t = (TTree*)f->Get("Ana/passedEvents");
	N = t->GetEntries();
	varstype.clear();
	for(int i=0; i<Nbs; i++){
		TBranch* b = t->GetBranch(TString(varsname.at(i)));
		string type = b->FindLeaf(b->GetName())->GetTypeName();
		varstype.push_back(type);
		if(type=="Double_t")ND++;
		if(type=="Float_t")NF++;
		if(type=="Int_t")NI++;
		if(type=="Bool_t")NO++;
		//delete b;
	}
	delete t;
	f->Close();
	
	SortByOIFD(varsname,varstype);
	for(int i=0; i<varsname.size(); i++){
		cout<<varsname[i]<<"			"<<varstype[i]<<endl;
	}

	TStopwatch s;
	s.Start();
	multibranchgetentry();
	s.Stop();
	cout<<s.RealTime()<<endl;
}
