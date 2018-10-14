// Fill out your copyright notice in the Description page of Project Settings.

#include "JsonData.h"
#include "CoreMinimal.h"

// Sets default values
bool FJsonData::JsonTest()
{

	FString JsonRaw = TEXT("{ \"exampleString\": \"Hello World\" }");

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*JsonRaw);
	
	//deserialize raw
	if (!FJsonSerializer::Deserialize(JsonReader, m_parsed) || !m_parsed.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CANNOT PARSE JSON FILE"));
		return false;
	}

	return true;
}

bool FJsonData::JsonInit(FString _path, FString _name)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	m_fileName = _name;
	m_filePath = _path;
	FString JsonRaw = "{ \"exampleString\": \"Hello World\" }";
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*m_filePath))
	{
		FFileHelper::LoadFileToString(JsonRaw, const_cast<TCHAR*>(*m_filePath));
	}
	else
	{
		UE_LOG(LogTemp, Warning,TEXT("JSON FILE DOES NOT EXIST or WRONG FILE PATH - %s"), *m_filePath)
		return false;
	}

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);
	//deserialize raw
	if (!FJsonSerializer::Deserialize(JsonReader, m_parsed) || !m_parsed.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CANNOT PARSE JSON FILE"));
		return false;
	}
	
	return true;
}
