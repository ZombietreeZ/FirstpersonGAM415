// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Firstperson415Projectile.generated.h"


class USphereComponent;

class UProjectileMovementComponent;

class UNiagaraSystem;


UCLASS(config=Game)
class AFirstperson415Projectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	// Create a static mesh component for the projectile mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ballMesh;
	// Create a material variable to hold the base material for the projectile
	UPROPERTY(EditAnywhere)
	UMaterial* baseMat;
	// Create a FLinearColor variable to hold the random color generated for the projectile.
	UPROPERTY()
	FLinearColor randColor;
	// create a material interface 		 
	UPROPERTY(EditAnywhere)
		UMaterialInterface* projMat;
		// create a dynamic material instance variable to hold the dynamic material instance created from the base material,
	UPROPERTY()
	UMaterialInstanceDynamic* dmiMat;
	// Uproperty for color_p 
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* colorP;


protected:
	virtual void BeginPlay();

public:
	AFirstperson415Projectile(); 

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

