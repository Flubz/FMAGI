#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleXNoiseGenerator.generated.h"

UCLASS()
class FMAGI_API ASimpleXNoiseGenerator : public AActor
{
	GENERATED_BODY()

public:
	ASimpleXNoiseGenerator();

	UPROPERTY(Category = Noise, EditAnywhere, BlueprintReadWrite)
		float _spawnWidth = 10.0f;
	UPROPERTY(Category = Noise, EditAnywhere, BlueprintReadWrite)
		int _objectSpawnCount = 50;
	UPROPERTY(Category = Noise, EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AActor> _objectToSpawn;
	UPROPERTY(Category = Noise, EditAnywhere, BlueprintReadWrite)
		float _heightMultiplier = 1.0f;
	UPROPERTY(Category = Noise, EditAnywhere, BlueprintReadWrite)
		float _noiseSize = 100.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = Noise)
		void GetNoiseValue(float x, float y, float& refNoiseValue);
	UFUNCTION(BlueprintCallable)
		void StartPerlinSpawn();
private:
	float _noiseVal;

};
