#ifndef CSCCalibration_CSCConditions_h
#define CSCCalibration_CSCConditions_h

#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "CondFormats/CSCObjects/interface/CSCDBNoiseMatrix.h"
#include "CondFormats/DataRecord/interface/CSCDBGainsRcd.h"
#include "CondFormats/DataRecord/interface/CSCBadStripsRcd.h"
#include "CondFormats/DataRecord/interface/CSCBadWiresRcd.h"
#include "CondFormats/DataRecord/interface/CSCBadChambersRcd.h"
#include "CondFormats/DataRecord/interface/CSCDBChipSpeedCorrectionRcd.h"
#include "CondFormats/DataRecord/interface/CSCChamberTimeCorrectionsRcd.h"
#include "CondFormats/DataRecord/interface/CSCDBGasGainCorrectionRcd.h"
#include <vector>
#include <bitset>

class CSCDBGains;
class CSCDBPedestals;
class CSCDBCrosstalk;
class CSCBadStrips;
class CSCBadWires;
class CSCBadChambers;
class CSCDBChipSpeedCorrection;
class CSCChamberTimeCorrections;
class CSCDBGasGainCorrection;

/**  Encapsulates a user interface into the CSC conditions
 *
 * \author Rick Wilkinson
 * \author Tim Cox
 */

class CSCConditions
{
public:
  explicit CSCConditions( const edm::ParameterSet& ps );
  ~CSCConditions();

  /// fetch the maps from the database
  void initializeEvent(const edm::EventSetup & es);

  /// channels count from 1
  float gain(const CSCDetId & detId, int channel) const;
  /// total calibration precision
  float gainSigma(const CSCDetId & detId, int channel) const {return 0.005;}

  /// in ADC counts
  float pedestal(const CSCDetId & detId, int channel) const;
  float pedestalSigma(const CSCDetId & detId, int channel) const;

  float crosstalkSlope(const CSCDetId & detId, int channel, bool leftRight) const;
  float crosstalkIntercept(const CSCDetId & detId, int channel, bool leftRight) const;

  /// return raw noise matrix (unscaled short int elements)
  const CSCDBNoiseMatrix::Item & noiseMatrix(const CSCDetId & detId, int channel) const;

  /// fill vector (dim 12, must be allocated by caller) with noise matrix elements (scaled to float)
  void noiseMatrixElements( const CSCDetId& id, int channel, std::vector<float>& me ) const;

  /// fill vector (dim 4, must be allocated by caller) with crosstalk sl, il, sr, ir
  void crossTalk( const CSCDetId& id, int channel, std::vector<float>& ct ) const;

  // returns chip speed correction in ns given detId (w/layer) and strip channel
  float chipCorrection( const CSCDetId & detId, int channel ) const;

  //return chamber timing correction in ns given detId of chamber 
  float chamberTimingCorrection( const CSCDetId & detId) const;

  //return anode bx offset in bx given detId of chamber 
  float anodeBXoffset( const CSCDetId & detId) const;

  /// return  bad channel words per CSCLayer - 1 bit per channel
  const std::bitset<80>& badStripWord( const CSCDetId& id ) const;
  const std::bitset<112>& badWireWord( const CSCDetId& id ) const;

  void print() const;

  /// Is the gven chamber flagged as bad?
  bool isInBadChamber( const CSCDetId& id ) const;

  /// did we request reading bad channel info from db?
  bool readBadChannels() const { return readBadChannels_; }

  /// did we request reading bad chamber info from db?
  bool readBadChambers() const { return readBadChambers_; }

  /// did we request reading timing correction info from db?
  bool useTimingCorrections() const { return useTimingCorrections_; }

  /// fill bad channel words
  void fillBadStripWords();
  void fillBadWireWords();

  /// average gain over entire CSC system (logically const although must be cached here).
  float averageGain() const;

  /// gas gain correction as a function of detId (w/layer), strip, and wire
  float gasGainCorrection( const CSCDetId & detId, int strip, int wire ) const;

  /// did we request reading gas gain correction info from db?
  bool useGasGainCorrections() const { return useGasGainCorrections_; }

private:

  edm::ESHandle<CSCDBGains> theGains;
  edm::ESHandle<CSCDBCrosstalk> theCrosstalk;
  edm::ESHandle<CSCDBPedestals> thePedestals;
  edm::ESHandle<CSCDBNoiseMatrix> theNoiseMatrix;
  edm::ESHandle<CSCBadStrips> theBadStrips;
  edm::ESHandle<CSCBadWires> theBadWires;
  edm::ESHandle<CSCBadChambers> theBadChambers;
  edm::ESHandle<CSCDBChipSpeedCorrection> theChipCorrections;
  edm::ESHandle<CSCChamberTimeCorrections> theChamberTimingCorrections;
  edm::ESHandle<CSCDBGasGainCorrection> theGasGainCorrections;

  bool readBadChannels_; // flag whether or not to even attempt reading bad channel info from db
  bool readBadChambers_; // flag whether or not to even attempt reading bad chamber info from db
  bool useTimingCorrections_; // flag whether or not to even attempt reading timing correction info from db
  bool useGasGainCorrections_; // flag whether or not to even attempt reading gas-gain correction info from db

  // cache bad channel words once created
  std::vector< std::bitset<80> > badStripWords;
  std::vector< std::bitset<112> > badWireWords;

  mutable float theAverageGain; // average over entire system, subject to some constraints!

  edm::ESWatcher<CSCDBGainsRcd> gainsWatcher_; 
  //@@ remove until we have real information to use
  //  edm::ESWatcher<CSCBadStripsRcd> badStripsWatcher_; 
  //  edm::ESWatcher<CSCBadWiresRcd> badWiresWatcher_; 

  // Total number of CSC layers in the system, with full ME42 installed.
  enum elayers{ MAX_LAYERS = 3240 };
};

#endif


