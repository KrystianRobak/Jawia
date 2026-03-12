#include "GraphEditor/ReGraphDetailCustomizations.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Graph/ReGraph.h"
#include "Graph/ReGraphNode.h"
#include "Graph/ReGraphTypes.h"
#include "Graph/ReGraphVariable.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
#include "UObject/UnrealType.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Text/STextBlock.h"

namespace
{
	static bool IsSupportedBindingProperty(const FProperty* Property)
	{
		return CastField<FBoolProperty>(Property) != nullptr
			|| CastField<FIntProperty>(Property) != nullptr
			|| CastField<FFloatProperty>(Property) != nullptr
			|| CastField<FStrProperty>(Property) != nullptr
			|| CastField<FNameProperty>(Property) != nullptr;
	}
}

TSharedRef<IDetailCustomization> FReGraphDetailCustomization::MakeInstance()
{
	return MakeShared<FReGraphDetailCustomization>();
}

void FReGraphDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraph, Nodes));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraph, Edges));
#if WITH_EDITORONLY_DATA
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraph, EditorGraph));
#endif

	IDetailCategoryBuilder& VariablesCategory = DetailBuilder.EditCategory(TEXT("Variables"), FText::FromString(TEXT("Variables")), ECategoryPriority::Important);
	VariablesCategory.AddCustomRow(FText::FromString(TEXT("VariablesHelp")))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(TEXT("Define default dialogue variables here. Each variable exposes a typed value editor and becomes part of runtime context when the graph starts.")))
		.AutoWrapText(true)
	];

	VariablesCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReGraph, DefaultVariables));

	IDetailCategoryBuilder& BindingCategory = DetailBuilder.EditCategory(TEXT("Binding"), FText::FromString(TEXT("Binding")), ECategoryPriority::Important);
	BindingCategory.AddCustomRow(FText::FromString(TEXT("BindingHelp")))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(TEXT("Pick the runtime object type this dialogue binds to, then map graph variables to object properties by name. Supported property types are bool, int, float, FString and FName.")))
		.AutoWrapText(true)
	];
	BindingCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReGraph, BoundObjectClass));
	BindingCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReGraph, VariableBindings));
}

TSharedRef<IDetailCustomization> FReGraphNodeDetailCustomization::MakeInstance()
{
	return MakeShared<FReGraphNodeDetailCustomization>();
}

void FReGraphNodeDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, NodeId));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, NodeName));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, bCanBeEntryPoint));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, EditorPosition));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, OutgoingEdges));

	IDetailCategoryBuilder& NodeCategory = DetailBuilder.EditCategory(TEXT("Node"), FText::FromString(TEXT("Node")), ECategoryPriority::Important);
	NodeCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, DisplayTitle));
}

TSharedRef<IPropertyTypeCustomization> FReGraphValueCustomization::MakeInstance()
{
	return MakeShared<FReGraphValueCustomization>();
}

void FReGraphValueCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	HeaderRow.NameContent()[StructPropertyHandle->CreatePropertyNameWidget()]
	.ValueContent()
	.MinDesiredWidth(300.0f)
	[
		StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FReGraphValue, Type))->CreatePropertyValueWidget()
	];
}

void FReGraphValueCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> TypeHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FReGraphValue, Type));
	TSharedPtr<IPropertyHandle> BoolHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FReGraphValue, BoolValue));
	TSharedPtr<IPropertyHandle> IntHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FReGraphValue, IntValue));
	TSharedPtr<IPropertyHandle> FloatHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FReGraphValue, FloatValue));
	TSharedPtr<IPropertyHandle> StringHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FReGraphValue, StringValue));

	if (TypeHandle.IsValid())
	{
		TypeHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([PropertyUtilities = StructCustomizationUtils.GetPropertyUtilities()]()
		{
			if (PropertyUtilities.IsValid())
			{
				PropertyUtilities->ForceRefresh();
			}
		}));
	}

	uint8 TypeValue = static_cast<uint8>(EReGraphValueType::Bool);
	if (TypeHandle.IsValid())
	{
		TypeHandle->GetValue(TypeValue);
	}

	switch (static_cast<EReGraphValueType>(TypeValue))
	{
	case EReGraphValueType::Bool:
		StructBuilder.AddProperty(BoolHandle.ToSharedRef());
		break;
	case EReGraphValueType::Int:
		StructBuilder.AddProperty(IntHandle.ToSharedRef());
		break;
	case EReGraphValueType::Float:
		StructBuilder.AddProperty(FloatHandle.ToSharedRef());
		break;
	case EReGraphValueType::String:
		StructBuilder.AddProperty(StringHandle.ToSharedRef());
		break;
	default:
		break;
	}
}

