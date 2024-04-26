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
        UE_LOG(LogTemp, Warning, TEXT("AToonTanksGameMode has invalid player controller!"));
        return;
    }

    PlayerController->SetPlayerEnabledState(false);

    // Send an event to trigger logic in blueprint.
    StartGame();

    // Set a timer to kick off game
    GetWorldTimerManager().SetTimer(StartGameTimerHandle,
        [this]()
        {
            PlayerController->SetPlayerEnabledState(true);
        },
        StartDelay, 
        false/*loops*/);
}
