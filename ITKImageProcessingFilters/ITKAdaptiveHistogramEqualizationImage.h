// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKAdaptiveHistogramEqualizationImage_h_
#define _ITKAdaptiveHistogramEqualizationImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKAdaptiveHistogramEqualizationImage class. See [Filter documentation](@ref ITKAdaptiveHistogramEqualizationImage) for details.
 */
class ITKAdaptiveHistogramEqualizationImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKAdaptiveHistogramEqualizationImage)
    SIMPL_STATIC_NEW_MACRO(ITKAdaptiveHistogramEqualizationImage)
    SIMPL_TYPE_MACRO_SUPER(ITKAdaptiveHistogramEqualizationImage, AbstractFilter)

    virtual ~ITKAdaptiveHistogramEqualizationImage();

    SIMPL_FILTER_PARAMETER(double, Radius)
    Q_PROPERTY(double Radius READ getRadius WRITE setRadius)

    SIMPL_FILTER_PARAMETER(float, Alpha)
    Q_PROPERTY(float Alpha READ getAlpha WRITE setAlpha)

    SIMPL_FILTER_PARAMETER(float, Beta)
    Q_PROPERTY(float Beta READ getBeta WRITE setBeta)

    SIMPL_FILTER_PARAMETER(bool, UseLookupTable)
    Q_PROPERTY(bool UseLookupTable READ getUseLookupTable WRITE setUseLookupTable)



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
    ITKAdaptiveHistogramEqualizationImage();

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
    * @brief CheckIntegerEntry: Input types can only be of certain types:
      -float
      -double
      -bool
      -int
      For the other type, we have to use one of this primitive type, and verify that the
      value corresponds to what is expected.
    */
    template<typename VarType, typename SubsType>
    void CheckIntegerEntry(SubsType value, QString name);

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

    ITKAdaptiveHistogramEqualizationImage(const ITKAdaptiveHistogramEqualizationImage&); // Copy Constructor Not Implemented
    void operator=(const ITKAdaptiveHistogramEqualizationImage&); // Operator '=' Not Implemented
};

#endif /* _ITKAdaptiveHistogramEqualizationImage_H_ */
