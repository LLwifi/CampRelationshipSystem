#include <Slate/CampEditor/OneCampSet.h>
#include "Slate/CampEditor/DefaultCampParameterOptions.h"
#include <CampRelationshipSystem/Public/CR_Config.h>

#define LOCTEXT_NAMESPACE "OneCampSet"

void SOneCampSet::Construct(const FArguments& InArgs)
{
	CampName = InArgs._CampName.Get();
	FillSize = InArgs._FillSize.Get();
	CheckBoxChangeEvent = InArgs._CheckBoxChangeEvent;
	InitCheckIndex = InArgs._InitCheckIndex.Get();

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(FillSize.X)
		[
			SNew(STextBlock)
			.Text(FText::FromString(*CampName))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(FillSize.Y)
		[
			SAssignNew(HorizontalBox, SHorizontalBox)
		]
	];

	TArray<FString> RelationshipName;
	UCR_Config::GetInstance()->AllRelationship.GenerateValueArray(RelationshipName);
	TSharedPtr<SCheckBox > CheckBox;
	for (int32 i = 0; i < RelationshipName.Num(); i++)
	{
		HorizontalBox->AddSlot()
			.FillWidth(1.0f)
			[
				SAssignNew(CheckBox, SCheckBox)
				.OnCheckStateChanged_Lambda([&](ECheckBoxState NewState)
					{
						CheckBoxChange(i);
					})
			];
		AllCheckBox.Add(CheckBox);
	}

	CheckBoxChange(InitCheckIndex);
}

void SOneCampSet::CheckBoxChange(int32 CheckBoxIndex)
{
	for (int32 i = 0; i < AllCheckBox.Num(); i++)
	{
		if (CheckBoxIndex == i)
		{
			AllCheckBox[i]->SetIsChecked(ECheckBoxState::Checked);
		}
		else
		{
			AllCheckBox[i]->SetIsChecked(ECheckBoxState::Unchecked);
		}
	}
	CheckBoxChangeEvent.ExecuteIfBound(CheckBoxIndex);
}

void SOneCampSet::SetEnabled(bool IsEnabled)
{
	for (TSharedPtr<SCheckBox> CheckBox : AllCheckBox)
	{
		CheckBox->SetEnabled(IsEnabled);
	}
}

#undef LOCTEXT_NAMESPACE
