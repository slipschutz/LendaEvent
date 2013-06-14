




#ifndef __CorrectionInfo_HH
#define __CorrectionInfo_HH

#include "TObject.h"

#include <vector>
#include <string>
using namespace std;

class CorrectionInfo : public TObject {
public:
  CorrectionInfo();

  Double_t *time;
  Double_t *correctingVar;
  string firstName;
  string secondName;
  Int_t channel;
  vector <Double_t> coefs;
public:
  ClassDef(CorrectionInfo,1);
};
#endif
