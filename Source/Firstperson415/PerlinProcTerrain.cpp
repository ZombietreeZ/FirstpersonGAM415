// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinProcTerrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
APerlinProcTerrain::APerlinProcTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh");
	ProcMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay();
	// creating vertices
	CreateVertices();
	// makeing triagles
	CreateTriangles();
	// making a sectiong to store every thing in the proc mesh
	ProcMesh->CreateMeshSection(sectionID, Vertices, Triangles, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>(), true);
	// setting the material to the Proc mesh 
	ProcMesh->SetMaterial(0, Mat);
}

// Called every frame
void APerlinProcTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// altering terrain mesh by taking in a impact point and altering the vertices that are within the radius of the impact point by moving them down by the depth and then updating the mesh section to show the change
void APerlinProcTerrain::AlterMesh(FVector impactPoint)
{
	// for and integer that equals 0 is less than the vertice number amout add to integer
	for (int i = 0; i < Vertices.Num(); i++)
	{
		// a temp dig point that is the impacted point minus the location of the actor to get the local space of the impact point
		FVector tempVector = impactPoint - this->GetActorLocation();

		// if the vert index is less than the radious then move the vert down by the depth and update the mesh section to show the change
		if (FVector(Vertices[i] - tempVector).Size() < radius)
		{
			Vertices[i] = Vertices[i] - Depth;
			// updating vertices to show the change in the mesh section
			ProcMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>());
		}
	}

}
// creating the vertices for the procedural terrain by using perlin noise to create a height map and then adding the vertices to the Vertices array and the UVs to the UV0 array
void APerlinProcTerrain::CreateVertices()
{
	for (int X = 0; X <= XSize; X++)
	{
		for (int Y = 0; Y <= YSize; Y++)
		{
			// Z calculation using perlin noise method to create a height map for the terrain and then multiplying it by the Z multiplier to get the final height of the vertex
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + 0.1, Y * NoiseScale + 0.1)) * ZMultiplier;
			
			// adding the vertex to the Vertices array and the UVs to the UV0 array
			Vertices.Add(FVector(X * Scale, Y * Scale, Z));
			// adding the UVs to the UV0 array by multiplying the X and Y by the UV scale to get the final UV coordinates for the vertex
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));

		}
	}
}

void APerlinProcTerrain::CreateTriangles()
{
	int Vertex = 0;

	for (int X = 0; X < XSize; X++)
	{
		for (int Y = 0; Y < YSize; Y++)
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);

			Vertex++;

		}

		Vertex++;
	}
}

