// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDMIMod.h"
#include "Firstperson415Character.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision")); // Create a box component and set it as the root component
	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube Mesh"); // Create a static mesh component and attach it to the box component

	RootComponent = boxComp; // Set the box component as the root component
	cubeMesh->SetupAttachment(boxComp); // Attach the cube mesh to the box component

}

// Called when the game starts or when spawned
void ACubeDMIMod::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverlapBegin);// Add a dynamic delegate to the box component's OnComponentBeginOverlap event, which will call the OnOverlapBegin function when another actor overlaps with the box component

	if (baseMat)
	{
		dmiMat = UMaterialInstanceDynamic::Create(baseMat, this); // Create a dynamic material instance from the base material, which allows us to change the material's parameters at runtime

	}
	if (cubeMesh)
	{
		cubeMesh->SetMaterial(0, dmiMat);
	}
}

// Called every frame
void ACubeDMIMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeDMIMod::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFirstperson415Character* overlappedActor = Cast<AFirstperson415Character>(OtherActor);

	if (overlappedActor) // Check if the overlapping actor is the player character
	{
		float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

		FVector randColor = FVector4(ranNumX, ranNumY, ranNumZ, 1.f);
		if (dmiMat)
		{
			dmiMat->SetVectorParameterValue("Color", randColor);
			dmiMat->SetScalarParameterValue("Darkness", ranNumX);
		}
	}
}
