#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "MapGenerator.generated.h"

UCLASS()
class FMAGI_API AMapGenerator : public AActor
{
	GENERATED_BODY()

public:
	AMapGenerator();

	// Noise

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

protected:
	// Mesh Generation

	UPROPERTY(Category = MeshGeneration, EditAnywhere, BlueprintReadWrite)
		USceneComponent* _thisScene;

	UPROPERTY(Category = MeshGeneration, EditAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* _thisMesh;

	UPROPERTY(Category = MeshGeneration, EditAnywhere, BlueprintReadWrite)
		FVector _cubeRadius = FVector(100, 100, 100);

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	void GenerateMesh();

protected:
	virtual void BeginPlay() override;
	void AddTriangleMesh(FVector topRight_, FVector bottomRight_, FVector bottomLeft_, int32& triIndex_, FProcMeshTangent tangent_);
	void AddQuadMesh(FVector topRight_, FVector bottomRight_, FVector topLeft_, FVector bottomLeft_, int32& triIndex_, FProcMeshTangent tangent_);

private:
	class MeshProperties
	{
		TArray<FVector>_verticies;
		TArray<int32>_triangles;
		TArray<FVector>_normals;
		TArray<FProcMeshTangent>_tangents;
		TArray<FVector2D>_uvs;
		TArray<FLinearColor>_colors;

	public:
		MeshProperties()
		{
			
		}

		void Reset()
		{
			_verticies.Reset();
			_triangles.Reset();
			_normals.Reset();
			_tangents.Reset();
			_uvs.Reset();
			_colors.Reset();
		}

		void AddVertex(FVector pos1_, FVector pos2_, FVector pos3_)
		{
			_verticies.Add(pos1_);
			_verticies.Add(pos2_);
			_verticies.Add(pos3_);
		}

		void AddVertex(FVector pos1_, FVector pos2_, FVector pos3_, FVector pos4_)
		{
			_verticies.Add(pos1_);
			_verticies.Add(pos2_);
			_verticies.Add(pos3_);
			_verticies.Add(pos4_);
		}

		void AddTriangle(int32 pos1_, int32 pos2_, int32 pos3_)
		{
			_triangles.Add(pos1_);
			_triangles.Add(pos2_);
			_triangles.Add(pos3_);
		}

		void SetVertexProperties(FVector normal_, FProcMeshTangent tangent_, FLinearColor colors_)
		{
			_normals.Add(normal_);
			_tangents.Add(tangent_);
			_colors.Add(colors_);
		}

		void AddUVs(FVector2D pos1_, FVector2D pos2_, FVector2D pos3_)
		{
			_uvs.Add(pos1_);
			_uvs.Add(pos2_);
			_uvs.Add(pos3_);
		}

		void AddUVs(FVector2D pos1_, FVector2D pos2_, FVector2D pos3_, FVector2D pos4_)
		{
			_uvs.Add(pos1_);
			_uvs.Add(pos2_);
			_uvs.Add(pos3_);
			_uvs.Add(pos4_);
		}

		void CreateMesh(int32 sectionIndex_, UProceduralMeshComponent* meshComponent_, bool collision_)
		{
			meshComponent_->CreateMeshSection_LinearColor(
				sectionIndex_, _verticies,
				_triangles, _normals,
				_uvs, _colors,
				_tangents, collision_);
		}
	};

	MeshProperties* _meshProperties;
};
