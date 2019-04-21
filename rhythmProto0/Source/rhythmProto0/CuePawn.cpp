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

	/* not using this
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
	*/
	//soundcue = cue.Object;
	m_inputLimit = 3;
	// setup audio array
	m_audio.punchCue.Reserve(m_inputLimit);
	m_audio.dodgeCue.Reserve(m_inputLimit);

	m_audio.punchSuccess.Reserve(m_inputLimit);
	m_audio.punchFail.Reserve(m_inputLimit);
	
	m_audio.dodgeSuccess.Reserve(m_inputLimit);
	m_audio.dodgeFail.Reserve(m_inputLimit);
	
	m_audio.PlayerPositiveGrunt.Reserve(m_inputLimit);
	m_audio.PlayerNegativeGrunt.Reserve(m_inputLimit);
	
	m_audio.EnemyPositiveGrunt.Reserve(m_inputLimit);
	m_audio.EnemyNegativeGrunt.Reserve(m_inputLimit);

	for (uint8 i = 0; i< m_inputLimit; ++i)
	{
		m_audio.punchCue.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Punch Cue" + FString::FromInt(i))));
		m_audio.punchCue[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.punchCue[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

		m_audio.dodgeCue.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Dodge Cue" + FString::FromInt(i))));
		m_audio.dodgeCue[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.dodgeCue[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));



		m_audio.punchSuccess.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Punch Successful" + FString::FromInt(i))));
		m_audio.punchSuccess[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.punchSuccess[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

		m_audio.punchFail.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Punch Failed " + FString::FromInt(i))));
		m_audio.punchFail[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.punchFail[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));



		m_audio.dodgeSuccess.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Dodge Successful " + FString::FromInt(i))));
		m_audio.dodgeSuccess[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.dodgeSuccess[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

		m_audio.dodgeFail.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Dodge Failed " + FString::FromInt(i))));
		m_audio.dodgeFail[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.dodgeFail[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));



		m_audio.PlayerPositiveGrunt.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Player Positive Sounds" + FString::FromInt(i))));
		m_audio.PlayerPositiveGrunt[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.PlayerPositiveGrunt[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

		m_audio.PlayerNegativeGrunt.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Player Negative Sounds" + FString::FromInt(i))));
		m_audio.PlayerNegativeGrunt[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.PlayerNegativeGrunt[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));



		m_audio.EnemyPositiveGrunt.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Enemy Positive Sounds" + FString::FromInt(i))));
		m_audio.EnemyPositiveGrunt[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.EnemyPositiveGrunt[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

		m_audio.EnemyNegativeGrunt.Add(CreateDefaultSubobject<UAudioComponent>(*FString("Enemy Negative Sounds" + FString::FromInt(i))));
		m_audio.EnemyNegativeGrunt[i]->bAutoActivate = false;
		// I want the sound to come from slighty in front of the pawn.
		m_audio.EnemyNegativeGrunt[i]->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	}

	m_inputType.Input.Reserve(3);
	m_inputType.Input.Add(EInputType::NONE);
	m_isFrameOpen = false;
	m_dodged = false;
	m_punched = false;
	m_isCorrect = false;
	m_score = 0.0f;
	m_numCorrect = 0;
	m_targetScore = 0.5f;
	m_numInput = 0;
	m_succStreak = 0;

}

void ACuePawn::BeginPlay()
{
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
	//PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &ACuePawn::punch);
	//PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ACuePawn::dodge);
}

void ACuePawn::punch()
{
	//m_audio.punch->Play();
	/// our punching input includes:
	bool punchInput = m_inputType.Input[0] == EInputType::PUNCH;
	if (m_isFrameOpen && punchInput)
	{
		m_punched = true;
	}
	else 
	{ 
		m_punched = false;
	}

	if(isInputCorrect())
	{
		m_audio.punchSuccess[0]->Play();
	}
	else
	{
		m_audio.punchFail[0]->Play();

	}

	setIsFrameOpen(false);
}

void ACuePawn::dodge()
{
	//m_audio.dodge->Play();
	/// our dodging input includes:
	bool dodgeInput = m_inputType.Input[0] == EInputType::DODGE;
	if (m_isFrameOpen && dodgeInput)
	{
		m_dodged = true;
	}
	else
	{
		m_dodged = false;
	}

	if (isInputCorrect())
	{
		m_audio.dodgeSuccess[0]->Play();
	}
	else
	{
		m_audio.dodgeFail[0]->Play();

	}

	setIsFrameOpen(false);
}
void ACuePawn::setHighestStreak(const int & _streak)
{
	if (_streak > m_highestStreak) 
	{
		m_highestStreak = _streak;
	}
}
void ACuePawn::resetState()
{
	//reset all triggers
	m_isCorrect = false;
	m_dodged = false;
	m_punched = false;
	m_inputType.Input.RemoveAt(0);
	if(m_inputType.Input.Num()<=0)
	{
		m_inputType.Input.Add(EInputType::NONE);
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

	return ret;
}


void ACuePawn::result()
{
	UE_LOG(LogTemp, Warning, TEXT("Your result : %d correct , accuracy : %f"), m_numCorrect,m_score);	

}

bool ACuePawn::onGameEnd()
{
	float score = calculateScore();

	// stop looping sequencer if target reached
	if (score > m_targetScore)
	{

		//show score and whatever
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
}

void ACuePawn::onBeatBegin()
{
	
	//prompt before input
	switch(m_inputType.Input[0])
	{
		//punch
		case EInputType::PUNCH:
		{			
			break;
		}
		case EInputType::DODGE:
		{			
			break;
		}
		default: break;
	}
	//begin to accept player input
	setIsFrameOpen(true);
}

/// input _i can be 1 to limit
void ACuePawn::playCue(const EInputType &_in, const uint8 &_i)
{
	// if arg for number of inputs are too high, we error out
	if (_i == 0 ||_i > m_inputLimit)
	{ 
		return; 
	}

	switch (_in)
	{
		//punch
	case EInputType::PUNCH:
	{
		m_audio.punchCue[_i-1]->Play();
		break;
	}
	case EInputType::DODGE:
	{

		m_audio.dodgeCue[_i-1]->Play();
		break;
	}
	default: break;
	}
	//begin to accept player input
}

void ACuePawn::setInputIndex(const EInputType &_in, const uint8 &_i)
{
	// if arg for number of inputs are too high, we error out
	if (_i == 0 || _i > m_inputLimit)
	{
		return;
	}
	// we push our input calls onto an array to allow triple or more repetitive input calls
	switch (_in)
	{
		//punch
	case EInputType::PUNCH:
	{
		for (uint8 i = 0; i < _i; ++i) 
		{
			m_inputType.Input.Insert(_in, 0);
		}
		
		break;
	}
	case EInputType::DODGE:
	{
		for (uint8 i = 0; i < _i; ++i)
		{
			m_inputType.Input.Insert(_in, 0);
		}
		
		break;
	}
	default: break;
	}
}

/// Called during end of beat
bool ACuePawn::endOfInputChecks()
{
	bool ret;
	++m_numInput;
	if(!m_isCorrect)
	{
		// Missed beat if key isn't pressed at the time
		m_succStreak = 0;
		//m_audio.fail->Play();
		ret = true;
	}
	else
	{
		++m_succStreak;
		++m_numCorrect;

		setHighestStreak(m_succStreak);
		ret = false;
	}
	//Result();
	m_score = calculateScore();
	return ret;
}

float ACuePawn::calculateScore() const 
{
	return (float)m_numCorrect / m_numInput;
}