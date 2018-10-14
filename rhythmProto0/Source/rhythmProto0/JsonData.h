// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "Json.h"
#include "JsonData.generated.h"


USTRUCT(BlueprintType)
struct FJsonData
{
	GENERATED_BODY()
	/// path of json file
	FString m_filePath;
	///
	FString m_fileName;
	/// deserialized json data
	//TSharedPtr<FJsonObject> m_parsed = MakeShared<FJsonObject>(FJsonObject());
	TSharedPtr<FJsonObject> m_parsed;
	// Sets default values for this actor's properties
	bool JsonTest();
	// Sets default values for this component's properties
	bool JsonInit(FString _path, FString _name);

};
