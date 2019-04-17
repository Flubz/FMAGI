#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;

UCLASS(config = Game)
class FMAGI_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* _cameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float _baseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float _baseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float _movementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		FRotator _armStartRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		FRotator _armEndRotation;

	UFUNCTION(BlueprintCallable)
		void SetupArm(USceneComponent* arm)
	{
		_armStartRotation = arm->GetComponentRotation();
		_arm = arm;
	};

	USceneComponent* _arm;
	AMainCharacter();


protected:
	UFUNCTION(BlueprintImplementableEvent)
		void StartFiring();
	UFUNCTION(BlueprintImplementableEvent)
		void StopFiring();

	void MoveForward(float val);
	void MoveRight(float val);
	void TurnAtRate(float rate);
	void LookUpAtRate(float rate);
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return _cameraComponent; }

};

