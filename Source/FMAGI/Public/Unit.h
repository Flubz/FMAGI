#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Unit.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FMAGI_API UUnit : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnit();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
