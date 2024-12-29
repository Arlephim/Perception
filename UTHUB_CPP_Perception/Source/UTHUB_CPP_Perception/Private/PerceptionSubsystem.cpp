// Fill out your copyright notice in the Description page of Project Settings.


#include "UTHUB_CPP_Perception/Public/PerceptionSubsystem.h"

#include "PerceptionComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


void UPerceptionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    PerceptionActorsComponents.Empty();
    
    FOnActorSpawned::FDelegate Delegate = FOnActorSpawned::FDelegate::CreateUObject(this, &ThisClass::RegisterNewActor);
    OnActorSpawnedDelegateHandle = GetWorld()->AddOnActorSpawnedHandler(Delegate);
    
    GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
    {
        RegisterAllPerceptionComponents(PerceptionActorsComponents);
    });
}

void UPerceptionSubsystem::Deinitialize()
{
    Super::Deinitialize();
    
    PerceptionActorsComponents.Empty();
    
    if (OnActorSpawnedDelegateHandle.IsValid())
    {
        GetWorld()->RemoveOnActorSpawnedHandler(OnActorSpawnedDelegateHandle);
    }
}

void UPerceptionSubsystem::InitPerceptionInfo(AActor* Actor, float InDetectionRadius, float InDetectionTime)
{
    if (UPerceptionComponent* PerceptionComponent = Actor->FindComponentByClass<UPerceptionComponent>())
    {
        PerceptionComponent->PerceptionInfo.DetectionRadius = InDetectionRadius;
        PerceptionComponent->PerceptionInfo.DetectionTime = InDetectionTime;
        PerceptionComponent->DetectionCollision->SetSphereRadius(InDetectionRadius);
    }
}

void UPerceptionSubsystem::ToggleSomeActors(TArray<AActor*> InActors, bool Active)
{
    for (AActor* Actor : InActors)
    {
        if (UPerceptionComponent* PerceptionComponent = Actor->FindComponentByClass<UPerceptionComponent>())
        {
            PerceptionComponent->TogglePerception(Active);
        }
    }
}

void UPerceptionSubsystem::RegisterNewActor(AActor* InActor)
{
    if (!InActor) return;
    
    UPerceptionComponent* PerceptionComp = InActor->FindComponentByClass<UPerceptionComponent>();
    if (PerceptionComp && !PerceptionActorsComponents.Contains(PerceptionComp))
    {
        PerceptionActorsComponents.Add(PerceptionComp);
        
        PerceptionComp->OnActorDetected.AddDynamic(this, &ThisClass::NotifyActor);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("New registered Actor: %s"), *InActor->GetName()));
    }
}

void UPerceptionSubsystem::RegisterAllPerceptionComponents(TArray<UPerceptionComponent*>& InPerceptionActorComponents) const
{
    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);

    for (AActor* Actor : OutActors)
    {
        if (!Actor) continue;
        
        UPerceptionComponent* PerceptionComp = Actor->FindComponentByClass<UPerceptionComponent>();
        if (PerceptionComp && !InPerceptionActorComponents.Contains(PerceptionComp))
        {
            InPerceptionActorComponents.Add(PerceptionComp);
            
            PerceptionComp->OnActorDetected.AddDynamic(this, &ThisClass::NotifyActor);

            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("Register actor: %s"), *Actor->GetName()));
        }
    }
}

void UPerceptionSubsystem::NotifyActor(AActor* DetectedActor)
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Actor Detected: %s"), *DetectedActor->GetName()));
}