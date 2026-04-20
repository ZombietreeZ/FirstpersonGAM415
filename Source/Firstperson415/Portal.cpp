// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Firstperson415Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Creating components for the portal actor and setting up the mesh
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	// adding a box component to the portal actor
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");
	// adding a scene capture component to the portal actor
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture");
	// adding an arrow component to the portal actor to show the direction the player will be teleported in
	rootArrow = CreateDefaultSubobject<UArrowComponent>("Root Arrow");
	//making boxcomp the root component of the portal actor
	RootComponent = boxComp;
	// setting up the attachment of the components to the root component
	mesh->SetupAttachment(boxComp);
	// setting up scene capture for mesh so that it captures the scene from the perspective of the portal
	sceneCapture->SetupAttachment(mesh);
	// setting up the attachments for the root arrow 
	rootArrow->SetupAttachment(RootComponent);
	// setting the collision response of the mesh to ignore all channels so that it does not interfere with the player's movement or the box component's collision
	mesh->SetCollisionResponseToAllChannels(ECR_Ignore);


}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	// adding a dynamic on overlape event to the box component so that when the player overlaps with the box component it will call the OnOverlapBegin function
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	// setting the scene capture to capture every frame so that it updates the view of the portal every frame
	mesh->SetHiddenInSceneCapture(true);
	// if there is a material set for the porta, set the material of the mesh to the material set for the portal 
	if (mat)
	{
		// setting the material of the mesh to the material set for the portal
		mesh->SetMaterial(0, mat);
	}
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// if there is another portal, update the scene capture to show the view from the other portal
	UpdatePortals();

}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if the other actor is the player character, teleport the player to the location of the other portal and set a timer to reset the teleporting bool after 1 second so that the player can teleport again
	AFirstperson415Character* playerChar = Cast<AFirstperson415Character>(OtherActor);
	// if the Player is valid and there is another portal, teleport the player to the location of the other portal and set a timer to reset the teleporting bool after 1 second so that the player can teleport again
	if (playerChar)
	{
		// if there is another portal, teleport the player to the location of the other portal and set a timer to reset
		if (OtherPortal)
		{
			// if the player is not currently teleporting, teleport the player to the location of the other portal and set a timer to reset the teleporting bool after 1 second so that the player can teleport again
			if (!playerChar->isTeleporting)
			{
				// setting the teleporting bool to true so that the player cannot teleport again until the timer resets it back to false
				playerChar->isTeleporting = true;
				// teleporting the player to the location of the other portal by setting the player's location to the location of the other portal's root arrow component
				FVector loc = OtherPortal->rootArrow->GetComponentLocation();
				// setting the player's location to the other portal's root arrow component so that the player faces the correct direction after teleporting
				playerChar->SetActorLocation(loc);

				// setting a timer to reset the teleporting bool after 1 second so that the player can teleport again
				FTimerHandle TimerHandle;
				// creating a timer delegate to call the SetBool function after 1 second to reset the teleporting bool so that the player can teleport again
				FTimerDelegate TimerDelegate;
				// binding the timer delegate to the SetBool function and passing in the player character as a parameter so that the SetBool function can reset the teleporting bool for the correct player character
				TimerDelegate.BindUFunction(this, "SetBool", playerChar);
				// setting the timer to call the set bool function after 1 second to reset the teleporting tool.
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false);

			}
		}
	}
}

void APortal::SetBool(AFirstperson415Character* playerChar)
{
	if (playerChar)
	{
		playerChar->isTeleporting = false;
	}
}

void APortal::UpdatePortals()
{
	FVector Location = this->GetActorLocation() - OtherPortal->GetActorLocation();
	FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation();
	FRotator camRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation();
	FVector CombinedLocation = camLocation + Location;

	sceneCapture->SetWorldLocationAndRotation(CombinedLocation, camRotation);
}

