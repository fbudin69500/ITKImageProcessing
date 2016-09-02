/*
 * Your License or Copyright can go here
 */

#include "Itk_GaussianBlur.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"


#include <itkDiscreteGaussianImageFilter.h>

// Include the MOC generated file for this class
#include "moc_Itk_GaussianBlur.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Itk_GaussianBlur::Itk_GaussianBlur() :
  ITKImageBaseFilter(),
  m_Variance(1),
  m_MaximumError(0.01),
  m_MaximumKernelWidth(4)
{
  this->initialize();
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Itk_GaussianBlur::~Itk_GaussianBlur()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_GaussianBlur::setupFilterParameters()
{
  FilterParameterVector parameters;
  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, Itk_GaussianBlur, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Blurred Array", NewCellArrayName, FilterParameter::CreatedArray, Itk_GaussianBlur));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to Blur", SelectedCellArrayPath, FilterParameter::RequiredArray, Itk_GaussianBlur, req));
  }
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Maximum Kernel Width", MaximumKernelWidth, FilterParameter::Parameter, Itk_GaussianBlur));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Variance", Variance, FilterParameter::Parameter, Itk_GaussianBlur));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Maximum Error", MaximumError, FilterParameter::Parameter, Itk_GaussianBlur));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_GaussianBlur::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void Itk_GaussianBlur::parametersCheck()
{
  if (m_MaximumKernelWidth <= 0)
  {
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), "Maximum kernel width must be >0", getErrorCondition());
    return;
  }
  if (m_Variance <= 0)
  {
    setErrorCondition(-2);
    notifyErrorMessage(getHumanLabel(), "Variance must be >0", getErrorCondition());
    return;
  }
  if (m_MaximumError <= 0)
  {
    setErrorCondition(-3);
    notifyErrorMessage(getHumanLabel(), "Maximum error must be >0", getErrorCondition());
    return;
  }
}

template<typename PixelType>
void Itk_GaussianBlur::gaussian()
{
  typedef typename itk::Dream3DImage<PixelType, 3> ImageType;
  typedef typename itk::DiscreteGaussianImageFilter<ImageType, ImageType> GaussianFilterType;
  GaussianFilterType::Pointer gaussianFilter = GaussianFilterType::New();
  gaussianFilter->SetVariance(1);
  gaussianFilter->SetMaximumKernelWidth(5);
  gaussianFilter->SetMaximumError(0.01);
  this->execute<GaussianFilterType::Pointer>(gaussianFilter.GetPointer());
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_GaussianBlur::execute()
{
  IDataArray::Pointer array = getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, getSelectedCellArrayPath());
  if (getErrorCondition() < 0) { return; }
  QString type = array->getTypeAsString();
  if (type.compare("int8_t") == 0)
  {
    gaussian<int8_t>();
  }
  else if (type.compare("uint8_t") == 0)
  {
    gaussian<uint8_t>();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer Itk_GaussianBlur::newFilterInstance(bool copyFilterParameters)
{
  Itk_GaussianBlur::Pointer filter = Itk_GaussianBlur::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_GaussianBlur::getHumanLabel()
{ return "[ITK] GaussianBlur (KW)"; }

