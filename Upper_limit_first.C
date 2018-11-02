//C++ include
#include <iterator>
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <cmath>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include <string>
#include <TRandom3.h>
#include <TRandom1.h>
#include <list>
#include <cctype>

//root include
#include "TH1F.h"
#include "TMath.h"
#include "TStyle.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TLine.h"
#include "TLegend.h"
using namespace std;

void PrintList(std::list<int> List2){
  for (std::list<int>::iterator it = List2.begin(); it != List2.end(); it++)
    std::cout << *it << ' ';
  return;
}

double Value95(std::list<int> List){
  double fraction=0.0;
  double count=0.0;
  double limit_value=0.0;
  double n = List.size();
  for (std::list<int>::iterator it = List.begin(); it != List.end(); it++){
    count++;
    fraction=count/n;
        
    if(fraction>=0.95){
      limit_value = *it;
      break;}     
  }
  return limit_value;
}

double Value5(std::list<int> List){
  double fraction=0.0;
  double count=0.0;
  double limit_value=0.0;
  double n = List.size();
  for (std::list<int>::iterator it = List.begin(); it != List.end(); it++){
    count++;
    fraction=count/n;

    if(fraction>=0.05){
      limit_value = *it;
      break;}
  }
  return limit_value;
}

//Calculate the upper limit
void Upper_limit_first(){
  //Given N number of expected background events in the signal region
  double N = 40.0;
  //number of trial
  const int trial = 1000000;

  TRandom *r0 = new TRandom();
  //Background and Signal systematic uncertainties, to be calculated based on syst study 
  double Bkg_st = 0.15;
  double Sig_st = 0.15;
  
  const int sig_evt_size = 16;//30;
  const int sig_evt_min = 5;
  
  std::list<int> mylist_sig[sig_evt_size]; 

 //We don't know the number of signal events to expect so we will loop over a range of possible number of events m that goes from 5 to 30
  double distribution2[trial][sig_evt_size];
  double distribution2_sys[trial][sig_evt_size];
  double distributionSIG2[trial][sig_evt_size];
  double distributionSIG2_sys[trial][sig_evt_size];
  Float_t CISIG_sys[sig_evt_size];
  Float_t ymax[sig_evt_size];

  TH1D** arrayN = new TH1D*[sig_evt_size]; for (int k=0;k<sig_evt_size;k++) { arrayN[k] = new TH1D(Form("b%d",k),"NullHypot; Number of Events; Entries;",100,0,125); }
  TH1D** array  = new TH1D*[sig_evt_size]; for (int y=0;y<sig_evt_size;y++) { array[y] = new TH1D(Form("h%d",y),"TestHypot; Number of Events; Entries;",100,0,125); }

  for(int m = sig_evt_min; m < (sig_evt_size); m++){
    for (int k=0; k < trial; k++){
      distribution2[k][m] = r0->Poisson(N);
      distribution2_sys[k][m] = gRandom->Gaus(0, Bkg_st*distribution2[k][m]);//first you calculate the poisson then you smear it with a percentage uncertainty on the distribution  
      //Poisson distribution for the signal first
      distributionSIG2[k][m] = r0->Poisson(m);
      //then smear the Poisson distribution for the signal with a gaussian
      distributionSIG2_sys[k][m] = gRandom->Gaus(0, Sig_st*distributionSIG2[k][m]);
      //add signal and bkg
      arrayN[m]->Fill(distribution2_sys[k][m]+distribution2[k][m]);
      array[m]->Fill(distributionSIG2[k][m]+distributionSIG2_sys[k][m]+distribution2[k][m]+distribution2_sys[k][m]);
      mylist_sig[m].push_back(distributionSIG2[k][m]+distributionSIG2_sys[k][m]+distribution2[k][m]+distribution2_sys[k][m]);
    }
    mylist_sig[m].sort();

    arrayN[m]->SetLineColor(kBlue+1);
    arrayN[m]->Draw();

    array[m]->SetLineColor(kGreen+2);
    array[m]->Draw("SAME");
    //    arrayN[m]->SetLineColor(kBlue+1);
    // arrayN[m]->Draw("SAME");
    //    array[m]->SaveAs(Form("sigANDback_distribution_%d.C",m));
    //deleta array[m]
    CISIG_sys[m] = Value5(mylist_sig[m]);
    
    ymax[m] = array[m]->GetMaximum();
    TLine *line = new TLine(CISIG_sys[m],0,CISIG_sys[m],ymax[m]);
    line->SetLineColor(kRed);
    line->SetLineWidth(5);
    line->Draw();

    TLegend *legend = new TLegend(0.58,0.61,0.88,0.89);
    legend->AddEntry(array[m],"Test Hypothesis", "l");
    legend->AddEntry(arrayN[m],"Null Hypothesis", "l");
    legend->AddEntry(line,"Upper Limit value, s_{up}", "l");

    legend->Draw();

    array[m]->SaveAs(Form("sigANDback_distribution_%d.pdf",m)); 
    std::cout<<"back"<<N<<" sig "<<m<<" s up"<<CISIG_sys[m]<<std::endl;
  }
}


