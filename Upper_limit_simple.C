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

//Use it to double check the value in your list
void PrintList(std::list<int> List2){
  for (std::list<int>::iterator it = List2.begin(); it != List2.end(); it++)
    std::cout << *it << ' ';
  return;
}

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
void Upper_limit_first(){
  //Given N number of predicted events                                                                                                                                
  double N = 40.0;
  //Poisson distributed uncertainty sqrt(N)                                                                                                                           
  double Nerr = sqrt(N);
  //number of trial                                                                                                                                                   
  int trial = 1000;
  
  std::list<int> mylist;
  std::list<int> mylist_bkg;
  std::list<int> mylist_sys;
  
  //Random generator from ROOT
  TRandom *r0 = new TRandom();
  gRandom = new TRandom3();
  
  double distribution[trial];
  //create an histogram ROOT [OPTIONAL]
  TH1D * hist = new TH1D("hist", ";x;Entries", 1000, 0.0, 100);

  for (int i=0; i < trial; i++){
    distribution[i] = r0->Poisson(N);
    //fill a list                                                                                                                                                     
    mylist.push_back(distribution[i]);
    //fill the histogram                                                                                                                                              
    hist->Fill(distribution[i]);
  }
//Sort the values in the list
  mylist.sort();
  
  // plot an histogram ROOT [OPTIONAL]
  TCanvas * c1= new TCanvas("c1", "random",5,5,800,600);
  hist->Draw();
  hist->SaveAs("random.pdf");

  //Select a 95% value from the distribution:                                                                                                                         
  double CI = Value95(mylist);

  //For a given N and CI the signal will be defined as S = CI - N                                                                                                     
  //This is the "lower limit" for the signal. If the signal was any bigger we would have it seen 95% of the time                                                        
  double S = CI - N;
 }
