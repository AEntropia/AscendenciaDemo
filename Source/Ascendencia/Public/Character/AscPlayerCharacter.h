// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AscBaseCharacter.h"
#include "AscPlayerCharacter.generated.h"

class UParticleSystemComponent;

UCLASS()
class ASCENDENCIA_API AAscPlayerCharacter : public AAscBaseCharacter
{
	GENERATED_BODY()
	
public:
	AAscPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UParticleSystemComponent> LeftPower;
	
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UParticleSystemComponent> RightPower;
	
};
