// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKLaplacianSharpeningImage_h_
#define _ITKLaplacianSharpeningImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkLaplacianSharpeningImageFilter.h>


/**
 * @brief The ITKLaplacianSharpeningImage class. See [Filter documentation](@ref ITKLaplacianSharpeningImage) for details.
 */
class ITKLaplacianSharpeningImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKLaplacianSharpeningImage)
    SIMPL_STATIC_NEW_MACRO(ITKLaplacianSharpeningImage)
    SIMPL_TYPE_MACRO_SUPER(ITKLaplacianSharpeningImage, AbstractFilter)

    virtual ~ITKLaplacianSharpeningImage();

    SIMPL_FILTER_PARAMETER(bool, UseImageSpacing)
    Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)



    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel();

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters();

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

  protected:
    ITKLaplacianSharpeningImage();

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

    ITKLaplacianSharpeningImage(const ITKLaplacianSharpeningImage&); // Copy Constructor Not Implemented
    void operator=(const ITKLaplacianSharpeningImage&); // Operator '=' Not Implemented
};

#endif /* _ITKLaplacianSharpeningImage_H_ */
