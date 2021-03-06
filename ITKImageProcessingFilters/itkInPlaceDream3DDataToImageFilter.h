#ifndef _itkInPlaceDream3DDataToImageFilter_h
#define _itkInPlaceDream3DDataToImageFilter_h

#include "itkDream3DImage.h"
#include <itkImportImageFilter.h>
#include <itkNumericTraits.h>
#include <itkNumericTraitsRGBAPixel.h>
#include <itkNumericTraitsRGBPixel.h>
#include <itkNumericTraitsVectorPixel.h>

namespace itk
{
template <typename PixelType, unsigned int VDimension> class InPlaceDream3DDataToImageFilter : public ImageSource<itk::Dream3DImage<PixelType, VDimension>>
{
public:
  /** Standard class typedefs. */
  typedef InPlaceDream3DDataToImageFilter Self;
  typedef SmartPointer<Self> Pointer;

  typedef typename itk::Dream3DImage<PixelType, VDimension> ImageType;
  typedef typename ImageType::PixelContainerType ImportImageContainerType;
  typedef typename ImageType::Pointer ImagePointer;
  typedef typename itk::NumericTraits<PixelType>::ValueType ValueType;
  typedef typename ::DataArray<ValueType> DataArrayPixelType;
  typedef typename itk::ImageSource<ImageType> Superclass;
  typedef typename itk::SizeValueType SizeValueType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  itkTypeMacro(InPlaceDream3DDataToImageFilter, ImportImageFilter);

  virtual void SetInput(DataContainer::Pointer& dc);

  itkSetMacro(DataArrayName, std::string);
  itkGetMacro(DataArrayName, std::string);

  itkSetMacro(AttributeMatrixArrayName, std::string);
  itkGetMacro(AttributeMatrixArrayName, std::string);

  itkSetMacro(PixelContainerWillOwnTheBuffer, bool);
  itkGetMacro(PixelContainerWillOwnTheBuffer, bool);

  itkSetMacro(InPlace, bool);
  itkGetConstMacro(InPlace, bool);
  itkBooleanMacro(InPlace);

protected:
  InPlaceDream3DDataToImageFilter();
  virtual ~InPlaceDream3DDataToImageFilter();

  virtual void VerifyPreconditions() ITK_OVERRIDE;

  virtual void GenerateOutputInformation() ITK_OVERRIDE;
  virtual void GenerateData() ITK_OVERRIDE;
  DataContainer::Pointer m_DataContainer;

private:
  InPlaceDream3DDataToImageFilter(const InPlaceDream3DDataToImageFilter&) = delete; // Copy Constructor Not Implemented
  void operator=(const InPlaceDream3DDataToImageFilter&);                  // Operator '=' Not Implemented
  using Superclass::SetInput;
  std::string m_DataArrayName;
  std::string m_AttributeMatrixArrayName;
  typename ImportImageContainerType::Pointer m_ImportImageContainer;
  bool m_InPlace;                        // enable the possibility of in-place
  bool m_PixelContainerWillOwnTheBuffer; // By default, this filter does not take data ownership
};
} // end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInPlaceDream3DDataToImageFilter.hxx"
#endif

#endif
