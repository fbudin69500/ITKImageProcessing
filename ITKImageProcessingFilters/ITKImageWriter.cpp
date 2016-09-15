/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice, this
* list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-10-D-5210
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "ITKImageWriter.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/OutputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessingPlugin.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"


// ITK includes
#include <itkImageFileWriter.h>

// Include the MOC generated file for this class
#include "moc_ITKImageWriter.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageWriter::ITKImageWriter() :
  AbstractFilter(),
  m_FileName(""),
  m_ImageArrayPath("", "", "")
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageWriter::~ITKImageWriter()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::setupFilterParameters()
{
	FilterParameterVector parameters;
  QString supportedExtensions = ITKImageProcessingPlugin::getListSupportedFileExtensions();
	parameters.push_back(SIMPL_NEW_OUTPUT_FILE_FP("Output File", FileName, FilterParameter::Parameter, ITKImageWriter, supportedExtensions));

	parameters.push_back(SeparatorFilterParameter::New("Image Data", FilterParameter::RequiredArray));
	{
		DataArraySelectionFilterParameter::RequirementType req =
			DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
			                                                     SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
	  parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Image", ImageArrayPath, FilterParameter::RequiredArray, ITKImageWriter, req));
	}
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setFileName(reader->readString("FileName", getFileName()));
  setImageArrayPath(reader->readDataArrayPath("ImageArrayPath", getImageArrayPath()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::initialize()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::dataCheck()
{
  //check file name exists
  QString filename = getFileName();
  if (filename.isEmpty())
  {
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), "Invalid filename.", getErrorCondition());
    return;
  }

  DataContainerArray::Pointer containerArray = getDataContainerArray();
  if (!containerArray)
  {
    setErrorCondition(-2);
    notifyErrorMessage(getHumanLabel(), "No container array.", getErrorCondition());
    return;
  }

  ImageGeom::Pointer imageGeometry =
    getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom, AbstractFilter>(
      this, getImageArrayPath().getDataContainerName());
  if (!imageGeometry.get())
  {
    setErrorCondition(-3);
    notifyErrorMessage(getHumanLabel(), "No image geometry.", getErrorCondition());
    return;
  }

  // If we got here, that means that there is no error
  setErrorCondition(0);
  setWarningCondition(0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename TPixel, unsigned int dimensions>
void ITKImageWriter::writeImage()
{
  typedef itk::Dream3DImage<TPixel, dimensions>   ImageType;
  typedef itk::ImageFileWriter<ImageType>         WriterType;
  typedef itk::InPlaceDream3DDataToImageFilter<TPixel, dimensions> ToITKType;
  DataArrayPath path = getImageArrayPath();
  DataContainer::Pointer container =
    getDataContainerArray()->getDataContainer(
    path.getDataContainerName());
  try
  {
      typename ToITKType::Pointer toITK = ToITKType::New();
      toITK->SetInput(container);
      toITK->SetAttributeMatrixArrayName(path.getAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(path.getDataArrayName().toStdString());
      toITK->SetInPlace(true);
      typename WriterType::Pointer writer = WriterType::New();
      writer->SetInput(toITK->GetOutput());
      writer->SetFileName(getFileName().toStdString().c_str());
      writer->UseCompressionOn();
      writer->Update();
  }
  catch (itk::ExceptionObject & err)
  {
      setErrorCondition(-5);
      QString errorMessage = "ITK exception was thrown while writing output file: %1";
      notifyErrorMessage(getHumanLabel(), errorMessage.arg(err.GetDescription()), getErrorCondition());
      return;
  }
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType>
void ITKImageWriter::writeImage()
{
  ImageGeom::Pointer imageGeometry =
    getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom, AbstractFilter>(
    this, getImageArrayPath().getDataContainerName());
  QVector<float> resolution(3, 0);
  imageGeometry->getResolution(resolution[0], resolution[1], resolution[2]);
  if (resolution[2] < 0.000001) // Epsilon since comparing floats, not ints
  {
    // 2D image
    writeImage<PixelType, 2>();
  }
  else
  {
    //3D
    writeImage<PixelType, 3>();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if (getErrorCondition() < 0)
  {
    return;
  }
  DataArrayPath path = getImageArrayPath();
  if (!getDataContainerArray()->doesDataContainerExist(path.getDataContainerName()))
  {
    setErrorCondition(-6);
    QString errorMessage = "Data container %1 does not exist.";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(path.getDataContainerName()), getErrorCondition());
    return;
  }
  DataContainer::Pointer container =
    getDataContainerArray()->getDataContainer(
    path.getDataContainerName());
  if (!container->doesAttributeMatrixExist(path.getAttributeMatrixName()))
  {
    setErrorCondition(-7);
    QString errorMessage = "Attribute matrix %1 does not exist.";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(path.getAttributeMatrixName()), getErrorCondition());
    return;
  }
  AttributeMatrix::Pointer attributeMatrix = container->getAttributeMatrix(
    path.getAttributeMatrixName());
  if (!attributeMatrix->doesAttributeArrayExist(path.getDataArrayName()))
  {
    setErrorCondition(-8);
    QString errorMessage = "Attribute array %1 does not exist.";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(path.getDataArrayName()), getErrorCondition());
    return;
  }
  Dream3DArraySwitchMacro(writeImage, getImageArrayPath(), -4);
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImageWriter::newFilterInstance(bool copyFilterParameters)
{
  ITKImageWriter::Pointer filter = ITKImageWriter::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageWriter::getCompiledLibraryName()
{ return ITKImageProcessingConstants::ITKImageProcessingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageWriter::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageWriter::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageWriter::getGroupName()
{ return "Image Processing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageWriter::getSubGroupName()
{ return "ITKImageProcessing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageWriter::getHumanLabel()
{ return "[ITK] ImageWriter (KW)"; }

