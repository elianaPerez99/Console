// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSBombActor.h"
#include "ExplodeProj.h"
#include "FPSCharacter.generated.h"


class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;
class AFPSBombActor;
class AExplodeProj;

//declare delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPartyTimeDelegate);


UCLASS()
class AFPSCharacter : public ACharacter
{

	GENERATED_BODY()
		
protected:

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	
	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;
	float startTime;

public:
	AFPSCharacter();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<AExplodeProj> AltProjClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		float AltFireDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Bomb")
	TSubclassOf<AFPSBombActor> BombClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

protected:
	bool canAlt;
	/** Fires a projectile. */
	void Fire();

	void SpawnBomb();
	void SetAlt();

	//the function that holds the delegate stuff
	UFUNCTION()
		void PartyTime();
	UFUNCTION()
	void AltFire();
	//when shift pressed
	UFUNCTION()
	void BoostHold();
	//when shift released
	UFUNCTION()
	void BoostRelease();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	FPartyTimeDelegate StartPartyTimeDelegate;

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }


};

