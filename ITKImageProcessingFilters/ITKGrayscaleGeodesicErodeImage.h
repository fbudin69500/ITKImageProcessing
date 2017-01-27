// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGrayscaleGeodesicErodeImage_h_
#define _ITKGrayscaleGeodesicErodeImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkGrayscaleGeodesicErodeImageFilter.h>
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>


/**
 * @brief The ITKGrayscaleGeodesicErodeImage class. See [Filter documentation](@ref ITKGrayscaleGeodesicErodeImage) for details.
 */
class ITKGrayscaleGeodesicErodeImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKGrayscaleGeodesicErodeImage)
    SIMPL_STATIC_NEW_MACRO(ITKGrayscaleGeodesicErodeImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKGrayscaleGeodesicErodeImage, AbstractFilter)

    virtual ~ITKGrayscaleGeodesicErodeImage();

    SIMPL_FILTER_PARAMETER(bool, RunOneIteration)
    Q_PROPERTY(bool RunOneIteration READ getRunOneIteration WRITE setRunOneIteration)

    SIMPL_FILTER_PARAMETER(bool, FullyConnected)
    Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)



    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) override;

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel() override;

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getSubGroupName() override;
    
    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters() override;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  protected:
    ITKGrayscaleGeodesicErodeImage();

    /**
     * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
     */
    void virtual dataCheckInternal() override;

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    template<typename InputImageType, typename OutputImageType, unsigned int Dimension>
    void dataCheck();

    /**
    * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
    */
    void virtual filterInternal() override;

    /**
    * @brief Applies the filter
    */
    template<typename InputImageType, typename OutputImageType, unsigned int Dimension>
    void filter();

  private:

    ITKGrayscaleGeodesicErodeImage(const ITKGrayscaleGeodesicErodeImage&); // Copy Constructor Not Implemented
    void operator=(const ITKGrayscaleGeodesicErodeImage&); // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKGrayscaleGeodesicErodeImage_H_ */