// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcMeshFromStatic.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
AProcMeshFromStatic::AProcMeshFromStatic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Proc Mesh");
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");

	RootComponent = procMesh;
	baseMesh->SetupAttachment(procMesh);


}
// Called every frame
void AProcMeshFromStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AProcMeshFromStatic::BeginPlay()
{
	Super::BeginPlay();

}
// This function is called when the actor is created in the editor or when it is spawned in the game, and it is used to get the mesh data from the static mesh and update the procedural mesh section with the new data.
void AProcMeshFromStatic::PostActorCreated()
{
	Super::PostActorCreated();
	// getting mesh data for post actor created for procmeshfrom static
	GetMeshData();
}
// This function is called when the actor is loaded from a saved game or when the level is loaded, and it is used to get the mesh data from the static mesh and update the procedural mesh section with the new data.
void AProcMeshFromStatic::PostLoad()
{
	Super::PostLoad();
	// getting data for post load of proc mesh from static
	GetMeshData();
}
// This function gets the mesh data from the static mesh and updates the procedural mesh section with the new data.
void AProcMeshFromStatic::GetMeshData()
{
	// Get the static mesh from the static mesh component, and then get the section data from the static mesh.
	UStaticMesh* Mesh = baseMesh->GetStaticMesh();
	// If the mesh is valid, get the section data from the static mesh and update the procedural mesh section with the new data.
	if (Mesh)
	{
		// Get the section data from the static mesh and update the procedural mesh section with the new data.
		UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(Mesh, 0, 0, Vertices, Triangles, Normals, UV0, Tangents);
		// Convert the vertex colors from FLinearColor to FColor and store them in the UpVertexColors array.
		procMesh->UpdateMeshSection(0, Vertices, Normals, UV0, UpVertexColors, Tangents);
		// create the procedural mesh section with the new data.
		CreateMesh();

	}
}
// This function creates the procedural mesh section with the new data.
void AProcMeshFromStatic::CreateMesh()
{
	if (baseMesh)
	{
		procMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, UpVertexColors, Tangents, true);
	}
	
}



void AProcMeshFromStatic::GetMeshData()
{
	UStaticMesh* mesh = baseMesh->GetStaticMesh();
	if (mesh)
	{
		UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(mesh, 0, 0, Vertices, Triangles, Normals, UV0, Tangents);
		procMesh->UpdateMeshSection(0, Vertices, Normals, UV0, UpVertexColors, Tangents);
		CreateMesh();
	}


}

void AProcMeshFromStatic::CreateMesh()
{
	if (baseMesh)
	{
		procMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, UpVertexColors, Tangents, true);
	}

}
