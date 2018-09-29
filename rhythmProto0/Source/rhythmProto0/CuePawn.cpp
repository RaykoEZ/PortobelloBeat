// Fill out your copyright notice in the Description page of Project Settings.

#include "CuePawn.h"
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
	PlayerInputComponent->BindAction("Beep", IE_Pressed, this, &ACuePawn::BeepAction);

}

void ACuePawn::BeepAction()
{
	audio->Play();
	if (m_onBeat)
	{
		m_pressed = true;
	}
	UE_LOG(LogTemp, Log, TEXT("Key pressed? - %s"), (m_pressed ? TEXT("true") : TEXT("false")));

	m_pressed = false;
	setOnBeat(false);
}

void ACuePawn::setOnBeat(const bool & value)
{
	m_onBeat = value;
	UE_LOG(LogTemp, Log, TEXT("NOW!"));
}

