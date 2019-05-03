#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MenuStateMachine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStateChangeDelegate, FString, newState);

UCLASS()
class FMAGI_API UMenuStateMachine : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States") TArray<FString> _states;
	UPROPERTY(BlueprintAssignable, Category = "StateChange") FStateChangeDelegate _onStateChanged;

	UFUNCTION(BlueprintCallable) void SetState(FString desiredstate_);
	UFUNCTION(BlueprintCallable) void AddInitialStates(TArray<FString> & initialstates_);

private:
	FString _currentState = "";
};