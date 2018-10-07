// Fill out your copyright notice in the Description page of Project Settings.

#include "CuePawn.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Camera/CameraComponent.h"
//#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
ACuePawn::ACuePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Create a camera and a visible object
	UCameraComponent* cam = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	visualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("visible component"));
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	cam->SetupAttachment(RootComponent);
	cam->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	cam->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	visualComponent->SetupAttachment(RootComponent);


	//audio cue
	//USoundCue* soundcue;
	//ConstructorHelpers::FObjectFinder<USoundCue> cue(
	//	TEXT("SoundCue'/Game/Dummy/test.test'")
	//);

	//soundcue = cue.Object;
	audio = CreateDefaultSubobject<UAudioComponent>(TEXT("audio"));
	audio->bAutoActivate = false;
	audio->SetupAttachment(RootComponent);
	// I want the sound to come from slighty in front of the pawn.
	audio->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	music = CreateDefaultSubobject<UAudioComponent>(TEXT("music"));
	music->bAutoActivate = false;
	music->SetupAttachment(RootComponent);
	// I want the sound to come from slighty in front of the pawn.
	music->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	//UE_LOG(LogTemp, Log, TEXT("%d correct, %d missed"), m_numCorrect, m_numMissed);
	m_inputType.Input = EInputType::NONE;

}

void ACuePawn::BeginPlay()
{
	music->Play();

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
	PlayerInputComponent->BindAction("Beep", IE_Pressed, this, &ACuePawn::Punch);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ACuePawn::Dodge);
}

void ACuePawn::Punch()
{
	audio->Play();
	if (m_onBeat && m_inputType.Input == EInputType::PUNCH)
	{
		m_punched = true;
	}
	else 
	{ 
		m_punched = false;
	}

	//UE_LOG(LogTemp, Log, TEXT("Key pressed? - %s"), (m_pressed ? TEXT("true") : TEXT("false")));

	SetOnBeat(false);
}

void ACuePawn::Dodge()
{
	audio->Play();
	if (m_onBeat && m_inputType.Input == EInputType::DODGE)
	{
		m_dodged = true;
	}
	else
	{
		m_dodged = false;
	}

	//UE_LOG(LogTemp, Log, TEXT("Key pressed? - %s"), (m_pressed ? TEXT("true") : TEXT("false")));

	SetOnBeat(false);
}
void ACuePawn::PressedOff()
{
	//reset all triggers
	m_pressed = false;
	m_dodged = false;
	m_punched = false;
}

void ACuePawn::OnBeatEnd()
{
	// you dodge or puch but not both, just in case
	m_pressed = m_dodged ^ m_punched;
	if (m_pressed) 
	{
		++m_numCorrect;
		//UE_LOG(LogTemp, Log, TEXT("%d correct, %d missed"), m_numCorrect, m_numMissed);
	}
	else 
	{
		++m_numMissed; 
		//UE_LOG(LogTemp, Log, TEXT("%d correct, %d missed"), m_numCorrect, m_numMissed);
	}
	

	//placeholder event for missed beat
	UE_LOG(LogTemp, Log, TEXT("Key pressed? - %s"), (m_pressed ? TEXT("true") : TEXT("false")));
	PressedOff();
}


void ACuePawn::ResultScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Your result - %f, %d correct, %d missed"), m_score, m_numCorrect,m_numMissed);	

}

bool ACuePawn::OnSessionEnd()
{
	//score = percentage correct
	float total = m_numCorrect + m_numMissed;
	m_score = 100 * m_numCorrect / total;

	// stop looping sequencer if target reached
	if ((int) m_score >= m_targetScore)
	{
		//yes, stop looping
		return true;
	}
	ResultScore();
	//keep looping
	return false;
}

void ACuePawn::SetOnBeat(const bool & value)
{
	m_onBeat = value;
	UE_LOG(LogTemp, Log, TEXT("NOW!"));
}

void ACuePawn::OnBeatBegin()
{
	//maximum value of enum class EInputType
	int range = static_cast<int>(EInputType::NONE);
	
	//out of range error checking
	if (static_cast<int>(m_inputType.Input) > range)
	{
		UE_LOG(LogTemp, Error, TEXT("Input Type Enum out of range"));
		return;
	}
	// Randomly prompt for dodge or punch
	//for now, only PUNCH or DODGE >> 0 or 1, can do more later
	int value = FMath::RandRange(0, 1);
	m_inputType.Input = static_cast<EInputType>(value);

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
	SetOnBeat(true);
}

