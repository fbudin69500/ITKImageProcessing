// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMorphologicalWatershedImage_h_
#define _ITKMorphologicalWatershedImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKMorphologicalWatershedImage class. See [Filter documentation](@ref ITKMorphologicalWatershedImage) for details.
 */
class ITKMorphologicalWatershedImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKMorphologicalWatershedImage)
    SIMPL_STATIC_NEW_MACRO(ITKMorphologicalWatershedImage)
    SIMPL_TYPE_MACRO_SUPER(ITKMorphologicalWatershedImage, AbstractFilter)

    virtual ~ITKMorphologicalWatershedImage();

    SIMPL_FILTER_PARAMETER(double, Level)
    Q_PROPERTY(double Level READ getLevel WRITE setLevel)

    SIMPL_FILTER_PARAMETER(bool, MarkWatershedLine)
    Q_PROPERTY(bool MarkWatershedLine READ getMarkWatershedLine WRITE setMarkWatershedLine)

    SIMPL_FILTER_PARAMETER(bool, FullyConnected)
    Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)



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
    ITKMorphologicalWatershedImage();

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

    ITKMorphologicalWatershedImage(const ITKMorphologicalWatershedImage&); // Copy Constructor Not Implemented
    void operator=(const ITKMorphologicalWatershedImage&); // Operator '=' Not Implemented
};

#endif /* _ITKMorphologicalWatershedImage_H_ */
