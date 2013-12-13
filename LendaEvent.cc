 

#include "LendaEvent.hh"
#include <iostream>
#include "TMath.h"
#include <sstream>

using namespace std;


#define BAD_NUM -1008
LendaEvent::LendaEvent()
{

  fgainCorrections.clear();

  fnumOfGainCorrections=0;
  
  GammaPeakTime=4.08274;
  GammaPeakTime2=0.249663;
  

  TOFFudge=1.0;
  Clear();


}
LendaEvent::LendaEvent(bool BuildMap){
  LendaEvent();
  if (BuildMap)
    DefineMap();
  
}



void LendaEvent::setGainCorrections(vector <pair <Double_t,Double_t> > in ){
  for (int i=0;i<(int)in.size();i++)
    setGainCorrections(in[i].first,in[i].second,i);
}

void LendaEvent::setGainCorrections(Double_t slope,Double_t c,Int_t channel){

  if (channel >= (Int_t)fgainCorrections.size()){
    int diff = channel - fgainCorrections.size();
    fgainCorrections.resize(fgainCorrections.size()+diff+1,make_pair(1.0,0));
  }
  fgainCorrections[channel]=make_pair(slope,c);


  fnumOfGainCorrections++;
}
void LendaEvent::PrintList(){
  /*  DumpCorrectionsMap();
  cout<<"The mapping between Corrections and names is"<<endl;
  CorMap=mapForCorrectionResults;
  for (map<string,int>::iterator ii =CorMap.begin();ii!=CorMap.end();ii++)
    cout<<ii->first<<"  "<<ii->second<<endl;
  */
  AddMapEntry("energies",&energies);
}

void LendaEvent::Clear(){
  ////REMEBER TO CLEAR THINGS THAT were thing.push_Back!!!!
  TOF=BAD_NUM;
  Dt=BAD_NUM;
  TOFEnergy=BAD_NUM;
  TOFEnergyInternal=BAD_NUM;

  ShiftTOF=BAD_NUM;
  ShiftTOFInternal=BAD_NUM;

  NumBadPoints=0;
  ErrorBit=0;

  
  
  GOE=BAD_NUM;
  CorGOE=BAD_NUM;
  PulseShape=BAD_NUM;
  
  times.clear();
  energies.clear();
  energiesCor.clear();
  internEnergies.clear();
  channels.clear();
  softwareCFDs.clear();
  internalCFDs.clear();
  softTimes.clear();
  cubicTimes.clear();
  cubicCFDs.clear();

  pulseHeights.clear();

  Traces.clear();
  Filters.clear();
  CFDs.clear();

  entryNums.clear();

  shortGates.clear();
  longGates.clear();

  
  // heDynamicCorrectionResults.clear();


  NumOfChannelsInEvent=0;
  N=NumOfChannelsInEvent;
}

void LendaEvent::pushTime(Double_t t){
  times.push_back(t);

}
void LendaEvent::pushEnergy(Double_t e){
  energies.push_back(e);

}
void LendaEvent::pushChannel(Double_t c){
  channels.push_back(c);
}

void LendaEvent::pushTrace(vector <UShort_t> in){
  Traces.push_back(in);
}
void LendaEvent::pushFilter(vector <Double_t> in){
  Filters.push_back(in);
}
void LendaEvent::pushCFD(vector <Double_t> in){
  CFDs.push_back(in);
}

void LendaEvent::pushInternalCFD(Double_t t){
  internalCFDs.push_back(t);
}
void LendaEvent::pushSoftwareCFD(Double_t t){
  softwareCFDs.push_back(t);
}
void LendaEvent::pushSoftTime(Double_t t){
  softTimes.push_back(t);
}

void LendaEvent::pushEntryNum(Long64_t t){
  entryNums.push_back(t);
}
void LendaEvent::pushCubicTime(Double_t t){
  cubicTimes.push_back(t);
}
void LendaEvent::pushCubicCFD(Double_t t){
  cubicCFDs.push_back(t);
}

void LendaEvent::pushLongGate(Double_t lg){
  longGates.push_back(lg);
}
void LendaEvent::pushShortGate(Double_t sg){
  shortGates.push_back(sg);
}
void LendaEvent::pushInternEnergy(Double_t t){
  internEnergies.push_back(t);
}

