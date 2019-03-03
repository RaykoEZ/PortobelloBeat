// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameEnums.h"
#include "JsonData.h"
#include "Components/AudioComponent.h"
#include "CuePawn.generated.h"


USTRUCT(BlueprintType)
struct FAudio
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	TArray<UAudioComponent*> punch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> dodge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* success;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* fail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* music;

};


UCLASS()
class RHYTHMPROTO0_API ACuePawn : public APawn
{
	GENERATED_BODY()

public:
	/// Sets default values for this pawn's properties
	ACuePawn();

protected:
	/// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void setIsFrameOpen(const bool &value);

	/// whether the correct key is played in the current beat
	bool m_isCorrect;

	/// json data for event scripting
	//FJsonData m_json;
	/// for puching and dodging trigger
	bool m_dodged;
	bool m_punched;
	uint8 m_inputLimit;
	// current score
	float m_score;

	UPROPERTY(BlueprintReadOnly)
		bool m_isFrameOpen;
	// number of correct keys played
	UPROPERTY(BlueprintReadOnly)
		int m_numCorrect;
	//number of keys misplayed
	UPROPERTY(BlueprintReadOnly)
		int m_numMissed;
	// target score to finish looping
	UPROPERTY(BlueprintReadOnly)
		int m_targetScore;
	UPROPERTY(BlueprintReadOnly)
		TArray<EInputType> m_sequence;
	UPROPERTY(EditAnywhere)
		FAudio m_audio;
	UPROPERTY(BlueprintReadWrite)
		FInputEnums m_inputType;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//beep action
	void punch();

	void dodge();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void resetState();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void onBeatBegin();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool isInputCorrect();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void result();
	
	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool onGameEnd();

	///@return true - player has missed this beat
	///@return false - player played this beat correctly
	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool onMissed();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	/// For forcasting incoming input alone
	/// _val - what type of cue to play
	void playCue(const EInputType &_in, const uint8 &_i);

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void setInputIndex(const EInputType &_in, const uint8 &_i);

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool isCorrect() const { return m_isCorrect; };

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool isInputFrameOpen() const { return m_isFrameOpen; };

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	EInputType getInputType() const { return m_inputType.Input[0]; }


};
