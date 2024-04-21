// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

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
	if (ProjectileClass.Get())
	{
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawningPoint->GetComponentTransform());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass set to None"));
	}
}

void ABasePawn::RotateTurret(FVector LookAtTarget, float DeltaTime)
{
	const FVector TargetVectorWorld = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator TargetRotationWorld(0.0, TargetVectorWorld.Rotation().Yaw, 0.0);
	TargetRotationWorld = FMath::RInterpTo(TurretMesh->GetComponentRotation(), TargetRotationWorld, DeltaTime, 20.0f);
	TurretMesh->SetWorldRotation(TargetRotationWorld);
}
