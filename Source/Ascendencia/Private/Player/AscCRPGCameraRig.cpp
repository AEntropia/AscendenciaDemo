#include "Player/AscCRPGCameraRig.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"

AAscCRPGCameraRig::AAscCRPGCameraRig()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SceneRoot);
	SpringArm->TargetArmLength = 1200.0f;
	SpringArm->SetUsingAbsoluteRotation(true);
	
	// Evita que obstáculos encurtem o braço durante o pan.
	SpringArm->bDoCollisionTest = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

}

void AAscCRPGCameraRig::BeginPlay()
{
	Super::BeginPlay();

	// Garante um intervalo válido.
	MaxZoomDistance = FMath::Max(MinZoomDistance, MaxZoomDistance);

	// Usa a distância configurada no SpringArm como início.
	DesiredArmLength = FMath::Clamp(SpringArm->TargetArmLength, MinZoomDistance, MaxZoomDistance);

	SpringArm->TargetArmLength = DesiredArmLength;
}

void AAscCRPGCameraRig::AddZoomInput(float AxisValue)
{
	DesiredArmLength = FMath::Clamp(DesiredArmLength - AxisValue * ZoomStep, MinZoomDistance, MaxZoomDistance);
}

void AAscCRPGCameraRig::AddRotationInput(float MouseDelta)
{
	FRotator Rotation = SpringArm->GetComponentRotation();

	Rotation.Yaw = FRotator::NormalizeAxis(Rotation.Yaw + MouseDelta * RotationSensitivity);

	SpringArm->SetWorldRotation(Rotation);
}

void AAscCRPGCameraRig::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	SpringArm->SetWorldRotation(DefaultViewRotation);
}

void AAscCRPGCameraRig::SetPanInput(const FVector2D& Input)
{
	PanInput = Input.GetClampedToMaxSize(1.0f);
}

void AAscCRPGCameraRig::FocusOnPlayer()
{
	bIsFollowingPlayer = true;
	
	SpringArm->SetWorldRotation(DefaultViewRotation);
}

void AAscCRPGCameraRig::UpdateCamera(float DeltaSeconds, const APawn* FollowTarget)
{
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength,DesiredArmLength, DeltaSeconds, ZoomInterpSpeed);
	if (!PanInput.IsNearlyZero())
	{
		bIsFollowingPlayer = false;
		
		const FRotator YawRotation = FRotator(0.0f, SpringArm->GetComponentRotation().Yaw, 0.0f);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector Direction = Forward * PanInput.Y + Right * PanInput.X;
		const FVector Displacement = Direction * PanSpeed * DeltaSeconds;
		
		AddActorWorldOffset(Displacement, false);
		return;
	}
	
	if (bIsFollowingPlayer && IsValid(FollowTarget))
	{
		SetActorLocation(FollowTarget->GetActorLocation());
	}
}

