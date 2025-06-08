// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SurvivingParadise1Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class ASimpleCubeActor;
class AGhostCubeActor;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASurvivingParadise1Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
        UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* LookAction;

       /** Action to spawn a cube */
       UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
       UInputAction* SpawnCubeAction;

       /** Mapping context for spawning cube */
       UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
       UInputMappingContext* SpawnCubeMappingContext;

       /** Action to begin ghost placement */
       UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
       UInputAction* StartPlacementAction;

       /** Action to confirm ghost placement */
       UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
       UInputAction* ConfirmPlacementAction;

       /** Action to rotate the ghost */
       UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
       UInputAction* RotatePlacementAction;

       /** Currently spawned placement ghost */
       UPROPERTY()
       AGhostCubeActor* PlacementGhost;

       /** Distance to spawn ghost in front of player */
       UPROPERTY(EditAnywhere, Category = Placement)
       float PlacementDistance = 200.f;

       /** Degrees to rotate the ghost on each input */
       UPROPERTY(EditAnywhere, Category = Placement)
       float RotationStep = 90.f;
	
public:
	ASurvivingParadise1Character();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
        void Look(const FInputActionValue& Value);

        /** Spawns a cube actor */
        void SpawnCube(const FInputActionValue& Value);

       /** Begin placement by spawning a ghost */
       void StartPlacement(const FInputActionValue& Value);

       /** Rotate the placement ghost */
       void RotatePlacement(const FInputActionValue& Value);

       /** Confirm placement and spawn final cube */
       void ConfirmPlacement(const FInputActionValue& Value);

protected:
       // APawn interface
       virtual void NotifyControllerChanged() override;
       virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
       // End of APawn interface

       /** Updates the ghost to follow the crosshair */
       void UpdatePlacementGhost();

       virtual void Tick(float DeltaTime) override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

