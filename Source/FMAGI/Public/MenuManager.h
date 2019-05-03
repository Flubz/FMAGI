// FM96.co.uk - Farjad Mohammad, University of Bolton, Games Programmer - 2018
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuStateMachine.h"
#include "MenuManager.generated.h"

UCLASS()
class FMAGI_API UMenuManager : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	UPROPERTY(BlueprintReadWrite) UMenuStateMachine* _menuStateMachine;
};
