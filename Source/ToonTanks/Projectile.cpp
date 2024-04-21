// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 1300.0f;
	ProjectileMovement->MaxSpeed = 1300.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Display, TEXT("AProjectile::OnHit HitComp %s OtherActor %s OtherComp %s"), 
	//	*HitComp->GetName(), *OtherActor->GetName(), *OtherComp->GetName());

	if (auto* ProjectileOwner = GetOwner();
		ProjectileOwner && 
		OtherActor && 
		OtherActor != this && 
		OtherActor != ProjectileOwner)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor, 
			Damage,
			ProjectileOwner->GetInstigatorController(),
			this, 
			UDamageType::StaticClass());

		Destroy();
	}
}
