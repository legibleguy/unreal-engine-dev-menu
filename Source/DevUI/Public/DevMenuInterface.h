// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuFolder.h"
#include "UObject/Interface.h"
#include "DevMenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDevMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEVUI_API IDevMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//Event that is triggered when a new submenu is requested
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DevMenu")
	void OnSubmenuRequested(const UDevMenuFolder* Submenu);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DevMenu")
	void ClearSubmenu(const int32 HigherThanDepth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DevMenu")
	void OnViewportChanged(FVector2D NewSize, int32 val);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DevMenu")
	void CreateFolderButton(UDevMenuFolder* Folder);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DevMenu")
	void CreateButton(const FDevMenuButton& Button);
};
