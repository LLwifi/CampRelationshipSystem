#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include <CampRelationshipSystem/Public/CR_Config.h>

//阵营操作结构体的自定义细节面板
class ICampInfoOperationPropertyTypeCustom : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(
	TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder,
	IPropertyTypeCustomizationUtils& CustomizationUtils) override;

public:
	void CampInfoChange(FCR_CampInfo NewCampInfo);

public:
	TSharedPtr<IPropertyHandle> CampInfoHandle;
	TSharedPtr<IPropertyHandle>	RelationshipKeyProperty;
	TSharedPtr<IPropertyHandle>	RelationshipValueProperty;
	TSharedPtr<IPropertyHandle> MyClamProperty;

};
