// Copyright Epic Games, Inc. All Rights Reserved.

#include "Firstperson415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AFirstperson415Projectile::AFirstperson415Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFirstperson415Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball Mesh"); /// Create a static mesh component for the projectile mesh and attach it to the collision component

	// Set as root component
	RootComponent = CollisionComp;

	ballMesh->SetupAttachment(CollisionComp); // seting up the collision for the ball mesh to be the same as the collision component so that the projectile will collide with other objects in the game when it hits them

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
	randColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 1.f); // generate a random color for the projectile when it is spawned to add variety to the projectiles in the game

	dmiMat = UMaterialInstanceDynamic::Create(projMat, this); // create a dynamic material instance from the base material to change the color of the projectile
	ballMesh->SetMaterial(0, dmiMat); // set the material of the projectile mesh to the dynamic material instance

	dmiMat->SetVectorParameterValue("projColor", randColor); // set the color parameter of the material to the random color generated above to change the color of the projectile
}

void AFirstperson415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	if (OtherActor != nullptr)
	{
		float frameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f); // generate a random number between 0 and 3 to use as the frame number for the decal. This will change the pattern of the decal to create more variety in the decals spawned on hit.

		// Spawn a decal at the hit location with a random color and frame number to create a unique pattern for each hit. The decal will have a random size between 20 and 40 units.
		auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 0.f);
		auto MatInstance = Decal->CreateDynamicMaterialInstance(); // create a dynamic material instance to change the parameters of the decal

		MatInstance->SetVectorParameterValue("Color", randColor); // make a random color for the decal
		MatInstance->SetScalarParameterValue("Frame", frameNum); // make a random frame for the decal to change the pattern of the decal
	}

}