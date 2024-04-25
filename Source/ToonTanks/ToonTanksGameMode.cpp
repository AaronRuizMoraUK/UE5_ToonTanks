// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "BasePawn.h"
#include "ToonTanksPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    // TODO: Update game logic

    // Destroy pawn
    if (ABasePawn* Pawn = Cast<ABasePawn>(DeadActor))
    {
        Pawn->HandleDestruction();
    }
}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    if (!PlayerController)
    {
        return;
    }

    PlayerController->SetPlayerEnabledState(false);

    // Set a timer to kick off game
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, 
        [this]()
        {
            PlayerController->SetPlayerEnabledState(true);
        },
        StartDelay, 
        false/*loops*/);
}
