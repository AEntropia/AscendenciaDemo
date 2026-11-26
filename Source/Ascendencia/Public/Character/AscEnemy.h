// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AscBaseCharacter.h"
#include "Interaction/SelectableInterface.h"
#include "AscEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ASCENDENCIA_API AAscEnemy : public AAscBaseCharacter, public ISelectableInterface
{
	GENERATED_BODY()
	
public:
	AAscEnemy();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	void CustomDepthOn();
	void CustomDepthOff();
	void WeaponsConstruction();
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
