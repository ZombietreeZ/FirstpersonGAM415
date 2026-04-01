// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDMIMod.h"
#include "Firstperson415Character.h"
#include "Kismet/KismetMathLibrary.h"
// Allows calling niagara spawn function
#include "NiagaraFunctionLibrary.h"

// allows casting to niagara component
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create a box component and set it as the root component
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// Create a static mesh component and attach it to the box component
	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube Mesh");
	// Set the box component as the root component
	RootComponent = boxComp;
	// Attach the cube mesh to the box component
	cubeMesh->SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void ACubeDMIMod::BeginPlay()
{
	Super::BeginPlay();
	// Add a dynamic material instance to the box component's OnComponentBeginOverlap event, which will call the OnOverlapBegin function when another actor overlaps with the box component
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverlapBegin);
	// if base material exists
	if (baseMat)
	{
		// Create a dynamic material instance from the base material, which allows us to change the material's parameters at runtime
		dmiMat = UMaterialInstanceDynamic::Create(baseMat, this);

	}
	//if cube mesh exists
	if (cubeMesh)
	{
		// set the material of the cube mesh to DMI to allow us to change the color of the cube when the player overlaps with it
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
	// Cast the overlapping actor to the player character class to check if the overlapping actor is the player character
	AFirstperson415Character* overlappedActor = Cast<AFirstperson415Character>(OtherActor);

	// Check if the overlapping actor is the player character
	if (overlappedActor)
	{
		float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

		FLinearColor randColor = FLinearColor(ranNumX, ranNumY, ranNumZ, 1.f);
		// if dynamic material instance 
		if (dmiMat)
		{
			//  set vector parameter value random color 
			dmiMat->SetVectorParameterValue("Color", randColor);
			// setting scalar parameter to random num on the x asis 
			dmiMat->SetScalarParameterValue("Darkness", ranNumX);
			
			if (colorP)
			{
				// spawning attatched color
				UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, OtherComp, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);
				// randomizing color
				particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);
			}
		}
	}
}
