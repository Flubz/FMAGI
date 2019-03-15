#include "MapGenerator.h"


AMapGenerator::AMapGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	StartPerlinSpawn();
}

void AMapGenerator::StartPerlinSpawn()
{
	for (int j = 0; j < _objectSpawnCount; j++)
	{
		for (int i = 0; i < _objectSpawnCount; i++)
		{
			GetNoiseValue((float) i / _noiseSize, (float) j / _noiseSize, _noiseVal);

			FVector spawnPosition = GetActorLocation() + GetActorRightVector() * (i * _spawnWidth);
			spawnPosition = spawnPosition + (GetActorForwardVector() * (j* _spawnWidth));
			spawnPosition = spawnPosition + (GetActorUpVector() * (spawnPosition.Z + (_noiseVal * _heightMultiplier)));

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FRotator rot = FRotator::ZeroRotator;

			AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(_objectToSpawn, spawnPosition, rot, params);
		}
	}
}
