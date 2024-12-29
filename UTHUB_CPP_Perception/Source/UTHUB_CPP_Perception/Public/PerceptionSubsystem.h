// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PerceptionSubsystem.generated.h"

class UPerceptionComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorDetected, AActor*, Detector, AActor*, DetectedActor);

UCLASS()
class UTHUB_CPP_PERCEPTION_API UPerceptionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void InitPerceptionInfo(AActor* Actor, float DetectionRadius, float DetectionTime);

	UFUNCTION(BlueprintCallable)
	void ToggleSomeActors(TArray<AActor*> InActors, bool Active);
	
	void RegisterNewActor(AActor* InActor);
	
	void RegisterAllPerceptionComponents(TArray<UPerceptionComponent*>& InPerceptionActorComponents) const;
	
	UFUNCTION()
	void NotifyActor(AActor* DetectedActor);

	FDelegateHandle OnActorSpawnedDelegateHandle;

public:
	
	UPROPERTY(BlueprintReadOnly)
	TArray<UPerceptionComponent*> PerceptionActorsComponents;
	
};
