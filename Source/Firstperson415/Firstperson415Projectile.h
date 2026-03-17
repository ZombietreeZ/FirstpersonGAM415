// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Firstperson415Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

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

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ballMesh; // Create a static mesh component for the projectile mesh

	UPROPERTY(EditAnywhere)
	UMaterial* baseMat; // Create a material variable to hold the base material for the projectile, which will be used to create a dynamic material instance to change the color of the projectile at runtime

	UPROPERTY()
	FLinearColor randColor; // Create a FLinearColor variable to hold the random color generated for the projectile, which will be used to set the color parameter of the dynamic material instance to change the color of the projectile at runtime
		 
	UPROPERTY(EditAnywhere)
		UMaterialInterface* projMat; // create a material interface 

	UPROPERTY()
	UMaterialInstanceDynamic* dmiMat; // create a dynamic material instance variable to hold the dynamic material instance created from the base material, which will be used to change the color of the projectile at runtime

public:
	AFirstperson415Projectile(); // Sets default values for this actor's properties

protected:
	virtual void BeginPlay();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

