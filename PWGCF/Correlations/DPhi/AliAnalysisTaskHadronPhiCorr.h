#ifndef AliAnalysisTaskHadronPhiCorr_cxx
#define AliAnalysisTaskHadronPhiCorr_cxx

//QA task for EMCAL electron analysis
#include "AliAnalysisTaskSE.h"
#include "TObject.h"
#include "THnSparse.h"
#include "TH3.h"
#include "TFile.h"

class TObject;
class TH1F;
class AliESDEvent;
class AliAODEvent;
class AliEventPool;
class AliEventPoolManager;
class AliCFParticle;
class AliAODMCParticle;
class AliMultSelection;

class AliAnalysisTaskHadronPhiCorr : public AliAnalysisTaskSE {
public:
    AliAnalysisTaskHadronPhiCorr();
    AliAnalysisTaskHadronPhiCorr(const char *name, Bool_t isHH, Float_t multLow, Float_t multHigh);
    virtual ~AliAnalysisTaskHadronPhiCorr();
    
    virtual void   UserCreateOutputObjects();
    virtual void   UserExec(Option_t *option);
    virtual void   Terminate(Option_t *);

    void SetAODAnalysis() { SetBit(kAODanalysis, kTRUE); };
    void SetESDAnalysis() { SetBit(kAODanalysis, kFALSE); };
    Bool_t IsAODanalysis() const { return TestBit(kAODanalysis); };
    
    struct AliHContainer{
        Int_t trackNum;
        Float_t charge;
        TLorentzVector particle;
    };

    struct AliPhiContainer{
        Int_t daughter1TrackNum;
        Int_t daughter2TrackNum;
        TLorentzVector particle;
    };

    void SetEtaPhiRegion(UInt_t etaphi){ ETA_PHI_REGION = etaphi; };

    void SetKaonEtaCut(Float_t eta) { KAON_ETA_CUT = eta; };
    void SetKaonTPCCut(Float_t tpcNSigma) { KAON_TPC_CUT = tpcNSigma; };
    void SetKaonTOFCut(Float_t tofNSigma) { KAON_TOF_CUT = tofNSigma; };
    void SetTOFVeto(Bool_t isVeto) { IS_KAON_TOF_VETO = isVeto; };
    void SetKaonTrkBit(UInt_t trkbit) { KAON_TRK_BIT = trkbit; };

    void SetTrigTrkBit(UInt_t trkbit) { TRIG_TRK_BIT = trkbit; };
    void SetAssocTrkBit(UInt_t trkbit) { ASSOC_TRK_BIT = trkbit; };

    void SetZVertexMin(Float_t zvtxMin) { Z_VTX_MIN = zvtxMin; };
    void SetZVertexMax(Float_t zvtxMax) { Z_VTX_MAX = zvtxMax; };
    void SetZVertexNbins(Int_t zvtxNbins) { Z_VTX_NBINS = zvtxNbins; };

    void SetCentEstimator(TString centEst) { CENT_ESTIMATOR = centEst; };

    void SetHH(Bool_t isHH) { IS_HH = isHH; };

    void SetIsMCTrue(Bool_t isMCTrue) {IS_MC_TRUE = isMCTrue; };
    void SetIsMCKaon(Bool_t isMCKaon) {IS_MC_KAON = isMCKaon; };
    void SetIsMCKTrack(Bool_t isMCKTrack) {IS_MC_KTRACK = isMCKTrack; };
    void SetUseAccpt(Bool_t useAccpt) {USE_ACCPT = useAccpt; };

    void SetMultLow(Float_t multLow) { MULT_LOW = multLow; };
    void SetMultHigh(Float_t multHigh) { MULT_HIGH = multHigh; };

    void SetSingleTrigger(Bool_t doSingleTrigger) { DO_SINGLE_TRIGGER = doSingleTrigger; };
    void SetSelectTrigger(Bool_t selectTriggerEvents) { SELECT_TRIGGER_EVENTS = selectTriggerEvents; };
    void SetHighestTriggerOnly(Bool_t doHighestTrigger) { DO_HIGHEST_TRIGGER = doHighestTrigger; };
    void SetPerEventScaling(Bool_t doPerEventScaling) { DO_PER_EVENT_SCALING = doPerEventScaling; };

    void LoadEfficiencies(TFile* filename);
    void LoadEfficiencies(TF1* phieff, TF1* heff, TF1* trigeff);
    void LoadEfficiencyHistos(TFile* efffile);

private:

