// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectilesDefault.h"
#include "ProjectilesDefault_Grenate.generated.h"

UCLASS()
class TDS_API AProjectilesDefault_Grenate : public AProjectilesDefault
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	void TimerExplose(float DeltaTime);

	virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	//virtual void ImpactProjectile() override;

	void Explose();

	bool bTimerEnbled = false; //<<Why need???
	float TimerToExplose = 0.0f;
	float TimeToExplose = 5.0f;

protected:

	virtual void BeginPlay() override;
};
