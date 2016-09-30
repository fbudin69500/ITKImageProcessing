// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMedianImage_h_
#define _ITKMedianImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkMedianImageFilter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>


/**
 * @brief The ITKMedianImage class. See [Filter documentation](@ref ITKMedianImage) for details.
 */
class ITKMedianImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKMedianImage)
    SIMPL_STATIC_NEW_MACRO(ITKMedianImage)
    SIMPL_TYPE_MACRO_SUPER(ITKMedianImage, AbstractFilter)

    virtual ~ITKMedianImage();

    SIMPL_FILTER_PARAMETER(FloatVec3_t, Radius)
    Q_PROPERTY(FloatVec3_t Radius READ getRadius WRITE setRadius)



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
    ITKMedianImage();

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

    ITKMedianImage(const ITKMedianImage&); // Copy Constructor Not Implemented
    void operator=(const ITKMedianImage&); // Operator '=' Not Implemented
};

#endif /* _ITKMedianImage_H_ */