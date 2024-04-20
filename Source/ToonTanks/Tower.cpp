// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	InitialAimingPoint = GetActorLocation() + GetActorForwardVector();

	// Set a timer to loop every FireRate seconds and call CheckFireCondition
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTankInFireRange())
	{
		RotateTurret(Tank->GetActorLocation(), DeltaTime);
	}
	else
	{
		RotateTurret(InitialAimingPoint, DeltaTime);
	}
}

void ATower::CheckFireCondition()
{
	if (IsTankInFireRange())
	{
		Fire();
	}
}

bool ATower::IsTankInFireRange() const
{
	return Tank && FVector::Distance(GetActorLocation(), Tank->GetActorLocation()) <= FireRange;
}
