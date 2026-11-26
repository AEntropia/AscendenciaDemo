// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AscEnemy.h"

#include "AbilitySystem/AscAbilitySystemComponent.h"
#include "AbilitySystem/AscAttributeSet.h"
#include "Ascendencia/Ascendencia.h"


AAscEnemy::AAscEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponsConstruction();
	
	AbilitySystemComponent = CreateDefaultSubobject<UAscAbilitySystemComponent>("AbilitySystemComponent");
	
	AttributeSet = CreateDefaultSubobject<UAscAttributeSet>("AttributeSet");
}

void AAscEnemy::HighlightActor()
{
	CustomDepthOn();
}

void AAscEnemy::UnHighlightActor()
{
	CustomDepthOff();
}

void AAscEnemy::CustomDepthOn()
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

void AAscEnemy::CustomDepthOff()
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

void AAscEnemy::WeaponsConstruction()
{
	if (RightWeaponMesh)
	{
		RightWeaponMesh->SetRenderCustomDepth(false);
		RightWeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
	if (LeftWeaponMesh)
	{
		LeftWeaponMesh->SetRenderCustomDepth(false);
		LeftWeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AAscEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
}

void AAscEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAscAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
