// Fill out your copyright notice in the Description page of Project Settings.


#include "FHUtilsFunctionalLibrary.h"

void UFHUtilsFunctionalLibrary::AppendTextToFile(const FString& TextToAppend, const FString& FilePath)
{
	FFileHelper::SaveStringToFile(FString::Printf(TEXT("%s\n"), *TextToAppend), *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

