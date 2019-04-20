#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Voxel.h"
#include "ChunkSpawnProperties.h"
#include "MainPlayerController.generated.h"

UCLASS()
class FMAGI_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Chunk, EditAnywhere, BlueprintReadOnly) FChunkSpawn _chunkSpawnProperties;
	UPROPERTY(Category = Chunk, EditAnywhere, BlueprintReadOnly) int32 _renderRange;
	UPROPERTY(Category = Chunk, EditAnywhere, BlueprintReadOnly) float _hitRange;
	UPROPERTY(Category = Chunk, EditAnywhere, BlueprintReadOnly) TSubclassOf<class AVoxel> _chunk;

	UFUNCTION(BlueprintCallable) void UpdateVoxel(bool isAdding);

private:
	int32 _voxelSizeHalf;
	TArray<AVoxel*> _chunks;
	TArray<FVector2D> _chunksCords;

	int32 _chunkSize;
	int32 _chunkSizeHalf;
	int32 _chunkX;
	int32 _chunkY;
	FVector _characterPosition;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	bool UpdatePosition();
	void AddChunk();
	void RemoveChunk();
	bool CheckRadius(float x, float y);
};
