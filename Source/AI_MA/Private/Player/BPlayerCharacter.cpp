// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABPlayerCharacter::ABPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 1000.f;

	bUseControllerRotationYaw = false;

	ViewCam = CreateDefaultSubobject<UCameraComponent>("View Cam");
	ViewCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1200.f);
}

void ABPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	APlayerController* OwningPlayerController = GetController<APlayerController>();
	if (OwningPlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem =
			OwningPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		EnhancedSubsystem->RemoveMappingContext(GameplayInputMappingContext);
		EnhancedSubsystem->AddMappingContext(GameplayInputMappingContext, 0);
	}
}

void ABPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleLookInput);
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleMoveInput);
	}
}

void ABPlayerCharacter::HandleLookInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(-InputValue.Y);
}

void ABPlayerCharacter::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	if (InputValue.Length() < 0)
	{
		return;
	}
	InputValue.Normalize();

	AddMovementInput(GetLookRightDirection() * InputValue.X + GetMoveForwardDirection() * InputValue.Y);
}

FVector ABPlayerCharacter::GetLookRightDirection() const
{
	return ViewCam->GetRightVector();
}

FVector ABPlayerCharacter::GetLookForwardDirection() const
{
	return ViewCam->GetForwardVector();
}

FVector ABPlayerCharacter::GetMoveForwardDirection() const
{
	return FVector::CrossProduct(GetLookRightDirection(), FVector::UpVector);
}