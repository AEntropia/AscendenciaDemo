// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AscPlayerState.h"

#include "AbilitySystem/AscAbilitySystemComponent.h"
#include "AbilitySystem/AscAttributeSet.h"

AAscPlayerState::AAscPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAscAbilitySystemComponent>("AbilitySystemComponent");
	
	AttributeSet = CreateDefaultSubobject<UAscAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AAscPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
