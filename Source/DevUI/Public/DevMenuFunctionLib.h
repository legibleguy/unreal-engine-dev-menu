// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DevMenuFunctionLib.generated.h"

//ENUM for different positions that the dev menu widget can be anchored to
UENUM(BlueprintType)
enum class EDevMenuAnchorPosition : uint8
{
	Top,
	Bottom,
	Left,
	Right,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};
 
UCLASS()
class DEVUI_API UDevMenuFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "DevMenu")
	static bool TriggerFunctionByName(UObject* World, FString ClassName, FName FunctionName, const TArray<FString> Parameters);

	UFUNCTION(BlueprintCallable, Category = "DevMenu")
	static void TurnDevMenuOn(UObject* World);
};
