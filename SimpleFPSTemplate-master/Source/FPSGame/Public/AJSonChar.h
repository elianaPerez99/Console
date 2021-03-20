// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DirectionalLightComponent.h"
#include "AJSonChar.generated.h"


class UDirectionalLightComponent;
class UStaticMeshComponent;
UCLASS()
class FPSGAME_API AAJSonChar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAJSonChar();

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Lighting")
		UDirectionalLightComponent* Sky;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Lighting")
		UStaticMeshComponent* MeshComp;
	virtual void BeginPlay() override;
	void ReadTemp();
	FColor ReturnSkyColor(FString forecast);
	void ChangeIntensity(bool intensity);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
