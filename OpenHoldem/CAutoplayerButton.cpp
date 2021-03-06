//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Clean interface for scraper, casino-interface amd autoplayer
//
//******************************************************************************

#include "stdafx.h"
#include "CAutoplayerButton.h"

#include "CCasinoHotkey.h"
#include "CCasinoInterface.h"
#include "CPreferences.h"
#include "CStringMatch.h"
#include "..\CTablemap\CTablemap.h"
#include "MagicNumbers.h"

CAutoplayerButton::CAutoplayerButton() {
  Reset();
}

CAutoplayerButton::~CAutoplayerButton() {
}

void CAutoplayerButton::SetTechnicalName(const CString name) {
  _technical_name = name;
  CString hotkey_name = name + "hotkey";
  _hotkey.SetName(hotkey_name);
}

void CAutoplayerButton::Reset() {
  _label = "";
  _technical_name = "";
  SetClickable(false);
}
  
bool CAutoplayerButton::Click() {
  if (_clickable) {
    // Try to send a hotkey first, if specified in tablemap
    if (_hotkey.PressHotkey()) {
     write_log(preferences.debug_autoplayer(), "[CasinoInterface] Pressed hotkey for button button %s\n", _label);
      return true;
    }
    // Lookup the region
    RECT button_region;
    p_tablemap->GetTMRegion(_technical_name, &button_region);
    // Otherwise: click the button the normal way
    p_casino_interface->ClickRect(button_region);
    write_log(preferences.debug_autoplayer(), "[CAutoplayerButton] Clicked button [%s] [%s]\n", _label, _technical_name);
    return true;
  } else {
    write_log(preferences.debug_autoplayer(), "[CAutoplayerButton] Could not click button %s. Either undefined or not visible.\n", _label);
    return false;
  }
}

void CAutoplayerButton::SetClickable(bool clickable) {
  _clickable = clickable;
}

void CAutoplayerButton::SetState(const CString state) {
  CString button_state_lower_case = state;
  button_state_lower_case.MakeLower();
  if (button_state_lower_case.Left(4) == "true"
      || button_state_lower_case.Left(2) == "on"
      || button_state_lower_case.Left(3) == "yes"
      || button_state_lower_case.Left(7) == "checked"
      || button_state_lower_case.Left(3) == "lit") {
    SetClickable(true);
  } else {
    SetClickable(false);
  }
}

void CAutoplayerButton::SetLabel(const CString label) {
  _label = label;
}

bool CAutoplayerButton::IsAllin() {
  return p_string_match->IsStringAllin(_label);
}

bool CAutoplayerButton::IsRaise() {
  CString s_lower_case = _label.MakeLower();
  s_lower_case = s_lower_case.Left(5);
  return (s_lower_case == "raise"
    || s_lower_case == "ra1se"
    || s_lower_case == "ralse"
    || s_lower_case.Left(3) == "bet"
    // Last occurence of swag for backward compatibility
    || s_lower_case.Left(4) == "swag");
}

bool CAutoplayerButton::IsCall() {
  CString s_lower_case = _label.MakeLower();
  s_lower_case = s_lower_case.Left(4);
  return (s_lower_case == "call" || s_lower_case == "caii" || s_lower_case == "ca11");
}

bool CAutoplayerButton::IsCheck() {
  CString s_lower_case = _label.MakeLower();
  s_lower_case = s_lower_case.Left(5);
  return (s_lower_case == "check" || s_lower_case == "cheok");
}

bool CAutoplayerButton::IsFold() {
  CString s_lower_case = _label.MakeLower();
  s_lower_case = s_lower_case.Left(4);
  return (s_lower_case == "fold" || s_lower_case == "fo1d" || s_lower_case == "foid");
}

bool CAutoplayerButton::IsAutopost() {
  CString s_lower_case = _label;
  s_lower_case.Remove(' ');
  s_lower_case.Remove('-');
  s_lower_case.MakeLower();
  s_lower_case = s_lower_case.Left(8);
  return (s_lower_case == "autopost" || s_lower_case == "aut0p0st");
}

bool CAutoplayerButton::IsSitin() {
  CString s_lower_case = _label;
  s_lower_case.MakeLower();
  s_lower_case.Remove(' ');
  s_lower_case.Remove('-');
  s_lower_case = s_lower_case.Left(5);
  return (s_lower_case == "sitin" || s_lower_case == "s1t1n");
}

bool CAutoplayerButton::IsSitout() {
  CString s_lower_case = _label;
  s_lower_case.MakeLower();
  s_lower_case.Remove(' ');
  s_lower_case.Remove('-');
  s_lower_case = s_lower_case.Left(6);
  return (s_lower_case == "sitout" || s_lower_case == "s1tout" || s_lower_case == "sit0ut" || s_lower_case == "s1t0ut");
}

bool CAutoplayerButton::IsLeave() {
  return (_label.MakeLower().Left(5) == "leave");
}

bool CAutoplayerButton::IsRematch() {
  return (_label.MakeLower().Left(5) == "rematch");
}

bool CAutoplayerButton::IsPrefold() {
  return (_label.MakeLower().Left(7) == "prefold");
}



