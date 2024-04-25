// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"
#include "GameFramework/Pawn.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    // Disable inputs
    if (bPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }

    // Disable the Tick function so no player logic is running
    GetPawn()->SetActorTickEnabled(bPlayerEnabled);

    // Show cursor while enabled
    bShowMouseCursor = bPlayerEnabled;
}
