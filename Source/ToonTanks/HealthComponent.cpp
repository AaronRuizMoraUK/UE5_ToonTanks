// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Display, TEXT("UHealthComponent::OnDamageTaken DamagedActor %s Damage %f DamageCauser %s"),
	//	*DamagedActor->GetName(), Damage, *DamageCauser->GetName());

	// Avoid triggering death logic more than once.
	// Avoid applying negative damage.
	if (Health <= 0 || Damage <= 0.0f)
	{
		return;
	}

	Health -= Damage;
	UE_LOG(LogTemp, Display, TEXT("Health %s (%s): %f"), *GetOwner()->GetName(), *DamagedActor->GetName(), Health);

	// Did actor die?
	if (Health <= 0)
	{
		if (GameMode)
		{
			GameMode->ActorDied(DamagedActor);
		}
	}
}
