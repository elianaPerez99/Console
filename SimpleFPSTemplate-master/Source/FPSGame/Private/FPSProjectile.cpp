// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}


void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
		// Only add impulse and destroy projectile if we hit a physics
		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
		{
			if (!OtherActor->IsA(ASmallBoi::StaticClass()))
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

				FVector Scale = OtherComp->GetComponentScale();
				Scale *= 0.25f; //>:)

				ASmallBoi* cubyBoi = GetWorld()->SpawnActor<ASmallBoi>(smallCube, GetActorLocation(), GetActorRotation());
				cubyBoi->MeshComp->SetWorldScale3D(Scale);
				cubyBoi->MeshComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
				ASmallBoi* cubyBoi2 = GetWorld()->SpawnActor<ASmallBoi>(smallCube, GetActorLocation(), GetActorRotation());
				cubyBoi2->MeshComp->SetWorldScale3D(Scale);
				cubyBoi2->MeshComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
				ASmallBoi* cubyBoi3 = GetWorld()->SpawnActor<ASmallBoi>(smallCube, GetActorLocation(), GetActorRotation());
				cubyBoi3->MeshComp->SetWorldScale3D(Scale);
				cubyBoi3->MeshComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
				ASmallBoi* cubyBoi4 = GetWorld()->SpawnActor<ASmallBoi>(smallCube, GetActorLocation(), GetActorRotation());
				cubyBoi4->MeshComp->SetWorldScale3D(Scale);
				cubyBoi4->MeshComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
				OtherActor->Destroy();
			}
			else
			{
				((ASmallBoi*)OtherActor)->Explode();
			}

			Destroy();
		}
}