// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKIntensityWindowingImage_h_
#define _ITKIntensityWindowingImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkIntensityWindowingImageFilter.h>

/**
 * @brief The ITKIntensityWindowingImage class. See [Filter documentation](@ref ITKIntensityWindowingImage) for details.
 */
class ITKIntensityWindowingImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKIntensityWindowingImage)
  SIMPL_STATIC_NEW_MACRO(ITKIntensityWindowingImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKIntensityWindowingImage, AbstractFilter)

  virtual ~ITKIntensityWindowingImage();

  SIMPL_FILTER_PARAMETER(double, WindowMinimum)
  Q_PROPERTY(double WindowMinimum READ getWindowMinimum WRITE setWindowMinimum)

  SIMPL_FILTER_PARAMETER(double, WindowMaximum)
  Q_PROPERTY(double WindowMaximum READ getWindowMaximum WRITE setWindowMaximum)

  SIMPL_FILTER_PARAMETER(double, OutputMinimum)
  Q_PROPERTY(double OutputMinimum READ getOutputMinimum WRITE setOutputMinimum)

  SIMPL_FILTER_PARAMETER(double, OutputMaximum)
  Q_PROPERTY(double OutputMaximum READ getOutputMaximum WRITE setOutputMaximum)

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
  ITKIntensityWindowingImage();

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

private:
  ITKIntensityWindowingImage(const ITKIntensityWindowingImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKIntensityWindowingImage&);             // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKIntensityWindowingImage_H_ */
