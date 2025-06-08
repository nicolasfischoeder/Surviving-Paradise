#include "GhostCubeActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"

AGhostCubeActor::AGhostCubeActor()
{
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    CubeMesh->SetupAttachment(RootScene);
    CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMeshAsset.Succeeded())
    {
        CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
    }

    // Attempt to use a translucent material to visualize the ghost
    static ConstructorHelpers::FObjectFinder<UMaterial> GhostMat(TEXT("/Engine/EngineMaterials/Widget3DPassThrough.Widget3DPassThrough"));
    if (GhostMat.Succeeded())
    {
        CubeMesh->SetMaterial(0, GhostMat.Object);
        CubeMesh->SetRenderCustomDepth(true);
    }
}

void AGhostCubeActor::MoveGhost(const FVector& Delta)
{
    AddActorWorldOffset(Delta, true);
}

