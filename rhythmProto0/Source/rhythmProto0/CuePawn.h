// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameEnums.h"
#include "Components/AudioComponent.h"
#include "CuePawn.generated.h"


USTRUCT(BlueprintType)
struct FAudio
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	UAudioComponent* punch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* dodge;
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
	// Sets default values for this pawn's properties
	ACuePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// whether the correct key is played in the current beat
	bool m_pressed = false;

	// for puching and dodging trigger
	bool m_dodged = false;
	bool m_punched = false;
	// current score
	float m_score = 0.0f;

	UPROPERTY(BlueprintReadOnly)
		bool m_onBeat = false;
	// number of correct keys played
	UPROPERTY(BlueprintReadOnly)
		int m_numCorrect = 0;
	//number of keys misplayed
	UPROPERTY(BlueprintReadOnly)
		int m_numMissed = 0;
	// target score to finish looping
	UPROPERTY(BlueprintReadOnly)
		int m_targetScore = 50;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//beep action
	void Punch();

	void Dodge();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void PressedOff();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void SetOnBeat(const bool &value);

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void OnBeatBegin();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void OnBeatEnd();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	void ResultScore();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool GetPressed()const { return m_pressed; };

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	bool OnSessionEnd();

	UFUNCTION(BlueprintCallable, Category = "Music Trigger")
	/// For forcasting incoming input alone
	/// _val - what type of cue to play
	void PlayCue(EInputType _val);

	UPROPERTY(EditAnywhere)
		USceneComponent* visualComponent;
	UPROPERTY(EditAnywhere)
		FAudio m_audio;
	UPROPERTY(BlueprintReadWrite)
		FInputEnums m_inputType;
};
