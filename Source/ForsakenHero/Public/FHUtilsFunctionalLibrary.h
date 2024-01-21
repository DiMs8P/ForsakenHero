// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FHUtilsFunctionalLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FORSAKENHERO_API UFHUtilsFunctionalLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void AppendTextToFile(const FString& TextToAppend, const FString& FilePath);
};
