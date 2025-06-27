#include "Slate/CampEditor/DefaultCampParameterOptions.h"
#include <CampRelationshipSystem/Public/CR_Config.h>


#define LOCTEXT_NAMESPACE "PresetCampSelect" 

void SDefaultCampParameterOptions::Construct(const FArguments& InArgs)
{
	ReferenceProperty = InArgs._ReferenceProperty.Get();
	ParameterChangeEvent = InArgs._ParameterChangeEvent;
	DefaultDisplayIndex = InArgs._DefaultDisplayIndex.Get();

	LoadOption();

	FText DefaultDisplay;
	if (ComboBoxOptions.Num() == 0)
	{
		DefaultDisplay = FText::FromString(TEXT("缺失选项"));
	}
	else
	{
		if (!ComboBoxOptions.IsValidIndex(DefaultDisplayIndex))
		{
			DefaultDisplayIndex = 0;
		}
		DefaultDisplay = FText::FromString(*ComboBoxOptions[DefaultDisplayIndex]);
	}

	ChildSlot
	[
		SAssignNew(ComboBoxSlate, SComboBox< TSharedPtr<FString> >)
		.OptionsSource(&ComboBoxOptions)
		.OnGenerateWidget(this, &SDefaultCampParameterOptions::CreateComboBoxRow)//每个下拉选项的样式通过函数构造
		.OnSelectionChanged(this, &SDefaultCampParameterOptions::HandleSelectionChanged)//改变选择的回调
		[
			SAssignNew(TextBlock, STextBlock)
			.Text(DefaultDisplay)
		]
	];
}

//每个下拉选择框的样式
TSharedRef<SWidget> SDefaultCampParameterOptions::CreateComboBoxRow(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
	.Text(FText::FromString(*InItem));
}

void SDefaultCampParameterOptions::HandleSelectionChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectionType)
{
	//SelectChangeEvent.Broadcast(Item);
	if (Item)
	{
		TextBlock->SetText(FText::FromString(*Item));
		//ID是从1开始的，Slate下标是从0开始的，所以这里+1
		ParameterChangeEvent.ExecuteIfBound(ComboBoxOptions.Find(Item) + 1,*Item);
	}
}

void SDefaultCampParameterOptions::RefreshOptions()
{
	if (ComboBoxSlate.IsValid())
	{
		ComboBoxSlate->RefreshOptions();
	}
}

void SDefaultCampParameterOptions::AddOption(FString Option)
{
	ComboBoxOptions.Add(MakeShareable(new FString(Option)));
	RefreshOptions();
}

void SDefaultCampParameterOptions::LoadOption()
{
	ComboBoxOptions.Empty();

	TArray<FString> InfoName;
	if (ReferenceProperty == "PresetCampInfoName")
	{
		ComboBoxOptions.Add(MakeShareable(new FString(LOCTEXT("OptionsSourceCustom", "自定义").ToString())));
		UCR_Config::GetInstance()->PresetCampInfo.GenerateKeyArray(InfoName);
	}
	else if (ReferenceProperty == "AllCamp")
	{
		UCR_Config::GetInstance()->AllCamp.GenerateValueArray(InfoName);
	}

	for (FString s : InfoName)
	{
		ComboBoxOptions.Add(MakeShareable(new FString(s)));
	}
}

#undef LOCTEXT_NAMESPACE
