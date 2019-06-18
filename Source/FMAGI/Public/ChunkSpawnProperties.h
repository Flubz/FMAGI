#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "ChunkSpawnProperties.generated.h"

USTRUCT(BlueprintType)
struct FChunkSpawn
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(Category = "Chunk", EditAnywhere, BlueprintReadOnly) int32 _randomSeed = 0;
	UPROPERTY(Category = "Chunk", EditAnywhere, BlueprintReadOnly) int32 _chunkXYSize = 20;
	UPROPERTY(Category = "Chunk", EditAnywhere, BlueprintReadOnly) int32 _chunkZSize = 80;
	// This value should be higher than the highest/lowest noise values.
	UPROPERTY(Category = "Chunk", EditAnywhere, BlueprintReadOnly) int32 _chunkGroundHeight = 30;
	UPROPERTY(Category = "Chunk", EditAnywhere, BlueprintReadOnly) int32 _voxelSize = 100;
	UPROPERTY(Category = "Chunk", EditAnywhere, BlueprintReadOnly) bool _voxelCollision = true;
	UPROPERTY(Category = "Chunk", EditAnywhere, BlueprintReadOnly) bool _generateTrees = true;
	UPROPERTY(Category = "Chunk", EditAnywhere, BlueprintReadOnly) bool _generateGrass = true;
};

UCLASS()
class FMAGI_API UChunkSpawnProperties : public UUserDefinedStruct
{
	GENERATED_BODY()
};
