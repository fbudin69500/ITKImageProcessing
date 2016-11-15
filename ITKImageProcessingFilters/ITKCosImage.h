// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKCosImage_h_
#define _ITKCosImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkCosImageFilter.h>


/**
 * @brief The ITKCosImage class. See [Filter documentation](@ref ITKCosImage) for details.
 */
class ITKCosImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKCosImage)
    SIMPL_STATIC_NEW_MACRO(ITKCosImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKCosImage, AbstractFilter)

    virtual ~ITKCosImage();



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
    ITKCosImage();

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

    ITKCosImage(const ITKCosImage&); // Copy Constructor Not Implemented
    void operator=(const ITKCosImage&); // Operator '=' Not Implemented
};

#endif /* _ITKCosImage_H_ */
