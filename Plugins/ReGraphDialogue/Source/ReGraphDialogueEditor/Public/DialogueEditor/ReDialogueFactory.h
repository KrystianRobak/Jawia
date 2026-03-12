#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ReDialogueFactory.generated.h"

UCLASS()
class REGRAPHDIALOGUEEDITOR_API UReDialogueFactory : public UFactory
{
	GENERATED_BODY()

public:
	UReDialogueFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override;
	virtual uint32 GetMenuCategories() const override;
};
