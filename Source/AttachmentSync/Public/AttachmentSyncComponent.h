// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASAttachment.h"
#include "Components/ActorComponent.h"
#include "Engine/StreamableManager.h"
#include "AttachmentSyncComponent.generated.h"



UENUM(BlueprintType)
enum class ETypeAttachment : uint8 {
	Mag			UMETA(DisplayName="Mag"),
	Muzzle      UMETA(DisplayName="Muzzle"),
	Sight		UMETA(DisplayName="Sight"),
	Grip		UMETA(DisplayName="Grip"),
	Stock		UMETA(DisplayName="Stock")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttachmentAdded, FAttachmentInfo, AttachmentInfo, FGameplayTag, AttachmentGameplayTag, ETypeAttachment, AttachmentType );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttachmentRemoved, FAttachmentInfo, AttachmentInfo, FGameplayTag, AttachmentGameplayTag, ETypeAttachment, AttachmentType );


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATTACHMENTSYNC_API UAttachmentSyncComponent : public UActorComponent
{
	GENERATED_BODY()
	//MeshPtr
	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;
	//AsyncLoad Manager
	FStreamableManager Manager;
	
	
	//SocketNames
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AttachmentName, meta = (AllowPrivateAccess = true))
	FName SightSocketName{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AttachmentName, meta = (AllowPrivateAccess = true))
	FName MagSocketName{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AttachmentName, meta = (AllowPrivateAccess = true))
	FName GripSocketName{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AttachmentName, meta = (AllowPrivateAccess = true))
	FName MuzzleSocketName{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AttachmentName, meta = (AllowPrivateAccess = true))
	FName StockSocketName{};
	
	//Current Equipped's 
	UPROPERTY(BlueprintReadOnly,Category=AttachmentSync, meta = (AllowPrivateAccess = true))
	TObjectPtr<AASAttachment> CurrentEquippedMuzzle;

	UPROPERTY(BlueprintReadOnly,Category=AttachmentSync, meta = (AllowPrivateAccess = true))
	TObjectPtr<AASAttachment> CurrentEquippedGrip;

	UPROPERTY(BlueprintReadOnly,Category=AttachmentSync, meta = (AllowPrivateAccess = true))
	TObjectPtr<AASAttachment> CurrentEquippedSight;

	UPROPERTY(BlueprintReadOnly,Category=AttachmentSync, meta = (AllowPrivateAccess = true))
	TObjectPtr<AASAttachment> CurrentEquippedMag;

	UPROPERTY(BlueprintReadOnly,Category=AttachmentSync, meta = (AllowPrivateAccess = true))
	TObjectPtr<AASAttachment> CurrentEquippedStock;

	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnAttachmentAdded OnAttachmentAdded;
	UPROPERTY(BlueprintAssignable)
	FOnAttachmentRemoved OnAttachmentRemoved;

	//Map for each Attachment/custom
	UPROPERTY(EditDefaultsOnly, Category=AttachmentSync, meta = (AllowPrivateAccess = true,EditFixedSize=true))
	TMap<FName,TSoftClassPtr<AASAttachment>> MuzzleAttachments;
	UPROPERTY(EditDefaultsOnly, Category=AttachmentSync, meta = (AllowPrivateAccess = true,EditFixedSize=true))
	TMap<FName,TSoftClassPtr<AASAttachment>> SightAttachments;
	UPROPERTY(EditDefaultsOnly, Category=AttachmentSync, meta = (AllowPrivateAccess = true,EditFixedSize=true))
	TMap<FName,TSoftClassPtr<AASAttachment>> GripAttachments;
	UPROPERTY(EditDefaultsOnly, Category=AttachmentSync, meta = (AllowPrivateAccess = true,EditFixedSize=true))
	TMap<FName,TSoftClassPtr<AASAttachment>> MagAttachments;
	UPROPERTY(EditDefaultsOnly, Category=AttachmentSync, meta = (AllowPrivateAccess = true,EditFixedSize=true))
	TMap<FName,TSoftClassPtr<AASAttachment>> StockAttachments;
	//UPROPERTY(EditDefaultsOnly, Category=AttachmentSync, meta = (AllowPrivateAccess = true))
	//TArray<FName> CustomAttachments;
	
	
	
	//Change Attachment Funcs
	UFUNCTION(BlueprintCallable,Category=AttachmentSync,meta = (AllowPrivateAccess = true))
	void ChangeSightByGameplayTag(FGameplayTag NewSight);
	UFUNCTION(BlueprintCallable,Category=AttachmentSync,meta = (AllowPrivateAccess = true))
	void ChangeGripByGameplayTag( FGameplayTag NewGrip);
	UFUNCTION(BlueprintCallable,Category=AttachmentSync,meta = (AllowPrivateAccess = true))
	void ChangeMuzzleByGameplayTag(FGameplayTag NewMuzzle);
	UFUNCTION(BlueprintCallable,Category=AttachmentSync,meta = (AllowPrivateAccess = true))
	void ChangeMagByGameplayTag(FGameplayTag NewMag);
	UFUNCTION(BlueprintCallable,Category=AttachmentSync,meta = (AllowPrivateAccess = true))
	void ChangeStockByGameplayTag(FGameplayTag NewMag);
	UFUNCTION(BlueprintCallable,Category=AttachmentSync,meta = (AllowPrivateAccess = true))
	void ChangeAttachmentByKey(FName AttachmentName,ETypeAttachment TypeAttachment); 
	
	void OnLoad( TSoftClassPtr<AASAttachment> AttachmentRef,FGameplayTag AttachmentTag,ETypeAttachment TypeAttachment);
	void OnLoadByKey( TSoftClassPtr<AASAttachment>*, ETypeAttachment AttachmentType);
	
	
	//Helper Functions
	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	FORCEINLINE AASAttachment* GetCurrentEquippedMuzzle() const {return CurrentEquippedMuzzle;}

	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	FORCEINLINE AASAttachment* GetCurrentEquippedGrip() const {return CurrentEquippedGrip;}
	
	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	FORCEINLINE AASAttachment* GetCurrentEquippedMag() const {return CurrentEquippedMag;}

	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	FORCEINLINE AASAttachment* GetCurrentEquippedSight() const {return CurrentEquippedSight;}

	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	FORCEINLINE TMap<FName,TSoftClassPtr<AASAttachment>> GetAllSightAttachment() const {return SightAttachments;}

	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	FORCEINLINE TMap<FName,TSoftClassPtr<AASAttachment>> GetAllMuzzleAttachment() const {return MuzzleAttachments;}

	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	FORCEINLINE TMap<FName,TSoftClassPtr<AASAttachment>> GetAllGripAttachment() const {return GripAttachments;}

	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	FORCEINLINE TMap<FName,TSoftClassPtr<AASAttachment>> GetAllStockAttachment() const {return SightAttachments;}
	
	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	void DestroyCurrentAttachment(ETypeAttachment TypeAttachment);
	//Callback
	AASAttachment* ChangeAttachment(FName AttachmentFName, const TSoftClassPtr<AASAttachment>& AttachmentSoftRef) const;

public:	
	// Sets default values for this component's properties
	UAttachmentSyncComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
