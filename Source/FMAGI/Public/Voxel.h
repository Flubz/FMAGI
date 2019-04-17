#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "ProceduralMeshComponent.h"
#include "Voxel.generated.h"

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
		float _zMult = 1;
	UPROPERTY(Category = Octave, EditAnywhere, BlueprintReadWrite)
		float _weight = 1;
	UPROPERTY(Category = Octave, EditAnywhere, BlueprintReadWrite)
		bool _clamp = false;
	UPROPERTY(EditAnywhere, Category = Octave, meta = (EditCondition = "_clamp"))
		float _clampMin = 0;
	UPROPERTY(EditAnywhere, Category = Octave, meta = (EditCondition = "_clamp"))
		float _clampMax = 0;
};

UCLASS()
class FMAGI_API AVoxel : public AActor
{
	GENERATED_BODY()

public:
	AVoxel();

	void SetSpawnProperties(int32 chunkXIndex, int32 chunkYIndex, FTransform& spawnTransform);
	void SetVoxel(FVector localPos, int32 value);

protected:

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		TArray<UMaterialInterface*>  _materials;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		float _freq = 1;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		int32 _chunkZElements = 80;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		int32 _chunkZMaxHeight = 30;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		bool _collision = false;
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		int32 _randomSeed = 0;
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		int32 _voxelSize = 200;
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		int32 _chunkLineElements = 10;
	UPROPERTY(Category = Voxel, VisibleAnywhere, BlueprintReadWrite)
		int32 _chunkXIndex = 200;
	UPROPERTY(Category = Voxel, VisibleAnywhere, BlueprintReadWrite)
		int32 _chunkYIndex = 200;
	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		TArray<FOctaves> _octaves;

	UFUNCTION(BlueprintImplementableEvent, Category = Noise)
		void GetNoiseValue(float x, float y, float& refNoiseValue);


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
};

