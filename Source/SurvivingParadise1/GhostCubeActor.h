#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostCubeActor.generated.h"

UCLASS()
class AGhostCubeActor : public AActor
{
    GENERATED_BODY()

public:
    AGhostCubeActor();

    /** Moves the ghost actor by the specified delta. */
    void MoveGhost(const FVector& Delta);

protected:
    UPROPERTY(VisibleAnywhere, Category="Components")
    USceneComponent* RootScene;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* CubeMesh;
};

