// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivingParadise1Character.h"
#include "SurvivingParadise1Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "SimpleCubeActor.h"
#include "GhostCubeActor.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASurvivingParadise1Character

ASurvivingParadise1Character::ASurvivingParadise1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
    Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


}

//////////////////////////////////////////////////////////////////////////// Input

void ASurvivingParadise1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
                if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
                {
                        Subsystem->AddMappingContext(DefaultMappingContext, 0);
                        if (SpawnCubeMappingContext)
                        {
                                Subsystem->AddMappingContext(SpawnCubeMappingContext, 0);
                        }
                }
	}
}

void ASurvivingParadise1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivingParadise1Character::Move);

                // Looking
                EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivingParadise1Character::Look);

                // Spawn cube if action is set
                if (SpawnCubeAction)
                {
                        EnhancedInputComponent->BindAction(SpawnCubeAction, ETriggerEvent::Started, this, &ASurvivingParadise1Character::SpawnCube);
                }

                if (StartPlacementAction)
                {
                        EnhancedInputComponent->BindAction(StartPlacementAction, ETriggerEvent::Started, this, &ASurvivingParadise1Character::StartPlacement);
                }

                if (ConfirmPlacementAction)
                {
                        EnhancedInputComponent->BindAction(ConfirmPlacementAction, ETriggerEvent::Started, this, &ASurvivingParadise1Character::ConfirmPlacement);
                }

                if (RotatePlacementAction)
                {
                        EnhancedInputComponent->BindAction(RotatePlacementAction, ETriggerEvent::Started, this, &ASurvivingParadise1Character::RotatePlacement);
                }
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ASurvivingParadise1Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASurvivingParadise1Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASurvivingParadise1Character::SpawnCube(const FInputActionValue& Value)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 200.f;
    World->SpawnActor<ASimpleCubeActor>(SpawnLocation, FRotator::ZeroRotator);
}

void ASurvivingParadise1Character::StartPlacement(const FInputActionValue& Value)
{
    if (PlacementGhost)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    const FVector SpawnLocation = FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * PlacementDistance;
    PlacementGhost = World->SpawnActor<AGhostCubeActor>(SpawnLocation, FRotator::ZeroRotator);
    UpdatePlacementGhost();
}

void ASurvivingParadise1Character::RotatePlacement(const FInputActionValue& Value)
{
    if (!PlacementGhost)
    {
        return;
    }

    PlacementGhost->AddActorWorldRotation(FRotator(0.f, RotationStep, 0.f));
}

void ASurvivingParadise1Character::ConfirmPlacement(const FInputActionValue& Value)
{
    if (!PlacementGhost)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (World)
    {
        World->SpawnActor<ASimpleCubeActor>(PlacementGhost->GetActorLocation(), PlacementGhost->GetActorRotation());
    }

    PlacementGhost->Destroy();
    PlacementGhost = nullptr;
}

void ASurvivingParadise1Character::UpdatePlacementGhost()
{
    if (!PlacementGhost)
    {
        return;
    }

    const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
    const FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * PlacementDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(TEXT("PlacementTrace"), false, this);
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        PlacementGhost->SetActorLocation(Hit.Location);
    }
    else
    {
        PlacementGhost->SetActorLocation(End);
    }
}

void ASurvivingParadise1Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdatePlacementGhost();
}
