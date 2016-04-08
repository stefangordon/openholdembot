//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#include "stdafx.h" 
#include "CParseTreeTerminalNodeUserVariable.h"

#include <math.h>
#include "CAutoplayerTrace.h"
#include "CEngineContainer.h"
#include "CFunctionCollection.h"
#include "CParserSymbolTable.h"
#include "CPreferences.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineMemorySymbols.h"
#include "CSymbolEngineOpenPPLUserVariables.h"
#include "CSymbolEngineTableLimits.h"
#include "FloatingPoint_Comparisions.h"
#include "NumericalFunctions.h"
#include "OH_MessageBox.h"
#include "StringFunctions.h"
#include "TokenizerConstants.h"

CParseTreeTerminalNodeUserVariable::CParseTreeTerminalNodeUserVariable(
  int relative_line_number, CString name) : 
  CParseTreeTerminalNodeIdentifier(relative_line_number, 
    name) {
  _node_type = kTokenActionUserVariableToBeSet;
}

CParseTreeTerminalNodeUserVariable::~CParseTreeTerminalNodeUserVariable() {
}

double CParseTreeTerminalNodeUserVariable::Evaluate(bool log /* = false */){
  CString name = _terminal_name;
   write_log(preferences.debug_formula(), 
    "[CParseTreeTerminalNode] Evaluating node type %i %s\n", 
		_node_type, name);
  p_autoplayer_trace->SetLastEvaluatedRelativeLineNumber(_relative_line_number);
  if (name.Left(4).MakeLower() == "user") {
    p_symbol_engine_openppl_user_variables->Set(name);
    return true;
  }
  if (name.Left(6) == "me_st_") {
    double temp_result;
    p_symbol_engine_memory_symbols->EvaluateSymbol(name,
      &temp_result, true);
    return temp_result;
  }
  assert(kThisMustNotHappen);
  return false;
}

CString CParseTreeTerminalNodeUserVariable::EvaluateToString(bool log /* = false */) {
  double numerical_result = Evaluate(log);
  CString result;
  if (IsInteger(numerical_result) && EvaluatesToBinaryNumber()) {
    // Generqate binary representation;
    result.Format("%s", IntToBinaryString(int(numerical_result)));
  } else {
    // Generate floating-point representation
    // with 3 digits precision
    result.Format("%.3f", numerical_result);
  }
  return result;
}

CString CParseTreeTerminalNodeUserVariable::Serialize() {
  if (_node_type == kTokenIdentifier) {
    return _terminal_name;
  } else {
    // Unhandled note-type, probably new and therefore not yet handled
    write_log(k_always_log_errors, "[CParseTreeTerminalNode] ERROR: Unhandled node-tzpe %i in serialiyation of parse-tree\n",
      _node_type);
    return "";
  }
}