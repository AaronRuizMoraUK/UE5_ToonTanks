// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

class AToonTanksPlayerController;

UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

protected:
	// Called when the game starts
	void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle StartGameTimerHandle;

private:
	UPROPERTY(EditAnywhere)
	float StartDelay = 4.0f;

private:
	AToonTanksPlayerController* PlayerController = nullptr;

	int32 TargetTowers = -1;

	void HandleGameStart();

	int32 GetTargetTowerCount() const;
};
