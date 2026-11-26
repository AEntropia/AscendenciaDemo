#include "Character/AscPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AscAbilitySystemComponent.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/AscPlayerCharacterController.h"
#include "Player/AscPlayerState.h"
#include "UI/HUD/AscHUD.h"

AAscPlayerCharacter::AAscPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	
	Movement->bOrientRotationToMovement = true;
	Movement->bUseControllerDesiredRotation = false;
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	Movement->MaxWalkSpeed = 400.0f;
	
	Movement->GetNavMovementProperties()->bUseAccelerationForPaths = true;

	LeftPower = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftPower"));
	LeftPower->SetupAttachment(GetMesh(), FName("LeftPowerSocket"));

	RightPower = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightPower"));
	RightPower->SetupAttachment(GetMesh(), FName("RightPowerSocket"));
}

void AAscPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AAscPlayerState* AscPlayerState = GetPlayerState<AAscPlayerState>();
	check(AscPlayerState);
	AscPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AscPlayerState, this);
	Cast<UAscAbilitySystemComponent>(AscPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AscPlayerState->GetAbilitySystemComponent();
	AttributeSet = AscPlayerState->GetAttributeSet();
	
	if (AAscPlayerCharacterController* AscPlayerController = Cast<AAscPlayerCharacterController>(GetController()))
	{
		if (AAscHUD* AuraHUD = Cast<AAscHUD>(AscPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AscPlayerController, AscPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
