// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKImageProcessing/ITKImageProcessingFilters/SimpleITKEnums.h"
#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>

class ITKClosingByReconstructionImageTest : public ITKTestBase
{

public:
  ITKClosingByReconstructionImageTest()
  {
  }
  virtual ~ITKClosingByReconstructionImageTest()
  {
  }

  int TestITKClosingByReconstructionImageClosingByReconstructionTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/STAPLE1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKClosingByReconstructionImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(input_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    {
      FloatVec3_t d3d_var;
      d3d_var.x = 1;
      d3d_var.y = 1;
      d3d_var.z = 1;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("KernelRadius", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    {
      int d3d_var;
      d3d_var = itk::simple::sitkBall;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("KernelType", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKClosingByReconstructionImageClosingByReconstruction.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("095f00a68a84df4396914fa758f34dcc"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKClosingByReconstructionImage"));

    DREAM3D_REGISTER_TEST(TestITKClosingByReconstructionImageClosingByReconstructionTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKClosingByReconstructionImageTest(const ITKClosingByReconstructionImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKClosingByReconstructionImageTest&);                      // Operator '=' Not Implemented
};
