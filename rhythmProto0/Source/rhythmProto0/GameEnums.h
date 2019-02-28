// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameEnums.generated.h"

// You can puch, dodge or whatever
UENUM(BlueprintType)
enum class EInputType : uint8
{
	PUNCH UMETA(DisplayName = "Punch"),
	DODGE UMETA(DisplayName = "Dodge"),
	OTHER UMETA(DisplayName = "Other"),
	PUNCH2 UMETA(DisplayName = "Punch 2"),
	DODGE2 UMETA(DisplayName = "Dodge 2"),
	NONE UMETA(DisplayName = "None")
};



USTRUCT(BlueprintType)
struct FInputEnums
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	TArray<EInputType> Input;
};