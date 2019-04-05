#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class FMAGI_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Settings, EditAnywhere, BlueprintReadWrite) int32 _renderRange;
	UPROPERTY(Category = Settings, EditAnywhere, BlueprintReadWrite) int32 _chunkLineElement;
	UPROPERTY(Category = Settings, EditAnywhere, BlueprintReadWrite) int32 _voxelSize;
	// Is ChunkLineElement * VoxelSize
	UPROPERTY(Category = Settings, VisibleAnywhere, BlueprintReadOnly) int32 _chunkSize;
	UPROPERTY(Category = Settings, VisibleAnywhere, BlueprintReadOnly) int32 _chunkSizeHalf;
	UPROPERTY(Category = Settings, EditAnywhere, BlueprintReadWrite) int32 _chunkX;
	UPROPERTY(Category = Settings, EditAnywhere, BlueprintReadWrite) int32 _chunkY;
	UPROPERTY(Category = Settings, EditAnywhere, BlueprintReadWrite) FVector _characterPosition;
	UPROPERTY(Category = Settings, EditAnywhere, BlueprintReadWrite) TSubclassOf<class AActor> _chunk;
	TArray<AActor*> _chunks;
	TArray<FVector2D> _chunksCords;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	bool UpdatePosition();
	void GenerateChunk();
	void RemoveChunk();
	bool CheckRadius(float x_, float y_);

};
