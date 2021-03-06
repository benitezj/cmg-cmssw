// $Id: TTUTwoORLogic.cc,v 1.1 2009/06/17 15:27:24 aosorio Exp $
// Include files 



// local
#include "L1Trigger/RPCTechnicalTrigger/interface/TTUTwoORLogic.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TTUTwoORLogic
// 
// This logic implements the OR of the two logics TTU or RBC
//
//
// 2009-06-15 : Andres Felipe Osorio Oliveros
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TTUTwoORLogic::TTUTwoORLogic(  ) {

  m_triggersignal = false;
 
  m_debug = false;

  m_ttuLogic = new TTUTrackingAlg();
  
  m_rbcLogic = new TTUSectorORLogic();
  
}
//=============================================================================
// Destructor
//=============================================================================
TTUTwoORLogic::~TTUTwoORLogic() {

  if ( m_ttuLogic ) delete m_ttuLogic;

  if ( m_rbcLogic ) delete m_rbcLogic;
  
} 

//=============================================================================

void TTUTwoORLogic::setBoardSpecs( const TTUBoardSpecs::TTUBoardConfig & boardspecs ) 
{
  
  m_ttuLogic->setBoardSpecs( boardspecs );
  
  m_rbcLogic->setBoardSpecs( boardspecs );
  
}

bool TTUTwoORLogic::process( const TTUInput & inmap )
{
  
  if( m_debug) std::cout << "TTUTwoORLogic::process starts" << std::endl;
  
  m_triggersignal = false;
  
  
  m_ttuLogic->process( inmap );
  m_rbcLogic->process( inmap );
  
  bool triggerFromTTU = m_ttuLogic->m_triggersignal;
  
  bool triggerFromRBC = m_rbcLogic->m_triggersignal;
    
  m_triggersignal = triggerFromTTU || triggerFromRBC;
  
  if( m_debug ) std::cout << "TTUTwoORLogic>process ends" << std::endl;
  
  return true;
  
}

