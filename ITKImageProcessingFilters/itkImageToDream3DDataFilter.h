#ifndef _ITKImageToDream3DDataFilter_h
#define _ITKImageToDream3DDataFilter_h

#include "itkImage.h"
#include "itkProcessObject.h"
#include "itkSimpleDataObjectDecorator.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"


namespace itk
{
	template<typename PixelType, unsigned int VDimension>
	class ImageToDream3DDataFilter: public ProcessObject
	{
	public:
		/** Standard class typedefs. */
		typedef ImageToDream3DDataFilter                                    Self;
		typedef ProcessObject                                               Superclass;
		typedef SmartPointer<Self>                                          Pointer;
		typedef typename Image<PixelType, VDimension>                       ImageType;
		typedef typename ImageType::Pointer                                 ImagePointer;
		typedef typename DataArray<PixelType>                               DataArrayPixelType;
		typedef typename SimpleDataObjectDecorator<DataContainer::Pointer>  DecoratorType;
		typedef DecoratorType::Pointer                                      DecoratorPointer;
		/** Method for creation through the object factory. */
		itkNewMacro(Self);
		void SetDataArrayPath(DataArrayPath dataArray);
		itkGetMacro(DataArrayPath, DataArrayPath);
		virtual void SetInput(const ImageType *image);
		virtual void GenerateData() ITK_OVERRIDE;
		DecoratorType* GetOutput();
		ProcessObject::DataObjectPointer MakeOutput(ProcessObject::DataObjectPointerArraySizeType);
	protected:
		void ValidInputCheck();
		ImageToDream3DDataFilter();
		virtual ~ImageToDream3DDataFilter();
	private:
		ImageToDream3DDataFilter(const ImageToDream3DDataFilter&); // Copy Constructor Not Implemented
		void operator=(const ImageToDream3DDataFilter&); // Operator '=' Not Implemented
		DataArrayPath m_DataArrayPath;
	};// end of class ImageToDream3DDataFilter
} //end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToDream3DDataFilter.hxx"
#endif

#endif