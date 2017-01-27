// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKWhiteTopHatImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/SimpleITKEnums.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"


#include <itkFlatStructuringElement.h>
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

// Include the MOC generated file for this class
#include "moc_ITKWhiteTopHatImage.cpp"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKWhiteTopHatImage::ITKWhiteTopHatImage() :
  ITKImageBase()
{
  m_SafeBorder=StaticCastScalar<bool,bool,bool>(true);
  m_KernelRadius=1;
  m_KernelType=StaticCastScalar<int,int,int>(itk::simple::sitkBall);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKWhiteTopHatImage::~ITKWhiteTopHatImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKWhiteTopHatImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  // Structuring element
  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Kernel Type");
    parameter->setPropertyName("KernelType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKWhiteTopHatImage, this, KernelType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKWhiteTopHatImage, this, KernelType));

    QVector<QString> choices;
    choices.push_back("Annulus");
    choices.push_back("Ball");
    choices.push_back("Box");
    choices.push_back("Cross");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Parameter);
    parameters.push_back(parameter);
  }
  // Other parameters
  parameters.push_back(SIMPL_NEW_INTEGER_FP("KernelRadius", KernelRadius, FilterParameter::Parameter, ITKWhiteTopHatImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKWhiteTopHatImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKWhiteTopHatImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKWhiteTopHatImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKWhiteTopHatImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setSafeBorder(reader->readValue("SafeBorder", getSafeBorder()));
  setKernelRadius(reader->readValue("KernelRadius", getKernelRadius()));
  setKernelType(reader->readValue("KernelType", getKernelType()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKWhiteTopHatImage::dataCheck()
{
  // Check consistency of parameters
  if(m_KernelRadius < 1)
  {
     setErrorCondition(-1);
     QString errorMessage = QString(
          "KernelRadius values must be greater or equal than 1");
     notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition() );
  }

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKWhiteTopHatImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKWhiteTopHatImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  typedef itk::FlatStructuringElement< Dimension > StructuringElementType;
  typedef typename StructuringElementType::RadiusType RadiusType;
  RadiusType elementRadius;
  elementRadius.Fill(m_KernelRadius);
  StructuringElementType structuringElement;
  switch(getKernelType())
  {
    case 0:
       structuringElement = StructuringElementType::Annulus(elementRadius, false);
       break;
    case 1:
       structuringElement = StructuringElementType::Ball(elementRadius, false);
       break;
    case 2:
       structuringElement = StructuringElementType::Box(elementRadius);
       break;
    case 3:
       structuringElement = StructuringElementType::Cross(elementRadius);
       break;
    default:
       setErrorCondition(-20);
       notifyErrorMessage(getHumanLabel(), "Unsupported structuring element", getErrorCondition());
       return;
  }
  //define filter
  typedef itk::WhiteTopHatImageFilter<InputImageType, OutputImageType, StructuringElementType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetKernel(structuringElement);
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKWhiteTopHatImage::filterInternal()
{
    Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKWhiteTopHatImage::newFilterInstance(bool copyFilterParameters)
{
  ITKWhiteTopHatImage::Pointer filter = ITKWhiteTopHatImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKWhiteTopHatImage::getHumanLabel()
{ return "ITK::White Top Hat Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKWhiteTopHatImage::getSubGroupName()
{ return "ITK MathematicalMorphology"; }

