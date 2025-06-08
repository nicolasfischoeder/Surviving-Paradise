#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleCubeActor.generated.h"

UCLASS()
class ASimpleCubeActor : public AActor
{
    GENERATED_BODY()

public:
    ASimpleCubeActor();

protected:
    UPROPERTY(VisibleAnywhere, Category="Components")
    USceneComponent* RootScene;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* CubeMesh;
};

