// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

const ATank::InputActionNameArray ATank::InputActionNames =
{
	TEXT("IA_MoveForward"),
	TEXT("IA_Turn")
};

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Srpint Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Load Input Mapping Context asset
	if (InputMapping.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Input Mapping Context found!"));
		return;
	}
	auto* InputMappingData = InputMapping.LoadSynchronous();

	// Add input mapping context to the input system.
	bool bAddedInputMapping = false;
	if (auto* PlayerController = GetLocalViewingPlayerController())
	{
		if (auto* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player))
		{
			if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (!InputSystem->HasMappingContext(InputMappingData))
				{
					UE_LOG(LogTemp, Display, TEXT("Adding Input Mapping Context %s to player's input system!"), *InputMapping->GetName());
					InputSystem->AddMappingContext(InputMappingData, 0);
				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("Input mapping %s already part of player's input system!"), *InputMapping->GetName());
				}
				bAddedInputMapping = true;
			}
		}
	}
	if (!bAddedInputMapping)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Mapping was not added to the player's input system!"));
		return;
	}

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not using enhanced input component!"));
		return;
	}

	auto GetActionItem = [InputMappingData](FStringView actionItemName) -> const UInputAction*
		{
			auto* foundMapping = InputMappingData->GetMappings().FindByPredicate(
				[&actionItemName](const FEnhancedActionKeyMapping& mapping)
				{
					return mapping.Action->GetName() == actionItemName;
				});

			if (foundMapping)
			{
				return foundMapping->Action.Get();
			}

			UE_LOG(LogTemp, Warning, TEXT("Action Item %.*s not found!"), actionItemName.Len(), actionItemName.GetData());
			return nullptr;
		};

	for (int32 i = 0; i < IA_Num; ++i)
	{
		EnhancedInputComponent->BindAction(GetActionItem(InputActionNames[i]), ETriggerEvent::Triggered, this, &ATank::UpdateInputs, i);
	}
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = 0; i < IA_Num; ++i)
	{
		UE_LOG(LogTemp, Display, TEXT("- %.*s: %f"), InputActionNames[i].Len(), InputActionNames[i].GetData(), InputActionValues[i]);
	}

	const FVector2D Speed(300.0f, 200.0f);

	FVector DeltaLocation(
		InputActionValues[IA_MoveForward] * Speed.X * DeltaTime,
		InputActionValues[IA_Turn] * Speed.Y * DeltaTime,
		0.0f
	);

	AddActorLocalOffset(DeltaLocation);
}

void ATank::UpdateInputs(const FInputActionInstance& Instance, int32 InputIndex)
{
	auto Value = Instance.GetValue().Get<float>();
	InputActionValues[InputIndex] = Value;

	//UE_LOG(LogTemp, Display, TEXT("I'm moving!! %f Index %d"), Value, InputIndex);
}
