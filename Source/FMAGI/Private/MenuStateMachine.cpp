// FM96.co.uk - Farjad Mohammad, University of Bolton, Games Programmer - 2018

#include "MenuStateMachine.h"

void UMenuStateMachine::SetState(FString desiredstate_)
{
	if (_states.Contains(desiredstate_) && !desiredstate_.Equals(_currentState, ESearchCase::IgnoreCase))
	{
		_onStateChanged.Broadcast(desiredstate_);
		_currentState = desiredstate_;
	}
}

void UMenuStateMachine::AddInitialStates(TArray<FString> & initialstates_)
{
	_states = initialstates_;
}

