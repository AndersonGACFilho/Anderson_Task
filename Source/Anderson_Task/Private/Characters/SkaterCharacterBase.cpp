#include "Characters/SkaterCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(LogSkaterCharacter);

ASkaterCharacterBase::ASkaterCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->InitCapsuleSize(42.f, 96.f);
	}

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bOrientRotationToMovement = false;
		MoveComp->bUseControllerDesiredRotation = false;
		MoveComp->MaxWalkSpeed = MaxSkateSpeed;
		MoveComp->BrakingDecelerationWalking = CoastDeceleration;
		MoveComp->GroundFriction = BaseGroundFriction;
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = CameraArmLength;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	SkateboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateboardMesh"));
	SkateboardMesh->SetupAttachment(GetMesh(), TEXT("SkateboardSocket"));
}

void ASkaterCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CachedMovementComponent = GetCharacterMovement();
	if (!CachedMovementComponent.IsValid())
	{
		UE_LOG(LogSkaterCharacter, Warning, TEXT("%s: CharacterMovementComponent is missing!"), *GetName());
	}
}

void ASkaterCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PreMovementUpdate(DeltaTime);
	UpdateSkaterMovement(DeltaTime);
	PostMovementUpdate(DeltaTime);
}

void ASkaterCharacterBase::SetMovementInput(FVector2D NewInput)
{
	CurrentInputVector = NewInput;
}

void ASkaterCharacterBase::ClearMovementInput()
{
	CurrentInputVector = FVector2D::ZeroVector;
}

void ASkaterCharacterBase::UpdateSkaterMovement(float DeltaTime)
{
	if (!CachedMovementComponent.IsValid())
	{
		return;
	}

	ProcessSteering(DeltaTime);
	ProcessAcceleration();
}

void ASkaterCharacterBase::ProcessSteering(float DeltaTime)
{
	UCharacterMovementComponent* CMC = GetCachedMovementComponent();
	if (!CMC)
	{
		return;
	}

	const float TargetTurn = CurrentInputVector.X;
	CurrentTurnValue = FMath::FInterpTo(CurrentTurnValue, TargetTurn, DeltaTime, SteeringInterpSpeed);

	if (FMath::Abs(CurrentTurnValue) <= TurnDeadzone)
	{
		return;
	}

	const float RotationAmount = CurrentTurnValue * TurnRate * DeltaTime;
	const FRotator RotationDelta(0.f, RotationAmount, 0.f);
	AddActorLocalRotation(RotationDelta);

	if (CMC->IsMovingOnGround())
	{
		CMC->Velocity = RotationDelta.RotateVector(CMC->Velocity);
		CMC->UpdateComponentVelocity();
	}
}

void ASkaterCharacterBase::ProcessAcceleration()
{
	const float ForwardInput = CurrentInputVector.Y;

	if (ForwardInput > InputDeadzone)
	{
		SetMovementState(ESkaterMovementState::Accelerating);
		AddMovementInput(GetActorForwardVector(), ForwardInput);
		return;
	}
	
	if (ForwardInput < -InputDeadzone)
	{
		SetMovementState(ESkaterMovementState::Braking);
		return;
	}

	SetMovementState(ESkaterMovementState::Coasting);
}

void ASkaterCharacterBase::SetMovementState(ESkaterMovementState NewState)
{
	if (CurrentMovementState == NewState)
	{
		return;
	}

	UCharacterMovementComponent* CMC = GetCachedMovementComponent();
	if (!CMC)
	{
		return;
	}

	CurrentMovementState = NewState;

	switch (NewState)
	{
	case ESkaterMovementState::Accelerating:
		CMC->BrakingDecelerationWalking = AccelerateDeceleration;
		break;
	case ESkaterMovementState::Braking:
		CMC->BrakingDecelerationWalking = BrakeDeceleration;
		break;
	case ESkaterMovementState::Coasting:
		CMC->BrakingDecelerationWalking = CoastDeceleration;
		break;
	default:
		break;
	}
}

float ASkaterCharacterBase::GetSpeedPercent() const
{
	const UCharacterMovementComponent* CMC = GetCachedMovementComponent();
	if (!CMC || MaxSkateSpeed <= 0.f)
	{
		return 0.f;
	}

	return FMath::Clamp(CMC->Velocity.Size() / MaxSkateSpeed, 0.f, 1.f);
}

float ASkaterCharacterBase::GetCurrentSpeed() const
{
	const UCharacterMovementComponent* CMC = GetCachedMovementComponent();
	return CMC ? CMC->Velocity.Size() : 0.f;
}