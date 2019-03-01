#include "Unit.h"


UUnit::UUnit()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UUnit::BeginPlay()
{
	Super::BeginPlay();

}


void UUnit::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

