
// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallBoi.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
ASmallBoi::ASmallBoi()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void ASmallBoi::BeginPlay()
{
	Super::BeginPlay();
}


//change color of this cube
void ASmallBoi::ChangeLeColor()
{
	int random = FMath::RandRange(1.0f, 3.0f);
	FColor color;
	color.A = 255;
	if (random == 1)
	{
		color.R = 255;
		color.G = 199;
		color.B = 227;
	}
	else if (random == 2)
	{
		color.R = 255;
		color.G = 255;
		color.B = 255;
	}
	else
	{
		color.R = 85;
		color.G = 205;
		color.B = 252;
	}
	UMaterialInterface* material = MeshComp->GetMaterial(0);
	UMaterialInstanceDynamic* MatInst = UMaterialInstanceDynamic::Create(material, this);
	if (MatInst)
	{
		MatInst->SetVectorParameterValue("Color", color);
	}
}

// Called every frame
void ASmallBoi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmallBoi::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionTemplate, GetActorLocation());
	Destroy();
}

