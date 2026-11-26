// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AscCompanion.h"

#include "AbilitySystem/AscAbilitySystemComponent.h"
#include "AbilitySystem/AscAttributeSet.h"
#include "Ascendencia/Ascendencia.h"

AAscCompanion::AAscCompanion()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	WeaponsConstruction();
	
	AbilitySystemComponent = CreateDefaultSubobject<UAscAbilitySystemComponent>("AbilitySystemComponent");
    
    AttributeSet = CreateDefaultSubobject<UAscAttributeSet>("AttributeSet");
}

void AAscCompanion::HighlightActor()
{
	CustomDepthOn();
}

void AAscCompanion::UnHighlightActor()
{
	CustomDepthOff();
}

void AAscCompanion::CustomDepthOn()
{
	GetMesh()->SetRenderCustomDepth(true);
	if (RightWeaponMesh)
	{
		RightWeaponMesh->SetRenderCustomDepth(true);
	}
	if (LeftWeaponMesh)
	{
		LeftWeaponMesh->SetRenderCustomDepth(true);
	}
}

void AAscCompanion::CustomDepthOff()
{
	GetMesh()->SetRenderCustomDepth(false);
	if (RightWeaponMesh)
	{
		RightWeaponMesh->SetRenderCustomDepth(false);
	}
	if (LeftWeaponMesh)
	{
		LeftWeaponMesh->SetRenderCustomDepth(false);
	}
}

void AAscCompanion::WeaponsConstruction()
{
	if (RightWeaponMesh)
	{
		RightWeaponMesh->SetRenderCustomDepth(false);
		RightWeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	}
	if (LeftWeaponMesh)
	{
		LeftWeaponMesh->SetRenderCustomDepth(false);
		LeftWeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	}
}

void AAscCompanion::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
}

void AAscCompanion::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAscAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
