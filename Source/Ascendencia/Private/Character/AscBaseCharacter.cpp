#include "Character/AscBaseCharacter.h"

AAscBaseCharacter::AAscBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	LeftWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("LeftWeapon");
	LeftWeaponMesh->SetupAttachment(GetMesh(), FName("LeftWeaponSocket"));
	LeftWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RightWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("RightWeapon");
	RightWeaponMesh->SetupAttachment(GetMesh(), FName("RightWeaponSocket"));
	RightWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

UAbilitySystemComponent* AAscBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAscBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAscBaseCharacter::InitAbilityActorInfo()
{
}

