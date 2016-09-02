/*
* Your License or Copyright can go here
*/

#include "ITKImageBaseFilter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"

#include "sitkExplicitITK.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageBaseFilter::ITKImageBaseFilter() :
AbstractFilter(),
m_SelectedCellArrayPath("", "", ""),
m_NewCellArrayName(""),
m_SaveAsNewArray(true)
{
  initialize();
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageBaseFilter::~ITKImageBaseFilter()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBaseFilter::initialize()
{
  setErrorCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename FilterType>
void ITKImageBaseFilter::process(bool checkData, FilterType *filter)
{
  IDataArray::Pointer array = getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, getSelectedCellArrayPath());
  if (getErrorCondition() < 0) { return; }
  QString type = array->getTypeAsString();
  if (type.compare("int8_t") == 0)
  {
    process<int8_t>(checkData, filter);
  }
  else if (type.compare("uint8_t") == 0)
  {
    process<uint8_t>(checkData, filter);
  }
  else if (type.compare("int16_t") == 0)
  {
    process<int16_t>(checkData, filter);
  }
  else if (type.compare("uint16_t") == 0)
  {
    process<uint16_t>(checkData, filter);
  }
  else if (type.compare("int32_t") == 0)
  {
    process<int32_t>(checkData, filter);
  }
  else if (type.compare("uint32_t") == 0)
  {
    process<uint32_t>(checkData, filter);
  }
  else if (type.compare("int64_t") == 0)
  {
    process<int64_t>(checkData, filter);
  }
  else if (type.compare("uint64_t") == 0)
  {
    process<uint64_t>(checkData, filter);
  }
  else if (type.compare("float") == 0)
  {
    process<float>(checkData, filter);
  }
  else if (type.compare("double") == 0)
  {
    process<double>(checkData, filter);
  }
  else
  {
    setErrorCondition(-4);
    QString errorMessage = QString("Unsupported pixel type: %1.").arg(type);
    notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition());
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, typename FilterType>
void ITKImageBaseFilter::process(bool checkData, FilterType *filter)
{
  if (checkData)
  {
    this->dataCheck<PixelType>();
  }
  else
  {
    this->filterFct<PixelType>(filter);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBaseFilter::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  typedef itk::Dream3DImage<uint8_t, 3> DefaultPixelType;
  typedef itk::ImageToImageFilter<DefaultPixelType, DefaultPixelType> DefaultFilterType;
  process<DefaultFilterType>(false, NULL); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, typename FilterType>
void ITKImageBaseFilter::filterFct(FilterType *filter)
{
  try
  {
    DataArrayPath dap = getSelectedCellArrayPath();
    DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());

    const unsigned int Dimension = 3;
    typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
    typedef typename itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension> ToImageType;
    // Create a Bridge to wrap an existing DREAM.3D array with an ItkImage container
    ToImageType::Pointer toImage = ToImageType::New();
    toImage->SetInput(dc);
    toImage->SetInPlace(true);
    toImage->SetAttributeMatrixArrayName(getSelectedCellArrayPath().getAttributeMatrixName().toStdString());
    toImage->SetDataArrayName(getSelectedCellArrayPath().getDataArrayName().toStdString());
    //filter->AddObserver(itk::ProgressEvent(), myCommand); // Progress events have to be added to Dream3DToITKImageFilter

    itk::Dream3DFilterInterruption::Pointer interruption = itk::Dream3DFilterInterruption::New();
    interruption->SetFilter(this);

    filter->SetInput(filter->GetOutput());
    filter->AddObserver(itk::ProgressEvent(), interruption);
    filter->Update();
    ImageType *outputImage;// = ImageType::New();
    outputImage = filter->GetOutput();
    outputImage->DisconnectPipeline();

    std::string outputArrayName(getNewCellArrayName().toStdString());

    if (getSaveAsNewArray() == false)
    {
      outputArrayName = m_SelectedCellArrayPath.getDataArrayName().toStdString();
      AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(m_SelectedCellArrayPath.getAttributeMatrixName());
      // Remove the original input data array
      attrMat->removeAttributeArray(m_SelectedCellArrayPath.getDataArrayName());
    }

    typename itk::InPlaceImageToDream3DDataFilter<PixelType, 3>::Pointer toDream3DFilter = itk::InPlaceImageToDream3DDataFilter<PixelType, 3>::New();
    toDream3DFilter->SetInput(outputImage);
    toDream3DFilter->SetInPlace(true);
    toDream3DFilter->SetAttributeMatrixArrayName(getSelectedCellArrayPath().getAttributeMatrixName().toStdString());
    toDream3DFilter->SetDataArrayName(outputArrayName);
    toDream3DFilter->SetDataContainer(dc);
    toDream3DFilter->Update();
  }
  catch (itk::ExceptionObject & err)
  {
    setErrorCondition(-5);
    QString errorMessage = "ITK exception was thrown while filtering input image: %1";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(err.GetDescription()), getErrorCondition());
    return;
  }

  notifyStatusMessage(getHumanLabel(), "Complete");

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename FilterType>
void ITKImageBaseFilter::execute(FilterType *filter)
{
  initialize();
  process<FilterType>(filter);
  if (getErrorCondition() < 0) { return; }
  if (getCancel() == true) { return; }
  process<FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType>
void ITKImageBaseFilter::dataCheck()
{
  this->parametersCheck();
  DataArray<PixelType>::WeakPointer selectedCellArrayPtr;
  PixelType* selectedCellArray;

  setErrorCondition(0);
  DataArrayPath tempPath;

  QVector<size_t> dims(1, 1);
  selectedCellArrayPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<PixelType>, AbstractFilter>(this, getSelectedCellArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if (NULL != selectedCellArrayPtr.lock().get()) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  {
    selectedCellArray = selectedCellArrayPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
  if (getErrorCondition() < 0) { return; }

  ImageGeom::Pointer image = getDataContainerArray()->getDataContainer(getSelectedCellArrayPath().getDataContainerName())->getPrereqGeometry<ImageGeom, AbstractFilter>(this);
  if (getErrorCondition() < 0 || NULL == image.get()) { return; }

  if (m_SaveAsNewArray == true)
  {
    tempPath.update(getSelectedCellArrayPath().getDataContainerName(), getSelectedCellArrayPath().getAttributeMatrixName(), getNewCellArrayName());
    m_NewCellArrayPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<PixelType>, AbstractFilter, PixelType>(this, tempPath, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
    if (NULL != m_NewCellArrayPtr.lock().get()) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
    {
      m_NewCellArray = m_NewCellArrayPtr.lock()->getVoidPointer(0);
    } /* Now assign the raw pointer to data from the DataArray<T> object */
  }
  else
  {
    m_NewCellArrayPtr = DataArray<PixelType>::NullPointer();
    m_NewCellArray = nullptr;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBaseFilter::getCompiledLibraryName()
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBaseFilter::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBaseFilter::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBaseFilter::getGroupName()
{
  return "Image Processing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBaseFilter::getSubGroupName()
{
  return "ITKImageProcessing";
}
