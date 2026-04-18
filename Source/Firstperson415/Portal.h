// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Portal.generated.h"

class Firstperson415Character;

UCLASS()
class FIRSTPERSON415_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// edit anywhere making a mesh component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;

	// scene capture component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* sceneCapture;

	// arrow component
	UPROPERTY(EditAnywhere)
	UArrowComponent* rootArrow;

	//texture render target 2d
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* renderTarget;

	// making a box component
	UPROPERTY(EditAnywhere)
	UBoxComponent* boxComp;

	// and instance for the other portal
	UPROPERTY(EditAnywhere)
	APortal* OtherPortal;

	// interface for material
	UPROPERTY(EditAnywhere)
	UMaterialInterface* mat;

	// funtion for overlapping event that teleports player when interacting
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// bool for player character 
	UFUNCTION()
	void SetBool(AFirstperson415Character* playerChar);

	// updating portals to teleport 
	UFUNCTION()
	void UpdatePortals();
};
