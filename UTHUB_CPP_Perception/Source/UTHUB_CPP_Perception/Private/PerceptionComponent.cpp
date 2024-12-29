// Fill out your copyright notice in the Description page of Project Settings.


#include "UTHUB_CPP_Perception/Public/PerceptionComponent.h"

#include "Components/SphereComponent.h"


// Sets default values for this component's properties
UPerceptionComponent::UPerceptionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	ActorsToIgnore.Add(GetOwner());
	
	DetectionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereDetection"));
	DetectionCollision->InitSphereRadius(PerceptionInfo.DetectionRadius);

	// Solo lo pongo para visualizarlo ingame
	DetectionCollision->SetHiddenInGame(false);
}

void UPerceptionComponent::TogglePerception(bool Active)
{
	if (Active)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DetectActors, PerceptionInfo.DetectionTime, true);
	}
	else
	{
		GWorld->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void UPerceptionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	DetectionCollision->SetSphereRadius(PerceptionInfo.DetectionRadius);
}

void UPerceptionComponent::DetectActors()
{
	TArray<AActor*> Actors;
	DetectionCollision->GetOverlappingActors(Actors, AActor::StaticClass());

	for (AActor* Actor : Actors)
	{
		if (Actor->FindComponentByClass<UPerceptionComponent>() && Actor != GetOwner())
		{
			DetectedActors.AddUnique(Actor);
			OnActorDetected.Broadcast(Actor);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("Detected Actor Name: %s"), *Actor->GetName()));
		}
	}
}

void UPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TogglePerception(true);
}

void UPerceptionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

