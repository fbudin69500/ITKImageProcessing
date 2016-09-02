/*
* Your License or Copyright can go here
*/

#ifndef _ITKImageBaseFilter_h_
#define _ITKImageBaseFilter_h_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/AbstractFilter.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DFilterInterruption.h"

/**
* @brief Virtual class to facilitate implementation of filters using ITK images and ITK filters. See [Filter documentation](@ref ITKImageBaseFilter) for details.
*/
class ITKImageBaseFilter : public AbstractFilter
{
  Q_OBJECT

public:
    SIMPL_SHARED_POINTERS(ITKImageBaseFilter)
    SIMPL_TYPE_MACRO_SUPER(ITKImageBaseFilter, AbstractFilter)

    virtual ~ITKImageBaseFilter();

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
  * @brief getGroupName Reimplemented from @see AbstractFilter class
  */
  virtual const QString getGroupName();

  /**
  * @brief getSubGroupName Reimplemented from @see AbstractFilter class
  */
  virtual const QString getSubGroupName();

  /**
  * @brief execute runs filter. @see AbstractFilter class
  */
  template<typename FilterType>
  void execute(FilterType *filter);

  /**
  * @brief preflight Reimplemented from @see AbstractFilter class
  */
  virtual void preflight();

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
  ITKImageBaseFilter();
  /**
  * @brief dataCheck Checks for the appropriate availability of arrays
  */
  template<typename PixelType>
  void dataCheck();
  
  /**
  * @brief parametersCheck Checks for the appropriate parameter values
  */
  virtual void parametersCheck() = 0;
  /**
  * @brief process checks the pixel type of input data
  */
  template<typename FilterType>
  void process(bool checkData, FilterType *filter);

  /**
  * @brief process (templated) starts either dataCheck() or filter()
  */
  template<typename PixelType, typename FilterType>
  void process(bool checkData, FilterType *filter);

  /**
  * @brief Extracts image from data container, starts filterITKImage(), and converts ITKImage back to data container
  */
  template<typename PixelType, typename FilterType>
  void filterFct(FilterType *filter);

  /**
  * @brief Initializes all the private instance variables.
  */
  void initialize();


private:

  DEFINE_IDATAARRAY_VARIABLE(NewCellArray)

  ITKImageBaseFilter(const ITKImageBaseFilter&); // Copy Constructor Not Implemented
  void operator=(const ITKImageBaseFilter&); // Operator '=' Not Implemented
};

#endif /* _ITKImageBaseFilter_H_ */
