// Fill out your copyright notice in the Description page of Project Settings.

#include "CuePawn.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Camera/CameraComponent.h"
//#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Core/Public/Misc/Paths.h"
// Sets default values
ACuePawn::ACuePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	// Json test
	//test filename
	FString filename = "testSequence.json";
	FString ContentPath = FPaths::ProjectContentDir() + "/Data/";
	if(m_json.JsonInit( ContentPath + filename, filename))
	{
		auto arrayData = m_json.m_parsed->GetArrayField("Sequence");
		// test out json array output
		for(auto i= arrayData.CreateConstIterator(); i; ++i)
		{
			// output A.I sequence for test
			auto item = arrayData[i.GetIndex()].Get();
			m_sequence.Add(static_cast<EInputType>((int)item->AsNumber()));
			//UE_LOG(LogTemp, Log, TEXT("JSON PATH TEST - %s"), *item->AsString());
		}
		for(auto i = m_sequence.CreateConstIterator(); i; ++i)
		{
			UE_LOG(LogTemp, Log, TEXT("VERIFY SEQUENCE - %d"), static_cast<int>(m_sequence[i.GetIndex()]));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Data parse failed"));
	}

	//soundcue = cue.Object;
	m_audio.punch = CreateDefaultSubobject<UAudioComponent>(TEXT("punch sound"));
	m_audio.punch->bAutoActivate = false;
	m_audio.punch->SetupAttachment(RootComponent);
	// I want the sound to come from slighty in front of the pawn.
	m_audio.punch->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	m_audio.dodge = CreateDefaultSubobject<UAudioComponent>(TEXT("dodge sound"));
	m_audio.dodge->bAutoActivate = false;
	m_audio.dodge->SetupAttachment(RootComponent);
	// I want the sound to come from slighty in front of the pawn.
	m_audio.dodge->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	m_audio.success = CreateDefaultSubobject<UAudioComponent>(TEXT("yes sound"));
	m_audio.success->bAutoActivate = false;
	m_audio.success->SetupAttachment(RootComponent);
	// I want the sound to come from slighty in front of the pawn.
	m_audio.success->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	m_audio.fail = CreateDefaultSubobject<UAudioComponent>(TEXT("no sound"));
	m_audio.fail->bAutoActivate = false;
	m_audio.fail->SetupAttachment(RootComponent);
	// I want the sound to come from slighty in front of the pawn.
	m_audio.fail->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));


	m_audio.music = CreateDefaultSubobject<UAudioComponent>(TEXT("music"));
	m_audio.music->bAutoActivate = false;
	m_audio.music->SetupAttachment(RootComponent);
	// I want the sound to come from slighty in front of the pawn.
	m_audio.music->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	m_inputType.Input = EInputType::NONE;

	m_isFrameOpen = false;
	m_dodged = false;
	m_punched = false;
	m_isCorrect = false;
	m_score = 0.0f;
	m_numCorrect = 0;
	m_numMissed = 0;
	m_targetScore = 70;
	m_sequenceIdx = 0;
}

void ACuePawn::BeginPlay()
{
	m_audio.music->Play();

	Super::BeginPlay();
}


// Called every frame
void ACuePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACuePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Beep", IE_Pressed, this, &ACuePawn::punch);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ACuePawn::dodge);
}

void ACuePawn::punch()
{
	//m_audio.punch->Play();
	if (m_isFrameOpen && m_inputType.Input == EInputType::PUNCH)
	{
		m_punched = true;
	}
	else 
	{ 
		m_punched = false;
	}

	if(isInputCorrect())
	{
		m_audio.success->Play();
	}
	else
	{
		m_audio.fail->Play();

	}

	setIsFrameOpen(false);
}

void ACuePawn::dodge()
{
	//m_audio.dodge->Play();
	if (m_isFrameOpen && m_inputType.Input == EInputType::DODGE)
	{
		m_dodged = true;
	}
	else
	{
		m_dodged = false;
	}

	if (isInputCorrect())
	{
		m_audio.success->Play();
	}
	else
	{
		m_audio.fail->Play();

	}

	setIsFrameOpen(false);
}
void ACuePawn::resetState()
{
	//reset all triggers
	m_isCorrect = false;
	m_dodged = false;
	m_punched = false;
	m_inputType.Input = EInputType::NONE;
	if (m_sequenceIdx > m_sequence.Num()-1)
	{
		//UE_LOG(LogTemp, Warning, TEXT("reset sequence index"));
		m_sequenceIdx = 0;
	}
}

bool ACuePawn::isInputCorrect()
{
	// you dodge or puch but not both, just in case
	m_isCorrect = m_dodged ^ m_punched;
	bool ret = false;
	if (m_isCorrect)
	{
		ret = true;	
	}
	else 
	{
		ret = false; 	
	}
	
	//placeholder event for missed beat
	UE_LOG(LogTemp, Log, TEXT("Key pressed? - %s"), (m_isCorrect ? TEXT("true") : TEXT("false")));

	return ret;
}


void ACuePawn::result()
{
	UE_LOG(LogTemp, Warning, TEXT("Your result - %f, %d correct, %d missed"), m_score, m_numCorrect,m_numMissed);	

}

bool ACuePawn::onGameEnd()
{
	//score = percentage correct
	float total = m_numCorrect + m_numMissed;
	m_score = 100 * m_numCorrect / total;

	// stop looping sequencer if target reached
	if ((int) m_score >= m_targetScore)
	{

		//show score and whatever
		m_audio.success->Play();
		//yes, stop looping
		return true;
	}
	result();
	//keep looping
	return false;
}

void ACuePawn::setIsFrameOpen(const bool & value)
{
	m_isFrameOpen = value;
	UE_LOG(LogTemp, Log, TEXT("NOW!"));
}

void ACuePawn::onBeatBegin()
{
	//prompt before input
	switch(m_inputType.Input)
	{
		//punch
		case EInputType::PUNCH:
		{			
			UE_LOG(LogTemp, Warning, TEXT("PUNCH!!!!!"));
			break;
		}
		case EInputType::DODGE:
		{			
			UE_LOG(LogTemp, Warning, TEXT("DODGE!!!"));
			break;
		}
		default: break;
	}
	//begin to accept player input
	setIsFrameOpen(true);
}


void ACuePawn::playCue(const EInputType &_in) 
{
	
	//EInputType val = m_sequence[m_sequenceIdx];

	switch (_in)
	{
		//punch
	case EInputType::PUNCH:
	{
		m_audio.punch->Play();
		break;
	}
	case EInputType::DODGE:
	{

		m_audio.dodge->Play();
		break;
	}
	default: break;
	}
	//begin to accept player input
}

void ACuePawn::incrementInputIndex()
{
	m_inputType.Input = m_sequence[m_sequenceIdx];
	//UE_LOG(LogTemp, Warning, TEXT("Getting Input index: %d"), m_sequenceIdx);
	++m_sequenceIdx;
}

/// Called during end of beat
bool ACuePawn::onMissed()
{
	bool ret;
	if(!m_isCorrect)
	{
		// Missed beat if key isn't pressed at the time
		++m_numMissed;
		m_audio.fail->Play();
		ret = true;
	}
	else
	{
		++m_numCorrect;
		ret = false;
	}
	//Result();
	return ret;
}