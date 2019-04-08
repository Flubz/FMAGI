#include "MainPlayerController.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	_chunkSize = _chunkLineElement * _voxelSize;
	_chunkSizeHalf = _chunkSize / 1;
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePosition();
	RemoveChunk();
	AddChunk();
}

bool AMainPlayerController::UpdatePosition()
{
	FVector charPos = GetPawn()->GetActorLocation() * FVector(1, 1, 0);
	_characterPosition = charPos;

	FVector2D flooredPos = FVector2D(
		UKismetMathLibrary::FFloor(charPos.X / (float) _chunkSizeHalf),
		UKismetMathLibrary::FFloor(charPos.Y / (float) _chunkSizeHalf));

	if (flooredPos.X != _chunkX || flooredPos.Y != _chunkY)
	{
		_chunkX = flooredPos.X;
		_chunkY = flooredPos.Y;
		return true;
	}

	return false;
}

void AMainPlayerController::AddChunk()
{
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	FVector spawnPos = FVector(0, 0, 0);
	FVector2D chunkPos = FVector2D(0, 0);

	for (int x = -_renderRange; x <= _renderRange; x++)
	{
		for (int y = -_renderRange; y <= _renderRange; y++)
		{
			chunkPos.X = _chunkX + x;
			chunkPos.Y = _chunkY + y;
			spawnPos = FVector(chunkPos.X * _chunkSize, chunkPos.Y * _chunkSize, 0);

			if (!CheckRadius(spawnPos.X + _chunkSizeHalf, spawnPos.Y + _chunkSizeHalf)) continue;
			if (_chunksCords.Contains(chunkPos)) continue;

			_chunksCords.Add(chunkPos);
			AVoxel* chunk = GetWorld()->SpawnActor<AVoxel>(_chunk, spawnPos, FRotator(0, 0, 0), spawnParams);
			chunk->SetSpawnProperties(chunkPos.X, chunkPos.Y);
			_chunks.Add(chunk);
		}
	}
}

void AMainPlayerController::RemoveChunk()
{
	for (int i = 0; i < _chunksCords.Num(); i++)
	{
		float x = (_chunksCords[i].X * _chunkSize + _chunkSizeHalf);
		float y = (_chunksCords[i].Y * _chunkSize + _chunkSizeHalf);
		if (CheckRadius(x, y)) continue;
		_chunksCords.RemoveAt(i);
		GetWorld()->DestroyActor(_chunks[i]);
		_chunks.RemoveAt(i);
	}
}

bool AMainPlayerController::CheckRadius(float x_, float y_)
{
	FVector vec = ((FVector(x_, y_, 0)) - _characterPosition);
	return (vec.Size() < ((float) _chunkSize * (float) _renderRange));
}
