// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DevMenuFolder.h"
#include "UnrealClient.h"
#include "DevMenuBaseWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DEVUI_API UDevMenuBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="DevMenu")
	TArray<UDevMenuFolder*> GetMenuFolders() const { return MenuFolders; }

	UFUNCTION(BlueprintCallable, Category="DevMenu")
	bool AddMenuDataFromJSON(FString JsonFilePath);

	UFUNCTION(BlueprintPure, Category="DevMenu")
	static float GetDevMenuElementHeight() { return 20.f; }

	void OnViewportChanged(FViewport* ViewPort, uint32 val);

	UFUNCTION(BlueprintImplementableEvent, Category="DevMenu")
	void OnViewportChangedBP(FVector2D NewSize, int32 val);

protected:
	// Doing setup in the C++ constructor is not as
	// useful as using NativeConstruct.
	virtual void NativeConstruct() override;

private:
	void DebugConfigJson(FString JsonString);

	UPROPERTY()
	TArray<FDevMenuButton> MenuButtons;

	UPROPERTY()
	TArray<UDevMenuFolder*> MenuFolders;

	UDevMenuFolder* FindFolderByRootName(const FString& RootName) const;

};
