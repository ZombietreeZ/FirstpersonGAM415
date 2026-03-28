// Fill out your copyright notice in the Description page of Project Settings.


#include "ProdPlain.h"
#include "ProceduralMeshComponent.h"


// Sets default values
AProdPlain::AProdPlain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Create a procedural mesh component and set it as the root component
	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Proc Mesh");
}

// Called when the game starts or when spawned
void AProdPlain::BeginPlay()
{
	Super::BeginPlay();
}
// Called when the actor is created in the level or when it is loaded into the level, this function is used to generate the procedural mesh and set its material
void AProdPlain::PostActorCreated()
{
	Super::PostActorCreated();
	// Call the CreateMesh function to generate the procedural mesh when the actor is created in the level
	CreateMesh();

	if (PlainMat)
	{
		// Set the material of the procedural mesh to the material defined in the PlainMat variable, which will be used to texture the mesh
		procMesh->SetMaterial(0, PlainMat);
		// Set the procedural mesh as the root component of the actor
		RootComponent = procMesh;

	}
}
// Called after the actor is loaded into the level, this function is used to generate the procedural mesh and set its material
void AProdPlain::PostLoad()
{
	Super::PostLoad();
	// Call the CreateMesh function to generate the procedural mesh when the actor is loaded into the level
	CreateMesh();
}

// Called every frame
void AProdPlain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Function to create the procedural mesh using the vertices, triangles, UVs, and material defined in the arrays above
void AProdPlain::CreateMesh()
{
	// Clear any existing mesh data from the procedural mesh component to ensure that we are starting with a clean slate when generating the new mesh
	procMesh->CreateMeshSection(0, Vertices, Triangles, TArray < FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);

}

