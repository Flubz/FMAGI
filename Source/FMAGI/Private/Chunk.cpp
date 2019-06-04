#include "Chunk.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NoExportTypes.h"
#include "SimplexNoiseBPLibrary.h"

const int32 bTriangles[] = { 2, 1, 0, 0, 3, 2 };
const FVector2D bUVs[] = { FVector2D(0.000000, 0.000000), FVector2D(0.000000, 1.000000), FVector2D(1.000000, 1.000000), FVector2D(1.000000, 0.000000) };
const FVector bNormals0[] = { FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1) };
const FVector bNormals1[] = { FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1) };
const FVector bNormals2[] = { FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0) };
const FVector bNormals3[] = { FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0) };
const FVector bNormals4[] = { FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0) };
const FVector bNormals5[] = { FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0) };
const FVector bMask[] = { FVector(0.000000, 0.000000, 1.000000), FVector(0.000000, 0.000000, -1.000000), FVector(0.000000, 1.000000, 0.000000), FVector(0.000000, -1.000000, 0.000000), FVector(1.000000, 0.000000, 0.000000), FVector(-1.000000, 0.000000, 0.000000) };

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AChunk::SetSpawnProperties(int32 chunkXIndex, int32 chunkYIndex, FTransform& spawnTransform, FChunkSpawn& chunkSpawnProperties)
{
	_chunkXIndex = chunkXIndex;
	_chunkYIndex = chunkYIndex;

	_csp = chunkSpawnProperties;

	_chunkLineElementsP2 = _csp._chunkLineElements * _csp._chunkLineElements;
	_chunkTotalElements = _chunkLineElementsP2 * _csp._chunkZElements;
	_voxelSizeHalf = _csp._voxelSize / 2;

	FString str = "Voxel_" + FString::FromInt(_chunkXIndex) + "_" + FString::FromInt(_chunkYIndex);
	FName name = FName(*str);
	_proceduralMeshComponent = NewObject<UProceduralMeshComponent>(this, name);
	_proceduralMeshComponent->RegisterComponent();

	RootComponent = _proceduralMeshComponent;
	RootComponent->SetWorldTransform((spawnTransform));

	GenerateChunk();
	UpdateMesh();
}

void AChunk::GenerateChunk()
{
	_chunkFields.SetNumUninitialized(_chunkTotalElements);
	TArray<int32> noise = CalculateNoise();
	_randStream = FRandomStream(_csp._randomSeed);

	TArray<FIntVector> treeCenters;

	for (int32 x = 0; x < _csp._chunkLineElements; x++)
	{
		for (int32 y = 0; y < _csp._chunkLineElements; y++)
		{
			for (int32 z = 0; z < _csp._chunkZElements; z++)
			{
				int32 noiseIndex = x + (y * _csp._chunkLineElements);
				int32 chunkIndex = noiseIndex + (z * _chunkLineElementsP2);
				int32 randChunkDepth = UKismetMathLibrary::RandomIntegerInRange(0, 2);

				if (z == (_csp._chunkZMaxHeight) + noise[noiseIndex])
				{
					_chunkFields[chunkIndex] = 11;
				}
				else if (z == (_csp._chunkZMaxHeight - randChunkDepth) + noise[noiseIndex])
				{
					_chunkFields[chunkIndex] = 12;
				}
				else if (z < (_csp._chunkZMaxHeight - randChunkDepth) + noise[noiseIndex])
				{
					_chunkFields[chunkIndex] = 13;
				}
				else
				{
					_chunkFields[chunkIndex] = 0;
				}
			}
		}
	}
	if (_csp._generateTrees) GenerateTrees(treeCenters, noise);
}

