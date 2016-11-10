// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKTanImage_h_
#define _ITKTanImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkTanImageFilter.h>


/**
 * @brief The ITKTanImage class. See [Filter documentation](@ref ITKTanImage) for details.
 */
class ITKTanImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKTanImage)
    SIMPL_STATIC_NEW_MACRO(ITKTanImage)
    SIMPL_TYPE_MACRO_SUPER(ITKTanImage, AbstractFilter)

    virtual ~ITKTanImage();



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
    ITKTanImage();

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

    ITKTanImage(const ITKTanImage&); // Copy Constructor Not Implemented
    void operator=(const ITKTanImage&); // Operator '=' Not Implemented
};

#endif /* _ITKTanImage_H_ */
