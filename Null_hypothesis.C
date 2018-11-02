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

using namespace std;

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
void Upper_limit_first_null(){
  //Given N number of expected background events in the signal region                                                      
  double N = 40.0;

  //number of trial                                                                                                        
  const int trial = 100000;
  std::list<int> mylist;
  std::list<int> mylist_bkg;
  std::list<int> mylist_sys;

  TRandom *r0 = new TRandom();
  double distribution[trial];
  double distribution_sys[trial];

  double Bkg_st = 0.15;//to be calculated based on syst study                                                              

  TH1D * Null     = new TH1D("Null",     "Null Hypothesis;Events;Entries;", 1000, 0.0, 100);
  TH1D * Null_sys = new TH1D("Null_sys", "Null Hypothesis;Events;Entries;", 1000, 0.0, 100);

  TCanvas * c1= new TCanvas("c1", "random",5,5,800,600);

  for (int i=0; i < trial; i++){
    distribution[i] = r0->Poisson(N);
    mylist.push_back(distribution[i]);
    distribution_sys[i] = gRandom->Gaus(0, Bkg_st*distribution[i]);
    //first you calculate the poisson then you smear it with a percentage uncertainty on the distribution                                                                             
    Null->Fill(distribution[i]);
    Null_sys->Fill(distribution[i]+distribution_sys[i]);
    mylist_bkg.push_back(distribution[i]+distribution_sys[i]);
  }
  mylist_bkg.sort();

//Expected limits in null hyp. (estimate sensitivity to new physics)                                                     

  //  Float_t CI_sys = Value95(mylist_bkg);                                                                                
  Float_t CI_sys = Value5(mylist_bkg);
  std::cout<<"Expected limits at 95% = "<<CI_sys<<std::endl;
  std::cout<<"Null entries = "<<Null->GetEntries()<<"; Null syst entries = "<<Null_sys->GetEntries()<<" max="<<Null->GetMa\
ximum()<<std::endl;
  Null_sys->SetLineColor(kBlue+1);
  Null_sys->SetFillColor(kBlue+1);
  Null_sys->SetFillStyle(3244);
  gPad->SetLogy();
  Null->Draw();
  Null_sys->Draw("SAME");
  
  Float_t ymax = Null->GetMaximum();
  TLine *line = new TLine(CI_sys,0,CI_sys,ymax);
  line->SetLineColor(kRed);
  line->SetLineWidth(5);
  //  line->Draw();                                                                                                        
  Null->SaveAs("NullHypothesis.pdf");
}

