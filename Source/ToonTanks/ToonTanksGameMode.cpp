// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "BasePawn.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    ABasePawn* DeadPawn = Cast<ABasePawn>(DeadActor);
    if (!DeadPawn)
    {
        return;
    }

    // Win/Lose game logic
    if (DeadPawn->IsPlayerControlled())
    {
        GameOver(false); // Lost game
    }
    else if (Cast<ATower>(DeadPawn))
    {
        --TargetTowers;
        if (TargetTowers == 0)
        {
            GameOver(true); // Won game
        }
    }

    // Destroy pawn
    DeadPawn->HandleDestruction();
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

    TargetTowers = GetTargetTowerCount();

    // Send an event to trigger logic in blueprint.
    StartGame();

    // Set a timer to kick off game
    GetWorldTimerManager().SetTimer(StartGameTimerHandle,
        [this]()
        {
            PlayerController->SetPlayerEnabledState(true);
        },
        StartDelay, 
        false/*doesLoop*/);
}

int32 AToonTanksGameMode::GetTargetTowerCount() const
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}
