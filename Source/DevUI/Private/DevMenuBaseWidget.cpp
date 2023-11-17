// Fill out your copyright notice in the Description page of Project Settings.


#include "DevMenuBaseWidget.h"

#include <ObjectArray.h>
#include "DevMenuFunctionLib.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DevMenuInterface.h"
#include "Dom/JsonObject.h"


void UDevMenuBaseWidget::OnViewportChanged(FViewport* ViewPort, uint32 val)
{
	//get all widgets that implement DevMenuInterface and send the OnViewportChanged event
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsWithInterface(this, Widgets, UDevMenuInterface::StaticClass(), false);

	for(auto Widget : Widgets)
	{
		IDevMenuInterface::Execute_OnViewportChanged(Widget, ViewPort->GetSizeXY(), val);
	}
	
}

void UDevMenuBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FViewport::ViewportResizedEvent.AddUObject(this, &UDevMenuBaseWidget::OnViewportChanged);

	//Get the absolute path to this plugin
	FString AbsPluginPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir());
	UE_LOG(LogTemp, Warning, TEXT("AbsPluginPath: %s"), *AbsPluginPath);
	
	FString PluginPath = FPaths::Combine(AbsPluginPath, TEXT("DevUI/Config"));
	UE_LOG(LogTemp, Warning, TEXT("PluginPath: %s"), *PluginPath);

	//load DevMenuFunctions.json
	const FString JsonFilePath = FPaths::Combine(PluginPath, TEXT("DevMenuFunctions.json"));
	UE_LOG(LogTemp, Warning, TEXT("JsonFilePath: %s"), *JsonFilePath);
	const bool readStatus = AddMenuDataFromJSON(JsonFilePath);
	UE_LOG(LogTemp, Warning, TEXT("Dev Menu readStatus: %s"), readStatus ? TEXT("true") : TEXT("false"));
	

	for(UDevMenuFolder* Folder : MenuFolders)
	{
		if(this->Implements<UDevMenuInterface>())
		{
			IDevMenuInterface::Execute_CreateFolderButton(this, Folder);
		}
	}

	for(FDevMenuButton Button : MenuButtons)
	{
		if(this->Implements<UDevMenuInterface>())
		{
			IDevMenuInterface::Execute_CreateButton(this, Button);
		}
	}
}
/*
*{
"functions": {
"Test Log": {
"Path": "Test",
"ClassName": "MyGameLib",
"FunctionName": "TestLog"
},
"Blueprint Log": {
"Path": "Test2",
"ClassName": "BPLib_C",
"FunctionName": "MyBPFun"
}
}
}
*/
void UDevMenuBaseWidget::DebugConfigJson(FString JsonFilePath)
{
	//load json
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *JsonFilePath);
	UE_LOG(LogTemp, Warning, TEXT("JsonString: %s"), *JsonString);

	//Create a json object to store the information from the json string
	//The json reader is used to deserialize the json object later on
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		auto functionsObj = JsonObject->GetObjectField("functions");
		TArray<FString> keys;
		functionsObj->Values.GetKeys(keys);

		for(auto key : keys)
		{
			auto functionObj = functionsObj->GetObjectField(key);
			auto buttonName = key;
			auto path = functionObj->GetStringField("Path");
			auto className = functionObj->GetStringField("ClassName");
			auto functionName = functionObj->GetStringField("FunctionName");

			UE_LOG(LogTemp, Warning, TEXT("ButtonName: %s"), *buttonName);
			UE_LOG(LogTemp, Warning, TEXT("Path: %s"), *path);
			UE_LOG(LogTemp, Warning, TEXT("ClassName: %s"), *className);
			UE_LOG(LogTemp, Warning, TEXT("FunctionName: %s"), *functionName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("JsonArray is empty"));
	}
	
}

UDevMenuFolder* UDevMenuBaseWidget::FindFolderByRootName(const FString& RootName) const
{
	for( auto Folder : MenuFolders)
	{
		if (Folder->GetFolderName() == RootName)
			return Folder;
	}
	return NULL;
}

bool UDevMenuBaseWidget::AddMenuDataFromJSON(FString JsonFilePath)
{

	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *JsonFilePath);
	
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("JsonArray is valid"));
		auto functionsObj = JsonObject->GetObjectField("functions");
		TArray<FString> keys;
		functionsObj->Values.GetKeys(keys);
	
		for(auto key : keys)
		{
			auto functionObj = functionsObj->GetObjectField(key);
			auto buttonName = key;
			auto path = functionObj->GetStringField("Path");
			auto className = functionObj->GetStringField("ClassName");
			auto functionName = functionObj->GetStringField("FunctionName");

			if(path.IsEmpty())
			{
				FDevMenuButton NewButton;
				NewButton.ClassName = className;
				NewButton.FunctionName = functionName;
				NewButton.UserFriendlyName = buttonName;

				MenuButtons.Add(NewButton);
			}
			else
			{
				UDevMenuFolder* RootFolderRef;
                
                TArray<FString> PathAr;
                path.ParseIntoArray(PathAr, TEXT("/"), true);
    
                UDevMenuFolder* MatchingRootFolder = FindFolderByRootName(PathAr[0]);
    
                if(MatchingRootFolder && UDevMenuFolder::TryMergeIntoFolder(this, MatchingRootFolder, path))
                {
                	RootFolderRef = MatchingRootFolder;
                }
                else
                {
                	UDevMenuFolder* NewRootFolder = UDevMenuFolder::CreateFolderFromPath(this, path);
                	MenuFolders.Add(NewRootFolder);
                	RootFolderRef = NewRootFolder;
                }
    
                for(int32 i = 1; i < PathAr.Num(); i++)
                	RootFolderRef->HasSubFolder(PathAr[i], RootFolderRef);
    
                RootFolderRef->AddButton(FName(*buttonName), className, functionName);
			}
		}
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("JsonArray is empty"));
		return false;
	}
}