void AChunk::GenerateTrees(TArray<FIntVector>& treeCenters, TArray<int32>& noise)
{
	int32 treeX = _treeSpawnProperties._treeLeavesDimensions;
	int32 treeY = _treeSpawnProperties._treeLeavesDimensions;
	int32 treeZ = _treeSpawnProperties._treeLeavesDimensions;

	for (int x = 2; x < _csp._chunkLineElements - 2; x++)
	{
		for (int y = 2; y < _csp._chunkLineElements - 2; y++)
		{
			for (int z = 0; z < _csp._chunkZElements; z++)
			{
				int32 noiseIndex = x + (y * _csp._chunkLineElements);
				int32 chunkIndex = noiseIndex + (z * _chunkLineElementsP2);

				if (_randStream.FRand() < _chanceToSpawnGrass
					&& z == (_csp._chunkZMaxHeight + 1) + noise[noiseIndex])
				{
					_chunkFields[chunkIndex] = -1;
				}

				if (_randStream.FRand() < _treeSpawnProperties._spawnPercentPerChunk
					&& z == (_csp._chunkZMaxHeight + 1) + noise[noiseIndex])
				{
					treeCenters.Add(FIntVector(x, y, z));
				}
			}
		}
	}

	for (FIntVector treeCenter : treeCenters)
	{
		int32 randX = FMath::RandRange(0, 2);
		int32 randY = FMath::RandRange(0, 2);
		int32 randZ = FMath::RandRange(0, 2);
		int32 randHeight = FMath::RandRange
		(_treeSpawnProperties._treeTrunkHeightMin, _treeSpawnProperties._treeTrunkHeightMax);

		for (int32 x = -treeX; x < treeX + 1; x++)
		{
			for (int32 y = -treeY; y < treeY + 1; y++)
			{
				for (int32 z = -treeZ; z < treeZ + 1; z++)
				{
					if (InRange(x + treeCenter.X, _csp._chunkLineElements) &&
						InRange(y + treeCenter.Y, _csp._chunkLineElements) &&
						InRange(z + treeCenter.Z, _csp._chunkZElements))
					{
						float radius = FVector(x * randX, y * randY, z * randZ).Size();
						if (radius <= _treeSpawnProperties._treeRadius)
						{
							if (FMath::FRand() < 0.5 || radius <= 1.4)
							{
								_chunkFields[treeCenter.X + x +
									(_csp._chunkLineElements * (treeCenter.Y + y)) +
									(_chunkLineElementsP2 * (treeCenter.Z + z + randHeight))] = 1;
							}
						}
					}
				}
			}
		}

		for (int32 h = 0; h < randHeight; h++)
		{
			_chunkFields[treeCenter.X + (treeCenter.Y * _csp._chunkLineElements) + ((treeCenter.Z + h) * _chunkLineElementsP2)] = 14;
		}
	}
}

TArray<int32> AChunk::CalculateNoise()
{
	TArray<int32> noiseArray;
	noiseArray.SetNum(_chunkLineElementsP2);

	float xNoiseMult = 0.0f;
	float yNoiseMult = 0.0f;
	int32 val = 0.0f;

	for (int x = 0; x < _csp._chunkLineElements; x++)
	{
		for (int y = 0; y < _csp._chunkLineElements; y++)
		{
			float cumulativeNoiseValue = 0.0f;

			for (int o = 0; o < _octaves.Num(); o++)
			{
				if (_octaves[o]._skip) continue;
				float noiseValue = 0.0f;
				xNoiseMult = (((_chunkXIndex + _csp._randomSeed) * _csp._chunkLineElements) + x) * _octaves[o]._xMult;
				yNoiseMult = (((_chunkYIndex + _csp._randomSeed) * _csp._chunkLineElements) + y) * _octaves[o]._yMult;
				noiseValue = USimplexNoiseBPLibrary::SimplexNoise2D(xNoiseMult, yNoiseMult);
				noiseValue *= _octaves[o]._weight;
				if (_octaves[o]._clamp)
				{
					noiseValue = FMath::Clamp(noiseValue, _octaves[o]._clampMin, _octaves[o]._clampMax);
				}
				cumulativeNoiseValue += noiseValue;
			}

			val = FMath::FloorToInt((cumulativeNoiseValue));
			noiseArray[(y * _csp._chunkLineElements) + x] = val;
		}
	}
	return noiseArray;
}

void AChunk::SetVerticies(int32 xVS, int32 yVS, int32 zVS, TArray<FVector>& verticies, bool posArray[])
{
	verticies.Add(FVector(GetVoxelSizeHalf(0, posArray) + (xVS), GetVoxelSizeHalf(1, posArray) + (yVS), GetVoxelSizeHalf(2, posArray) + (zVS)));
	verticies.Add(FVector(GetVoxelSizeHalf(3, posArray) + (xVS), GetVoxelSizeHalf(4, posArray) + (yVS), GetVoxelSizeHalf(5, posArray) + (zVS)));
	verticies.Add(FVector(GetVoxelSizeHalf(6, posArray) + (xVS), GetVoxelSizeHalf(7, posArray) + (yVS), GetVoxelSizeHalf(8, posArray) + (zVS)));
	verticies.Add(FVector(GetVoxelSizeHalf(9, posArray) + (xVS), GetVoxelSizeHalf(10, posArray) + (yVS), GetVoxelSizeHalf(11, posArray) + (zVS)));
}

