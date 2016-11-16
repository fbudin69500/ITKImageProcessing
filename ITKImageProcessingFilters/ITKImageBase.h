/*
 * Your License or Copyright can go here
 */

#ifndef _ITKImageBase_h_
#define _ITKImageBase_h_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/AbstractFilter.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

#include "itkImageToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DFilterInterruption.h"

/**
 * @brief The ITKImageBase class. See [Filter documentation](@ref ITKImageBase) for details.
 */
class ITKImageBase : public AbstractFilter
{
  Q_OBJECT

  public:
    //SIMPL_SHARED_POINTERS(ITKImageBase)
    //SIMPL_TYPE_MACRO_SUPER(ITKImageBase, AbstractFilter)

    virtual ~ITKImageBase();

    SIMPL_FILTER_PARAMETER(DataArrayPath, SelectedCellArrayPath)
    Q_PROPERTY(DataArrayPath SelectedCellArrayPath READ getSelectedCellArrayPath WRITE setSelectedCellArrayPath)

    SIMPL_FILTER_PARAMETER(QString, NewCellArrayName)
    Q_PROPERTY(QString NewCellArrayName READ getNewCellArrayName WRITE setNewCellArrayName)

    SIMPL_FILTER_PARAMETER(bool, SaveAsNewArray)
    Q_PROPERTY(bool SaveAsNewArray READ getSaveAsNewArray WRITE setSaveAsNewArray)

    /**
     * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getCompiledLibraryName();

    /**
     * @brief getBrandingString Returns the branding string for the filter, which is a tag
     * used to denote the filter's association with specific plugins
     * @return Branding string
    */
    virtual const QString getBrandingString();

