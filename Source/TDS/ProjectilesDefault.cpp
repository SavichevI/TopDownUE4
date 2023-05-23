// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectilesDefault.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectilesDefault::AProjectilesDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphera"));
	BulletCollisionSphere->SetSphereRadius(16.0f);
	BulletCollisionSphere->bReturnMaterialOnMove = true;
	BulletCollisionSphere->SetCanEverAffectNavigation(false);

	RootComponent = BulletCollisionSphere;
	
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletProjectileMesh"));
	BulletMesh->SetStaticMesh(ProjectileSetting.BulletMesh);
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BulletMesh->SetCanEverAffectNavigation(false);

	BulletFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BulletFX"));
	BulletFX->SetTemplate(ProjectileSetting.BulletFX);
	BulletFX->SetupAttachment(RootComponent);

	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletProjectileMovement"));
	BulletProjectileMovement->UpdatedComponent = RootComponent;
	BulletProjectileMovement->InitialSpeed = 99999.0f;
	BulletProjectileMovement->MaxSpeed = 99999.0f;

	BulletProjectileMovement->bRotationFollowsVelocity = true;
	BulletProjectileMovement->bShouldBounce = true;

	SphereOfDistanceDamage = CreateDefaultSubobject<USphereComponent>(TEXT("SpheraOfDistanceDamage"));
	SphereOfDistanceDamage->SetSphereRadius(ProjectileSetting.ProjectileMinRadiusDamage);
	SphereOfDistanceDamage->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereOfDistanceDamage->SetupAttachment(RootComponent);
}

void AProjectilesDefault::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && Hit.PhysMaterial.IsValid())
	{
		EPhysicalSurface mySurfaceType = UGameplayStatics::GetSurfaceType(Hit);
		if (ProjectileSetting.HitDecals.Contains(mySurfaceType))
		{
			UMaterialInstance* myMeteril = ProjectileSetting.HitDecals[mySurfaceType];
			if (myMeteril && OtherComp)
			{
				UGameplayStatics::SpawnDecalAttached(myMeteril, FVector(20.0f), OtherComp, NAME_None, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
			}
		}
		if (ProjectileSetting.HitFXs.Contains(mySurfaceType))
		{
			UParticleSystem* myParticle = ProjectileSetting.HitFXs[mySurfaceType];
			if (myParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), myParticle, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint, FVector(1.0f)));
			}
		}
		if (ProjectileSetting.HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.HitSound, Hit.ImpactPoint);
		}
	}
	UGameplayStatics::ApplyDamage(OtherActor, ProjectileSetting.PtojectileDamage, GetInstigatorController(), this, NULL);
	ImpactProjectile();
}

void AProjectilesDefault::BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectilesDefault::BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AProjectilesDefault::ImpactProjectile()
{
	this->Destroy();
}

void AProjectilesDefault::InitProjectile(FProjectileInfo InitParam)
{
 	BulletProjectileMovement->InitialSpeed = InitParam.ProjectileInitSpeed;
 	BulletProjectileMovement->MaxSpeed = InitParam.ProjectileInitSpeed;
 	this->SetLifeSpan(InitParam.ProjectileLifeTime);
 
 	ProjectileSetting = InitParam;
}

void AProjectilesDefault::InitShellBullet(FShellBullets ShellBullet)
{
 	ShellBulletSetting = ShellBullet;
 	BulletProjectileMovement->InitialSpeed = ShellBullet.SpeedShellBullet;
	BulletProjectileMovement->MaxSpeed = ShellBullet.SpeedShellBullet;
 	this->SetLifeSpan(ShellBullet.LifeTimeShellBullet);
 
 	ShellBulletSetting = ShellBullet;
}

// Called when the game starts or when spawned
void AProjectilesDefault::BeginPlay()
{
	Super::BeginPlay();

	BulletCollisionSphere->OnComponentHit.AddDynamic(this, &AProjectilesDefault::BulletCollisionSphereHit);
	BulletCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectilesDefault::BulletCollisionSphereBeginOverlap);
	BulletCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AProjectilesDefault::BulletCollisionSphereEndOverlap);

	if (!BulletMesh->GetStaticMesh())
	{
		BulletMesh->DestroyComponent(true);
	}
	if (!BulletFX->Template)
	{
		BulletFX->DestroyComponent(true);
	}
}

// Called every frame
void AProjectilesDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

