// Fill out your copyright notice in the Description page of Project Settings.
#include "AttachmentSyncComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Engine/StreamableManager.h"



// Sets default values for this component's properties
UAttachmentSyncComponent::UAttachmentSyncComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	WeaponMesh=nullptr;


	
}


// Called when the game starts
void UAttachmentSyncComponent::BeginPlay()
{
	Super::BeginPlay();
	WeaponMesh=Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	
}
// Called every frame
void UAttachmentSyncComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}



void UAttachmentSyncComponent::ChangeSightByGameplayTag(FGameplayTag NewSight)
{
	for (const TTuple<FName, TSoftClassPtr<AASAttachment>> Sight :SightAttachments)
	{
		Manager.RequestAsyncLoad(Sight.Value.ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoad,Sight.Value,NewSight,ETypeAttachment::Sight));
	}
	
}

void UAttachmentSyncComponent::ChangeGripByGameplayTag(FGameplayTag NewGrip)
{
	for (const TTuple<FName, TSoftClassPtr<AASAttachment>> Grip :GripAttachments)
	{
		Manager.RequestAsyncLoad(Grip.Value.ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoad,Grip.Value,NewGrip,ETypeAttachment::Grip));
	}
}

void UAttachmentSyncComponent::ChangeMuzzleByGameplayTag(FGameplayTag NewMuzzle)
{
	for (const TTuple<FName, TSoftClassPtr<AASAttachment>> Muzzle :MuzzleAttachments)
	{
		Manager.RequestAsyncLoad(Muzzle.Value.ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoad,Muzzle.Value,NewMuzzle,ETypeAttachment::Muzzle));
		
	}
}

void UAttachmentSyncComponent::ChangeMagByGameplayTag(FGameplayTag NewMag)
{
	
	for (const TTuple<FName, TSoftClassPtr<AASAttachment>> Mag :MagAttachments)
	{
		Manager.RequestAsyncLoad(Mag.Value.ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoad,Mag.Value,NewMag,ETypeAttachment::Mag));
		
	}
}

void UAttachmentSyncComponent::ChangeStockByGameplayTag(FGameplayTag NewStock)
{
		
	for (const TTuple<FName, TSoftClassPtr<AASAttachment>> Stock :StockAttachments)
	{
		
		Manager.RequestAsyncLoad(Stock.Value.ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoad,Stock.Value,NewStock,ETypeAttachment::Mag));
		
	}
}

void UAttachmentSyncComponent::ChangeAttachmentByKey(FName AttachmentName, ETypeAttachment TypeAttachment)
{
	switch (TypeAttachment)
	{
	case ETypeAttachment::Grip:
		if (const auto AttachmentRef=GripAttachments.Find(AttachmentName))
		{
			if (CurrentEquippedGrip)
			{
				OnAttachmentRemoved.Broadcast(CurrentEquippedGrip->GetAttachmentInfo(),CurrentEquippedGrip->GetAttachmentInfo().GameplayTag,ETypeAttachment::Grip);
				CurrentEquippedGrip->Destroy();
				
			}
			Manager.RequestAsyncLoad(AttachmentRef->ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoadByKey,AttachmentRef,ETypeAttachment::Grip));
		}
		break;
	case ETypeAttachment::Mag:
		if (const auto AttachmentRef=MagAttachments.Find(AttachmentName))
		{
			
			if (CurrentEquippedMag)
			{
				OnAttachmentRemoved.Broadcast(CurrentEquippedMag->GetAttachmentInfo(),CurrentEquippedMag->GetAttachmentInfo().GameplayTag,ETypeAttachment::Mag);
				CurrentEquippedMag->Destroy();
				
			}
			Manager.RequestAsyncLoad(AttachmentRef->ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoadByKey,AttachmentRef,ETypeAttachment::Mag));
		}
		break;
	case ETypeAttachment::Muzzle:
		if (const auto AttachmentRef=MuzzleAttachments.Find(AttachmentName))
		{
			if (CurrentEquippedMuzzle)
			{
				OnAttachmentRemoved.Broadcast(CurrentEquippedMuzzle->GetAttachmentInfo(),CurrentEquippedMuzzle->GetAttachmentInfo().GameplayTag,ETypeAttachment::Muzzle);
				CurrentEquippedMuzzle->Destroy();
			}
			Manager.RequestAsyncLoad(AttachmentRef->ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoadByKey,AttachmentRef,ETypeAttachment::Muzzle));
		
		}
		break;
	case ETypeAttachment::Sight:
		if (const auto AttachmentRef=SightAttachments.Find(AttachmentName))
		{
			if (CurrentEquippedSight)
			{
				OnAttachmentRemoved.Broadcast(CurrentEquippedSight->GetAttachmentInfo(),CurrentEquippedSight->GetAttachmentInfo().GameplayTag,ETypeAttachment::Sight);
				CurrentEquippedSight->Destroy();
			}
			Manager.RequestAsyncLoad(AttachmentRef->ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoadByKey,AttachmentRef,ETypeAttachment::Sight));
		}
		break;
	case ETypeAttachment::Stock:
		if (const auto AttachmentRef=StockAttachments.Find(AttachmentName))
		{
			if (CurrentEquippedStock)
			{
				OnAttachmentRemoved.Broadcast(CurrentEquippedStock->GetAttachmentInfo(),CurrentEquippedStock->GetAttachmentInfo().GameplayTag,ETypeAttachment::Stock);
				CurrentEquippedSight->Destroy();
			}
			Manager.RequestAsyncLoad(AttachmentRef->ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this,&ThisClass::OnLoadByKey,AttachmentRef,ETypeAttachment::Stock));
		}
		break;
		default:
			break;
	}
	
}

