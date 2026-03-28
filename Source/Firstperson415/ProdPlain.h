// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProdPlain.generated.h"

class UProceduralMeshComponent;

UCLASS()
class FIRSTPERSON415_API AProdPlain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProdPlain();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostActorCreated() override;

	virtual void PostLoad() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Arrays to hold the vertices, triangles, UVs, and material for the procedural mesh
	UPROPERTY(EditAnywhere)
	TArray<FVector> Vertices;
	// triangle arrays to define the triangles of the mesh, 
	UPROPERTY(EditAnywhere)
	TArray<int> Triangles;
	// UV array to define the UV coordinates for the mesh, which will be used for texturing the mesh
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> UV0;
	// Color array to define the vertex colors for the mesh, which can be used to add color variation to the mesh
	UPROPERTY(EditAnywhere)
	UMaterialInterface* PlainMat;

	// Function to create the procedural mesh using the vertices, triangles, UVs, and material defined in the arrays above
	UFUNCTION()
	void CreateMesh();

private:
	// Procedural mesh component to hold the generated mesh
	UProceduralMeshComponent* procMesh;

};
