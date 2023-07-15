// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ASAttachment.generated.h"

USTRUCT(BlueprintType)
struct FAttachmentInfo 
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AttachmentSync)
	FGameplayTag GameplayTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AttachmentSync)
	FGameplayTagContainer GameplayTagContainer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AttachmentSync)
	float AccuracyBonus{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AttachmentSync)
	float DamageBonus{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AttachmentSync)
	float RecoilModification{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AttachmentSync)
	int32 MagazineSizeBonus{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AttachmentSync)
	float ReloadSpeedBonus{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AttachmentSync)
	float Price{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category=AttachmentSync)
	TObjectPtr<UTexture2D> ToHUD;
	
};

UCLASS()
class ATTACHMENTSYNC_API AASAttachment : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category=AttachmentSync, meta=(AllowPrivateAccess=true)); 
	FAttachmentInfo AttachmentAttributes;
	
public:	
	// Sets default values for this actor's properties
	AASAttachment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	 FAttachmentInfo GetAttachmentInfo() {return AttachmentAttributes;}
	UFUNCTION(BlueprintCallable,Category=AttachmentSync)
	void SetAttachmentInfo(FAttachmentInfo AttachmentInfo);
};