void UAttachmentSyncComponent::OnLoad(const TSoftClassPtr<AASAttachment> AttachmentRef, FGameplayTag AttachmentTag,ETypeAttachment TypeAttachment)
{
	
	if (AttachmentRef)
	{
		
		const auto actor=Cast<AASAttachment>(AttachmentRef->GetDefaultObject());
		ensure(actor);
		switch (TypeAttachment)
		{
		case ETypeAttachment::Muzzle:
			{
				if (actor->GetAttachmentInfo().GameplayTag.MatchesTagExact(AttachmentTag) )
				{
					if (CurrentEquippedMuzzle)
					{
						
						OnAttachmentRemoved.Broadcast(CurrentEquippedMuzzle->GetAttachmentInfo(),CurrentEquippedMuzzle->GetAttachmentInfo().GameplayTag,ETypeAttachment::Muzzle);
						CurrentEquippedMuzzle->Destroy();
					}
					CurrentEquippedMuzzle=ChangeAttachment(MuzzleSocketName,AttachmentRef);
					OnAttachmentAdded.Broadcast(CurrentEquippedMuzzle->GetAttachmentInfo(),CurrentEquippedMuzzle->GetAttachmentInfo().GameplayTag,ETypeAttachment::Muzzle );
				}
				
				
			}
			break;
		case ETypeAttachment::Grip:
			if (actor->GetAttachmentInfo().GameplayTag.MatchesTagExact(AttachmentTag))
			{
				if (CurrentEquippedGrip)
				{
					
					OnAttachmentRemoved.Broadcast(CurrentEquippedGrip->GetAttachmentInfo(),CurrentEquippedGrip->GetAttachmentInfo().GameplayTag,ETypeAttachment::Grip);
					CurrentEquippedGrip->Destroy();
				}
				CurrentEquippedGrip=ChangeAttachment(GripSocketName,AttachmentRef);
				OnAttachmentAdded.Broadcast(CurrentEquippedGrip->GetAttachmentInfo(),CurrentEquippedGrip->GetAttachmentInfo().GameplayTag,ETypeAttachment::Grip);
			}
			else
				Manager.Unload(AttachmentRef.ToSoftObjectPath());
			break;
		case ETypeAttachment::Mag:
			if (actor->GetAttachmentInfo().GameplayTag.MatchesTagExact(AttachmentTag))
			{
				if (CurrentEquippedMag)
				{
					
					OnAttachmentRemoved.Broadcast(CurrentEquippedMag->GetAttachmentInfo(),CurrentEquippedMag->GetAttachmentInfo().GameplayTag,ETypeAttachment::Mag);
					CurrentEquippedMag->Destroy();
				}
				CurrentEquippedMag=ChangeAttachment(MagSocketName,AttachmentRef);
				OnAttachmentAdded.Broadcast(CurrentEquippedMag->GetAttachmentInfo(),CurrentEquippedMag->GetAttachmentInfo().GameplayTag,ETypeAttachment::Mag);
			}
			else
				Manager.Unload(AttachmentRef.ToSoftObjectPath());
			break;
		case ETypeAttachment::Sight:
			if (actor->GetAttachmentInfo().GameplayTag.MatchesTagExact(AttachmentTag))
			{
				if (CurrentEquippedSight)
				{
					
					OnAttachmentRemoved.Broadcast(CurrentEquippedSight->GetAttachmentInfo(),CurrentEquippedSight->GetAttachmentInfo().GameplayTag,ETypeAttachment::Sight);
					CurrentEquippedSight->Destroy();
				}
				CurrentEquippedSight=ChangeAttachment(SightSocketName,AttachmentRef);
				OnAttachmentAdded.Broadcast(CurrentEquippedSight->GetAttachmentInfo(),CurrentEquippedSight->GetAttachmentInfo().GameplayTag,ETypeAttachment::Sight);
			}
			else
				Manager.Unload(AttachmentRef.ToSoftObjectPath());
			break;
		case ETypeAttachment::Stock:
			if (actor->GetAttachmentInfo().GameplayTag.MatchesTagExact(AttachmentTag))
			{
				if (CurrentEquippedStock)
				{
					
					OnAttachmentRemoved.Broadcast(CurrentEquippedStock->GetAttachmentInfo(),CurrentEquippedStock->GetAttachmentInfo().GameplayTag,ETypeAttachment::Stock);
					CurrentEquippedStock->Destroy();
				}
				CurrentEquippedStock=ChangeAttachment(SightSocketName,AttachmentRef);
				OnAttachmentAdded.Broadcast(CurrentEquippedStock->GetAttachmentInfo(),CurrentEquippedStock->GetAttachmentInfo().GameplayTag,ETypeAttachment::Stock);
			}
			break;
				
			default:
				break;
			
		}
		
		
	}

}

