/*
//
// Class providing the map between variable names and their instances
// in the child classes.
//
// With this map the class  provides the dynamically defined corrections 
//
*/

#ifndef __CORRECTABLE_HH
#define __CORRECTABLE_HH

#include "TObject.h"
#include <map>
#include <string>
#include <utility> 
#include <iostream>
#include "CorrectionInfo.hh"
using namespace std;



class Correctable : public TObject {
public:
  Correctable();
  map <std::string,void*> theVariableMap;
  map <std::string,vector<Double_t>* > theVectorVariableMap;

  vector <CorrectionInfo> corrections;

  void AddMapEntry(std::string,void*,bool isArray=false);

  //Define pure virutal method for defining the table
  virtual void DefineMap()=0;//method defining the variables in the map should be auto-generated


  void * Get(std::string);
  vector<Double_t>* GetVector(string);
  
  void DefineCorrection(string time, string otherVar,vector<Double_t> coefs,Int_t channel);

  Double_t theDynamicCorrectionResults[100];

  map <string,int> mapForCorrectionResults;
  vector <string> correctionKeys;
  Int_t correctionCount;


  void DumpCorrectionsMap();
  void DumpMappedVariables();
  void DumpResultMap();
  void DumpCorrectable();
  void DumpResultVector();


  void Reset();
  void ApplyDynamicCorrections(); // MAgic function

  void ParseVectorInput(string in,string & out,int &index);
private:



public:
  ClassDef(Correctable,1);
};

#endif