    Bool_t IS_MC_TRUE;
    Bool_t IS_MC_KAON;
    Bool_t IS_MC_KTRACK;
    Bool_t USE_ACCPT;
    
    Bool_t DO_SINGLE_TRIGGER;
    Bool_t SELECT_TRIGGER_EVENTS;
    Bool_t DO_HIGHEST_TRIGGER;
    Bool_t DO_PER_EVENT_SCALING;

    Bool_t IS_HH;
    Float_t MULT_LOW;
    Float_t MULT_HIGH;
    
    UInt_t ETA_PHI_REGION;

    Float_t KAON_ETA_CUT;
    Float_t KAON_TPC_CUT;
    Float_t KAON_TOF_CUT;
    Bool_t IS_KAON_TOF_VETO;
    UInt_t KAON_TRK_BIT;

    UInt_t TRIG_TRK_BIT;
    UInt_t ASSOC_TRK_BIT;

    Float_t Z_VTX_MIN;
    Float_t Z_VTX_MAX;
    Int_t Z_VTX_NBINS;

    TString CENT_ESTIMATOR;


    enum{
        kAODanalysis = BIT(20),
    };
 
       
    TObjArray* AddToTracks();
    Bool_t MakeCorrelations(Int_t itrack, const AliHContainer& trigger, const std::vector<AliPhiContainer>& phiVec, THnSparse *fDphi, Double_t zVtx, Int_t numtrigs);
    Bool_t MakeCorrelations(Int_t itrack, AliAODMCParticle *trigger, const std::vector<AliPhiContainer>& phiVec, THnSparse *fDphi, Double_t zVtx);
    void MakeMixCorrelations(AliPhiContainer* phiVec, THnSparse *fDphiMixed, Float_t mult, Double_t zVtx, AliEventPool* fPool, Bool_t isLS);
    void MakeHHMixCorrelations(AliCFParticle *cfPart, THnSparse *fDphiMixed, Float_t mult, Double_t zVtx);
  
    AliVEvent   *fVevent;  //!event object
    AliEventPoolManager *fPoolMgr; //! Event pool manager for mixed event
    AliEventPoolManager *fTruePoolMgr; //!
    AliEventPoolManager *fLSPoolMgr; //! Event pool manager for LS mixed event
    AliEventPoolManager *fHHPoolMgr; //! Event pool manager for HH
    AliESDEvent *fESD;    //!ESD object
    AliAODEvent *fAOD;    //!AOD object
    AliPIDResponse *fpidResponse; //!pid response
    AliMultSelection *fMultSelection; //!mult selection

    TF1         *fphiEff;// phi Efficiency
    TF1         *fhEff;// hadron Efficiency
    TF1         *ftrigEff;// trigger Efficiency
 
    TH1D         *fphiEffHist;// phi Efficiency
    TH1D         *fhEffHist;// hadron Efficiency
    TH1D         *ftrigEffHist;// trigger Efficiency
   
    TList       *fOutputList; //!Output list
    TH1F        *fNevents;//! no of events
    TH1F        *fNumTracks;//! number of Tracks/evt
    TH1F        *fVtxZ;//!Vertex z
    TH1F        *fVtxX;//!Vertex x
    TH1F        *fVtxY;//!Vertex y
    TH1F        *fVtxZmixbins;//! Vertex z, mixing bins
    TH2F        *fTrigMulti;//!trigger 
    TH1F        *fTrkPt;//!track pt
    TH1F        *fTrketa;//!track eta
    TH1F        *fTrkphi;//!track phi
    TH1F        *fHybridTrkPt;//!hybridTPC track pt
    TH1F        *fHybridTrketa;//!hybridTPC track eta
    TH1F        *fHybridTrkphi;//!hybridTPC track phi
    TH1F        *fHybridGlobalTrkPt;//!hybridGlobal track pt
    TH1F        *fHybridGlobalTrketa;//!hybridGlobal track eta
    TH1F        *fHybridGlobalTrkphi;//!hybridGlobal track phi
    TH2F        *fdEdx;//!dedx vs pt
    TH2F        *fTPCNpts;//!TPC Npoints used for dedx

    TH1D        *fHadronPT;//!Hadron PT distribution
    TH1D        *fHadronTrigPT;//!Hadron PT distribution for events with triggers
    TH1D        *fHadronTrigPhiPT;//!Hadron PT distribution for events with triggers and phi candidate

