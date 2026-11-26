// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AscPlayerCharacterController.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/EngineTypes.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "NavigationSystem.h"
#include "Player/AscCRPGCameraRig.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Interaction/SelectableInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogAscendenciaInput, Log, All);

AAscPlayerCharacterController::AAscPlayerCharacterController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bAutoManageActiveCameraTarget = false;
	CameraRigClass = AAscCRPGCameraRig::StaticClass();
}

void AAscPlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return;
	}
	
	CreateCameraRig();
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	
	if (!InputSubsystem)
	{
		UE_LOG(LogAscendenciaInput, Error, TEXT("Enhanced Input subsystem is unavailable."));
		return;
	}

	if (!GameplayMappingContext)
	{
		UE_LOG(LogAscendenciaInput, Error, TEXT("Assign GameplayMappingContext in the controller Blueprint."));
		return;
	}
	
	InputSubsystem->AddMappingContext(GameplayMappingContext, 0);
	
}

void AAscPlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInputComponent)
	{
		UE_LOG(LogAscendenciaInput, Error, TEXT("Expected an EnhancedInputComponent."));
		return;
	}
	if (!ClickMoveAction || !CameraPanAction || !CameraFocusAction || !CameraZoomAction || !CameraRotateHoldAction || !CameraRotateAction)
	{
		UE_LOG(LogAscendenciaInput, Error, TEXT("Assign all input actions in the controller Blueprint."));
		return;
	}

	EnhancedInputComponent->BindAction(ClickMoveAction, ETriggerEvent::Started, this, &AAscPlayerCharacterController::OnClickMoveStarted);

	EnhancedInputComponent->BindAction(CameraPanAction, ETriggerEvent::Triggered, this, &AAscPlayerCharacterController::OnCameraPanTriggered);

	EnhancedInputComponent->BindAction(CameraPanAction, ETriggerEvent::Completed, this, &AAscPlayerCharacterController::OnCameraPanFinished);

	EnhancedInputComponent->BindAction(CameraPanAction, ETriggerEvent::Canceled, this, &AAscPlayerCharacterController::OnCameraPanFinished);

	EnhancedInputComponent->BindAction(CameraFocusAction, ETriggerEvent::Started,this, &AAscPlayerCharacterController::OnCameraFocusStarted);
	
	EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &AAscPlayerCharacterController::OnCameraZoomTriggered);
	
	EnhancedInputComponent->BindAction(CameraRotateHoldAction, ETriggerEvent::Started, this, &AAscPlayerCharacterController::OnCameraRotateStarted);

	EnhancedInputComponent->BindAction(CameraRotateHoldAction, ETriggerEvent::Completed, this, &AAscPlayerCharacterController::OnCameraRotateFinished);

	EnhancedInputComponent->BindAction(CameraRotateHoldAction, ETriggerEvent::Canceled, this, &AAscPlayerCharacterController::OnCameraRotateFinished);

	EnhancedInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &AAscPlayerCharacterController::OnCameraRotateTriggered);
}

void AAscPlayerCharacterController::OnClickMoveStarted()
{
	if (!IsLocalController() || !GetPawn() || IsMoveInputIgnored() || bIsRotatingCamera) return;
	
	FVector Destination;
	if (!TryGetMoveDestination(Destination)) return;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
	
}

bool AAscPlayerCharacterController::TryGetMoveDestination(FVector& OutDestination) const
{
	FHitResult HitResult;
	
	const bool bHit = GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, HitResult);
	
	if (!bHit || !HitResult.bBlockingHit) return false;
	
	UNavigationSystemV1*  NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavigationSystem) return false;
	
	FNavLocation ProjectedLocation; 
	
	const bool bProjected = NavigationSystem->ProjectPointToNavigation(HitResult.ImpactPoint, ProjectedLocation, NavigationProjectionExtent, &GetNavAgentPropertiesRef());
	if (!bProjected) return false;
	OutDestination = ProjectedLocation.Location;
	return true;
}

