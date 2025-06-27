#include "Customization/CampInfoOperationPropertyTypeCustom.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include <CampRelationshipSystem/Public/CR_Config.h>
#include "Misc/Paths.h"
#include "Slate/CampEditor/CampSet.h"


#define LOCTEXT_NAMESPACE "ICampInfoOperationPropertyTypeCustom"

TSharedRef<IPropertyTypeCustomization> ICampInfoOperationPropertyTypeCustom::MakeInstance()
{
	return MakeShareable(new ICampInfoOperationPropertyTypeCustom());
}

void ICampInfoOperationPropertyTypeCustom::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		];
}

void ICampInfoOperationPropertyTypeCustom::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	CampInfoHandle = PropertyHandle;//->GetParentHandle();
	RelationshipKeyProperty = CampInfoHandle->GetChildHandle("RelationshipKey");
	RelationshipValueProperty = CampInfoHandle->GetChildHandle("RelationshipValue");
	MyClamProperty = CampInfoHandle->GetChildHandle("MyClam");

	void* ValuePtr = nullptr;
	FCR_CampInfo CampInfo;
	if (CampInfoHandle->GetValueData(ValuePtr) != FPropertyAccess::Fail)
	{
		CampInfo = *(FCR_CampInfo*)ValuePtr;
	}

	ChildBuilder.AddCustomRow(FText())
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		[
			SNew(SCampSet)
			.CampInfo(CampInfo)
			.CampSetChangeEvent(this,&ICampInfoOperationPropertyTypeCustom::CampInfoChange)
		]

		];
}

void ICampInfoOperationPropertyTypeCustom::CampInfoChange(FCR_CampInfo NewCampInfo)
{
	if (MyClamProperty.IsValid())
	{
		MyClamProperty->SetValue(NewCampInfo.MyClam);
	}

	TSharedPtr<IPropertyHandleArray> ArrayKeyProperty = RelationshipKeyProperty->AsArray();
	TSharedPtr<IPropertyHandleArray> ArrayValueProperty = RelationshipValueProperty->AsArray();

	TArray<int32> Keys = NewCampInfo.RelationshipKey;
	TArray<int32> Values = NewCampInfo.RelationshipValue;

	if (ArrayKeyProperty.IsValid() && ArrayValueProperty.IsValid())
	{
		uint32 ArrayNum;
		RelationshipKeyProperty->GetNumChildren(ArrayNum);
		UCR_Config* CDS = UCR_Config::GetInstance();
		if (ArrayNum != CDS->AllCamp.Num())
		{
			ArrayKeyProperty->EmptyArray();
			ArrayValueProperty->EmptyArray();
			//先往Array中把需要的元素数量全部创建出来
			for (int32 ChildIdx = 0; ChildIdx < CDS->AllCamp.Num(); ChildIdx++)
			{
				ArrayKeyProperty->AddItem();
				ArrayValueProperty->AddItem();
			}
		}

		//再根据真实的值去设置每个键值对的值
		for (int32 ChildIdx = 0; ChildIdx < CDS->AllCamp.Num(); ChildIdx++)
		{
			TSharedPtr<IPropertyHandle> ChildKeyProperty = ArrayKeyProperty->GetElement(ChildIdx);
			TSharedPtr<IPropertyHandle> ChildValueProperty = ArrayValueProperty->GetElement(ChildIdx);

			if (ChildKeyProperty.IsValid() && ChildValueProperty.IsValid() && Keys.IsValidIndex(ChildIdx) && Values.IsValidIndex(ChildIdx))
			{
				// Set Key
				ChildKeyProperty->SetValue(Keys[ChildIdx]);

				// Set Value
				ChildValueProperty->SetValue(Values[ChildIdx]);
			}
		}


	}

	//UCampDeveloperSettings::GetInstance()->SaveConfig(CPF_Config, *UCampDeveloperSettings::GetInstance()->GetDefaultConfigFilename());
}

#undef LOCTEXT_NAMESPACE