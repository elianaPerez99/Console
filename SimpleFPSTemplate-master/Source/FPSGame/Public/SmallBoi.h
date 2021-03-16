// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmallBoi.generated.h"

class UParticleSystem;
class UProjectileMovementComponent;

UCLASS()
class FPSGAME_API ASmallBoi : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmallBoi();
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovement;
	UFUNCTION()
		void Explode();
	UFUNCTION()
		void ChangeLeColor();
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "BombActor")
		UParticleSystem* ExplosionTemplate;
	virtual void BeginPlay() override;
	UMaterialInstanceDynamic* MatInst;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
