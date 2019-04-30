#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Voxel.generated.h"

UCLASS()
class FMAGI_API AVoxel : public AActor
{
	GENERATED_BODY()

public:
	int32 GetId() const { return _id; }

	UMaterialInterface* GetMaterialInterface() const { return _materialInterface; }
	void SetMaterialInterface(UMaterialInterface* val) { _materialInterface = val; }

private:
	UPROPERTY(Category = Voxel, EditAnywhere) int32 _id;
	UPROPERTY(Category = Voxel, EditAnywhere) UMaterialInterface* _materialInterface;
};
