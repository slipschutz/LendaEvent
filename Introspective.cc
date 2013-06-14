




#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "Introspective.hh"
#include "TMath.h"

using namespace std;


Introspective::Introspective(){
  correctionCount=0;
  //  Reset();
}


void Introspective::AddMapEntry(string s,void* p){
  if ( theMap.find(s) == theMap.end() ){ //not in there already
    theMap[s] = p;
  } else {
    cout<<"***Warning entry "<<s<<" already in theMap***"<<endl;
  }
}


void * Introspective::Get(string s){

  if (theMap.find(s) != theMap.end() ){
    
    return theMap[s];

  } else {
    cout<<"***Bad input "<<s<<" ***"<<endl;
    return NULL;
  }
  
}




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
    s<<time<<"_"<<otherVar<<"ch_"<<channel;

    corrections.push_back(i);
    correctionKeys.push_back(s.str());
    mapForCorrectionResults[s.str()]=correctionCount;
    correctionCount++;
    //    theDynamicCorrectionResults.resize(correctionCount,-1);
    AddMapEntry(s.str(),&theDynamicCorrectionResults[correctionCount-1]);
    //cout<<"***Waring correction with tags "<<time <<" "<<otherVar <<" already in map***"<<endl;

  } 
}


void Introspective::Reset(){
  mapForCorrectionResults.clear();
  //  mapForCorrectionResults["null"]=-1;
}


void Introspective::DumpMappedVariables(){

  cout<<"****Dump Mapped Variables****"<<endl<<endl;
  for (map<string,void*>::iterator ii=theMap.begin();ii!=theMap.end();ii++){
    
    cout<<ii->first<<endl;
  }
}
void Introspective::DumpResultVector(){
  cout<<"****Dump Result Vector****"<<endl;
  for (int i=0;i<correctionCount;i++){
    cout<<"Corrected value at "<<i<<" is "<<theDynamicCorrectionResults[i]<<endl;

  }

}



void Introspective::DumpCorrectionsMap(){
  
  cout<<"\n****Dumping Dynamically defined corrections****\n"<<endl;
  for (int i=0;i<corrections.size();i++){
    if (Get(correctionKeys[i]) != NULL && Get(correctionKeys[i])!=NULL){
      cout<<"Correction for variables "<<correctionKeys[i]<<" For Channel "<<corrections[i].channel<<endl;
      
      int size = corrections[i].coefs.size();
      cout<<"Corrected time = "<<corrections[i].firstName<<" - (";
      for (int j=0;j<size;j++){
	cout<<corrections[i].coefs[j]<<"*"<<corrections[i].secondName<<"^"<<j+1;
	if (i == size-1) //the last one
	  cout<<")"<<endl;
	else
	  cout<<"+";
	
      }
      
    }
  }
}

void Introspective::DumpResultMap(){
  cout<<"****Dump Results Map****"<<endl;
  for (map <string,int>::iterator ii = mapForCorrectionResults.begin();
       ii!=mapForCorrectionResults.end();ii++){
    
    cout<<"The correction "<<ii->first<<" is at spot "<<ii->second<<" in result vector"<<endl;

  }
}
void Introspective::DumpIntrospective(){
  cout<<"__________Dump_all_______________"<<endl;
  DumpCorrectionsMap();
  DumpMappedVariables();
  DumpResultMap();
  DumpResultVector();
}



void Introspective::ApplyDynamicCorrections(){
  int spot=-1;
  //  for (map <string,CorrectionInfo>::iterator ii=correctionsMap.begin();ii!=correctionsMap.end();++ii){  
  for (int ii=0;ii<corrections.size();ii++){ 
    CorrectionInfo theInfo = corrections[ii];
    string theName = correctionKeys[ii];
    if (mapForCorrectionResults.find(theName) != mapForCorrectionResults.end() ){
      //      cout<<"Apply correction for "<<ii->first<<endl;

      spot=mapForCorrectionResults[theName];//get the spot for this correction
      //Calculate the correction;
      
      int degree = theInfo.coefs.size();
      Double_t tempTotal=0;
      for (int i=0;i<degree;i++){
	tempTotal=tempTotal+theInfo.coefs[i]*(TMath::Power(*theInfo.correctingVar,i+1));
      }
      theDynamicCorrectionResults[spot]=(*theInfo.time-tempTotal);
    } else {
      cout<<"*** Warning the correction "<<theName<<" not found"<<endl;
    }
  }

}

















/*
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
*/
