// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Camera/CameraShakeBase.h"

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

void ABasePawn::HandleDestruction()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());

	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}

void ABasePawn::Fire()
{
	if (ProjectileClass.Get())
	{
		if (auto* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawningPoint->GetComponentTransform()))
		{
			Projectile->SetOwner(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to spawn projectile!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass set to None"));
	}
}

void ABasePawn::RotateTurret(FVector LookAtTarget, float DeltaTime, float Speed)
{
	const FVector TargetVectorWorld = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator TargetRotationWorld(0.0, TargetVectorWorld.Rotation().Yaw, 0.0);
	TargetRotationWorld = FMath::RInterpTo(TurretMesh->GetComponentRotation(), TargetRotationWorld, DeltaTime, Speed);
	TurretMesh->SetWorldRotation(TargetRotationWorld);
}
