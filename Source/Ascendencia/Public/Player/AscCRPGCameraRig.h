#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AscCRPGCameraRig.generated.h"

class APawn;
class UCameraComponent;
class USceneComponent;
class USpringArmComponent;

UCLASS()
class ASCENDENCIA_API AAscCRPGCameraRig : public AActor
{
	GENERATED_BODY()
	
public:	
	AAscCRPGCameraRig();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	void SetPanInput(const FVector2D& Input);
	void FocusOnPlayer();
	void UpdateCamera(float DeltaSeconds, const APawn* FollowTarget);
	void AddZoomInput(float AxisValue);
	void AddRotationInput(float MouseDelta);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera|Movement", meta = (ClampMin = "0.0"))
	float PanSpeed = 1500.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera|View")
	FRotator DefaultViewRotation = FRotator(-55.0f, 45.0f, 0.0f);
	
	UPROPERTY(
	EditDefaultsOnly,
	Category = "Camera|Zoom",
	meta = (ClampMin = "1.0"))
	float MinZoomDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Zoom", meta = (ClampMin = "1.0"))
	float MaxZoomDistance = 2200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Zoom", meta = (ClampMin = "0.0"))
	float ZoomStep = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Zoom", meta = (ClampMin = "0.1"))
	float ZoomInterpSpeed = 10.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera|Rotation", meta = (ClampMin = "0.0"))
	float RotationSensitivity = 0.2f;

	float DesiredArmLength = 0.0f;
	
	FVector2D PanInput = FVector2D::ZeroVector;

	bool bIsFollowingPlayer = true;

};
