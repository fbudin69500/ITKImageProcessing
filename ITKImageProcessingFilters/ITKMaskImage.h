// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMaskImage_h_
#define _ITKMaskImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkMaskImageFilter.h>

/**
 * @brief The ITKMaskImage class. See [Filter documentation](@ref ITKMaskImage) for details.
 */
class ITKMaskImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKMaskImage)
  SIMPL_STATIC_NEW_MACRO(ITKMaskImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMaskImage, AbstractFilter)

  virtual ~ITKMaskImage();

  SIMPL_FILTER_PARAMETER(double, OutsideValue)
  Q_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)

  SIMPL_FILTER_PARAMETER(DataArrayPath, MaskCellArrayPath)
  Q_PROPERTY(DataArrayPath MaskCellArrayPath READ getMaskCellArrayPath WRITE setMaskCellArrayPath)

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  virtual const QString getHumanLabel() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  virtual const QUuid getUuid() override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  SIMPL_INSTANCE_PROPERTY(DataContainerArray::Pointer, MaskContainerArray)

  ITKMaskImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void virtual dataCheckInternal() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheck();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void virtual filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

  /**
  * @brief Converts data container MarkerCellArrayPath to uint16
  */

  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> typename std::enable_if<std::is_scalar<InputPixelType>::value>::type convertDataContainerType();

  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> typename std::enable_if<!std::is_scalar<InputPixelType>::value>::type convertDataContainerType();

private:
  ITKMaskImage(const ITKMaskImage&);   // Copy Constructor Not Implemented
  void operator=(const ITKMaskImage&) = delete; // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKMaskImage_H_ */
