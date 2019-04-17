#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Voxel.h"
#include "MainPlayerController.generated.h"

UCLASS()
class FMAGI_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite) int32 _renderRange;
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite) int32 _chunkLineElement;
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite) int32 _voxelSize;
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite) float _hitRange = 1000.0f;
	// Is ChunkLineElement * VoxelSize
	UPROPERTY(Category = Voxel, VisibleAnywhere, BlueprintReadOnly) int32 _chunkSize;
	UPROPERTY(Category = Voxel, VisibleAnywhere, BlueprintReadOnly) int32 _chunkSizeHalf;
	UPROPERTY(Category = Voxel, VisibleAnywhere, BlueprintReadOnly) int32 _chunkX;
	UPROPERTY(Category = Voxel, VisibleAnywhere, BlueprintReadOnly) int32 _chunkY;
	UPROPERTY(Category = Voxel, VisibleAnywhere, BlueprintReadOnly) FVector _characterPosition;
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite) TSubclassOf<class AVoxel> _chunk;
	
	TArray<AVoxel*> _chunks;
	TArray<FVector2D> _chunksCords;

	UFUNCTION(BlueprintCallable)
		void UpdateVoxel(bool isAdding);

protected:
	int32 _voxelSizeHalf;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	bool UpdatePosition();
	void AddChunk();
	void RemoveChunk();
	bool CheckRadius(float x_, float y_);
};
