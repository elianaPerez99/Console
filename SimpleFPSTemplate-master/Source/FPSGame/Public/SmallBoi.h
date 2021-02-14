// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmallBoi.generated.h"

UCLASS()
class FPSGAME_API ASmallBoi : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmallBoi();
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "BombActor")
		UParticleSystem* ExplosionTemplate;
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
