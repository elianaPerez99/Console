// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequence.h"
#include "Components/PrimitiveComponent.h"
#include "Math/Color.h"


AFPSCharacter::AFPSCharacter()
{
	ChangeColorDelegate.Add(&ASmallBoi::ChangeLeColor);
	AltFireDelay = 3.0f;
	canAlt = true;
	startTime = 0;
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

	PlayerInputComponent->BindAction("SpawnBomb", IE_Pressed, this, &AFPSCharacter::SpawnBomb);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("BoostHold", IE_Pressed, this, &AFPSCharacter::BoostHold);
	PlayerInputComponent->BindAction("BoostRelease", IE_Released, this, &AFPSCharacter::BoostRelease);
	PlayerInputComponent->BindAction("AltFire", IE_Pressed, this, &AFPSCharacter::AltFire);
	PlayerInputComponent->BindAction("PartyTime", IE_Pressed, this, &AFPSCharacter::PartyTime);
}


void AFPSCharacter::PartyTime()
{
	//tell all the objects to change le color
	ChangeColorDelegate.Broadcast();
}

//get the time when shift starts being pressed
void AFPSCharacter::BoostHold()
{
	startTime = GetWorld()->GetTimeSeconds();
}

//bind scale
void AFPSCharacter::BoostRelease()
{
	//resetting start time for next shot
	startTime = 0;

}
void AFPSCharacter::AltFire()
{
	//getting the scale for the explosion
	float scale = GetWorld()->GetTimeSeconds() - startTime;

	if (canAlt)
	{
		if (AltProjClass)
		{
			// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
			FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
			// Use controller rotation which is our view direction in first person
			FRotator MuzzleRotation = GetControlRotation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			AExplodeProj* projectile = GetWorld()->SpawnActor<AExplodeProj>(AltProjClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
			projectile->SetScale(scale);
		}

		// try and play the sound if specified
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
			}
		}
		canAlt = false;
		FTimerHandle Cooldown_TimerHandle;
		GetWorldTimerManager().SetTimer(Cooldown_TimerHandle, this, &AFPSCharacter::SetAlt, AltFireDelay);
	}
}

void AFPSCharacter::SetAlt()
{
	canAlt = true;
}

void AFPSCharacter::Fire()
{
	// try and fire a projectile
	if (ProjectileClass)
	{
		// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		FRotator MuzzleRotation = GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}

void AFPSCharacter::SpawnBomb()
{
	AFPSBombActor* MyBomb = GetWorld()->SpawnActor<AFPSBombActor>(BombClass, GetActorLocation(),GetActorRotation());
}


void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}
