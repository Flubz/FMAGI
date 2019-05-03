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

	AMainCharacter();

protected:
	void MoveForward(float val);
	void MoveRight(float val);
	void TurnAtRate(float rate);
	void LookUpAtRate(float rate);
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
};

