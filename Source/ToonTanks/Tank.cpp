// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "DrawDebugHelpers.h"

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

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not using enhanced input component!"));
		return;
	}

	// Load Input Mapping Context asset
	auto* InputMappingData = InputMapping.LoadSynchronous();
	if (!InputMappingData)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid Input Mapping Context!"));
		return;
	}

	// Obtain Player Controller
	PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player controller found!"));
		return;
	}

	// Add input mapping context to the input system.
	if (auto* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player))
	{
		if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputSystem->HasMappingContext(InputMappingData))
			{
				UE_LOG(LogTemp, Display, TEXT("Adding Input Mapping Context %s to player's input system!"), *InputMappingData->GetName());
				InputSystem->AddMappingContext(InputMappingData, 0);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No enhanced input local player subsystem found!"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No local player found!"));
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

	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false/*TraceComplex*/, HitResult);
		if (HitResult.bBlockingHit)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 50.0f, 20, FColor::Blue);
		}
	}

	//for (int32 i = 0; i < IA_Num; ++i)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("- %.*s: %f"), InputActionNames[i].Len(), InputActionNames[i].GetData(), InputActionValues[i]);
	//}

	const FVector DeltaLocation(
		InputActionValues[IA_MoveForward] * Speed * DeltaTime,
		0.0,
		0.0
	);

	const FRotator DeltaRotation(
		0.0,
		InputActionValues[IA_Turn] * TurnRate * DeltaTime,
		0.0
	);

	AddActorLocalOffset(DeltaLocation, true/*Sweep*/);

	AddActorLocalRotation(DeltaRotation, true/*Sweep*/);
	}

void ATank::UpdateInputs(const FInputActionInstance& Instance, int32 InputIndex)
{
	InputActionValues[InputIndex] = Instance.GetValue().Get<float>();

	//UE_LOG(LogTemp, Display, TEXT("Input triggered '%s' with value %.2f"), InputActionNames[InputIndex], InputActionValues[InputIndex]);
}
