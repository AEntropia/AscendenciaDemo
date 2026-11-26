// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AscAbilitySystemComponent.h"
#include "AbilitySystem/AscAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAscAttributeSet* AscAttributeSet = CastChecked<UAscAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AscAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AscAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAscAttributeSet* AscAttributeSet = CastChecked<UAscAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AscAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AscAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	Cast<UAscAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
[](const FGameplayTagContainer& AssetTags)
	{
		
	}
	);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
