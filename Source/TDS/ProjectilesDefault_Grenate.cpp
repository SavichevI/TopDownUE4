// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilesDefault_Grenate.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void AProjectilesDefault_Grenate::TimerExplose(float DeltaTime)
{	
	if (TimerToExplose > TimeToExplose)
	{
		Explose();
	} 
	else
	{
		TimerToExplose += DeltaTime;
	}
	
}

void AProjectilesDefault_Grenate::BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::BulletCollisionSphereHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

// void AProjectilesDefault_Grenate::ImpactProjectile()
// {	
// 	//Super::ImpactProjectile();
// 	bTimerEnbled = true;
// }

void AProjectilesDefault_Grenate::Explose()
{
	bTimerEnbled = false;
	if (ProjectileSetting.ExplosionFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExplosionFX, GetActorLocation(), GetActorRotation(), FVector(1.0f));

		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMaxRadiusDamage, 12, FColor::Green, false, 12.0f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMinRadiusDamage, 9, FColor::Blue, false, 12.0f);
	}
	if (ProjectileSetting.ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.ExplosionSound, GetActorLocation());		
	}

	TArray<AActor*> IgnoredActor;
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ProjectileSetting.ExplosionMaxDamage, ProjectileSetting.ExplosionMaxDamage * 2.0f, GetActorLocation(), 1000.0f, 2000.0f, 5, NULL, IgnoredActor, nullptr, nullptr);
	
	this->Destroy();
}

void AProjectilesDefault_Grenate::BeginPlay()
{
	Super::BeginPlay();	
}

void AProjectilesDefault_Grenate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerExplose(DeltaTime);
}