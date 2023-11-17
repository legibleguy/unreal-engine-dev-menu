// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DevMenuFolder.generated.h"

USTRUCT(BlueprintType)
struct FDevMenuButton
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UserFriendlyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FunctionName;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DEVUI_API UDevMenuFolder : public UObject
{
	GENERATED_BODY()

	UDevMenuFolder();

public:

	UFUNCTION(BlueprintPure, Category = "DevMenu")
	int32 GetDepth() const { return Depth; }

	UFUNCTION(BlueprintPure, Category = "DevMenu")
	FString GetFolderName() const { return FolderName; }

	UFUNCTION(BlueprintPure, Category = "DevMenu")
	TArray<UDevMenuFolder*> GetSubFolders() const { return SubFolders; }

	UFUNCTION(BlueprintCallable, Category = "DevMenu")
	static bool TryMergeIntoFolder(UObject* WorldContext, UDevMenuFolder* ExistingFolder, const FString& NewFolderPath);

	UFUNCTION(BlueprintPure, Category = "DevMenu")
	static UDevMenuFolder* CreateFolderFromPath(UObject* WorldContext, const FString& FolderPath);

	UFUNCTION(BlueprintPure, Category = "DevMenu")
	bool HasSubFolder(const FString& SubfolderName, UDevMenuFolder*& OutSubfolder) const;

	UFUNCTION(BlueprintPure, Category = "DevMenu")
	bool HasButton(const FName& ButtonName, FDevMenuButton& OutButton) const;

	UFUNCTION(BlueprintPure, Category = "DevMenu")
	int32 GetNumSubElements() const { return SubFolders.Num() + Buttons.Num(); }

	UFUNCTION(BlueprintCallable, Category="DevMenu")
	bool AddButton(const FName ButtonName, const FString& ClassName, const FString& FunctionName);

	UFUNCTION(BlueprintPure, Category="DevMenu")
	TArray<FDevMenuButton> GetButtons() const { TArray<FDevMenuButton> Out; Buttons.GenerateValueArray(Out); return Out; }
	
private:

	UPROPERTY()
	int32 Depth;

	UPROPERTY()
	FString FolderName;

	UPROPERTY()
	TArray<UDevMenuFolder*> SubFolders;

	UPROPERTY()
	TMap<FName, FDevMenuButton> Buttons;

	void Init(UObject* WorldContext, FString InFolderName);
};
