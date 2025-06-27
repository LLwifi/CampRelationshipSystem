// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Widgets/SCompoundWidget.h>

DECLARE_DELEGATE_TwoParams(FParameterChangeEvent, int32, /*Index,*/ FString/*, Option*/);


/*默认阵营参数选择面板
*/
class SDefaultCampParameterOptions : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SDefaultCampParameterOptions)
	{}

	SLATE_ATTRIBUTE(FString, ReferenceProperty)
	SLATE_EVENT(FParameterChangeEvent, ParameterChangeEvent)
	SLATE_ATTRIBUTE(int32, DefaultDisplayIndex)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	//每个下拉选择框的样式
	TSharedRef<SWidget> CreateComboBoxRow(TSharedPtr<FString> InItem);

	//选择回调
	void HandleSelectionChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectionType);

	//刷新选项
	void RefreshOptions();

	//添加选项
	void AddOption(FString Option);

public:
	//重新读取选项
	void LoadOption();

public: 
	//Slate
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBoxSlate;
	//显示选中的TextSlate
	TSharedPtr<STextBlock> TextBlock;
	//引用属性
	FString ReferenceProperty;
	//下拉选项
	TArray<TSharedPtr<FString>> ComboBoxOptions;

	int32 DefaultDisplayIndex = 0;

	FParameterChangeEvent ParameterChangeEvent;
};

