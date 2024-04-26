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
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTankInFireRange())
	{
		RotateTurret(Tank->GetActorLocation(), DeltaTime, 10.0f);

		if (!FireRateTimerHandle.IsValid())
		{
			// Set a timer to loop every FireRate seconds and call Fire
			GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::Fire, FireRate, true/*doesLoop*/);
		}
	}
	else
	{
		RotateTurret(InitialAimingPoint, DeltaTime, 5.0f);

		if (FireRateTimerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
		}
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	// Invalidate the timer
	FireRateTimerHandle.Invalidate();

	// Destroy tower
	Destroy();
}

bool ATower::IsTankInFireRange() const
{
	if (Tank && Tank->IsAlive())
	{
		FVector towerToTank =  Tank->GetActorLocation() - GetActorLocation();

		return towerToTank.SquaredLength() <= FireRange * FireRange &&
			FMath::Acos(towerToTank.GetSafeNormal().Dot(GetActorForwardVector())) <= FMath::DegreesToRadians(FireCone);
	}
	return false;
}
