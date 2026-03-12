#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ReGraphFactory.generated.h"

UCLASS()
class REGRAPHBASEEDITOR_API UReGraphFactory : public UFactory
{
	GENERATED_BODY()

public:
	UReGraphFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