int32 AChunk::GetVoxelSizeHalf(int element, bool positiveArray[])
{
	return positiveArray[element] ? _voxelSizeHalf : -_voxelSizeHalf;
}

void AChunk::SetVoxel(FVector localPos, int32 value)
{
	int32 x = localPos.X / _csp._voxelSize;
	int32 y = localPos.Y / _csp._voxelSize;
	int32 z = localPos.Z / _csp._voxelSize;
	int32 index = x + (y * _csp._chunkLineElements) + (z * _chunkLineElementsP2);

	_chunkFields[index] = value;

	UpdateMesh();
}

void AChunk::UpdateMesh()
{
	TArray<FMeshSection> _meshSections;
	_meshSections.SetNum(_materials.Num());

	int32 elementNumber = 0;

	for (int32 x = 0; x < _csp._chunkLineElements; x++)
	{
		for (int32 y = 0; y < _csp._chunkLineElements; y++)
		{
			for (int32 z = 0; z < _csp._chunkZElements; z++)
			{
				int32 index = x + (_csp._chunkLineElements * y) + (_chunkLineElementsP2 * z);
				int32 meshIndex = _chunkFields[index];

				if (meshIndex > 0)
				{
					meshIndex--;

					TArray<FVector>& mVerticies = _meshSections[meshIndex].Verticies;
					TArray<int32>& mTriangles = _meshSections[meshIndex].Triangles;
					TArray<FVector>& mNormals = _meshSections[meshIndex].Normals;
					TArray<FVector2D>& mUVS = _meshSections[meshIndex].UVS;
					TArray<FColor>& mVertexColors = _meshSections[meshIndex].VertexColors;
					TArray<FProcMeshTangent>& mTangents = _meshSections[meshIndex].Tangents;
					int32 elementID = _meshSections[meshIndex].elementID;

					int triangleNum = 0;
					for (int i = 0; i < 6; i++)
					{
						int newIndex = index + bMask[i].X + (bMask[i].Y * _csp._chunkLineElements) + (bMask[i].Z * _chunkLineElementsP2);
						bool flag = false;

						if (meshIndex >= 20) flag = true;
						else if ((x + bMask[i].X < _csp._chunkLineElements) && (x + bMask[i].X >= 0) && (y + bMask[i].Y < _csp._chunkLineElements) && (y + bMask[i].Y >= 0))
						{
							if (newIndex < _chunkFields.Num() && newIndex >= 0)
								if (_chunkFields[newIndex] < 10) flag = true;
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

							int32 xVS = x * _csp._voxelSize;
							int32 yVS = y * _csp._voxelSize;
							int32 zVS = z * _csp._voxelSize;

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
					elementNumber += triangleNum;
					_meshSections[meshIndex].elementID += triangleNum;
				}
				else if (_csp._generateGrass && meshIndex == -1)
				{
					AddInstanceVoxel(FVector(x * _csp._voxelSize, y * _csp._voxelSize, (z * _csp._voxelSize) - _voxelSizeHalf));
				}
			}
		}
	}

	_proceduralMeshComponent->ClearAllMeshSections();

	for (int i = 0; i < _meshSections.Num(); i++)
	{
		if (_meshSections[i].Verticies.Num() > 0)
			_proceduralMeshComponent->CreateMeshSection(i, _meshSections[i].Verticies,
				_meshSections[i].Triangles, _meshSections[i].Normals, _meshSections[i].UVS,
				_meshSections[i].VertexColors, _meshSections[i].Tangents, _csp._voxelCollision);
	}

	for (int i = 0; i < _materials.Num(); i++)
	{
		_proceduralMeshComponent->SetMaterial(i, _materials[i]);
	}
}

void AChunk::AddInstanceVoxel_Implementation(FVector instanceLocation)
{

}