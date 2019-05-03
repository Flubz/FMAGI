#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "ProceduralMeshComponent.h"
#include "ChunkSpawnProperties.h"
#include "NoExportTypes.h"
#include "Chunk.generated.h"

struct FMeshSection
{
	TArray<FVector> Verticies;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVS;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;
	int32 elementID = 0;
};

USTRUCT(BlueprintType)
struct FOctaves
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(Category = Octave, EditAnywhere, BlueprintReadWrite)
		bool _skip = false;
	UPROPERTY(Category = Octave, EditAnywhere, BlueprintReadWrite)
		float _xMult = 1;
	UPROPERTY(Category = Octave, EditAnywhere, BlueprintReadWrite)
		float _yMult = 1;
	UPROPERTY(Category = Octave, EditAnywhere, BlueprintReadWrite)
		float _weight = 1;
	UPROPERTY(Category = Octave, EditAnywhere, BlueprintReadWrite)
		bool _clamp = false;
	UPROPERTY(EditAnywhere, Category = Octave, meta = (EditCondition = "_clamp"))
		float _clampMin = 0;
	UPROPERTY(EditAnywhere, Category = Octave, meta = (EditCondition = "_clamp"))
		float _clampMax = 0;
};

USTRUCT(BlueprintType)
struct FTreeSpawnProperties
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(Category = TreeSpawnProperties, EditAnywhere, BlueprintReadWrite)
		float _spawnPercentPerChunk = 0.01f;
	UPROPERTY(Category = TreeSpawnProperties, EditAnywhere, BlueprintReadWrite)
		int32 _treeLeavesDimensions = 2;
	UPROPERTY(Category = TreeSpawnProperties, EditAnywhere, BlueprintReadWrite)
		int32 _treeTrunkHeightMin = 3;
	UPROPERTY(Category = TreeSpawnProperties, EditAnywhere, BlueprintReadWrite)
		int32 _treeTrunkHeightMax = 6;
	UPROPERTY(Category = TreeSpawnProperties, EditAnywhere, BlueprintReadWrite)
		float _treeRadius = 2.8f;
};

UCLASS()
class FMAGI_API AChunk : public AActor
{
	GENERATED_BODY()

public:
	AChunk();

	void SetSpawnProperties(int32 chunkXIndex, int32 chunkYIndex, FTransform& spawnTransform, FChunkSpawn& chunkSpawnProperties);
	void SetVoxel(FVector localPos, int32 value);

	UFUNCTION(BlueprintNativeEvent)
		void AddInstanceVoxel(FVector instanceLocation);
	virtual void AddInstanceVoxel_Implementation(FVector instanceLocation);

protected:
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadOnly)
		TArray<UMaterialInterface*>  _materials;
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadOnly)
		TArray<FOctaves> _octaves;
	UPROPERTY(Category = Spawn, EditAnywhere, BlueprintReadOnly)
		float _chanceToSpawnGrass = 0.01;
	UPROPERTY(Category = Spawn, EditAnywhere, BlueprintReadOnly)
		float _chanceToSpawnCrystal = 0.1f;
	UPROPERTY(Category = Spawn, EditAnywhere, BlueprintReadOnly)
		FTreeSpawnProperties _treeSpawnProperties;

	UPROPERTY(Category = Voxel, VisibleAnywhere, BlueprintReadOnly)
		int32 _chunkXIndex = 200;
	UPROPERTY(Category = Voxel, VisibleAnywhere, BlueprintReadOnly)
		int32 _chunkYIndex = 200;

private:
	void GenerateChunk();
	void UpdateMesh();
	TArray<int32> CalculateNoise();
	void SetVerticies(int32 xVS, int32 yVS, int32 zVS, TArray<FVector>& verticies, bool posArray[]);
	int32 GetVoxelSizeHalf(int element, bool positiveArray[]);

	int32 _chunkTotalElements;
	int32 _chunkLineElementsP2;
	int32 _voxelSizeHalf;
	TArray<int32> _chunkFields;
	UProceduralMeshComponent* _proceduralMeshComponent;
	FChunkSpawn _csp;
	FRandomStream _randStream;
	void GenerateTrees(TArray<FIntVector>& treeCenters, TArray<int32>& noise);
	bool InRange(int32 value, int32 range) { return (value >= 0 && value < range); }
	UFUNCTION(BlueprintCallable, Category = "Chunk") bool GetCanSpawnCrystal() { return FMath::FRand() < _chanceToSpawnCrystal; }
};



