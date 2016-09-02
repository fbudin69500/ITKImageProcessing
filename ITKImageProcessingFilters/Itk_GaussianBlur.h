/*
 * Your License or Copyright can go here
 */

#ifndef _itk_gaussianblur_h_
#define _itk_gaussianblur_h_

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageBaseFilter.h"

/**
 * @brief The Itk_GaussianBlur class. See [Filter documentation](@ref itk_gaussianblur) for details.
 */
class Itk_GaussianBlur : public ITKImageBaseFilter
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(Itk_GaussianBlur)
    SIMPL_STATIC_NEW_MACRO(Itk_GaussianBlur)
    SIMPL_TYPE_MACRO_SUPER(Itk_GaussianBlur, AbstractFilter)

    virtual ~Itk_GaussianBlur();

    SIMPL_FILTER_PARAMETER(int, MaximumKernelWidth)
    Q_PROPERTY(int MaximumKernelWidth READ getMaximumKernelWidth WRITE setMaximumKernelWidth)

    SIMPL_FILTER_PARAMETER(double, Variance)
    Q_PROPERTY(double Variance READ getVariance WRITE setVariance)

    SIMPL_FILTER_PARAMETER(double, MaximumError)
    Q_PROPERTY(double MaximumError READ getMaximumError WRITE setMaximumError)

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

    /**
    * @brief execute Reimplemented from @see AbstractFilter class
    */
    virtual void execute();

    template<typename PixelType>
    void gaussian();
  protected:
    Itk_GaussianBlur();
    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    virtual void parametersCheck();

  private:

    DEFINE_IDATAARRAY_VARIABLE(NewCellArray)

    Itk_GaussianBlur(const Itk_GaussianBlur&); // Copy Constructor Not Implemented
    void operator=(const Itk_GaussianBlur&); // Operator '=' Not Implemented
};

#endif /* _Itk_GaussianBlur_H_ */
