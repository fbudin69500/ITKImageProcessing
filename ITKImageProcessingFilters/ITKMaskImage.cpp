// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKMaskImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#define DREAM3D_USE_Vector 1
#define DREAM3D_USE_RGBA   1
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

// Include the MOC generated file for this class
#include "moc_ITKMaskImage.cpp"

template<class PixelType>
struct MaskOutsideValue
{
static void Value(double value, PixelType &pixel)
{
  pixel = static_cast<PixelType>( value );
}
};

template<class PixelType >
struct MaskOutsideValue<itk::RGBAPixel<PixelType> >
{
static void Value(double value, itk::RGBAPixel<PixelType> &pixel)
{
  pixel = static_cast<itk::RGBAPixel<PixelType> >( value );
  pixel.SetAlpha(0);
}
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMaskImage::ITKMaskImage() :
  ITKImageBase()
{
  m_OutsideValue=StaticCastScalar<double,double,double>(0);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMaskImage::~ITKMaskImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaskImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKMaskImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKMaskImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKMaskImage, req));
      parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Mask Array", MaskCellArrayPath, FilterParameter::RequiredArray, ITKMaskImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKMaskImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaskImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setSelectedCellArrayPath( reader->readDataArrayPath( "MaskCellArrayPath", getMaskCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMaskImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  QVector<QString> supportedTypes;
  supportedTypes << "uint8_t" << "uint16_t" << "uint32_t" << "uint64_t";
  checkImageType(supportedTypes, getMaskCellArrayPath());
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaskImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMaskImage::convertDataContainerType()
{
  typedef itk::Dream3DImage<InputPixelType,Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType,Dimension> OutputImageType;
  typedef itk::InPlaceDream3DDataToImageFilter<InputPixelType, Dimension> toITKType;
  typedef itk::InPlaceImageToDream3DDataFilter<OutputPixelType, Dimension> toDream3DType;
  typedef itk::CastImageFilter<InputImageType, OutputImageType> CastType;
  DataArrayPath dap = getMaskCellArrayPath();
  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());
  try
  {
    // Create a bridge to wrap an existing DREAM.3D array with an ItkImage container
    typename toITKType::Pointer toITK = toITKType::New();
    toITK->SetInput(dc);
    toITK->SetInPlace(true);
    toITK->SetAttributeMatrixArrayName(dap.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(dap.getDataArrayName().toStdString());
    toITK->Update();
    typename CastType::Pointer cast = CastType::New();
    cast->SetInput(toITK->GetOutput());
    cast->Update();
    // Convert back to dream3D array
    DataContainer::Pointer container =
    getMaskContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, dap.getDataContainerName());
    if (!container.get())
    {
      setErrorCondition(-3);
      notifyErrorMessage(getHumanLabel(), "No container.", getErrorCondition());
      return;
    }
    QVector<size_t> dims = ITKDream3DHelper::GetComponentsDimensions<OutputPixelType>();
    DataContainer::Pointer dcMask = getMaskContainerArray()->getDataContainer(dap.getDataContainerName());
    typename toDream3DType::Pointer toDream3D = toDream3DType::New();
    toDream3D->SetInput(cast->GetOutput());
    toDream3D->SetInPlace(true);
    toDream3D->SetAttributeMatrixArrayName(dap.getAttributeMatrixName().toStdString());
    toDream3D->SetDataArrayName(dap.getDataArrayName().toStdString());
    toDream3D->SetDataContainer(dcMask);
    toDream3D->Update();
  }
  catch (itk::ExceptionObject & err)
  {
    setErrorCondition(-17);
    QString errorMessage = "ITK exception was thrown while converting mask image: %1";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(err.GetDescription()), getErrorCondition());
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMaskImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<uint64_t, Dimension> MaskImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  //define filter
  typedef  itk::MaskImageFilter< InputImageType, MaskImageType, OutputImageType > FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  // Set mask image.
  try
  {
    DataArrayPath dap = getMaskCellArrayPath();
    DataContainer::Pointer dcMask = getMaskContainerArray()->getDataContainer(dap.getDataContainerName());
    typedef itk::InPlaceDream3DDataToImageFilter<uint64_t, Dimension> toITKType;
    typename toITKType::Pointer toITK = toITKType::New();
    toITK->SetInput(dcMask);
    toITK->SetInPlace(true);
    toITK->SetAttributeMatrixArrayName(dap.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(dap.getDataArrayName().toStdString());
    toITK->Update();
    filter->SetMaskImage(toITK->GetOutput());
  }
  catch (itk::ExceptionObject & err)
  {
    setErrorCondition(-18);
    QString errorMessage = "ITK exception was thrown while converting mask image: %1";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(err.GetDescription()), getErrorCondition());
    return;
  }
  typename OutputImageType::PixelType v;
  size_t NumberOfComponents = 1;
  QVector<size_t> cDims = ITKDream3DHelper::GetComponentsDimensions<InputPixelType>();
  for( int ii = 0 ; ii < cDims.size() ; ii++)
  {
    NumberOfComponents *= cDims[ii];
  }
  //itk::NumericTraits<typename OutputImageType::PixelType>::SetLength( v, NumberOfComponents );
  MaskOutsideValue<typename OutputImageType::PixelType>::Value(m_OutsideValue, v);
  //v = static_cast<OutputPixelType>( m_OutsideValue );
  filter->SetOutsideValue( v );
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaskImage::filterInternal()
{
  m_MaskContainerArray = DataContainerArray::New();
  // Convert Mask image to uint64
  Dream3DArraySwitchMacroOutputType(this->convertDataContainerType, getMaskCellArrayPath(), -4, uint64_t, 0);

  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
  m_MaskContainerArray = nullptr; // Free the memory used by the casted mask image
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKMaskImage::newFilterInstance(bool copyFilterParameters)
{
  ITKMaskImage::Pointer filter = ITKMaskImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMaskImage::getHumanLabel()
{ return "ITK::Mask Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMaskImage::getSubGroupName()
{ return "ITK ImageIntensity"; }


