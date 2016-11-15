// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinaryProjectionImage_h_
#define _ITKBinaryProjectionImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkBinaryProjectionImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


/**
 * @brief The ITKBinaryProjectionImage class. See [Filter documentation](@ref ITKBinaryProjectionImage) for details.
 */
class ITKBinaryProjectionImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBinaryProjectionImage)
    SIMPL_STATIC_NEW_MACRO(ITKBinaryProjectionImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBinaryProjectionImage, AbstractFilter)

    virtual ~ITKBinaryProjectionImage();

    SIMPL_FILTER_PARAMETER(double, ProjectionDimension)
    Q_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)

    SIMPL_FILTER_PARAMETER(double, ForegroundValue)
    Q_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)

    SIMPL_FILTER_PARAMETER(double, BackgroundValue)
    Q_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)



    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) override;

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel() override;

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters() override;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  protected:
    ITKBinaryProjectionImage();

    /**
     * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
     */
    void virtual dataCheckInternal() override;

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    template<typename PixelType, unsigned int Dimension>
    void dataCheck();

    /**
    * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
    */
    void virtual filterInternal() override;

    /**
    * @brief Applies the filter
    */
    template<typename PixelType, unsigned int Dimension>
    void filter();

  private:

    ITKBinaryProjectionImage(const ITKBinaryProjectionImage&); // Copy Constructor Not Implemented
    void operator=(const ITKBinaryProjectionImage&); // Operator '=' Not Implemented
};

#endif /* _ITKBinaryProjectionImage_H_ */
