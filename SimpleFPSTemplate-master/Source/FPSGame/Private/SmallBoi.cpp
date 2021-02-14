
// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallBoi.h"

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

// Called every frame
void ASmallBoi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

