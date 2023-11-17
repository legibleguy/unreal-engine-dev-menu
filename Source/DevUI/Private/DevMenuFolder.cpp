// Fill out your copyright notice in the Description page of Project Settings.


#include "DevMenuFolder.h"

UDevMenuFolder::UDevMenuFolder()
{
	
}

bool UDevMenuFolder::TryMergeIntoFolder(UObject* WorldContext, UDevMenuFolder* ExistingFolder, const FString& NewFolderPath)
{
	const auto newFolder = UDevMenuFolder::CreateFolderFromPath(WorldContext, NewFolderPath);

	if (newFolder->GetFolderName() != ExistingFolder->GetFolderName())
	{
		UE_LOG(LogTemp, Warning, TEXT("TryMergeIntoFolder: Folder names do not match. ExistingFolder: %s, newFolder: %s"), *ExistingFolder->GetFolderName(), *newFolder->GetFolderName());
		return false;
	}

	if (newFolder->GetSubFolders().Num() == 0)
	{
		return true;
	}
			
	auto currExistingFolder = ExistingFolder;
	auto currNewFolder = newFolder;
	while(currExistingFolder->HasSubFolder(currNewFolder->GetSubFolders()[0]->GetFolderName(), currExistingFolder))
	{
		currNewFolder = currNewFolder->GetSubFolders()[0];
		if(currNewFolder->GetSubFolders().Num() == 0)
		{
			return true;
		}
	}
	currExistingFolder->SubFolders.Add(currNewFolder->GetSubFolders()[0]);
	return true;
}

UDevMenuFolder* UDevMenuFolder::CreateFolderFromPath(UObject* WorldContext, const FString& FolderPath)
{
	UDevMenuFolder* NewFolder = NewObject<UDevMenuFolder>(WorldContext, UDevMenuFolder::StaticClass());
	NewFolder->Init(WorldContext, FolderPath);
	return NewFolder;
}

bool UDevMenuFolder::HasSubFolder(const FString& SubfolderName, UDevMenuFolder*& OutSubfolder) const
{
	for(auto& subfolder : SubFolders)
	{
		if(subfolder->GetFolderName() == SubfolderName)
		{
			OutSubfolder = subfolder;
			return true;
		}
	}
	return false;
}

bool UDevMenuFolder::HasButton(const FName& ButtonName, FDevMenuButton& OutButton) const
{
	for(auto& button : Buttons)
	{
		if(button.Key == ButtonName)
		{
			OutButton = button.Value;
			return true;
		}
	}
	return false;
}

bool UDevMenuFolder::AddButton(const FName ButtonName, const FString& ClassName, const FString& FunctionName)
{
	FDevMenuButton NewButton;
	NewButton.ClassName = ClassName;
	NewButton.FunctionName = FunctionName;
	NewButton.UserFriendlyName = ButtonName.ToString();

	if(HasButton(ButtonName, NewButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("AddButton: Duplicate buttons detected. ButtonName: %s, keeping the original. Original class: %s, function: %s"), *ButtonName.ToString(), *NewButton.ClassName, *NewButton.FunctionName);
		return false;
	}
	
	Buttons.Add(ButtonName, NewButton);
	return true;
}

void UDevMenuFolder::Init(UObject* WorldContext, FString InFolderName)
{
	//Check if the name conntains /
	if(InFolderName.Contains("/"))
	{
		UE_LOG(LogTemp, Warning, TEXT("InFolderName contains /. Splitting into multiple folders."));
		
		TArray<FString> SplitString;
		InFolderName.ParseIntoArray(SplitString, TEXT("/"), true);

		FolderName = SplitString[0];
		auto lastFolderRef = this;

		for(int32 i =1; i < SplitString.Num(); i++)
		{
			auto newFolder = NewObject<UDevMenuFolder>(WorldContext, UDevMenuFolder::StaticClass());
			newFolder->Depth = i;
			newFolder->FolderName = SplitString[i];
			lastFolderRef->SubFolders.Add(newFolder);
			lastFolderRef = newFolder;
		}
	}
	else
	{
		FolderName = InFolderName;
	}
}
