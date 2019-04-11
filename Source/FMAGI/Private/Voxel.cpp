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
	PrimaryActorTick.bCanEverTick = false;

}

void AVoxel::SetSpawnProperties(int32 chunkXIndex, int32 chunkYIndex, FTransform& spawnTransform)
{
	_chunkXIndex = chunkXIndex;
	_chunkYIndex = chunkYIndex;

	_chunkLineElementsP2 = _chunkLineElements * _chunkLineElements;
	_chunkTotalElements = _chunkLineElementsP2 * _chunkZElements;
	_voxelSizeHalf = _voxelSize / 2;

	FString str = "Voxel_" + FString::FromInt(_chunkXIndex) + "_" + FString::FromInt(_chunkYIndex);
	FName name = FName(*str);
	SetActorLabel(str);
	_proceduralMeshComponent = NewObject<UProceduralMeshComponent>(this, name);
	_proceduralMeshComponent->RegisterComponent();

	RootComponent = _proceduralMeshComponent;
	RootComponent->SetWorldTransform((spawnTransform));

	GenerateChunk();
	UpdateMesh();
}

void AVoxel::GenerateChunk()
{
	_chunkFields.SetNumUninitialized(_chunkTotalElements);

	TArray<int32> noise = CalculateNoise();

	for (int32 x = 0; x < _chunkLineElements; x++)
	{
		for (int32 y = 0; y < _chunkLineElements; y++)
		{
			for (int32 z = 0; z < _chunkZElements; z++)
			{
				int32 index = x + (y * _chunkLineElements) + (z * _chunkLineElementsP2);
				_chunkFields[index] = z < _chunkZMaxHeight + noise[(y * _chunkLineElements) + x] ? 1 : 0;
			}
		}
	}
}

TArray<int32> AVoxel::CalculateNoise()
{
	TArray<int32> noiseArray;
	noiseArray.SetNum(_chunkLineElementsP2);

	float cumulativeNoiseValue = 0.0f;
	float xNoiseMult = 0.0f;
	float yNoiseMult = 0.0f;
	int32 val = 0.0f;

	for (int x = 0; x < _chunkLineElements; x++)
	{
		for (int y = 0; y < _chunkLineElements; y++)
		{
			for (int o = 0; o < _octaves.Num(); o++)
			{
				float noiseValue = 0.0f;
				xNoiseMult = ((_chunkXIndex * _chunkLineElements) + x) * _octaves[o]._xMult;
				yNoiseMult = ((_chunkYIndex * _chunkLineElements) + y) * _octaves[o]._yMult;
				GetNoiseValue(xNoiseMult, yNoiseMult, noiseValue);
				noiseValue *= _octaves[o]._weight;
				if (_octaves[o]._clamp)
				{
					noiseValue = FMath::Clamp(noiseValue, _octaves[o]._clampMin, _octaves[o]._clampMax);
				}
				cumulativeNoiseValue += noiseValue;
			}

			val = FMath::FloorToInt((cumulativeNoiseValue));
			noiseArray[(y * _chunkLineElements) + x] = val;
		}
	}
	return noiseArray;
}

void AVoxel::SetVerticies(int32 xVS, int32 yVS, int32 zVS, TArray<FVector>& verticies, bool posArray[])
{
	verticies.Add(FVector(GetVoxelSizeHalf(0, posArray) + (xVS), GetVoxelSizeHalf(1, posArray) + (yVS), GetVoxelSizeHalf(2, posArray) + (zVS)));
	verticies.Add(FVector(GetVoxelSizeHalf(3, posArray) + (xVS), GetVoxelSizeHalf(4, posArray) + (yVS), GetVoxelSizeHalf(5, posArray) + (zVS)));
	verticies.Add(FVector(GetVoxelSizeHalf(6, posArray) + (xVS), GetVoxelSizeHalf(7, posArray) + (yVS), GetVoxelSizeHalf(8, posArray) + (zVS)));
	verticies.Add(FVector(GetVoxelSizeHalf(9, posArray) + (xVS), GetVoxelSizeHalf(10, posArray) + (yVS), GetVoxelSizeHalf(11, posArray) + (zVS)));
}

int32 AVoxel::GetVoxelSizeHalf(int element, bool positiveArray[])
{
	return positiveArray[element] ? _voxelSizeHalf : -_voxelSizeHalf;
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
								bool posArray[12] =
								{
									false, true, true,
									false, false, true,
									true, false, true,
									true, true, true
								};
								SetVerticies(xVS, yVS, zVS, mVerticies, posArray);
								mNormals.Append(bNormals0, ARRAY_COUNT(bNormals0));
								break;
							}
							case 1:
							{
								bool posArray[12] =
								{
									true, false, false,
									false, false, false,
									false, true, false,
									true, true, false
								};
								SetVerticies(xVS, yVS, zVS, mVerticies, posArray);
								mNormals.Append(bNormals1, ARRAY_COUNT(bNormals1));
								break;
							}
							case 2:
							{
								bool posArray[12] =
								{
									true, true, true,
									true, true, false,
									false, true, false,
									false, true, true
								};
								SetVerticies(xVS, yVS, zVS, mVerticies, posArray);
								mNormals.Append(bNormals2, ARRAY_COUNT(bNormals2));
								break;
							}
							case 3:
							{
								bool posArray[12] =
								{
									false, false, true,
									false, false, false,
									true, false, false,
									true, false, true
								};
								SetVerticies(xVS, yVS, zVS, mVerticies, posArray);
								mNormals.Append(bNormals3, ARRAY_COUNT(bNormals3));
								break;
							}
							case 4:
							{
								bool posArray[12] =
								{
									true, false, true,
									true, false, false,
									true, true, false,
									true, true, true
								};
								SetVerticies(xVS, yVS, zVS, mVerticies, posArray);
								mNormals.Append(bNormals5, ARRAY_COUNT(bNormals4));
								break;
							}
							case 5:
							{
								bool posArray[12] =
								{
									false, true, true,
									false, true, false,
									false, false, false,
									false, false, true
								};
								SetVerticies(xVS, yVS, zVS, mVerticies, posArray);
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
	_proceduralMeshComponent->CreateMeshSection(0, mVerticies, mTriangles, mNormals, mUVS, mVertexColors, mTangents, _collision);
}