void AAscPlayerCharacterController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();
	
	/**
	* Line trace from cursor. There are several scenarios
	* A. LastActor is null && ThisActor is null
	*	- Do nothing
	* B. LastActor is null && ThisActor is valid
	*	- Highlight ThisActor
	* C. LastActor is Valid && ThisActor is null
	*	- UnHilight LastActor
	* D. Both actor are valid, but LastActor != ThisActor
	*	- UnHilight LastActor, and Highlight ThisActor
	* E. Both actors are valid, and are the same actor
	*	- Do nothing
	*/
	
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//Case B
			ThisActor->HighlightActor();
		}
		else
		{
			//Case A
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			//Case C
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				//Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//Case E
			}
		}
	}
}

void AAscPlayerCharacterController::OnCameraZoomTriggered(const FInputActionValue& Value)
{
	if (IsValid(CameraRig))
	{
		CameraRig->AddZoomInput(Value.Get<float>());
	}
}

void AAscPlayerCharacterController::CreateCameraRig()
{
	if (!IsLocalController() || IsValid(CameraRig))
	{
		return;
	}

	if (!CameraRigClass)
	{
		UE_LOG(LogAscendenciaInput, Error, TEXT("Assign CameraRigClass in the controller Blueprint."));
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector InitialLocation = GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector;

	CameraRig = GetWorld()->SpawnActor<AAscCRPGCameraRig>(CameraRigClass, InitialLocation, FRotator::ZeroRotator, SpawnParameters);

	if (!IsValid(CameraRig))
	{
		UE_LOG(LogAscendenciaInput, Error, TEXT("Failed to spawn the camera rig."));
		return;
	}

	CameraRig->FocusOnPlayer();

	SetViewTarget(CameraRig);
}

void AAscPlayerCharacterController::UpdateCameraManager(
	float DeltaSeconds)
{
	if (IsLocalController() && IsValid(CameraRig))
	{
		CameraRig->UpdateCamera(
			DeltaSeconds,
			GetPawn());
	}

	Super::UpdateCameraManager(DeltaSeconds);
}

void AAscPlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
}

void AAscPlayerCharacterController::EndPlay(
	const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(CameraRig))
	{
		CameraRig->Destroy();
		CameraRig = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AAscPlayerCharacterController::OnCameraPanTriggered(
	const FInputActionValue& Value)
{
	if (IsValid(CameraRig))
	{
		CameraRig->SetPanInput(Value.Get<FVector2D>());
	}
}

void AAscPlayerCharacterController::OnCameraPanFinished()
{
	if (IsValid(CameraRig))
	{
		CameraRig->SetPanInput(FVector2D::ZeroVector);
	}
}

void AAscPlayerCharacterController::OnCameraFocusStarted()
{
	if (IsValid(CameraRig))
	{
		CameraRig->FocusOnPlayer();
	}
}

void AAscPlayerCharacterController::OnCameraRotateStarted()
{
	if (!IsLocalController() || !IsValid(CameraRig) || bIsRotatingCamera)
	{
		return;
	}

	float MouseX = 0.0f;
	float MouseY = 0.0f;

	bHasSavedCursorPosition = GetMousePosition(MouseX, MouseY);

	if (bHasSavedCursorPosition)
	{
		SavedCursorPosition = FVector2D(MouseX, MouseY);
	}

	bIsRotatingCamera = true;
	bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);
}

void AAscPlayerCharacterController::OnCameraRotateTriggered(
	const FInputActionValue& Value)
{
	if (!bIsRotatingCamera || !IsValid(CameraRig))
	{
		return;
	}

	CameraRig->AddRotationInput(Value.Get<float>());
}

void AAscPlayerCharacterController::OnCameraRotateFinished()
{
	if (!bIsRotatingCamera)
	{
		return;
	}

	bIsRotatingCamera = false;
	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	if (bHasSavedCursorPosition)
	{
		SetMouseLocation(FMath::RoundToInt(SavedCursorPosition.X), FMath::RoundToInt(SavedCursorPosition.Y));
	}

	bHasSavedCursorPosition = false;
}

