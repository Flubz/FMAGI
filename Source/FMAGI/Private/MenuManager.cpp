// FM96.co.uk - Farjad Mohammad, University of Bolton, Games Programmer - 2018

#include "MenuManager.h"


void UMenuManager::Init()
{
	_menuStateMachine = NewObject<UMenuStateMachine>(this);
	Super::Init();
}

