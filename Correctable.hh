/*
//
// Class providing the map between variable names and their instances
// in the child classes.
//
// With this map the class provides dynamically defined corrections between variables in the map
// The results of these corrections are put into the array theDynamicCorrectionResults
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
  Correctable(); //Constructor 
  map <std::string,void*> theVariableMap; //map for variables (only for simples types)
  map <std::string,vector<Double_t>* > theVectorVariableMap; //map for vectors all Double_t's

  vector <CorrectionInfo> corrections; //Vector holding the correction info for each correction

  void AddMapEntry(std::string,void*,bool isArray=false);//method to add entry to variableMap

  //Define pure virutal method for defining the Maps
  virtual void DefineMap()=0;//method defining the variables in the map can be auto-generated

  void * Get(std::string); //Get variable from variable Map
  vector<Double_t>* GetVector(string); //get vector from vector map
  
  //Define a correction.  
  void DefineCorrection(string time, string otherVar,vector<Double_t> coefs,Int_t channel);

  Double_t theDynamicCorrectionResults[100];

  map <string,int> mapForCorrectionResults;//map to where each correction is in the Results vector
  vector <string> correctionKeys; //vector holding the correction names in the order they were defined
  Int_t correctionCount; //how many corrections have been defined

  //Methods for printing info containted in this class
  void DumpCorrectable(); //Dumps everything
  void DumpCorrectionsMap();
  void DumpMappedVariables();
  void DumpResultMap();
  void DumpResultVector();


  void Reset();
  void ApplyDynamicCorrections(); // Method to calculate the corrections for each event

private:
  void ParseVectorInput(string in,string & out,int &index);//methods for parsing array[index] input


public:
  ClassDef(Correctable,1);
};

#endif

