// Copyright Epic Games, Inc. All Rights Reserved.

#include "Firstperson415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PerlinProcTerrain.h"
#include "NiagaraSystem.h"
// Allows calling niagara spawn function
#include "NiagaraFunctionLibrary.h"

// allows casting to niagara component
#include "NiagaraComponent.h"



AFirstperson415Projectile::AFirstperson415Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFirstperson415Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can not walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	
	// Create a static mesh component for the projectile mesh and attach it to the collision component
	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball Mesh"); 

	// Set as root component
	RootComponent = CollisionComp;
	// set attatchment for ballMesh
	ballMesh->SetupAttachment(CollisionComp); 
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AFirstperson415Projectile::BeginPlay()
{
	Super::BeginPlay();
	//random color for ballmesh
	randColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 1.f);

	//dynamic material instance and set ball mesh to random color
	dmiMat = UMaterialInstanceDynamic::Create(projMat, this);
	if (dmiMat)
	{
		// set the dynamic material instance to the ball mesh so that we can change the color of the projectile at runtime 
		ballMesh->SetMaterial(0, dmiMat);

		// set the color of projectile to a random color
		dmiMat->SetVectorParameterValue("ProjColor", randColor);
	}
}

void AFirstperson415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		// add an impulse to the object hit by the projectile to make it react to the hit, the impulse is calculated by multiplying the velocity of the projectile by 100 to make it strong enough to move the object hit by the projectile
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		//supose to destroy projectile after collision
		Destroy();
	}

	if (OtherActor != nullptr)
	{
		if (colorP)
		{
			// creates new component spawning attached
			UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, HitComp, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);

			//sets particle color to randColor value
			particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);

			//removes projectile after hitting surface
			ballMesh->DestroyComponent();

			//Prevents a projectile from bouncing around causing a visual mess by disabling collision
			CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
		}
		// generate a random number between 0 and 3 to use as the frame number for the decal. This will change the pattern of the decal to create more variety in the decals spawned on hit.
		float frameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);
		// Spawn a decal at the hit location with a random color and frame number to create a unique pattern for each hit. The decal will have a random size between 20 and 40 units.
		auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 0.f);
		// create a dynamic material instance to change the parameters of the decal
		auto MatInstance = Decal->CreateDynamicMaterialInstance(); 
		
		// make a random color for the decal
		MatInstance->SetVectorParameterValue("Color", randColor); 
		// make a random frame for the decal to change the pattern of the decal
		MatInstance->SetScalarParameterValue("Frame", frameNum); 
		// casting procedural terrain to alter mesh on hit
		APerlinProcTerrain* procTerrain = Cast<APerlinProcTerrain>(OtherActor);
		// if the procedural terrain is hit, call the AlterMesh function to change the mesh of the terrain at the hit location
		if (procTerrain)
		{
			// call the AlterMesh function to change the mesh of the terrain at the hit location
			procTerrain->AlterMesh(Hit.ImpactPoint);
		}
	}

}