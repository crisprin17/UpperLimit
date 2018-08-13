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

//Use it if you need to double check the values in your list
/*void PrintList(std::list<int> List2){
  for (std::list<int>::iterator it = List2.begin(); it != List2.end(); it++)
    std::cout << *it << ' ';
  return;
}*/

//Integrate the distribution until the 95% of its total
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

//Calculate the upper limit                                                                                                                                           
void Null_hypothesis(){
  //Given N number of predicted events                                                                                                                                
  double N = 40.0;
  
  //number of trial                                                                                                                                                   
  const int trial = 100000;
  
  std::list<int> mylist;
  std::list<int> mylist_bkg;
  
  //Random generator from ROOT
  TRandom *r0 = new TRandom();
  
  double distribution[trial];
  double distribution_sys[trial];
  //Systematic uncertainties on the background
  double Bkg_st = 0.15;//to be calculated based on syst study 
  
  //create an histogram ROOT [OPTIONAL]
  TCanvas * c1    = new TCanvas("c1", "NullHyp",5,5,800,600);
  TH1D * Null     = new TH1D("Null",     "Events;Entries;", 1000, 0.0, 100);
  TH1D * Null_sys = new TH1D("Null_sys", "Events;Entries;", 1000, 0.0, 100);

  for (int i=0; i < trial; i++){
    distribution[i] = r0->Poisson(N);
    //fill the histogram                                                                                                                                              
    Null->Fill(distribution[i]);
    //add systematic to the backgound distribution
    distribution_sys[i] = gRandom->Gaus(0, Bkg_st*distribution[i]);
    Null_sys->Fill(distribution[i]+distribution_sys[i]);
    //fill a list
    mylist_bkg.push_back(distribution[i]+distribution_sys[i]);
  }
//Sort the values in the list
  mylist_bkg.sort();
  //Expected limits in null hypothesis (select a 95% value from the distribution), estimate sensitivity to new physics
  //Can be used to quantify the likelihood of an excess
  Float_t CI_sys = Value95(mylist_bkg);
  
  // plot an histogram ROOT [OPTIONAL]
  Null_sys->SetLineColor(kBlue+1);
  Null_sys->SetFillColor(kBlue+1);
  Null_sys->SetFillStyle(3244);
  gPad->SetLogy();
  Null->Draw();
  Null_sys->Draw("SAME");

  //Draw a line at 95% value
  Float_t ymax = Null->GetMaximum();
  TLine *line = new TLine(CI_sys,0,CI_sys,ymax);
  line->SetLineColor(kRed);
  line->SetLineWidth(5);
  line->Draw();

  Null->SaveAs("NullHypothesis.pdf");
 }