void UAttachmentSyncComponent::OnLoadByKey( TSoftClassPtr<AASAttachment>* AttachmentRef, ETypeAttachment AttachmentType)
{
	switch (AttachmentType)
	{
	case ETypeAttachment::Grip:
		if (CurrentEquippedGrip)
		{
					
			OnAttachmentRemoved.Broadcast(CurrentEquippedGrip->GetAttachmentInfo(),CurrentEquippedGrip->GetAttachmentInfo().GameplayTag,ETypeAttachment::Grip);
			CurrentEquippedGrip->Destroy();
		}
		CurrentEquippedGrip=ChangeAttachment(GripSocketName,AttachmentRef->Get());
		OnAttachmentAdded.Broadcast(CurrentEquippedGrip->GetAttachmentInfo(),CurrentEquippedGrip->GetAttachmentInfo().GameplayTag,ETypeAttachment::Grip );
		break;
	case ETypeAttachment::Muzzle:
		if (CurrentEquippedMuzzle)
		{
						
			OnAttachmentRemoved.Broadcast(CurrentEquippedMuzzle->GetAttachmentInfo(),CurrentEquippedMuzzle->GetAttachmentInfo().GameplayTag,ETypeAttachment::Muzzle);
			CurrentEquippedMuzzle->Destroy();
		}
		CurrentEquippedMuzzle=ChangeAttachment(MuzzleSocketName,AttachmentRef->Get());
		OnAttachmentAdded.Broadcast(CurrentEquippedMuzzle->GetAttachmentInfo(),CurrentEquippedMuzzle->GetAttachmentInfo().GameplayTag,ETypeAttachment::Muzzle );
		break;
	case ETypeAttachment::Mag:
		if (CurrentEquippedMag)
		{
					
			OnAttachmentRemoved.Broadcast(CurrentEquippedMag->GetAttachmentInfo(),CurrentEquippedMag->GetAttachmentInfo().GameplayTag,ETypeAttachment::Mag);
			CurrentEquippedMag->Destroy();
		}
		CurrentEquippedMag=ChangeAttachment(MagSocketName,AttachmentRef->Get());
		OnAttachmentAdded.Broadcast(CurrentEquippedMag->GetAttachmentInfo(),CurrentEquippedMag->GetAttachmentInfo().GameplayTag,ETypeAttachment::Mag );
		break;
	case ETypeAttachment::Sight:
		if (CurrentEquippedSight)
		{
					
			OnAttachmentRemoved.Broadcast(CurrentEquippedSight->GetAttachmentInfo(),CurrentEquippedSight->GetAttachmentInfo().GameplayTag,ETypeAttachment::Sight);
			CurrentEquippedSight->Destroy();
		}
		CurrentEquippedSight=ChangeAttachment(SightSocketName,AttachmentRef->Get());
		OnAttachmentAdded.Broadcast(CurrentEquippedSight->GetAttachmentInfo(),CurrentEquippedSight->GetAttachmentInfo().GameplayTag,ETypeAttachment::Sight);
		break;
	case ETypeAttachment::Stock:
		if (CurrentEquippedStock)
		{
					
			OnAttachmentRemoved.Broadcast(CurrentEquippedStock->GetAttachmentInfo(),CurrentEquippedStock->GetAttachmentInfo().GameplayTag,ETypeAttachment::Stock);
			CurrentEquippedStock->Destroy();
		}
		CurrentEquippedStock=ChangeAttachment(StockSocketName,AttachmentRef->Get());
		OnAttachmentAdded.Broadcast(CurrentEquippedStock->GetAttachmentInfo(),CurrentEquippedSight->GetAttachmentInfo().GameplayTag,ETypeAttachment::Stock);
		break;
		
		default:
			break;
	}
}
	

