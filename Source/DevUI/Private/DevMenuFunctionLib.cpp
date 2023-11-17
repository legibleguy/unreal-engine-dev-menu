// Fill out your copyright notice in the Description page of Project Settings.


#include "DevMenuFunctionLib.h"
#include "Misc/OutputDeviceNull.h"
#include "Dom/JsonObject.h"
#include "Kismet/GameplayStatics.h"
#include "DevMenuFolder.h"
#include "DevMenuFunctionLib.h"
#include "DevMenuBaseWidget.h"
#include "Blueprint/UserWidget.h"

bool UDevMenuFunctionLib::TriggerFunctionByName(UObject* World, FString ClassName, FName FunctionName, const TArray<FString> Parameters)
{
    UE_LOG(LogTemp, Warning, TEXT("TriggerFunctionByName"));

    //load class
    UClass* TargetClass = FindObject<UClass>(ANY_PACKAGE, *ClassName);

    if (TargetClass == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetClass is nullptr"));
        return false;
    }
    else
    {
        FString* FoundClassName = new FString(TargetClass->GetName());
        UE_LOG(LogTemp, Warning, TEXT("FoundClassName: %s"), **FoundClassName);

        //create instance
        UObject* TargetObject = NewObject<UObject>(World, TargetClass);
        
        if(TargetObject)
        {
            FString Command = FunctionName.ToString();
            for (FString Param : Parameters)
                Command += (" " + Param);
            
            FOutputDeviceNull Ar;
	        return TargetObject->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);
        }
        else
            return false;
    }
}

void UDevMenuFunctionLib::TurnDevMenuOn(UObject* World)
{
    //Load DevMenu class UMG class and add it to the viewport
    //path /DevUI/UI/UMG_DevMenu.UMG_DevMenu
    const UClass* DevMenuClass = LoadClass<UUserWidget>(NULL, TEXT("/Script/UMGEditor.WidgetBlueprint'/DevUI/UI/UMG_DevMenu.UMG_DevMenu_C'"));

    if (DevMenuClass == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("DevMenuClass is nullptr"));
        return;
    }
    else
    {
        FString* FoundClassName = new FString(DevMenuClass->GetName());
        UE_LOG(LogTemp, Warning, TEXT("FoundClassName: %s"), **FoundClassName);

        //create widget instance
        

        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        if(PlayerController)
        {

            auto asUserWidget = DevMenuClass->GetDefaultObject<UUserWidget>();
            UUserWidget* DevMenuWidget = CreateWidget<UUserWidget>(PlayerController, asUserWidget->GetClass(), FName("DevMenuWidget"));
            //Add DevMenu to viewport
            DevMenuWidget->AddToViewport();

            //Set Input Mode
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(DevMenuWidget->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;
        }

    }
}
