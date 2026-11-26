// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AscPlayerCharacterController.generated.h"

class ISelectableInterface;
class UInputMappingContext;
class UInputAction;
class AAscCRPGCameraRig;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class ASCENDENCIA_API AAscPlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	AAscPlayerCharacterController();
	virtual void UpdateCameraManager(float DeltaSeconds) override;
	
	virtual void PlayerTick(float DeltaTime) override;
protected:
    	virtual void BeginPlay() override;
		virtual void SetupInputComponent() override;
	virtual void EndPlay(
	const EEndPlayReason::Type EndPlayReason) override;
    	
private:
    	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> GameplayMappingContext = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ClickMoveAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationProjectionExtent = FVector(50.f, 50.f, 150.f);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> CameraPanAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> CameraFocusAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<AAscCRPGCameraRig> CameraRigClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> CameraZoomAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> CameraRotateHoldAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> CameraRotateAction = nullptr;

	bool bIsRotatingCamera = false;

	FVector2D SavedCursorPosition = FVector2D::ZeroVector;
	bool bHasSavedCursorPosition = false;

	void OnCameraRotateStarted();
	void OnCameraRotateFinished();

	void OnCameraRotateTriggered(const FInputActionValue& Value);

	void OnCameraZoomTriggered(const FInputActionValue& Value);

	UPROPERTY(Transient)
	TObjectPtr<AAscCRPGCameraRig> CameraRig = nullptr;
	
	void CreateCameraRig();

	void OnCameraPanTriggered(const FInputActionValue& Value);
	void OnCameraPanFinished();
	void OnCameraFocusStarted();
	
	void OnClickMoveStarted();
	bool TryGetMoveDestination(FVector& OutDestination) const;

	//Highlight
	void CursorTrace();
	TScriptInterface<ISelectableInterface> LastActor;
	TScriptInterface<ISelectableInterface> ThisActor;
	
};
