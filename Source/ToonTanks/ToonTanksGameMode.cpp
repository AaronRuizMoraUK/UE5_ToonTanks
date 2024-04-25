// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "BasePawn.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    // TODO: Update game logic

    // Destroy pawn
    if (ABasePawn* Pawn = Cast<ABasePawn>(DeadActor))
    {
        Pawn->HandleDestruction();
    }
}