void LendaEvent::pushPulseHeight(Double_t t){
  pulseHeights.push_back(t);
}

void LendaEvent::gainCor(){

  //Applying gain correction to each of the channels for Lenda bars
  
  for (int i=0;i<(int)energies.size();i++){
    energiesCor[i]=energies[i]*fgainCorrections[channels[i]].first +
      fgainCorrections[channels[i]].second;

  }
}

void LendaEvent::Finalize(){
  NumOfChannelsInEvent = times.size();//the number of channels pushed to event
  N=NumOfChannelsInEvent;
  energiesCor.resize(energies.size());

 
  if (fgainCorrections.size()!=0)//only apply gain correctins if 
    gainCor();                   //they have be provided

  /*
  if (Traces.size()!=0){
    for (int j=0;j<Traces.size();j++){
      double avg =0;
      if (Traces[j].size()>20){
	for (int a=0;a<20;a++)
	  avg = Traces[j][a] +avg;
	avg=avg/20.0;
	for (int i=0;i<Traces[j].size();i++){
	  if (Traces[j][i]< 0.9*avg)
	    NumBadPoints++;
	}
      }
    }
  }
  */
  
  if (cubicTimes.size()==4)
    TOF = 0.5*(cubicTimes[0]+cubicTimes[1]- cubicTimes[2]-cubicTimes[3]);
  else 
    TOF=BAD_NUM;

  
  Dt = times[0]-times[1];
  
  CDt= cubicTimes[0]-cubicTimes[1];

  GOE = (energies[0]-energies[1])/(energies[0]+energies[1]);
  CorGOE = (energiesCor[0]-energiesCor[1])/(energiesCor[0]+energiesCor[1]);
  
  //  posCor();  

  //  if (fwalkCorrections.size()!=0)
  //  walkCor();
  for (int i=0;i<energies.size();i++){
    if (energies[i]<0 || softwareCFDs[i]<0 ){
      ErrorBit=true;
      break;
    }

  }

  
  Double_t shiftTime=BAD_NUM;
  Double_t shiftTime2=BAD_NUM;
  if ( NumOfChannelsInEvent==3){
  
    PulseShape = longGates[2]/shortGates[2];
    Double_t shift=(GammaPeakTime-0.334448);
    shiftTime = (0.5*(cubicTimes[0]+cubicTimes[1])-cubicTimes[2]) - shift;
    shiftTime2 =(0.5*(times[0]+times[1])-times[2]) - shift;
    shiftTime=shiftTime/TOFFudge;
    shiftTime2=shiftTime2/TOFFudge;

  } else if (NumOfChannelsInEvent==2){   //For liquid scint case

    Double_t shift=(GammaPeakTime2-0.334448);
    shiftTime = cubicTimes[0]-cubicTimes[1] - shift;
    shiftTime2 = times[0]-times[1]- shift;
    shiftTime=shiftTime/TOFFudge;
    shiftTime2=shiftTime2/TOFFudge;

  }
  ShiftTOF=shiftTime;
  ShiftTOFInternal=shiftTime2;
  Double_t c= 2.99 * TMath::Power(10,8);    
  shiftTime =10.0*shiftTime*(1.0/(TMath::Power(10,9)));// put time in secs
  
  Double_t beta = (1.0/c)*(1.0/shiftTime);
  Double_t gamma = 1.0/(TMath::Sqrt(1-beta*beta));
  Double_t KE = (gamma-1.0)*939.5650; // MEV
  TOFEnergy=KE;

  
  
  shiftTime2 =10.0*shiftTime2*(1.0/(TMath::Power(10,9)));// put time in secs
  
  beta = (1.0/c)*(1.0/shiftTime2);
  gamma = 1.0/(TMath::Sqrt(1-beta*beta));
  KE = (gamma-1.0)*939.5650; // MEV
  TOFEnergyInternal=KE;
  
  //   if (NumOfChannelsInEvent==2){
  //     cout<<"Shift Time 2 "<<shiftTime2<<endl;
  //     cout<<"channels "<<channels[0]<<" "<<channels[1]<<endl;
  // }
  //ApplyDynamicCorrections();
  
}


