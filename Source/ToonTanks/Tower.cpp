// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	InitialAimingPoint = GetActorLocation() + GetActorForwardVector();
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Tank)
	{
		return;
	}

	if (FVector::Distance(GetActorLocation(), Tank->GetActorLocation()) <= FireRange)
	{
		RotateTurret(Tank->GetActorLocation(), DeltaTime);
	}
	else
	{
		RotateTurret(InitialAimingPoint, DeltaTime);
	}
}
