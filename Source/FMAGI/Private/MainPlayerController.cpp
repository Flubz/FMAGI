#include "MainPlayerController.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	_chunkSize = _chunkSpawnProperties._chunkLineElements * _chunkSpawnProperties._voxelSize;
	_chunkSizeHalf = _chunkSize / 1;
	_voxelSizeHalf = _chunkSpawnProperties._voxelSize / 2;
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
	FTransform spawnTransform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0));
	FVector2D chunkPos = FVector2D(0, 0);

	for (int x = -_renderRange; x <= _renderRange; x++)
	{
		for (int y = -_renderRange; y <= _renderRange; y++)
		{
			chunkPos.X = _chunkX + x;
			chunkPos.Y = _chunkY + y;
			spawnTransform.SetLocation(FVector(chunkPos.X * _chunkSize, chunkPos.Y * _chunkSize, 0));

			if (!CheckRadius(spawnTransform.GetLocation().X + _chunkSizeHalf, spawnTransform.GetLocation().Y + _chunkSizeHalf)) continue;
			if (_chunksCords.Contains(chunkPos)) continue;

			_chunksCords.Add(chunkPos);

			// AVoxel* chunk = Cast<AVoxel>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, _chunk, spawnTransform));
			AVoxel* chunk = GetWorld()->SpawnActor<AVoxel>(_chunk, spawnTransform);

			if (chunk != nullptr)
			{
				chunk->SetSpawnProperties(chunkPos.X, chunkPos.Y, spawnTransform, _chunkSpawnProperties);
				// UGameplayStatics::FinishSpawningActor(chunk, spawnTransform);
			}

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

void AMainPlayerController::UpdateVoxel(bool isAdding)
{
	if (GetWorld())
	{
		APlayerCameraManager* playerCam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("HitTrace")));
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;
		traceParams.AddIgnoredActor(this->GetPawn());

		FHitResult hit;

		GetWorld()->LineTraceSingleByChannel
		(
			hit,
			playerCam->GetCameraLocation(),
			playerCam->GetCameraLocation() + (playerCam->GetActorForwardVector() * _hitRange),
			ECC_Visibility,
			traceParams
		);


		if (hit.GetActor() != NULL)
		{
			FVector voxelHalfVec = _voxelSizeHalf * FVector::OneVector;

			FVector dir = UKismetMathLibrary::GetDirectionUnitVector(hit.Location, playerCam->GetCameraLocation());
			int32 adding = (UKismetMathLibrary::Conv_BoolToFloat(isAdding) * 2) - 1;
			FVector hitOffset = hit.Location + (dir * adding);
			FVector hitOffsetVoxelHalf = hitOffset + voxelHalfVec;

			FVector hitOffsetChunkSize = hitOffsetVoxelHalf / _chunkSize;
			FVector2D chunkCoords = FVector2D(UKismetMathLibrary::FFloor(hitOffsetChunkSize.X), UKismetMathLibrary::FFloor(hitOffsetChunkSize.Y));

			AVoxel* targetChunk = _chunks[_chunksCords.Find(chunkCoords)];
			FVector voxelPos = ((targetChunk->GetActorLocation() * -1) + hitOffset) + voxelHalfVec;
			targetChunk->SetVoxel(voxelPos, UKismetMathLibrary::Conv_BoolToInt(isAdding));
		}
	}
}