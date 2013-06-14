/*
//
// Class providing the map between variable names and their instances
// in the child classes
//
*/

#ifndef __INTROSPECTIVE_HH
#define __INTROSPECTIVE_HH

#include "TObject.h"
#include <map>
#include <string>
#include <utility> 
#include "CorrectionInfo.hh"
using namespace std;



class Introspective : public TObject {

public:
  Introspective();
  map <std::string,void*> theMap;
  vector <CorrectionInfo> corrections;

  void AddMapEntry(std::string,void*);

  //Define pure virutal method for defining the table
  virtual void DefineMap()=0;//method defining the variables in the map should be auto-generated

  void * Get(std::string);
  void DefineCorrection(string time, string otherVar,vector<Double_t> coefs,Int_t channel);

  Double_t theDynamicCorrectionResults[100];

  map <string,int> mapForCorrectionResults;
  vector <string> correctionKeys;
  Int_t correctionCount;


  void DumpCorrectionsMap();
  void DumpMappedVariables();
  void DumpResultMap();
  void DumpIntrospective();
  void DumpResultVector();


  void Reset();
  void ApplyDynamicCorrections(); // MAgic function



public:
  ClassDef(Introspective,1);
};

#endif

