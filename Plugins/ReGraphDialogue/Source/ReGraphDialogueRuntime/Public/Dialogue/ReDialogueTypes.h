#pragma once

#include "CoreMinimal.h"
#include "Graph/ReGraphTypes.h"
#include "ReDialogueTypes.generated.h"

UENUM(BlueprintType)
enum class EReDialogueNodeType : uint8
{
	Start,
	Line,
	Choice,
	Conditional,
	Event,
	End
};

UENUM(BlueprintType)
enum class EReDialogueActionType : uint8
{
	SetVariable,
	AddInt,
	AddFloat,
	TriggerEvent,
	RequestBlueprintCallback
};

USTRUCT(BlueprintType)
struct REGRAPHDIALOGUERUNTIME_API FReDialogueChoice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText ChoiceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FGuid TargetNodeId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 TransitionIndex = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct REGRAPHDIALOGUERUNTIME_API FReDialogueChoiceEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName PinName = TEXT("Choice");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText ChoiceText;
};

USTRUCT(BlueprintType)
struct REGRAPHDIALOGUERUNTIME_API FReDialogueLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TSoftObjectPtr<UObject> VoiceAsset;
};

class UReDialogueCondition;

USTRUCT(BlueprintType)
struct REGRAPHDIALOGUERUNTIME_API FReDialogueConditionBranch
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName PinName = TEXT("Branch");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Dialogue")
	TObjectPtr<UReDialogueCondition> Condition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bIsDefaultBranch = false;
};
