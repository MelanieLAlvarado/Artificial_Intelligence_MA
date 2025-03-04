// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BCharacter.h"
#include "BPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ABPlayerCharacter : public ABCharacter
{
	GENERATED_BODY()
public:
	ABPlayerCharacter();
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	class UCameraComponent* ViewCam;

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	class USpringArmComponent* CameraBoom;

	FVector GetLookRightDirection() const;

	FVector GetLookForwardDirection() const;

	FVector GetMoveForwardDirection() const;
	/*********************************************/
	/*                Input                      */
	/*********************************************/
private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* GameplayInputMappingContext;

	void HandleLookInput(const struct FInputActionValue& InputActionValue);
	void HandleMoveInput(const struct FInputActionValue& InputActionValue);

};
