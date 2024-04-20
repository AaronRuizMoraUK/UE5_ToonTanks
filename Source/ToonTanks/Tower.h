// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

class ATank;

UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	void BeginPlay() override;


public:
	// Called every frame
	void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRange = 500.0f;

private:
	ATank* Tank = nullptr;

	FVector InitialAimingPoint = FVector::Zero();

	FTimerHandle FireRateTimerHandle;

	float FireRate = 2.0f;

	void CheckFireCondition();

	bool IsTankInFireRange() const;
};