TSharedRef<IPropertyTypeCustomization> FReGraphVariableBindingCustomization::MakeInstance()
{
	return MakeShared<FReGraphVariableBindingCustomization>();
}

void FReGraphVariableBindingCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	HeaderRow.NameContent()[StructPropertyHandle->CreatePropertyNameWidget()];
}

void FReGraphVariableBindingCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> VariableNameHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FReGraphVariableBinding, VariableName));
	TSharedPtr<IPropertyHandle> PropertyNameHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FReGraphVariableBinding, PropertyName));
	TSharedPtr<IPropertyHandle> WriteBackHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FReGraphVariableBinding, bWriteBackToObject));

	TArray<UObject*> OuterObjects;
	StructPropertyHandle->GetOuterObjects(OuterObjects);

	UReGraph* Graph = nullptr;
	for (UObject* OuterObject : OuterObjects)
	{
		Graph = Cast<UReGraph>(OuterObject);
		if (Graph)
		{
			break;
		}
	}

	VariableOptions.Reset();
	if (Graph)
	{
		for (UReGraphVariable* Variable : Graph->DefaultVariables)
		{
			if (Variable && !Variable->Name.IsNone())
			{
				VariableOptions.Add(MakeShared<FName>(Variable->Name));
			}
		}
	}

	PropertyOptions.Reset();
	if (Graph && Graph->BoundObjectClass)
	{
		for (TFieldIterator<FProperty> PropertyIt(Graph->BoundObjectClass, EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
		{
			if (IsSupportedBindingProperty(*PropertyIt))
			{
				PropertyOptions.Add(MakeShared<FName>(PropertyIt->GetFName()));
			}
		}
	}

	StructBuilder.AddCustomRow(FText::FromString(TEXT("VariableName")))
	.NameContent()
	[
		VariableNameHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(250.0f)
	[
		SNew(SComboBox<TSharedPtr<FName>>)
		.OptionsSource(&VariableOptions)
		.OnGenerateWidget_Lambda([](TSharedPtr<FName> Item)
		{
			return SNew(STextBlock).Text(Item.IsValid() ? FText::FromName(*Item) : FText::GetEmpty());
		})
		.OnSelectionChanged_Lambda([VariableNameHandle](TSharedPtr<FName> SelectedItem, ESelectInfo::Type)
		{
			if (VariableNameHandle.IsValid() && SelectedItem.IsValid())
			{
				VariableNameHandle->SetValue(*SelectedItem);
			}
		})
		[
			SNew(STextBlock)
			.Text_Lambda([VariableNameHandle]()
			{
				FName CurrentValue;
				return VariableNameHandle.IsValid() && VariableNameHandle->GetValue(CurrentValue) == FPropertyAccess::Success
					? FText::FromName(CurrentValue)
					: FText::FromString(TEXT("Select graph variable"));
			})
		]
	];

	StructBuilder.AddCustomRow(FText::FromString(TEXT("PropertyName")))
	.NameContent()
	[
		PropertyNameHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(250.0f)
	[
		SNew(SComboBox<TSharedPtr<FName>>)
		.OptionsSource(&PropertyOptions)
		.OnGenerateWidget_Lambda([](TSharedPtr<FName> Item)
		{
			return SNew(STextBlock).Text(Item.IsValid() ? FText::FromName(*Item) : FText::GetEmpty());
		})
		.OnSelectionChanged_Lambda([PropertyNameHandle](TSharedPtr<FName> SelectedItem, ESelectInfo::Type)
		{
			if (PropertyNameHandle.IsValid() && SelectedItem.IsValid())
			{
				PropertyNameHandle->SetValue(*SelectedItem);
			}
		})
		[
			SNew(STextBlock)
			.Text_Lambda([PropertyNameHandle, Graph]()
			{
				FName CurrentValue;
				if (PropertyNameHandle.IsValid() && PropertyNameHandle->GetValue(CurrentValue) == FPropertyAccess::Success && !CurrentValue.IsNone())
				{
					return FText::FromName(CurrentValue);
				}

				return Graph && Graph->BoundObjectClass
					? FText::FromString(TEXT("Select bound object property"))
					: FText::FromString(TEXT("Set Bound Object Class first"));
			})
		]
	];

	StructBuilder.AddProperty(WriteBackHandle.ToSharedRef());
}
