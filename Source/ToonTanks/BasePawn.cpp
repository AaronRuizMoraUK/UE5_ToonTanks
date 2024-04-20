// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = Capsule;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(Capsule);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawningPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawningPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::Fire()
{
	DrawDebugSphere(GetWorld(), ProjectileSpawningPoint->GetComponentLocation(), 10.0f, 20, FColor::Red, false, 2.0f);
}

void ABasePawn::RotateTurret(FVector LookAtTarget, float DeltaTime)
{
	const FVector TargetVectorWorld = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator TargetRotationWorld(0.0, TargetVectorWorld.Rotation().Yaw, 0.0);
	TargetRotationWorld = FMath::RInterpTo(TurretMesh->GetComponentRotation(), TargetRotationWorld, DeltaTime, 20.0f);
	TurretMesh->SetWorldRotation(TargetRotationWorld);
}