void UAttachmentSyncComponent::DestroyCurrentAttachment(ETypeAttachment TypeAttachment)
{
	switch (TypeAttachment)
	{
	case ETypeAttachment::Grip:
		if (CurrentEquippedGrip)
		{
			
			OnAttachmentRemoved.Broadcast(CurrentEquippedGrip->GetAttachmentInfo(),CurrentEquippedGrip->GetAttachmentInfo().GameplayTag,ETypeAttachment::Grip);
			CurrentEquippedGrip->Destroy();
			
		}
			
		break;
	case ETypeAttachment::Mag:
		if (CurrentEquippedMag)
		{
			
			OnAttachmentRemoved.Broadcast(CurrentEquippedMag->GetAttachmentInfo(),CurrentEquippedMag->GetAttachmentInfo().GameplayTag,ETypeAttachment::Mag);
			CurrentEquippedMag->Destroy();
			
		}
		break;
	case ETypeAttachment::Muzzle:
		if (CurrentEquippedMuzzle)
		{
			
			OnAttachmentRemoved.Broadcast(CurrentEquippedMuzzle->GetAttachmentInfo(),CurrentEquippedMuzzle->GetAttachmentInfo().GameplayTag,ETypeAttachment::Muzzle);
			
		}
			
		break;
	case ETypeAttachment::Sight:
		if (CurrentEquippedSight)
		{
			
			OnAttachmentRemoved.Broadcast(CurrentEquippedSight->GetAttachmentInfo(),CurrentEquippedSight->GetAttachmentInfo().GameplayTag,ETypeAttachment::Sight);
			CurrentEquippedSight->Destroy();
			
		}
		
	
		break;
	case ETypeAttachment::Stock:
		if (CurrentEquippedStock)
		{
		
			OnAttachmentRemoved.Broadcast(CurrentEquippedStock->GetAttachmentInfo(),CurrentEquippedStock->GetAttachmentInfo().GameplayTag,ETypeAttachment::Stock);
			CurrentEquippedStock->Destroy();
			
		}
			
		default:
		break;
	}
}

AASAttachment* UAttachmentSyncComponent::ChangeAttachment(FName AttachmentFName, const
	TSoftClassPtr<AASAttachment>& AttachmentSoftRef) const 
{
	
	const FVector SocketLocation=WeaponMesh->GetSocketLocation(AttachmentFName);
	const FRotator SocketRotation=WeaponMesh->GetSocketRotation(AttachmentFName);
	const FActorSpawnParameters SpawnParams{};
	
	AASAttachment* ActorAttachment=nullptr;
	ActorAttachment=GetWorld()->SpawnActor<AASAttachment>(AttachmentSoftRef.Get(),SocketLocation,SocketRotation,SpawnParams);
	ActorAttachment->AttachToComponent(WeaponMesh,FAttachmentTransformRules::KeepWorldTransform,AttachmentFName);
	return ActorAttachment;
}