    /**
     * @brief getFilterVersion Returns a version string for this filter. Default
     * value is an empty string.
     * @return
     */
    virtual const QString getFilterVersion();

    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) = 0;

    /**
     * @brief getGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getGroupName();

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel() = 0;

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters() = 0;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) = 0;

    /**
     * @brief execute Reimplemented from @see AbstractFilter class
     */
    virtual void execute();

    /**
    * @brief preflight Reimplemented from @see AbstractFilter class
    */
    virtual void preflight();

    /**
     * @brief CastVec3ToITK Input type should be FloatVec3_t or IntVec3_t, Output
       type should be some kind of ITK "array" (itk::Size, itk::Index,...)
     */
    template<typename InputType, typename OutputType, typename ComponentType>
    OutputType CastVec3ToITK(const InputType &inputVec3, unsigned int dimension) const
    {
      OutputType output;
      if (dimension > 0)
      {
        output[0] = static_cast<ComponentType>(inputVec3.x);
        if (dimension > 1)
        {
          output[1] = static_cast<ComponentType>(inputVec3.y);
          if (dimension > 2)
          {
            output[2] = static_cast<ComponentType>(inputVec3.z);
          }
        }
      }
      return output;
    }
    /**
     * @brief StaticCast Performs a static cast on a value. 'unused' type is to match 'CastStdToVec3' template format to simplify template declaration in conversion script.
     */
    template<typename InputType, typename OutputType, typename unused>
    OutputType StaticCastScalar(const InputType &val) const
    {
      return static_cast<OutputType>(val);
    }

    /**
     * @brief CastStdToVec3 Input type should be std::vector<float> or std::vector<int>
       and Output type should be FloatVec3_t or IntVec3_t
     */
    template<typename InputType, typename OutputType, typename ComponentType>
    OutputType CastStdToVec3(const InputType &inputVector) const
    {
      OutputType outputVec3;
      if (inputVector.size() > 0)
      {
        outputVec3.x = static_cast<ComponentType>(inputVector[0]);
        if (inputVector.size() > 1)
        {
          outputVec3.y = static_cast<ComponentType>(inputVector[1]);
          if (inputVector.size() > 2)
          {
            outputVec3.z = static_cast<ComponentType>(inputVector[2]);
          }
        }
      }
      return outputVec3;
    }

  signals:
    /**
     * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
     * be pushed from a user-facing control (such as a widget)
     * @param filter Filter instance pointer 
     */
    void updateFilterParameters(AbstractFilter* filter);

    /**
     * @brief parametersChanged Emitted when any Filter parameter is changed internally
     */
    void parametersChanged();

    /**
     * @brief preflightAboutToExecute Emitted just before calling dataCheck()
     */
    void preflightAboutToExecute();

    /**
     * @brief preflightExecuted Emitted just after calling dataCheck()
     */
    void preflightExecuted();

  protected:
    ITKImageBase();

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    void virtual dataCheckInternal() = 0;

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    template<typename PixelType, unsigned int Dimension>
    void dataCheck()
    {
      // Check data array
      typename DataArray<PixelType>::WeakPointer selectedCellArrayPtr;
      PixelType* selectedCellArray;

      DataArrayPath tempPath;

      QVector<size_t> dims(1, 1);
      selectedCellArrayPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<PixelType>, AbstractFilter>(this, getSelectedCellArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
      if (nullptr != selectedCellArrayPtr.lock().get()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
      {
        selectedCellArray = selectedCellArrayPtr.lock()->getPointer(0);
      } /* Now assign the raw pointer to data from the DataArray<T> object */
      if (getErrorCondition() < 0) { return; }

      ImageGeom::Pointer image = getDataContainerArray()->getDataContainer(getSelectedCellArrayPath().getDataContainerName())->getPrereqGeometry<ImageGeom, AbstractFilter>(this);
      if (getErrorCondition() < 0 || nullptr == image.get()) { return; }

      if (m_SaveAsNewArray == true)
      {
        tempPath.update(getSelectedCellArrayPath().getDataContainerName(), getSelectedCellArrayPath().getAttributeMatrixName(), getNewCellArrayName());
        m_NewCellArrayPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<PixelType>, AbstractFilter, PixelType>(this, tempPath, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
        if (nullptr != m_NewCellArrayPtr.lock().get()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
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

    /**
    * @brief Applies the filter
    */
    template<typename PixelType, unsigned int Dimension, typename FilterType>
    void filter(FilterType* filter)
    {
      try
    {
      DataArrayPath dap = getSelectedCellArrayPath();
      DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());

      typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
      typedef itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension> toITKType;
      // Create a Bridge to wrap an existing DREAM.3D array with an ItkImage container
      typename toITKType::Pointer toITK = toITKType::New();
      toITK->SetInput(dc);
      toITK->SetInPlace(true);
      toITK->SetAttributeMatrixArrayName(getSelectedCellArrayPath().getAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(getSelectedCellArrayPath().getDataArrayName().toStdString());

      itk::Dream3DFilterInterruption::Pointer interruption = itk::Dream3DFilterInterruption::New();
      interruption->SetFilter(this);

      // Set up filter
      filter->SetInput(toITK->GetOutput());
      filter->AddObserver(itk::ProgressEvent(), interruption);
      filter->Update();

      typename ImageType::Pointer image = ImageType::New();
      image = filter->GetOutput();
      image->DisconnectPipeline();
      std::string outputArrayName(getNewCellArrayName().toStdString());

      if (getSaveAsNewArray() == false)
      {
        outputArrayName = getSelectedCellArrayPath().getDataArrayName().toStdString();
        AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(getSelectedCellArrayPath().getAttributeMatrixName());
        // Remove the original input data array
        attrMat->removeAttributeArray(getSelectedCellArrayPath().getDataArrayName());
      }

      typename itk::InPlaceImageToDream3DDataFilter<PixelType, Dimension>::Pointer toDream3DFilter = itk::InPlaceImageToDream3DDataFilter<PixelType, Dimension>::New();
      toDream3DFilter->SetInput(image);
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


    /**
    * @brief CheckIntegerEntry: Input types can only be of certain types (float, double, bool, int).
      For the other type, we have to use one of this primitive type, and verify that the
      value corresponds to what is expected.
      The 3rd parameter, 'bool' is given to match the definition of CheckVectorEntry. This allows
      to use a dictionary in Python to choose between the 2 functions.
    */
    template<typename VarType, typename SubsType>
    void CheckIntegerEntry(SubsType value, QString name, bool)
    {
      SubsType lowest = static_cast<SubsType>(std::numeric_limits<VarType>::lowest());
      SubsType max = static_cast<SubsType>(std::numeric_limits<VarType>::max());
      if (value < lowest
       || value > max
       || value != floor(value)
         )
      {
        setErrorCondition(-1);
        QString errorMessage = name + QString(
          " must be greater or equal than %1 and lesser or equal than %2 and an integer");
        notifyErrorMessage(getHumanLabel(), errorMessage.arg(lowest).arg(max)
                                                             , getErrorCondition()
                                                             );
      }
    }

    /**
    * @brief CheckVectorEntry: Vector input types can only be of certain types (float or int).
      For the other type, we have to use one of this primitive type, and verify that the
      value corresponds to what is expected.
    */
    template<typename VarType, typename SubsType>
    void CheckVectorEntry(SubsType value, QString name, bool integer)
    {
      float lowest = static_cast<float>(std::numeric_limits<VarType>::lowest());
      float max = static_cast<float>(std::numeric_limits<VarType>::max());
      if (value.x < lowest || value.x > max
         || (integer && value.x != floor(value.x))
         || value.y < lowest || value.y > max
         || (integer && value.y != floor(value.y))
         || value.z < lowest || value.z > max
         || (integer && value.z != floor(value.z))
         )
      {
        setErrorCondition(-1);
        QString errorMessage = name + QString(
          " values must be greater or equal than %1 and lesser or equal than %2");
        if(integer)
        {
          errorMessage += QString(" and integers");
        }
        notifyErrorMessage(getHumanLabel(), errorMessage.arg(lowest).arg(max),
                           getErrorCondition() );
      }
    }

    /**
    * @brief Applies the filter
    */
    void virtual filterInternal() = 0 ;

    /**
     * @brief Initializes all the private instance variables.
     */
    void initialize();

  private:

    DEFINE_IDATAARRAY_VARIABLE(NewCellArray)

    ITKImageBase(const ITKImageBase&); // Copy Constructor Not Implemented
    void operator=(const ITKImageBase&); // Operator '=' Not Implemented
};

#endif /* _ITKImageBase_H_ */
