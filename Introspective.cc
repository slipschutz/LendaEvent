




#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Introspective.hh"
#include "TMath.h"

using namespace std;


void Introspective::AddMapEntry(string s,void* p){
  cout<<"Not written yet"<<endl;


}
Introspective::Introspective(){

  Reset();
}

void Introspective::Reset(){
  mapForCorrectionResults.clear();
  mapForCorrectionResults["null"]=-1;
}

void * Introspective::Get(string s){

  if (theMap.find(s) != theMap.end() ){
    
    return theMap[s];

  } else {
    cout<<"***Bad input "<<s<<" ***"<<endl;
    return NULL;
  }
  
}

void Introspective::DumpMappedVariables(){

  cout<<"Known variables are "<<endl<<endl;
  for (map<string,void*>::iterator ii=theMap.begin();ii!=theMap.end();ii++){
    
    cout<<ii->first<<endl;
  }
}
#include <sstream>
void Introspective::DefineCorrection(string time, string otherVar,vector<Double_t> coefs,Int_t channel){
  
  if (Get(time)!=NULL && Get(otherVar)!=NULL){
    CorrectionInfo i;
    i.time = (Double_t*)Get(time);
    i.correctingVar = (Double_t*)Get(otherVar);
    i.firstName = time;
    i.secondName = otherVar; 
    i.coefs = coefs;
    i.channel =channel;
    stringstream s;
    s<<time<<":"<<otherVar<<"ch_"<<channel;
    correctionsMap[s.str()]=i;
    
    //cout<<"***Waring correction with tags "<<time <<" "<<otherVar <<" already in map***"<<endl;

  } 
}


void Introspective::DumpCorrectionsMap(){

  cout<<"\n****Dumping Dynamically defined corrections****\n"<<endl;
  for (map <string,CorrectionInfo>::iterator ii=correctionsMap.begin();ii!=correctionsMap.end();++ii){
    if (Get(ii->second.firstName) != NULL && Get(ii->second.secondName)!=NULL){
      cout<<"Correction for variables "<<ii->first<<" For Channel "<<ii->second.channel<<endl;
      
      int size = ii->second.coefs.size();
      cout<<"Corrected time = "<<ii->second.firstName<<" - (";
      for (int i=0;i<size;i++){
	cout<<ii->second.coefs[i]<<"*"<<ii->second.secondName<<"^"<<i;
	if (i == size-1) //the last one
	  cout<<")"<<endl;
	else
	  cout<<"+";
	
      }
      
    }
  }
}
void Introspective::PrintCorrection(string in){
  if (correctionsMap.find(in) == correctionsMap.end() ){

    cout<<"Correction "<<in <<" not found in map "<<endl;
    cout<<"Avaiable Corrections are "<<endl;
    DumpCorrectionsMap();
  } else {
    
    CorrectionInfo II = correctionsMap[in];
    
    cout<<"Correction for Variable "<<in <<" for channel "<<II.channel<<endl;
    int size = II.coefs.size();
    cout<<"Corrected time = "<<II.firstName<<" - (";
    for (int i=0;i<size;i++){
      cout<<II.coefs[i]<<"*"<<II.secondName<<"^"<<i;
      if (i == size-1) //the last one
	cout<<")"<<endl;
      else
	cout<<"+";
    }
  }
}
 

void Introspective::ApplyDynamicCorrections(){

  int count=0;//I should put more comments
  for (map <string,CorrectionInfo>::iterator ii=correctionsMap.begin();ii!=correctionsMap.end();++ii){  
    CorrectionInfo theInfo = ii->second;
    
    if (mapForCorrectionResults.find(ii->first) == mapForCorrectionResults.end() ){
      //      cout<<"Apply correction for "<<ii->first<<endl;
      count++;
      mapForCorrectionResults[ii->first]=count;
      //Calculate the correction;

      int degree = theInfo.coefs.size();
      Double_t tempTotal=0;
      for (int i=0;i<degree;i++){
	tempTotal=tempTotal+theInfo.coefs[i]*(TMath::Power(*theInfo.correctingVar,i+1));
      }
      theDynamicCorrectionResults.push_back(*theInfo.time-tempTotal);

    } else {
      cout<<"*** Warning the correction "<<ii->first<<" already done"<<endl;
    }
  }
  Reset();
}
