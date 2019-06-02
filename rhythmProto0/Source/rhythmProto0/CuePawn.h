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
	TArray<UAudioComponent*> punchCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> dodgeCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> punchSuccess;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> punchFail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> dodgeSuccess;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> dodgeFail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* finalPunch;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> PlayerPositiveGrunt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> PlayerNegativeGrunt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> EnemyPositiveGrunt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> EnemyNegativeGrunt;
*/
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
	UPROPERTY(BlueprintReadOnly)
	bool m_dodged;
	UPROPERTY(BlueprintReadOnly)
	bool m_punched;
	uint8 m_inputLimit;
	// current score
	float m_score;
	UPROPERTY(BlueprintReadWrite)
	bool m_isFinalPunch;
	UPROPERTY(BlueprintReadOnly)
	bool m_isFrameOpen;
	// number of correct keys played
	UPROPERTY(BlueprintReadOnly)
	int m_numCorrect;
	/// number of inputs correctly input in a row
	UPROPERTY(BlueprintReadOnly)
	int m_succStreak;
	/// stores the highest succ streak
	UPROPERTY(BlueprintReadOnly)
	int m_highestStreak;
	/// total number of input in a section
	/// extendable to be for the entire game , will decide on that later
	UPROPERTY(BlueprintReadOnly)
	int m_numInput;
	// target score to finish looping
	UPROPERTY(BlueprintReadOnly)
	float m_targetScore;

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
	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void punch();
	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void dodge();

	void setHighestStreak(const int &_streak);

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void resetState();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void onBeatBegin();
	/// checks if input was correct and sets m_isCorrect for later use to handle missed inputs 
	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool isInputCorrect();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void result();
	
	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool onGameEnd();

	///@return true - player has missed this beat
	///@return false - player played this beat correctly
	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool endOfInputChecks();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	/// For forcasting incoming input alone
	/// _val - what type of cue to play
	void playCue(const EInputType &_in, const uint8 &_i);

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void setInputIndex(const EInputType &_in, const uint8 &_i);

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void setIsFinalPunch( const bool &_val ) { m_isFinalPunch = _val;}

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool isCorrect() const { return m_isCorrect; };

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool isInputFrameOpen() const { return m_isFrameOpen; };

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	EInputType getInputType() const { return m_inputType.Input[0]; }

	/// This resets score and its total score factor;
	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void resetScore() 
	{ 
		m_score = 0.0f; 
		m_numInput = 0; 
		m_numCorrect = 0; 
		m_succStreak = 0;
		m_highestStreak = 0;
	}

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	float calculateScore() const;

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	float getScore() const { return m_score; }
	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	int getStreak() const { return m_succStreak; }

	
};
