// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerceptionSubsystem.h"
#include "Components/ActorComponent.h"
#include "PerceptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorDetectedDelegate, AActor*, DetectedActor);

USTRUCT(BlueprintType)
struct FPerceptionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float DetectionRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float DetectionTime = 2.f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTHUB_CPP_PERCEPTION_API UPerceptionComponent : public UActorComponent
{
	GENERATED_BODY()

	friend UPerceptionSubsystem;

public:
	// Sets default values for this component's properties
	UPerceptionComponent();

	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FActorDetectedDelegate OnActorDetected;

	UFUNCTION(BlueprintCallable)
	void TogglePerception(bool Active);

	UPROPERTY(VisibleAnywhere, Category = "Perception")
	TArray<AActor*> DetectedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	FPerceptionInfo PerceptionInfo;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:

	FTimerHandle TimerHandle;

	UFUNCTION(BlueprintCallable)
	void DetectActors();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Perception")
	class USphereComponent* DetectionCollision;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	TArray<AActor*> ActorsToIgnore;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};

