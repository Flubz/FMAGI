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
	GenerateChunk();
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

void AMainPlayerController::GenerateChunk()
{
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator rot = FRotator(0, 0, 0);
	FVector spawnPos = FVector(0, 0, 0);
	FVector2D chunkPos = FVector2D();

	for (int x = -_renderRange; x <= _renderRange; x++)
	{
		for (int y = -_renderRange; y <= _renderRange; y++)
		{
			chunkPos.X = _chunkX + x;
			chunkPos.Y = _chunkY + y;
			if (!_chunksCords.Contains(chunkPos))
			{
				_chunksCords.Add(chunkPos);
				spawnPos = FVector(chunkPos.X * _chunkSize, chunkPos.Y  * _chunkSize, 0);
				AActor* chunk = GetWorld()->SpawnActor<AActor>(_chunk, spawnPos, rot, spawnParams);
				_chunks.Add(chunk);
			}
		}
	}
}