void LendaEvent::DumpGainCorrections(){
  cout<<"\n***Dump gain Corrections***"<<endl;
  for (int i=0;i<(int)fgainCorrections.size();++i){
    cout<<"gain correction for channel "<<i<<" "<<fgainCorrections[i].first<<" "
	<<fgainCorrections[i].second<<endl;
  }
}

void LendaEvent::DumpAllCorrections(){
  DumpGainCorrections();
}


void LendaEvent::Fatal(){
  cout<<"This Method should not exist.  Don't call it"<<endl;
}


void LendaEvent::MakeC(int spot){
  /*  
  cout<<"this is CTrace "<<CTrace<<endl;
  
  if (CTrace != 0){
    cout<<"Free CTrace"<<endl;
    free(CTrace);
    CTrace=0;
    
  }
  if (CFilter !=0){
    cout<<"Free CFilter"<<endl;
    free(CFilter);
    CFilter=0;
  }

  if (CCFD !=0 ){
    cout<<"Free CCFD"<<endl;
    free(CCFD);
    CCFD=0;
  }

  if (Traces.size()!=0 &&Traces[spot].size() != 0 ){
    cout<<"Allocate CTrace"<<endl;
    CTrace = (UShort_t*)calloc(sizeof(UShort_t),Traces[spot].size());
  }
  
  if (Filters.size() !=0 && Filters[spot].size() != 0){
    cout<<"Allocate CFilter"<<endl;
    CFilter = (Double_t*)calloc(sizeof(Double_t),Traces[spot].size());
  }
  
  if (CFDs.size() !=0 && CFDs[spot].size() != 0 ){
    cout<<"Allocate CCFD"<<endl;
    CCFD = (Double_t*)calloc(sizeof(Double_t),Traces[spot].size());
  }
  
  for (int i=0;i<Traces[spot].size();i++){
    if (CTrace != 0)
      CTrace[i]=Traces[spot][i];
    if (CFilter !=0 )
      CFilter[i]=Filters[spot][i];
    if ( CCFD !=0 )
      CCFD[i]=CFDs[spot][i];
  }
  */
}



LendaEvent & LendaEvent::operator = (const LendaEvent&  right){

  this->Clear();

  this->energiesCor = right.energiesCor;
  this->times = right.times;
  this->softTimes = right.softTimes;
  this->cubicTimes = right.cubicTimes;
  this->energies = right.energies;
  this->internEnergies=right.internEnergies;
  this->channels = right.channels;
  this->softwareCFDs =right.softwareCFDs;
  this->cubicCFDs =right.cubicCFDs;
  this->internalCFDs =right.internalCFDs;
  this->entryNums=right.entryNums;
  this->Traces = right.Traces;
  this->Filters = right.Filters;
  this->CFDs = right.CFDs;
  this->longGates =right.longGates;
  this->shortGates =right.shortGates;

  //to prevent mismatch in sizes between different 
  //versions with different features
  this->softwareCFDs.resize(this->times.size(),-1);
  this->cubicCFDs.resize(this->times.size(),-1);
  this->longGates.resize(this->times.size(),-1);
  this->shortGates.resize(this->times.size(),-1);
  this->pulseHeights.resize(this->times.size(),-1);
  
  return *this;

}


///////BEGIN __AUTO__ GENERATED///////
void LendaEvent::DefineMap(){
theVariableMap["TOF"]=&TOF;
theVariableMap["Dt"]=&Dt;
theVariableMap["E0"]=&E0;
theVariableMap["E1"]=&E1;
theVariableMap["E2"]=&E2;
theVariableMap["E3"]=&E3;
theVariableMap["CDt"]=&CDt;
theVariableMap["NumBadPoints"]=&NumBadPoints;
theVariableMap["PulseShape"]=&PulseShape;
theVariableMap["GOE"]=&GOE;
theVariableMap["CorGOE"]=&CorGOE;
theVariableMap["energies"]=&energies;
theVectorVariableMap["energies"]=&energies;
theVariableMap["energiesCor"]=&energiesCor;
theVectorVariableMap["energiesCor"]=&energiesCor;
}
 
