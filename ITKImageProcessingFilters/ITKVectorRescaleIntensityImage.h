// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKVectorRescaleIntensityImage_h_
#define _ITKVectorRescaleIntensityImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkVectorRescaleIntensityImageFilter.h>

/**
 * @brief The ITKVectorRescaleIntensityImage class. See [Filter documentation](@ref ITKVectorRescaleIntensityImage) for details.
 */
class ITKVectorRescaleIntensityImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKVectorRescaleIntensityImage)
  SIMPL_STATIC_NEW_MACRO(ITKVectorRescaleIntensityImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKVectorRescaleIntensityImage, AbstractFilter)

  virtual ~ITKVectorRescaleIntensityImage();

  SIMPL_FILTER_PARAMETER(double, OutputMaximumMagnitude)
  Q_PROPERTY(double OutputMaximumMagnitude READ getOutputMaximumMagnitude WRITE setOutputMaximumMagnitude)

  SIMPL_FILTER_PARAMETER(int, OutputType)
  Q_PROPERTY(int OutputType READ getOutputType WRITE setOutputType)

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
  ITKVectorRescaleIntensityImage();

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
  * @brief Checks 'value' can be casted to OutputPixelType.
  */
  template <typename OutputPixelType> void CheckEntryBounds(double value, QString name);

private:
  ITKVectorRescaleIntensityImage(const ITKVectorRescaleIntensityImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKVectorRescaleIntensityImage&);                 // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKVectorRescaleIntensityImage_H_ */
