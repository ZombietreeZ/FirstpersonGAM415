// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CubeDMIMod.generated.h"

class UNiagaraSystem;

UCLASS()
class FIRSTPERSON415_API ACubeDMIMod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeDMIMod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere) // EditAnywhere allows us to edit this variable in the blueprints
		UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere) // EditAnywhere allows us to edit this variable in the blueprints
		UStaticMeshComponent* cubeMesh;

	UPROPERTY(EditAnywhere) // EditAnywhere allows us to edit this variable in the blueprints
		UMaterialInterface* baseMat;

	UPROPERTY() // UPROPERTY() means that this variable will be visible to the engine, but not editable in the blueprints
		UMaterialInstanceDynamic* dmiMat;

	UFUNCTION() // UFUNCTION() allows us to use this function as a callback for the overlap event
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Create a FLinearColor variable to hold the random color generated for the cube.
	// Uproperty for colorp 
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* colorP;
};
