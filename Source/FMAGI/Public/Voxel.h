#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "ProceduralMeshComponent.h"
#include "Voxel.generated.h"

UCLASS()
class FMAGI_API AVoxel : public AActor
{
	GENERATED_BODY()

public:
	AVoxel();

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		TArray<UMaterialInterface*>  _materials;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		float _xMult = 1;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		float _yMult = 1;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		float _zMult = 1;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		float _weight = 1;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		float _freq = 1;

	UPROPERTY(Category = Voxel, EditAnywhere, BlueprintReadWrite)
		int32 _chunkZElements = 80;

	UPROPERTY()
		int32 _chunkTotalElements;

	UPROPERTY()
		int32 _chunkLineElementsP2;

	UPROPERTY()
		int32 _voxelSizeHalf;

	UPROPERTY()
		TArray<int32> _chunkFields;

	UPROPERTY()
		UProceduralMeshComponent* _proceduralMeshComponent;

	void SetSpawnProperties(int32 chunkXIndex, int32 chunkYIndex)
	{
		// int32 randomSeed, int32 voxelSize, int32 voxelSizeHalf, int32 chunkLineElements,
		// _randomSeed = randomSeed;
		// _voxelSize = voxelSize;
		// _chunkLineElements = chunkLineElements;
		_chunkXIndex = chunkXIndex;
		_chunkYIndex = chunkYIndex;
	}

protected:
	virtual void BeginPlay() override;

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

	UFUNCTION(BlueprintNativeEvent)
		TArray<int32> CalculateNoise();
	virtual TArray<int32> CalculateNoise_Imlpementation();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	void GenerateChunk();
	void UpdateMesh();
};
