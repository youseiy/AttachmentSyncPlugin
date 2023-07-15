// Fill out your copyright notice in the Description page of Project Settings.


#include "ASAttachment.h"

// Sets default values
AASAttachment::AASAttachment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASAttachment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASAttachment::SetAttachmentInfo(FAttachmentInfo AttachmentInfo)
{
	AttachmentAttributes=AttachmentInfo;
	
}

