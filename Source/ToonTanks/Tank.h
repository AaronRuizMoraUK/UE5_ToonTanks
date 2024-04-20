// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "InputMappingContext.h"
#include "Tank.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	// Called to bind functionality to input
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Called every frame
	void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnRate = 90.0f;

private:
	enum InputAction
	{
		IA_MoveForward = 0,
		IA_Turn,
		IA_Fire,

		IA_Num
	};

	using InputActionNameArray = FStringView[IA_Num];
	using InputActionValueArray = TStaticArray<FInputActionValue, IA_Num>;

	static const InputActionNameArray InputActionNames;

	InputActionValueArray InputActionValues = InputActionValueArray(InPlace, 0.0f);

	APlayerController* PlayerController = nullptr;

	void UpdateInputs(const FInputActionInstance& Instance, int32 InputIndex);
};
