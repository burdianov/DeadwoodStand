// @ 2026 Brevis. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class DEADWOODSTAND_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
