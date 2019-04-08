#include "Voxel.h"
#include "ProceduralMeshComponent.h"

const int32 bTriangles[] = { 2, 1, 0, 0, 3, 2 };
const FVector2D bUVs[] = { FVector2D(0.000000, 0.000000), FVector2D(0.000000, 1.000000), FVector2D(1.000000, 1.000000), FVector2D(1.000000, 0.000000) };
const FVector bNormals0[] = { FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1) };
const FVector bNormals1[] = { FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1) };
const FVector bNormals2[] = { FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0) };
const FVector bNormals3[] = { FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0) };
const FVector bNormals4[] = { FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0) };
const FVector bNormals5[] = { FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0) };
const FVector bMask[] = { FVector(0.000000, 0.000000, 1.000000), FVector(0.000000, 0.000000, -1.000000),
						FVector(0.000000, 1.000000, 0.000000), FVector(0.000000, -1.000000, 0.000000),
						FVector(1.000000, 0.000000, 0.000000), FVector(-1.000000, 0.000000, 0.000000) };

AVoxel::AVoxel()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AVoxel::BeginPlay()
{
	Super::BeginPlay();

}

void AVoxel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVoxel::OnConstruction(const FTransform & Transform)
{
	_chunkTotalElements = _chunkLineElements * _chunkLineElements * _chunkZElements;
	_chunkLineElementsP2 = _chunkLineElements * _chunkLineElements;
	_voxelSizeHalf = _voxelSize / 2.0f;

	FString str = "Voxel_" + FString::FromInt(_chunkXIndex) + "_" + FString::FromInt(_chunkYIndex);
	FName name = FName(*str);
	_proceduralMeshComponent = NewObject<UProceduralMeshComponent>(this, name);
	_proceduralMeshComponent->RegisterComponent();

	RootComponent = _proceduralMeshComponent;
	RootComponent->SetWorldTransform((Transform));

	Super::OnConstruction(Transform);

	GenerateChunk();
	UpdateMesh();
}

void AVoxel::GenerateChunk()
{
	_chunkFields.SetNumUninitialized(_chunkTotalElements);

	for (int32 x = 0; x < _chunkLineElements; x++)
	{
		for (int32 y = 0; y < _chunkLineElements; y++)
		{
			for (int32 z = 0; z < _chunkZElements; z++)
			{
				int32 index = x + (y * _chunkLineElements) + (z * _chunkLineElementsP2);
				_chunkFields[index] = (z < 30) ? 1 : 0;
			}
		}
	}
}

void AVoxel::UpdateMesh()
{
	TArray<FVector> mVerticies;
	TArray<int32> mTriangles;
	TArray<FVector> mNormals;
	TArray<FVector2D> mUVS;
	TArray<FColor> mVertexColors;
	TArray<FProcMeshTangent> mTangents;
	int32 elementID = 0;

	for (int32 x = 0; x < _chunkLineElements; x++)
	{
		for (int32 y = 0; y < _chunkLineElements; y++)
		{
			for (int32 z = 0; z < _chunkZElements; z++)
			{
				int32 index = x + (_chunkLineElements * y) + (_chunkLineElementsP2 * z);
				int32 meshIndex = _chunkFields[index];
				if (meshIndex > 0)
				{
					meshIndex--;

					int triangleNum = 0;
					for (int i = 0; i < 6; i++)
					{
						int32 newIndex = index + bMask[i].X + (bMask[i].Y * _chunkLineElements) + (bMask[i].Z * _chunkLineElementsP2);
						bool flag = false;
						if (meshIndex >= 20) flag = true;
						else if ((x + bMask[i].X < _chunkLineElements) && (x + bMask[i].X >= 0) && (y + bMask[i].Y < _chunkLineElements) && (y + bMask[i].Y >= 0))
						{
							if (newIndex < _chunkFields.Num() && newIndex >= 0)
								if (_chunkFields[newIndex] < 1) flag = true;
						}
						else flag = true;
						if (flag)
						{
							mTriangles.Add(bTriangles[0] + triangleNum + elementID);
							mTriangles.Add(bTriangles[1] + triangleNum + elementID);
							mTriangles.Add(bTriangles[2] + triangleNum + elementID);
							mTriangles.Add(bTriangles[3] + triangleNum + elementID);
							mTriangles.Add(bTriangles[4] + triangleNum + elementID);
							mTriangles.Add(bTriangles[5] + triangleNum + elementID);
							triangleNum += 4;

							int32 xVS = x * _voxelSize;
							int32 yVS = y * _voxelSize;
							int32 zVS = z * _voxelSize;

							switch (i)
							{
							case 0:
							{
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mNormals.Append(bNormals0, ARRAY_COUNT(bNormals0));
								break;
							}
							case 1:
							{
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mNormals.Append(bNormals1, ARRAY_COUNT(bNormals1));
								break;
							}
							case 2:
							{
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mNormals.Append(bNormals2, ARRAY_COUNT(bNormals2));
								break;
							}
							case 3:
							{
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mNormals.Append(bNormals3, ARRAY_COUNT(bNormals3));
								break;
							}
							case 4:
							{
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mNormals.Append(bNormals5, ARRAY_COUNT(bNormals4));
								break;
							}
							case 5:
							{
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), _voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), -_voxelSizeHalf + (zVS)));
								mVerticies.Add(FVector(-_voxelSizeHalf + (xVS), -_voxelSizeHalf + (yVS), _voxelSizeHalf + (zVS)));
								mNormals.Append(bNormals4, ARRAY_COUNT(bNormals5));
								break;
							}
							}

							mUVS.Append(bUVs, ARRAY_COUNT(bUVs));
							FColor color = FColor(255, 255, 255, i);
							mVertexColors.Add(color); mVertexColors.Add(color); mVertexColors.Add(color); mVertexColors.Add(color);
						}
					}
					elementID += triangleNum;
				}
			}
		}
	}

	_proceduralMeshComponent->ClearAllMeshSections();
	_proceduralMeshComponent->CreateMeshSection(0, mVerticies, mTriangles, mNormals, mUVS, mVertexColors, mTangents, true);
}