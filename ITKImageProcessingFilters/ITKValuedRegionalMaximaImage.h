// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKValuedRegionalMaximaImage_h_
#define _ITKValuedRegionalMaximaImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkValuedRegionalMaximaImageFilter.h>
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>


/**
 * @brief The ITKValuedRegionalMaximaImage class. See [Filter documentation](@ref ITKValuedRegionalMaximaImage) for details.
 */
class ITKValuedRegionalMaximaImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKValuedRegionalMaximaImage)
    SIMPL_STATIC_NEW_MACRO(ITKValuedRegionalMaximaImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKValuedRegionalMaximaImage, AbstractFilter)

    virtual ~ITKValuedRegionalMaximaImage();

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
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters() override;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  protected:
    ITKValuedRegionalMaximaImage();

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

    ITKValuedRegionalMaximaImage(const ITKValuedRegionalMaximaImage&); // Copy Constructor Not Implemented
    void operator=(const ITKValuedRegionalMaximaImage&); // Operator '=' Not Implemented
};

#endif /* _ITKValuedRegionalMaximaImage_H_ */