    TH3F        *fKaonPID;//!Kaon PID
    TH3F        *fKaonDist;//!Kaon pt, phi, eta
    TH2F        *fTPCKaonNSig;//!TPC Nsigma

    THnSparseF  *fTrigDist;//! trigger distribution
    TH1D        *fTrigPerEvent;//! trigger tracks per event
    TH2D        *fTrigSameUSDist;//! trigger count for same dist, US pairs
    TH2D        *fTrigSameLSDist;//! trigger count for same dist, LS pairs
    TH2D        *fTrigHHDist;//! trigger count for hh pairs
    
    TH2D        *fLSMixStatZVtx;//! stats for mixed events
    TH2D        *fLSMixTrackStatZVtx;//! stats for mixed events
    TH1D        *fLSNoMixEvents;//! number of mixed events
    TH2D        *fUSMixStatZVtx;//! stats for mixed events
    TH2D        *fUSMixTrackStatZVtx;//! stats for mixed events
    TH1D        *fUSNoMixEvents;//! number of mixed events
    TH2D        *fHHMixStatZVtx;//! stats for mixed events
    TH2D        *fHHMixTrackStatZVtx;//! stats for mixed events
    TH1D        *fHHNoMixEvents;//! number of mixed events

    THnSparseF  *fKKUSDist;//! unlike sign kaon distribution
    THnSparseF  *fKKLSDist;//! like sign kaon distribution
    THnSparseF  *fKKUSTrigDist;//! unlike sign kaon distribution for events with trigger
    THnSparseF  *fKKLSTrigDist;//! like sign kaon distribution for events with trigger
    TH1D        *fkplusPerEvent;//! K+ per Event
    TH1D        *fkminusPerEvent;//! K- per Event
    TH1D        *fLSpairsPerEvent;//! LS pairs per Event in mass range
    TH1D        *fUSpairsPerEvent;//! US pairs per Event in mass range

    THnSparseF  *fTruePhiDist;//! Distribution for MC True phi's
    THnSparseF  *fTrueHDist;//! Distriubution for MC True hadrons
    THnSparseF  *fTruePrimHDist;//! Distriubution for MC True Physical Primary hadrons
    THnSparseF  *fTrueSecHDist;//! Distriubution for MC True Secondary hadrons
    THnSparseF  *fTrueNotPrimHDist;//! Distriubution for MC True Not Physical Primary hadrons

    THnSparseF  *fTriggeredTruePhiDist;//! Distribution for MC TriggeredTrue phi's
    THnSparseF  *fTriggeredTrueHDist;//! Distriubution for MC TriggeredTrue hadrons
    THnSparseF  *fTriggeredTruePrimHDist;//! Distriubution for MC TriggeredTrue Physical Primary hadrons
    THnSparseF  *fTriggeredTrueSecHDist;//! Distriubution for MC TriggeredTrue Secondary hadrons
    THnSparseF  *fTriggeredTrueNotPrimHDist;//! Distriubution for MC TriggeredTrue Physical Primary hadrons

    TH2D        *fHadronsVsPhi;//! hadrons vs. phi per event
    TH2D        *fHadronsVsPhiPairs;//! h-h vs h-phi per event
    TH1D        *fRatio;//! phi/hadron ratio per event
    TH1D        *fPairRatio;//! h-phi/h-h ratio per event
    
    THnSparseF  **fDphiHPhi;//! delta-phi distribution with unlike sign kaon pairs
    THnSparseF  **fDphiTrueHPhi;//! delta-phi distribution with true MC phi
    THnSparseF  **fDphiTrueAcceptanceHPhi;//! delta-phi distribution with true MC phi in acceptance
    THnSparseF  **fDphiTrueHPhiMixed;//! mixed distribution with true MC phi
    THnSparseF  **fDphiHKK;//! delta-phi distribution with like sign kaon pairs
    THnSparseF  **fDphiHPhiMixed;//! hadron-US mixed correlation
    THnSparseF  **fDphiHKKMixed;//! hadron-LS mixed correlation
    THnSparseF  **fDphiHH;//! hadron-hadron correlation
    THnSparseF  **fDphiHHMixed;//! hadron-hadron mixed correlation

    AliAnalysisTaskHadronPhiCorr(const AliAnalysisTaskHadronPhiCorr&); // not implemented
    AliAnalysisTaskHadronPhiCorr& operator=(const AliAnalysisTaskHadronPhiCorr&); // not implemented
   
    ClassDef(AliAnalysisTaskHadronPhiCorr, 3); // 
};

#endif

