#ifndef _ITKImageToDream3DDataFilter_hxx
#define _ITKImageToDream3DDataFilter_hxx

#include "itkImageToDream3DDataFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

namespace itk
{
	template<typename PixelType, unsigned int VDimension>
	ImageToDream3DDataFilter<PixelType,VDimension>
	::ImageToDream3DDataFilter()
	{
		m_DataArrayPath.update(SIMPL::Defaults::ImageDataContainerName,
			SIMPL::Defaults::CellAttributeMatrixName,
			SIMPL::CellData::ImageData);
		m_DataContainer = DataContainer::NullPointer();
		m_OutputUpdated.Modified();
	}

	template<typename PixelType, unsigned int VDimension>
	ImageToDream3DDataFilter<PixelType, VDimension>
	::~ImageToDream3DDataFilter()
	{
	}

	template< typename PixelType, unsigned int VDimension>
	void
	ImageToDream3DDataFilter< PixelType, VDimension >
	::SetInput(const ImageType *input)
	{
		// Process object is not const-correct so the const_cast is required here
		this->ProcessObject::SetNthInput(0,
			const_cast< ImageType * >(input));
	}

	template< typename PixelType, unsigned int VDimension>
	void
	ImageToDream3DDataFilter< PixelType, VDimension >
	::SetDataArrayPath(DataArrayPath dataArray)
	{
		if (!(dataArray == m_DataArrayPath))
		{
			m_DataArrayPath = dataArray;
			this->Modified();
		}
	}


	template<typename PixelType, unsigned int VDimension>
	void
	ImageToDream3DDataFilter<PixelType, VDimension>
	::Update()
	{
		m_DataContainer = DataContainer::NullPointer(); // In case of an error, we want to return the NULL pointer
		//m_DataContainerArray->createNonPrereqDataContainer<AbstractFilter>(this, m_DataArrayPath.getDataContainerName());
		ImagePointer inputPtr = dynamic_cast<ImageType*>(this->GetInput(0));
		if (!inputPtr || !this->Check())
		{
			return;
		}
		m_DataContainer = DataContainer::New(m_DataArrayPath.getDataContainerName());
		if (!m_DataContainer)
		{
			return;
		}
		//// Create image geometry (data container)
		ImageGeom::Pointer image = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
		if (!image)
		{
			return;
		}
		// Get Input image properties
		ImageType::PointType origin = inputPtr->GetOrigin();
		ImageType::SpacingType spacing = inputPtr->GetSpacing();
		ImageType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
		//Configure global settings
		QVector<float> torigin(3, 0);
		QVector<float> tspacing(3, 1);
		size_t numberOfTuples = 1;
		QVector<size_t> tDims(3, 1);
		for (size_t i = 0; i < VDimension; i++)
		{
			torigin[i] = origin[i];
			tspacing[i] = spacing[i];
			tDims[i] = size[i];
			numberOfTuples *= tDims[i];
		}
		image->setOrigin(torigin[0], torigin[1], torigin[2]);
		image->setResolution(tspacing[0], tspacing[1], tspacing[2]);
		image->setDimensions(tDims[0], tDims[1], tDims[2]);
		m_DataContainer->setGeometry(image);

		//// // Create data array
		//// UInt8ArrayType::Pointer data = UInt8ArrayType::NullPointer();
		QVector<size_t> cDims(1, 0);
		cDims[0] = 1;

		/////* data = UInt8ArrayType::CreateArray(m_Dimensions.x*m_Dimensions.y*m_Dimensions.z, cDims, SIMPL::CellData::ImageData);
		//// data->initializeWithValue(128);
		AttributeMatrix::Pointer ma = m_DataContainer->createAndAddAttributeMatrix(tDims, m_DataArrayPath.getAttributeMatrixName(), SIMPL::AttributeMatrixType::Cell);
		inputPtr->SetBufferedRegion(inputPtr->GetLargestPossibleRegion());
		DataArray<PixelType>::Pointer data = DataArray<PixelType>::WrapPointer(inputPtr->GetBufferPointer(), numberOfTuples, cDims, m_DataArrayPath.getDataArrayName(), false);
		//inputPtr->ReleaseData();
		ma->addAttributeArray(m_DataArrayPath.getDataArrayName(), data);
		//// Allocates memory only if this==NULL or this->preflight returns error code
		////IDataArray::Pointer data;
		//// Create an array but do not allocate memory
		////data = UInt8ArrayType::CreateArray(tDims, cDims, getDataAttributeName(), false);

		//// //UInt8ArrayType::Pointer attributeArray = UInt8ArrayType::CreateArray(tDims, cDims, getDataAttributeName(), true);
		////if (data.get() != NULL)
		////{
		//// ma->createAndAddAttributeArray<UInt8ArrayType, AbstractFilter, unsigned char>(this, getDataAttributeName(), 0, cDims);
		////}
		////ma->createAndAddAttributeArray<UInt8ArrayType, AbstractFilter, unsigned char>(this, getDataAttributeName(), 0, cDims);
		//// Allocates memory all the time
		////UInt8ArrayType::Pointer data = ma->createNonPrereqArray<UInt8ArrayType, AbstractFilter, unsigned char>(this, getDataAttributeName(), 0, cDims);
		//Int16ArrayType::Pointer data = Int16ArrayType::CreateArray(tDims, cDims, getDataAttributeName(), true);//UInt8ArrayType::
		//ma->addAttributeArray(getDataAttributeName(), data);
		//ma->getAttributeArray(getDataAttributeName());
		//QString p = QString("ma, nb of tuples:%1").arg((qulonglong)(ma->getAttributeArray(getDataAttributeName())->getNumberOfTuples()));

		//notifyWarningMessage(getHumanLabel(), p, 1);
		//p = QString("data address first:%1").arg((qulonglong)(data->getPointer(0)));
		//notifyWarningMessage(getHumanLabel(), p, 1);
		//p = QString("data get:%1").arg((qulonglong)(data.get()));
		//notifyWarningMessage(getHumanLabel(), p, 1);


		//itk::ImageFileReader<ImageType>::Pointer reader = itk::ImageFileReader < ImageType >::New();
		//reader->SetFileName(filename.toStdString());
		////reader->Update();
		//PixelType* outputData = static_cast<PixelType*>(data->getPointer(0));
		//size_t num = data->getNumberOfTuples();
		//reader->GetOutput()->GetPixelContainer()->SetImportPointer(outputData, num, false);
		//reader->Update();
		//ImageType::Pointer loadedimage = reader->GetOutput();

		m_OutputUpdated.Modified();
	}

	// Check that m_DataArrayPath has been initialized correctly
	template<typename PixelType, unsigned int VDimension>
	bool
	ImageToDream3DDataFilter<PixelType, VDimension>
	::Check()
	{
		if (m_DataArrayPath.getDataContainerName().contains('/'))
		{
			return false;
		}
		if (m_DataArrayPath.getAttributeMatrixName().contains('/'))
		{
			return false;
		}
		if (m_DataArrayPath.getDataArrayName().contains('/'))
		{
			return false;
		}
		return true;
	}

	template<typename PixelType, unsigned int VDimension>
	DataContainer::Pointer
	ImageToDream3DDataFilter<PixelType, VDimension>
	::GetOutput()
	{
		if (this->GetInput(0) && this->GetMTime() > m_OutputUpdated )
		{
			this->Update();
		}
		return m_DataContainer;
	}

} // end of itk namespace

#endif