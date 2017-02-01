#!/usr/bin/env python
""" Creates Dream3D filters from JSON file

This script reads a directory containing JSON files that describe
ITK filters and creates a Dream3D filter for each filter based on
the information found in the JSON file.

This script:
1) Checks content of the JSON file
2) Convert JSON information into C++
3) Uses defined variables to configure template files

1) Python dictionaries are used to describe the information that is
expected to be found in the JSON file. Some information is required
in the JSON file while some is optional (See dictionaries definition below).

2) Information found in the JSON file is used to set some python variables.

3) Python variables are used to configure template files (one header file,
one source file, one test file, one documentation file). Multiple template
files are available (in Utilities/SimpleITKJSONDream3DFilterCreationTemplates)
and are selected based on the template_code_filename and template_test_filename
keywords that are found in the JSON file.


Generated C++ code can be modified if necessary (e.g. some keywords
are not handled by this script) or used as is. Generated filter
names should be added to ITKImageProcessingFilters/SourceList.cmake
and Test/CMakeLists.txt .

ITK source code directory is required to find filter module name. Filter
name is extracted from the ITK header file and used as the DREAM3D filter
subgroup name.

JSON files are taken from [1].
Documentation about the JSON files is available here [2].
Test implementation in SimpleITK: [3]
ITK can be downloaded here: [4]

Usage:

./SimpleITKJSONDream3DFilterCreation.py  -j ~/Devel/SimpleITK/Code/BasicFilters/json/ -d .. -n -o -I ~/Devel/ITK

If errors occur, try creating a folder containing only the JSON descriptions of the filters that are currently
supported by this script.

[1] https://github.com/SimpleITK/SimpleITK
[2] https://itk.org/SimpleITKDoxygen/html/FilterTemplatesPage.html
[3] https://github.com/SimpleITK/SimpleITK/blob/master/Testing/Unit/sitkImageFilterTestTemplate.cxx.in
[4] https://itk.org/ITK/resources/software.html
"""
import json
import argparse
import os
import sys


# The 'general' dictionary contains the expected keywords in JSON file at highest level.
# This dictionary is used to verify that all the required keywords are given
# in the JSON file.
# This dictionary contains 3 main sections:
# * processed: This section contains all the keywords that are used by this Python script
# * ignored: This section contains the keywords that are not (currently) useful to generated
#            the C++ DREAM3D filter code.
# * not_implemented: This section contains the keywords that should be used by the Python
#                    script but that are not handled yet.
#
# Each item in each section must contain a name, a type, and a boolean that specifies if
# the item is required. Each item can also contain additional information such as if
# this item has limitations.
general={
         'name': 'general',
         'processed':
             [
               {'name':['name','itk_name'], 'type':str, 'required':True},
               {'name':['number_of_inputs'],'type':int, 'required':True,'limitations':{'maximum':1, 'minimum':1}},
               {'name':['include_files'],'type':list,'required':False,'Filter':'sitk'},
               {'name':['members'],'type':list,'required':False},
               {'name':['inputs'],'type':str,'required':False},
               {'name':['tests'],'type':list,'required':False},
               {'name':['briefdescription'],'type':str,'required':False},
               {'name':['detaileddescription'],'type':str,'required':False},
               {'name':['output_image_type'],'type':str,'required':False},  # 37 occurences in JSON
               {'name':['output_pixel_type'],'type':str,'required':False},  # 60 occurences in JSON
               {'name':['template_code_filename'], 'type':str, 'required':True},  # 281 occurences in JSON
               {'name':['template_test_filename'], 'type':str, 'required':True}, # 280 occurences in JSON
               {'name':['filter_type'],'type':str,'required':False},  # 102 occurences in JSON
               {'name':['measurements'],'type':str,'required':False}  # 109 occurences in JSON
             ],
        'ignored':
             [
               {'name':['public_declarations']},  # Dream3D does not require additional C++ code
               {'name':['custom_methods']},  # Dream3D does not require to add methods to the filter
               {'name':['doc']},  # Dream3D does not require doc
               {'name':['additional_template_types'],'type':str,'required':False},  # 0 occurences in JSON
               {'name':['custom_set_intput'],'type':str,'required':False}  # 0 occurences in JSON
             ],
         'not_implemented':
             [
               {'name':['pixel_types'], 'type':str, 'required':True},  # 337 occurences in JSON
               {'name':['vector_pixel_types_by_component'],'type':str,'required':False},  # 42 occurences in JSON
               {'name':['no_procedure'],'type':str,'required':False},  # 31 occurences in JSON
               {'name':['no_output_type'],'type':str,'required':False},  # 2 occurences in JSON
               {'name':['constant_type'],'type':str,'required':False},  # 23 occurences in JSON
               {'name':['pixel_types2'],'type':str,'required':False},  # 14 occurences in JSON
               {'name':['vector_pixel_types_by_component2'],'type':str,'required':False},  # 1 occurences in JSON
               {'name':['template_type'],'type':str,'required':False},  # 7 occurences in JSON
               {'name':['no_return_image'],'type':str,'required':False},  # 8 occurences in JSON
               {'name':['custom_type2'],'type':str,'required':False},  # 3 occurences in JSON
               {'name':['custom_register'],'type':str,'required':False},  # 2 occurences in JSON
               {'name':['long'],'type':int,'required':False},  # 32 occurences in JSON
               {'name':['custom_set_input'],'type':str,'required':False}  # 24 occurences in JSON
             ]
         
       }

# 'members' describes each input required by the filter. This describes the type of
# data, its name, and other information such as the default value for this variable.
# 'member' structure is used both for 'members' data and 'measurements' data
members={
        'name':'members',
        'processed':
         [
           {'name':['name','itk_name'],'type':str,'required':True},
           {'name':['type'],'type':str,'required':False},
           {'name':['default'],'type':str,'required':True},
           {'name':['dim_vec'],'type':int,'required':False },  # 159 occurences in JSON
           {'name':['itk_type'],'type':str,'required':False},  # 121 occurences in JSON
           {'name':['briefdescriptionSet'],'type':str,'required':False},
           {'name':['detaileddescriptionSet'],'type':str,'required':False},
           {'name':['detaileddescriptionGet'],'type':str,'required':False},  # For some filters, the description is only written in this field, not in the equivalent "Set" field
           {'name':['briefdescriptionGet'],'type':str,'required':False},  # For some filters, the description is only written in this field, not in the equivalent "Set" field
           {'name':['custom_itk_cast'],'type':str,'required':False},  # 123 occurences in JSON (including in inputs)
           {'name':['ignore_setup'],'type':str,'required':False}, #  Not in the original JSON file. Adds this information when member should not be automatically added in the setup (e.g. if the template already initializes this member with a specific layout)
           {'name':['ignore_impl'],'type':str,'required':False} #  Not in the original JSON file. Adds this information when member should not be automatically added in the filter implementation (e.g. when the member is not used to initialize the filter, but another object).
         ],
        'ignored':
         [
            {'name':['no_get_method']},  # #Get functions are not used. Only #Set functions are used
            {'name':['doc']},  # Dream3D does not require doc
            {'name':['no_print']},  # Dream3D does not print filter information
            {'name':['no_set_method'],'type':bool,'required':False }  # 0 occurences in JSON
         ],
         'not_implemented':
         [
           {'name':['set_as_scalar']},  # 40; The parameter values will be set individually, not altogether using the same scalar value
           {'name':['pixeltype'],'type':str,'required':False},  # 158 occurences in JSON
           {'name':['point_vec'],'type':str,'required':False},  # 7 occurences in JSON
           {'name':['enum'],'type':list,'required':False}  # 29 occurences in JSON
         ]
      }


# The 'DefaultMembers' dictionary contains items that are automatically added to the list of members
# for certain templates.
DefaultMembers={
                'ImageFilter':[],
                'KernelImageFilter':
                [
{'name': u'KernelRadius', 'default': u'std::vector<unsigned int>(3, 1)', 'briefdescriptionGet': u'', 'detaileddescriptionGet': u'', 'detaileddescriptionSet': u'', 'dim_vec': 1, 'briefdescriptionSet': u'', 'itk_type': 'typename FilterType::RadiusType','type': u'unsigned int', 'ignore_impl':1},
{'name': u'KernelType', 'default': u'itk::simple::sitkBall', 'briefdescriptionGet': u'', 'detaileddescriptionGet': u'', 'detaileddescriptionSet': u'', 'dim_vec': 0, 'briefdescriptionSet': u'', 'itk_type': 'typename itk::simple::KernelEnum','type': u'int', 'ignore_setup':1, 'ignore_impl':1}
                ]
               }

# The 'inputs' dictionary contains the list of items that the filter requires as
# well as their description (name, type)
inputs={
        'name':'inputs',
        'processed':
         [
           {'name':['name'],'type':str,'required':True},
           {'name':['type'],'type':str,'required':True}
         ],
         'ignored':
         [],
         'not_implemented':
         [
           {'name':['no_size_check'],'type':str,'required':False},  # 3 occurences in JSON
           {'name':['custom_itk_cast'],'type':str,'required':False},  # 123 (including in members)
           {'name':['optional'],'type':str,'required':False}  # 20 occurences in JSON
         ]
       }

# 'tests' dictionary contains the description of the tests that will
# be automatically implemented.
tests={
       'name':'tests',
       'processed':
        [
          {'name':['tag'],'type':str,'required':True},  # 498 occurences
          {'name':['md5hash'], 'type':str, 'required':False},  # 329 occurences
          {'name':['settings'], 'type':list,'required':False},  # 621 occurences
          {'name':['inputs'],'type':list,'required':True,'limitations':{'len_maximum':1, 'len_minimum':1}},  # 816 occurences in JSON (not limited to tests)
          {'name':['tolerance'],'type':float,'required':False},  # 249 occurences
          {'name':['measurements_results'],'type':list,'required':False} # 65 occurences
        ],
        'ignored':
        [
          {'name':['description'],'type':str,'required':False},  # 3162 (not limited to tests)
          {'name':['inputA'],'type':str,'required':False},  # 0 occurence in JSON files
          {'name':['inputB'],'type':str,'required':False},  # 0 occurence in JSON files
          {'name':['sha1hash'],'type':str,'required':False}  # 0 occurence in JSON files
        ],
        'not_implemented':
        [
          {'name':['no_procedure'],'type':str,'required':False},  # 31
          {'name':['inputA_cast'],'type':str,'required':False},  # after inputA is read the CastImageFilter is run, to covert the image type./24 occurences
          {'name':['inputB_cast'],'type':str,'required':False}  # after inputB is read the CastImageFilter is run, to covert the image type./ 9 occurences
        ]
}

# 'tests_settings' describes the settings used for the tests that are automatically implemented.
# Used both for test settings and test measurements
tests_settings={
       'name':'tests_settings',
       'processed':
        [
          {'name':['parameter','name'],'type':str,'required':True},
          {'name':['value'], 'type':str, 'required':True},
          {'name':['dim_vec'], 'type':int, 'required':False},
          {'name':['type'], 'type':str, 'required':False}
        ],
        'ignored':
        [
          {'name':['cxx_value'],'type':str,'required':False},  # 45 occurences
          {'name':['python_value'],'type':str,'required':False},  # 82 occurences
          {'name':['lua_value'],'type':str,'required':False},  # 44 occurences
          {'name':['ruby_value'],'type':str,'required':False},  # 53 occurences
          {'name':['java_value'],'type':str,'required':False},  # 56 occurences
          {'name':['tcl_value'],'type':str,'required':False},  # 54 occurences
          {'name':['R_value'],'type':str,'required':False},  # 80 occurences
          {'name':['csharp_value'],'type':str,'required':False},  # 43 occurences
          {'name':['no_get_method'],'type':bool,'required':False}
        ],
        'not_implemented':
        []
}

##########################################################################################################
##########################################################################################################

# Dictionaries used to convert data
# * Conversion between ITK and DREAM3D
# * Conversion from string to function calls
# * Simplification for function calls

Dream3DTypeToMacro={
  'double':{'include':'SIMPLib/FilterParameters/DoubleFilterParameter.h', 'macro':'SIMPL_NEW_DOUBLE_FP','component':'double','read':'readValue'},
  'float':{'include':'SIMPLib/FilterParameters/FloatFilterParameter.h','macro':'SIMPL_NEW_FLOAT_FP','component':'float','read':'readValue'},
  'int':{'include':'SIMPLib/FilterParameters/IntFilterParameter.h','macro':'SIMPL_NEW_INTEGER_FP','component':'int','read':'readValue'},
  'bool':{'include':'SIMPLib/FilterParameters/BooleanFilterParameter.h','macro':'SIMPL_NEW_BOOL_FP','component':'bool','read':'readValue'},
  'FloatVec3_t':{'include':'SIMPLib/FilterParameters/FloatVec3FilterParameter.h','macro':'SIMPL_NEW_FLOAT_VEC3_FP','component':'float','read':'readFloatVec3'},
  'IntVec3_t':{'include':'SIMPLib/FilterParameters/IntVec3FilterParameter.h','macro':'SIMPL_NEW_INT_VEC3_FP','component':'int','read':'readIntVec3'}
}

TypeIsInt={
'uint64_t':1,
'uint8_t':1,
'double':0,
'float':0,
'int':1,
'unsigned int':1,
'int64_t':1,
'uint32_t':1,
'int32_t':1,
'bool':1
}

ITKToDream3DType={
  ('double',0):{'d3d':'double','std':'double'},
  ('float',0):{'d3d':'float','std':'float'},
  ('int',0):{'d3d':'int','std':'int'},
  ('int32_t',0):{'d3d':'int','std':'int'},
  ('unsigned int',0):{'d3d':'double','std':'double'},
  ('uint64_t',0):{'d3d':'double','std':'double'},
  ('uint32_t',0):{'d3d':'double','std':'double'},
  ('uint8_t',0):{'d3d':'int','std':'int'},
  ('bool',0):{'d3d':'bool','std':'bool'},
  ('double',1):{'d3d':'FloatVec3_t','std':'std::vector<double>'},
  ('float',1):{'d3d':'FloatVec3_t','std':'std::vector<float>'},
  ('int',1):{'d3d':'IntVec3_t','std':'std::vector<int>'},
  ('unsigned int',1):{'d3d':'FloatVec3_t','std':'std::vector<unsigned int>'},
  ('int64_t',1):{'d3d':'FloatVec3_t','std':'std::vector<int64_t>'},
  ('uint32_t',1):{'d3d':'FloatVec3_t','std':'std::vector<uint32_t>'},
  ('int32_t',1):{'d3d':'FloatVec3_t','std':'std::vector<int32_t>'},
  ('bool',1):{'d3d':'IntVec3_t','std':'std::vector<bool>'}
}

VariantToType={
  'double':'toDouble()',
  'float':'toFloat()',
  'int':'toInt()',
  'int32_t':'toInt()',
  'unsigned int':'toUInt()',
  'uint64_t':'toULongLong()',
  'uint32_t':'toUInt()',
  'bool':'toBool()'
}

# Used in filter implementation
# CastDream3DToITKFunction=['StaticCastScalar','CastVec3ToITK']
# Used in initialization
CastStdToDream3D=['StaticCastScalar','CastStdToVec3']


## Enums
# typename FilterType::OutputRegionModeType:{'enum'},
# typename FilterType::TopologyCheckType:{'enum'}
# typename FilterType::ColormapEnumType
# typename FilterType::MethodChoice
# typename FilterType::DirectionCollapseStrategyEnum
# typename FilterType::NoiseModelType
# typename FilterType::ConnectivityEnumType
# typename FilterType::OrderEnumType

itkVectorTypes={
'typename FilterType::SizeType':{'type':'Size','namespace':'FilterType::SizeType'},
'typename FilterType::ArrayType':{'type':'Array','namespace':'FilterType::ArrayType'},
'typename FilterType::PointType':{'type':'Point','namespace':'FilterType::PointType'},
'typename FilterType::SpacingType':{'type':'Vector','namespace':'FilterType::SpacingType'},
'typename FilterType::IndexType':{'type':'Index','namespace':'FilterType::IndexType'},
'typename FilterType::InputSizeType':{'type':'Size','namespace':'FilterType::InputSizeType'},
'typename FilterType::StandardDeviationsType':{'type':'Array','namespace':'FilterType::StandardDeviationsType'},
'typename FilterType::OriginPointType':{'type':'Point','namespace':'FilterType::OriginPointType'},
'typename FilterType::PatternArrayType':{'type':'Array','namespace':'FilterType::PatternArrayType'},
'typename FilterType::FlipAxesArrayType':{'type':'Array','namespace':'FilterType::FlipAxesArrayType'},
'typename InputImageType::SizeType':{'type':'Size','namespace':'InputImageType::SizeType'},
'typename FilterType::OffsetType':{'type':'Offset','namespace':'FilterType::OffsetType'},
'typename FilterType::OrderArrayType':{'type':'Array','namespace':'FilterType::OrderArrayType'},
'typename FilterType::WeightsType':{'type':'Array','namespace':'FilterType::WeightsType'},
'typename InputImageType::IndexType':{'type':'Index','namespace':'InputImageType::IndexType'},
'typename FilterType::InputImageIndexType':{'type':'Index','namespace':'FilterType::InputImageIndexType'},
'typename FilterType::LayoutArrayType':{'type':'Index','namespace':'FilterType::LayoutArrayType'},
'typename FilterType::RadiusType':{'type':'Size','namespace':'FilterType::RadiusType'},
'typename FilterType::PermuteOrderArrayType':{'type':'Array','namespace':'FilterType::PermuteOrderArrayType'},
'typename FilterType::ExpandFactorsType':{'type':'Array','namespace':'FilterType::ExpandFactorsType'},
'typename FilterType::ShrinkFactorsType':{'type':'Array','namespace':'FilterType::ShrinkFactorsType'}
}

ValueAndDimensionTypes={
'Size':{'vtype':'SizeValueType','dim':'Dimension'},
'Array':{'vtype':'ValueType','dim':'Dimension'},
'Point':{'vtype':'ValueType','dim':'PointDimension'},
'Vector':{'vtype':'ValueType','dim':'Dimension'},
'Index':{'vtype':'IndexValueType','dim':'Dimension'},
'Offset':{'vtype':'OffsetValueType','dim':'Dimension'},
}

CheckEntry=['CheckIntegerEntry','CheckVectorEntry']

##########################################################################################################
##########################################################################################################
#
def ExtractDescription(data_json, fields, filter_description, verbose=False, not_implemented=False):
    """ ExtractDescription
    
    Extract description of the filter based on the given dictionary (loaded from JSON) and
    the expected structure ('fields').
    
    inputs:
      Data structure: input data (dict)
      fields: global dictionary describing expected fields (dict)
      verbose: boolean to print extra information (bool)
      not_implemented: boolean to ignore fields that are not implemented/used yet (bool)
    outputs:
      filter_description: output dictionary containing filtered results (dict)
      return value: False if an error occurs (bool)
    """
    # Print which fields are being processed
    if verbose:
        print("Processing %s"%fields['name'])
    # Check all 'processed' field
    for field in fields['processed']:
        for field_name in reversed(field['name']):
            if field_name in data_json:
                filter_description[field['name'][0]]=data_json[field_name]
        if field['name'][0] not in filter_description:
            if field['required']==True:
                print("While processing %s, %s required - Not found"%(fields['name'],field['name'][0]))
                return False
            else:
                filter_description[field['name'][0]] = field['type']()
        if verbose == True:
            print("%s: %s"%(field['name'][0],str(filter_description[field['name'][0]])))
    # Verify that there is no unknown field.
    # Fields should belong to either 'processed' or 'ignored'
    for field in data_json:
        hide=False
        if not_implemented and len([x for x in fields['not_implemented'] if field in x['name']]) != 0:
            hide=True
        if len([x for x in fields['processed'] if field in x['name']]) == 0 and \
           len([x for x in fields['ignored'] if field in x['name']]) == 0 and \
           hide == False:
            print("While processing %s, %s was found as a field but is neither in 'processed' nor in 'ignored'"%(fields['name'],field))
            return False
    return True

def GetDREAM3DFilterName(filter_name):
    """ GetDREAM3DFilterName
    
    Simply create the DREAM3D filter name based on the ITK filter name.
    
    inputs:
      filter_name: name of the filter (string)
    outputs:
      return value: DREAM3D filter name (string)
    """
    return 'ITK'+filter_name.replace('Filter','')

def FormatIncludes(include_files):
    """FormatIncludes
    
    Converts a list of include files into the corresponding
    C++ code that is inserted in the template files.
    
    inputs:
      include_files: include file(s) (string or list)
    outputs:
      return value: C++ code to include C++ header files (string)
    """
    if type(include_files) == list:  # There are multiple include files
      include_string=""
      clean_list=list(set(include_files))
      for include_file in clean_list:
          include_string +='#include <'+include_file+'>\n'
    else:  # There is only one include file
        include_string +='#include <'+include_files+'>\n'
    return include_string

def GetDREAM3DITKInclude(filter_name):
    """ GetDREAM3DITKInclude
    
    Return ITK header file name corresponding to the ITK filter name.
    
    outputs:
      return value: ITK header file name (string)
    """
    return 'itk'+filter_name+'.h'

def CheckAutomaticallyGenerated(filename):
    """ CheckAutomaticallyGenerated
    
    Verifies that the file has been generated automatically. The scrip
    should only overwrite files that have been generated automatically,
    not files that have been manually created.
    
    inputs:
      filename: file name (string)
    outputs:
      return value: file was generated automatically (bool)
    """
    try:
        with open(filename, 'r') as f:
            first_line = f.readline().rstrip('\n')
            if first_line == '// File automatically generated':
                return True
    except IOError:
        pass
    return False

def GetDREAM3DFilterFilePathNoExt(filter_name, directory):
    """ GetDREAM3DFilterFilePathNoExt
    
    Returns the file path of the DREAM3D filter without the extension
    of the file. This is useful to configure the template files.
    
    inputs:
      filter_name: filter name (string)
      directory: directory name (string)
    outputs:
      return value: File path without extension (string)
    """
    return os.path.join(directory, GetDREAM3DFilterName(filter_name))

def CheckFileExists(filename):
    """CheckFileExists
    
    Checks both if the file exists on the system and if it is empty.
    If the file is empty, return that it does not exist.
    
    inputs:
      filename: file name (string)
    outputs:
      return value: File exists (bool)
    """
    return (os.path.isfile(filename) and os.path.getsize(filename)>0)

def FilterFilesAlreadyExist(filter_name, directory, overwrite):
    """FilterFilesAlreadyExist
    
    Verifies is any of the files that we want to automatically generate
    exist: checks for the header and cpp file as well as for the test file.
    
    inputs:
      filter_name: filter name (string)
      directory: directory name (string)
      overwrite: overwrite file (bool)
    outputs:
      return value: Filter files can be written (bool)
    """
    exists={}
    automatic={}
    for ext in ['.h','.cpp']:
        filename=GetDREAM3DFilterFilePathNoExt(filter_name, directory)+ext
        exists[ext]=CheckFileExists(filename)
        automatic[ext]=CheckAutomaticallyGenerated(filename)
    test_Filename=os.path.join(os.path.join(directory,"Test"),filter_name+"Test.cpp")
    exists['test'] = CheckFileExists(test_Filename)
    automatic['test'] = CheckAutomaticallyGenerated(test_Filename)
    error = False
    if exists['.h'] != exists['.cpp'] or automatic['.h'] != automatic['.cpp']\
       or exists['.h'] != exists['test'] or automatic['.h'] != automatic['test']:
        print("Error checking filter files. Check them manually.")
        return False
    if exists['.h']:
        print("%s already exist."%filter_name)
        if automatic['.h']:
            if overwrite:
                print('Overwriting file')
            else:
                print('Select "--overwrite" to overwrite existing filters.')
                return False
        if not automatic['.h']:
            print("Existing filter was not automatically generated. Skipping.")
            return False
    return True

def ReplaceVariablesInTemplate(lines,DREAM3DFilter):
    """ReplaceVariablesInTemplate

    Configure template files: Remplaces variables in un-initialized template
    files with value that has been configured with the JSON information.   
    
    inputs:
      lines: File content (list of string)
      DREAM3DFilter: Generated C++ code (dictionary of string)
    outputs:
      return value: Configured C++ file (list of string)
    """
    for ii in range(0,len(lines)):
        for key,val in DREAM3DFilter.iteritems():
            lines[ii]=lines[ii].replace('${'+key+'}', val)
    return lines

def VerifyLimitations(fields, descriptions, verbose, islist=False):
    """VerifyLimitations
    
    Certain items in the JSON files may have limitations: Verify that it
    is supported before generating the C++ code.
    
    inputs:
      fields: global dictionary describing expected fields (dict)
      descriptions: output dictionary containing filtered results (dict)
      verbose:
      islist:
    outputs:
      return value: True if inside limitations (bool)
    """
    # This is a list of list
    if islist:
        for description in descriptions:
            if not VerifyLimitations(fields, description, verbose):
                return False
        return True
    # If not a list of list
    for description in descriptions:  # Description is expected to be a list
        for key,val in description.iteritems():        
            field = [x for x in fields['processed'] if key in x['name']][0]  # There should be one and only one element in the list
            if 'limitations' in field:
                limitation_field=field['limitations']
                if 'maximum' in limitation_field:
                    if val > limitation_field['maximum']:
                        if verbose:
                            print("values (%d) in field (%s) is too large. Maximum accepted is %d"%(val,field['name'], limitation_field['maximum']))
                        return False  # Not handled yet
                if 'minimum' in limitation_field:
                    if val < limitation_field['minimum']:
                        if verbose:
                            print("values (%d) in field (%s) is too small. Minimum accepted is %d"%(val,field['name'], limitation_field['minimum']))
                        return False  # Not handled yet
                if 'len_maximum' in limitation_field:
                    if len(val) > limitation_field['len_maximum']:
                        if verbose:
                            print("Length (%d) in field (%s) is too large. Maximum accepted is %d"%(len(val),field['name'], limitation_field['len_maximum']))
                        return False  # Not handled yet
                if 'len_minimum' in limitation_field:
                    if len(val) < limitation_field['len_minimum']:
                        if verbose:
                            print("Length (%d) in field (%s) is too small. Minimum accepted is %d"%(len(val),field['name'], limitation_field['len_minimum']))
                        return False  # Not handled yet


    return True

def FilterFields(fields, descriptions, isList=False):
    """FilterFields
    
    Reads JSON dictionary and convert it to expected structure. If the input is a list
    of list, call this function once for each item of the first list.
    
    input:
      fields: dictionary describing the expected structure of the items of 'descriptions'
      descriptions: list or list of list containing 
      isList: boolean
    """
    # If description is list of list
    if isList:
        for description in descriptions:
            FilterFields(fields, description)
        return
    # else: description is not list of list
    for ii in range(0,len(descriptions)):  # Description is expected to be a list
        for key,val in descriptions[ii].iteritems():
            field = [x for x in fields['processed'] if key in x['name']][0]  # There should be one and only one element in the list
            if 'Filter' in field:
                filter_field=field['Filter']  # This field is expect to be a list that needs to be filtered
                descriptions[ii][key] = [x for x in val if filter_field not in x]

def CheckTypeSupport(filter_members):
    """CheckTypeSupport
    
    For each element of 'filter_members',
    check that the type of the filter member is supported. The C++ code of
    the filter cannot be generated if the type is not supported.
    
    input:
      filter_members: list
    """
    for filter_member in filter_members:  # Expected to be a list
        if (filter_member['type'],filter_member['dim_vec']) not in ITKToDream3DType:
            print("Type not supported: %s (vector: %d)"%(filter_member['type'],filter_member['dim_vec']))
            print filter_member
            return False
    return True

def CheckITKTypeSupport(filter_members):
    """CheckITKTypeSupport
    
    Checks that the 'itk_type' of all items is supported. If the type is not
    supported, the C++ code of the filter cannot be generated.
        
    input:
      filter_members: list
    """
    for filter_member in filter_members:  # Expected to be a list
        if 'itk_type' in filter_member:
            if filter_member['dim_vec'] > 0 and filter_member['itk_type'] not in itkVectorTypes:
                print("itk_type not supported: %s "%filter_member['itk_type'])
                print filter_member
                return False
    return True

def GetDREAM3DParameters(filter_member):
    dream3D_type = ITKToDream3DType[(filter_member['type'],filter_member['dim_vec'])]['d3d']
    parameter='    SIMPL_FILTER_PARAMETER('+dream3D_type+', '+filter_member['name']+')\n'
    parameter+='    Q_PROPERTY('+dream3D_type+' '+filter_member['name']+' READ get'+filter_member['name']+' WRITE set'+filter_member['name']+')\n\n'
    return parameter

def GetDREAM3DMeasurementParameters(filter_measurement):
    dream3D_type = ITKToDream3DType[(filter_measurement['type'],filter_measurement['dim_vec'])]['d3d']
    parameter='    SIMPL_FILTER_PARAMETER('+dream3D_type+', '+filter_measurement['name']+')\n'
    parameter+='    Q_PROPERTY('+dream3D_type+' '+filter_measurement['name']+' READ get'+filter_measurement['name']+')\n\n'
    return parameter

def GetDREAM3DSetupFilterParametersFromMembers(filter_member, filter_name):
    dream3D_type = ITKToDream3DType[(filter_member['type'],filter_member['dim_vec'])]['d3d']
    include=Dream3DTypeToMacro[dream3D_type]['include']
    macro=Dream3DTypeToMacro[dream3D_type]['macro']
    if 'ignore_setup' in filter_member and filter_member['ignore_setup'] == 1:
        setup=''
    else:
        setup='parameters.push_back('+macro+'("'+filter_member['name']+'", '+filter_member['name']+', FilterParameter::Parameter, '+filter_name+'));\n'
    limits=""
    component_type = Dream3DTypeToMacro[dream3D_type]['component']
    if component_type != filter_member['type']:
        limits='this->'+CheckEntry[filter_member['dim_vec']]+'<'+filter_member['type']+','+dream3D_type+'>(m_'+filter_member['name']+', "'+filter_member['name']+'",'+str(TypeIsInt[filter_member['type']])+');\n'
    return [include, setup, limits]

def GetDREAM3DReadFilterParameters(filter_member):
  dream3D_type = ITKToDream3DType[(filter_member['type'],filter_member['dim_vec'])]['d3d']
  return '  set'+filter_member['name']+'(reader->'+Dream3DTypeToMacro[dream3D_type]['read']+'("'+filter_member['name']+'", get'+filter_member['name']+'()));\n'

def ImplementFilter(filter_description, filter_members, filter_measurements):
    if 'filter_type' in filter_description and filter_description['filter_type'] != '':
        filt = '  typedef ' + filter_description['filter_type'] + ' FilterType;\n'
    # Code specific for each template
    elif filter_description['template_code_filename'] == "KernelImageFilter":
        filt = '  typedef itk::'+filter_description['name']+'<InputImageType, OutputImageType, StructuringElementType> FilterType;\n'
    else:
        filt = '  typedef itk::'+filter_description['name']+'<InputImageType, OutputImageType> FilterType;\n'
    filt+= '  typename FilterType::Pointer filter = FilterType::New();\n'
    for filter_member in filter_members:
        if 'ignore_impl' in filter_member and filter_member['ignore_impl'] == 1:
            continue  # Skip this member. Do not try to use it to initialize the filter.
        if filter_member['dim_vec'] == 0:
            call = 'static_cast<'+filter_member['type']+'>(m_'+filter_member['name']+')'
        elif filter_member['dim_vec'] == 1:
            if 'itk_type' in filter_member:
                d3d_type = ITKToDream3DType[(filter_member['type'],filter_member['dim_vec'])]['d3d']
                itk_type = itkVectorTypes[filter_member['itk_type']]['type']
                itk_type_namespace = itkVectorTypes[filter_member['itk_type']]['namespace']
                itk_type_dimension = itk_type_namespace+"::"+ValueAndDimensionTypes[itk_type]['dim']
                itk_type_component = "typename "+itk_type_namespace+"::"+ValueAndDimensionTypes[itk_type]['vtype']
                call = 'CastVec3ToITK<'+d3d_type+','+filter_member['itk_type']+','+itk_type_component+'>(m_'+filter_member['name']+','+itk_type_dimension+')'
            else:
                raise Exception("Vector with no itk_type - %s"%filter_member)
        else:
            raise Exception("dim_vec = % - %s"%(filter_member['dim_vec'],filter_member))
        if 'custom_itk_cast' in filter_member and filter_member['custom_itk_cast'] != '':
            filt+='  '+filter_member['custom_itk_cast']+'\n'
        else:
            filt+= '  filter->Set'+filter_member['name']+'('+call+');\n'
    # Code specific for each template
    if filter_description['template_code_filename'] == "KernelImageFilter":
        filt+='  filter->SetKernel(structuringElement);\n'
    filt+='  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);\n'
    # Post processing (e.g. Print measurements)
    for filter_measurement in filter_measurements:
        filt+='  QString outputVal = "'+filter_measurement['name']+' :%1";\n'
        filt+='  m_'+filter_measurement['name'] + ' = filter->Get'+filter_measurement['name'] + '();\n'
        filt+='  notifyWarningMessage(getHumanLabel(),outputVal.arg(m_'+filter_measurement['name']+'),0);\n'
    return filt

def TypenameOutputPixelType(output_pixel_type):
    if 'InputImageType::' in output_pixel_type:
        return '1'
    elif 'TImageType::' in output_pixel_type:
        return '2'
    else:
        return '0'

def ImplementInternal(filter_description, fct):
    if 'output_image_type' in filter_description and filter_description['output_image_type'] != '':
        return '  Dream3DArraySwitchMacroOutputType('+fct+', getSelectedCellArrayPath(), -4,'\
        +filter_description['output_image_type']+'::PixelType,'\
        +TypenameOutputPixelType(filter_description['output_image_type'])+');'
    elif 'output_pixel_type' in filter_description and filter_description['output_pixel_type'] != '':
        return '  Dream3DArraySwitchMacroOutputType('+fct+', getSelectedCellArrayPath(), -4,'\
        +filter_description['output_pixel_type']+','\
        +TypenameOutputPixelType(filter_description['output_pixel_type'])+');'
    else:
        return '  Dream3DArraySwitchMacro('+fct+', getSelectedCellArrayPath(), -4);'

def ConfigureFiles(ext, template_directory, directory, DREAM3DFilter, rootTemplateName):
    templateFilePath=os.path.join(template_directory, rootTemplateName+'Template')+ext
    # Verify that template file exists
    if not os.path.isfile(templateFilePath):
        raise Exception("Missing template file %s"%templateFilePath)
    # Read template file
    with open(templateFilePath) as f:
        lines = f.readlines()
    # Replace variables in template file to create new filter
    filter_file = ReplaceVariablesInTemplate(lines, DREAM3DFilter)
    # Write new filter in output directory
    output_file = GetDREAM3DFilterFilePathNoExt(DREAM3DFilter['RawFilterName'], directory)+ext
    with open(output_file,'w') as f:
        for line in filter_file:
            f.write("%s" % line)

def ExpandFilterName(filter_name):
  expanded_name=""
  for x in filter_name:
      if x.isupper():
          expanded_name += ' '
      expanded_name += x
  return expanded_name[1:]  # Remove first space

def RemoveBadSymbolsFromFilterDescription(description):
    description = description.replace("``","'")
    description = description.replace("''","'")
    return description

def InitializeParsingValues(DREAM3DFilter, filter_description):
    DREAM3DFilter['RawFilterName']=filter_description['name']
    DREAM3DFilter['ITKModule'] = ''
    DREAM3DFilter['FilterName']=GetDREAM3DFilterName(filter_description['name'])
    DREAM3DFilter['FilterNameLowerCase']=DREAM3DFilter['FilterName'].lower()
    DREAM3DFilter['FilterNameExpanded']=ExpandFilterName(filter_description['name'])
    DREAM3DFilter['mdunderline']='='*len(DREAM3DFilter['FilterName'])
    DREAM3DFilter['Parameters']= ''
    DREAM3DFilter['SetupFilterParameters'] = ''
    DREAM3DFilter['ReadFilterParameters'] = ''
    DREAM3DFilter['InitializationParameters'] = ''
    DREAM3DFilter['CheckIntegerEntry'] = ''
    DREAM3DFilter['RegisterTests'] = ''
    DREAM3DFilter['FilterTests'] = ''
    DREAM3DFilter['FilterDescription'] = ''
    DREAM3DFilter['FilterParameterDescription'] = ''
    DREAM3DFilter['DataCheckInternal'] = ''
    DREAM3DFilter['FilterInternal'] = ''
    DREAM3DFilter['TestsIncludeName'] = ''
    if 'briefdescription' in filter_description:
        DREAM3DFilter['FilterDescription'] += filter_description['briefdescription']+'\n\n'
    if 'detaileddescription' in filter_description:
        DREAM3DFilter['FilterDescription'] += filter_description['detaileddescription']
    DREAM3DFilter['FilterDescription'] = RemoveBadSymbolsFromFilterDescription(DREAM3DFilter['FilterDescription'])
    if 'output_pixel_type' in filter_description:
        DREAM3DFilter['FilterOutputType'] = filter_description['output_pixel_type']
    else:
        DREAM3DFilter['FilterOutputType'] = 'N/A'


def GetDREAM3DInitializationParameters(filter_member):
    type_conversion_dict = ITKToDream3DType[(filter_member['type'],filter_member['dim_vec'])]
    input_type = type_conversion_dict['std']
    output_type = type_conversion_dict['d3d']
    component_type = Dream3DTypeToMacro[output_type]['component']
    return '  m_'+filter_member['name']+'='+CastStdToDream3D[filter_member['dim_vec']]+'<'+input_type+','+output_type+','+component_type+'>('+str(filter_member['default'])+');\n'

def GetOutputDirectory(directory):
    return os.path.join(directory, 'ITKImageProcessingFilters')

def ExtractDescriptionList(fields, description, filter_fields, verbose, not_implemented):
    """ ExtractDescriptionList
    
    inputs:
      fields: description of the expected fields (gloabal variable)
      description: list containing input to be read and filtered
      verbose: print extra information (bool)
      not_implemented: ignore fields that are not implemented/used yet (bool)
    outputs:
      filter_fields: output filtered dictionary (dict)
      return value: False if error occurs (bool)
    """
    for descr in description:
        filter_field={}
        if not ExtractDescription(descr, fields, filter_field, verbose, not_implemented):
            return False
        filter_fields.append(filter_field)
    return True

def GetDream3DTestName(filter_description, test):
    """GetDream3DTestName
    
    Returns DREAM3D Test name based on the filter description and test description.
    
    inputs:
      filter_description: 
      test: 
    outputs:
      return value: Test name (string)
    """
    print "------------------------------"
    print type(filter_description)
    print filter_description
    print type(test)
    print test
    print "------------------------------"
    sys.exit(0)
    filterName=GetDREAM3DFilterName(filter_description['name'])
    testName=test['tag']
    return 'Test'+filterName+testName+'Test()'

def GetDream3DFilterTests(filter_description, test, test_settings, filter_test_measurements_results):
    testFunctionCode = 'int ' + GetDream3DTestName(filter_description, test) + '\n'
    testFunctionCode += '{\n'
    # Create and initialized necessary variables
    testFunctionCode += '    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/'+test['inputs'][0]+'");\n'  # Only reads the first file
    testFunctionCode += '    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");\n'
    testFunctionCode += '    DataContainerArray::Pointer containerArray = DataContainerArray::New();\n'
    # Read input image
    testFunctionCode += '    this->ReadImage(input_filename, containerArray, input_path);\n'
    # Filter image
    testFunctionCode += '    QString filtName = "'+GetDREAM3DFilterName(filter_description['name'])+'";\n'
    testFunctionCode += '    FilterManager* fm = FilterManager::Instance();\n'
    testFunctionCode += '    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);\n'
    testFunctionCode += '    DREAM3D_REQUIRE_NE(filterFactory.get(),0);\n'
    testFunctionCode += '    AbstractFilter::Pointer filter = filterFactory->create();\n'
    testFunctionCode += '    QVariant var;\n'
    testFunctionCode += '    bool propWasSet;\n'
    testFunctionCode += '    var.setValue(input_path);\n'
    testFunctionCode += '    propWasSet = filter->setProperty("SelectedCellArrayPath", var);\n'
    testFunctionCode += '    DREAM3D_REQUIRE_EQUAL(propWasSet, true);\n'
    testFunctionCode += '    var.setValue(false);\n'
    testFunctionCode += '    propWasSet = filter->setProperty("SaveAsNewArray", var);\n'
    testFunctionCode += '    DREAM3D_REQUIRE_EQUAL(propWasSet, true);\n'
    for settings in test_settings:
        if len(settings.keys()) == 0:
            continue
        testFunctionCode += '    {\n'
        d3d_type = ITKToDream3DType[(settings['type'],settings['dim_vec'])]['d3d']
        testFunctionCode += '        '+d3d_type+' d3d_var;\n'
        if settings['dim_vec'] == 0:
            testFunctionCode += '        d3d_var = '+str(settings['value'])+';\n'
        else:
            convertLettersToDims = {'x':0,'y':1,'z':2}
            for key,val in convertLettersToDims.iteritems():
                if len(settings['value']) > val:
                    testFunctionCode += '        d3d_var.'+key+' = '+str(settings['value'][val])+';\n'
                else:
                    testFunctionCode += '        d3d_var.'+key+' = 0; // should not be taken into account. Dim <\n'
        testFunctionCode += '        var.setValue(d3d_var);\n'
        testFunctionCode += '        propWasSet = filter->setProperty("'+settings['parameter']+'", var);\n'
        testFunctionCode += '        DREAM3D_REQUIRE_EQUAL(propWasSet, true);\n'
        testFunctionCode += '    }\n'
    testFunctionCode += '    filter->setDataContainerArray(containerArray);\n'
    testFunctionCode += '    filter->execute();\n'
    testFunctionCode += '    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);\n'
    testFunctionCode += '    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);\n'
    # Save filtered image for debugging purposes
    testFunctionCode += '    WriteImage("'+GetDREAM3DFilterName(filter_description['name'])+test['tag']+'.nrrd", containerArray, input_path);\n'
    if 'md5hash' in test and test['md5hash'] != '' and test['md5hash'] != None:
        # If MD5
        # Get MD5 from output image
        testFunctionCode += '    QString md5Output;\n'
        testFunctionCode += '    GetMD5FromDataContainer(containerArray, input_path, md5Output);\n'
        testFunctionCode += '    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("'+test['md5hash']+'"));\n'
    else:
        # Compare to expected MD5
        # Read baseline image
        testFunctionCode += '    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_'+filter_description['name']+'_'+test['tag']+'.nrrd");\n'
        testFunctionCode += '    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");\n'
        testFunctionCode += '    this->ReadImage(baseline_filename, containerArray, baseline_path);\n'
        testFunctionCode += '    int res = this->CompareImages(containerArray, input_path, baseline_path, '+str(test['tolerance'])+');\n'
        testFunctionCode += '    DREAM3D_REQUIRE_EQUAL(res,0);\n'
    for measurement in filter_test_measurements_results:
        if measurement != {}:
            testFunctionCode += '    var = filter->property("'+measurement['parameter']+'");\n'
            testFunctionCode += '    DREAM3D_REQUIRE_EQUAL(var.'+VariantToType[measurement['type']]+','+measurement['value']+');\n'
    testFunctionCode += '    return 0;\n'
    testFunctionCode += '}\n\n'
    return testFunctionCode

def GetDream3DRegisterTests(filter_description, test, test_settings):
    testName = GetDream3DTestName(filter_description, test)
    return '    DREAM3D_REGISTER_TEST( ' + testName + ');\n'


def VerifyFilterParameterTypes(filter_members,test_settings):
    for settings in test_settings:
        if len(settings.keys()) == 0:
            continue
        if settings['type'] == '':
            print("No type in test for %s - searching..."%settings['parameter'])
            print "settings:"+str(settings)
            print "filter_members:"+str(filter_members)
            list_types = [ x['type'] for x in filter_members if x['name'] == settings['parameter'] ]
            print list_types
            if len(list_types) != 1:
                print("No type in test for %s - No type found in filter_members."%settings['parameter'])
                return 0
            print("No type in test for %s - %s"%(settings['parameter'],list_types[0]))
            settings['type']=list_types[0]
    return 1
            
def GetDREAM3DParameterDescription(filter_member):
    description = '| '+filter_member['name']+' | '
    type_conversion_dict = ITKToDream3DType[(filter_member['type'],filter_member['dim_vec'])]
    member_type = type_conversion_dict['d3d']
    description += member_type
    description += '| '
    if 'detaileddescriptionSet' in filter_member and filter_member['detaileddescriptionSet'].strip() != '':
        description += filter_member['detaileddescriptionSet']
    elif 'briefdescriptionSet' in filter_member and filter_member['briefdescriptionSet'].strip() != '':
        description += filter_member['briefdescriptionSet']
    elif 'detaileddescriptionGet' in filter_member and filter_member['detaileddescriptionGet'].strip() != '':
        description += filter_member['detaileddescriptionGet']
    elif 'briefdescriptionGet' in filter_member and filter_member['briefdescriptionGet'].strip() != '':
        description += filter_member['briefdescriptionGet']
    else:
        description += 'N/A'
    description += ' |'
    return description


def FindModuleInIncludeFile(name, path):
    for root, dirs, files in os.walk(path):
        if name in files:
            file_path = os.path.join(root, name)
            datafile = file(file_path)
            for line in datafile:
                if '\ingroup' in line:
                    split_line=line.split('\ingroup')
                    module_name=split_line[1].strip()
                    if module_name[:3] == 'ITK':
                        return module_name[3:]
    return "NoModule"
            

def main(argv=None):
    # Parse arguments
    if argv == None:
        argv = sys.argv
    parser = argparse.ArgumentParser(
            prog=argv[0],
            description=__doc__
    )
    parser.add_argument('-j', '--JSONDirectory', dest='json_directory', required=True, help="Directory containing JSON files (inside SimpleITK)")
    parser.add_argument('-d', '--RootDirectory', dest='root_directory', required=True, help="Filter root directory (containing .git)")
    parser.add_argument('-v', '--Verbose', dest='verbose', action='store_true', help="Verbose")
    parser.add_argument('-vv', '--ExtraVerbose', dest='extra_verbose', action='store_true', help="Extra verbose")
    parser.add_argument('-n', '--NotImplemented', dest='not_implemented', action='store_true', help="Hide errors due to keyword not yet being implemented")
    parser.add_argument('-o', '--Overwrite', dest='overwrite', action='store_true', help="Overwrite files previously generated automatically")
    parser.add_argument('-b', '--DisableVerifications', dest='disable_verifications', action='store_true', help="Disable Verifications (this might result in errors)")
    parser.add_argument('-I', '--ITK_SRC_DIR', dest='itk_dir', required=True, help="ITK source directory (used to find ITK filter's module names)")
    parser.set_defaults(verbose=False)
    options = parser.parse_args(argv[1:])
    documentation_directory = os.path.join(options.root_directory, 'Documentation/ITKImageProcessingFilters')
    template_directory = os.path.join(options.root_directory, 'Utilities/SimpleITKJSONDream3DFilterCreationTemplates')
    filters_output_directory = GetOutputDirectory(options.root_directory)
    if options.verbose:
        print("JSON directory: %s"%options.json_directory)
        print("Root directory: %s"%options.root_directory)
        print("Output directory: %s"%filters_output_directory)
        print("Template directory: %s"%template_directory)
        print("NotImplemented: %r"%options.not_implemented)
        print("ITK source directory: %s"%options.itk_dir)
        print("Overwrite: %r"%options.overwrite)
        print("Verbose: %r"%options.verbose)
        print("Extra Verbose: %r"%options.extra_verbose)
    # Get list of JSON files
    all_json = [os.path.join(options.json_directory,f) for f in os.listdir(options.json_directory)\
                if os.path.isfile(os.path.join(options.json_directory, f)) and os.path.splitext(f)[1] == '.json']
    filter_list=[]
    # Processes each JSON file individually: one JSON file = one ITK filter = one DREAM.3D filter
    for current_json in all_json:
        print("Current JSON file: %s"%current_json)
        with open(current_json) as file_json:
            data_json = json.load(file_json)
        filter_description={}
        # Read input JSON.
        # Checks content of dictionary with 'general' dictionary as reference.
        # Output is written in 'filter_description'
        if not ExtractDescription(data_json, general, filter_description, options.extra_verbose, options.not_implemented):
            continue
        # Does filter already exist and has it been generated automatically
        # Skip generating filter if already exist and overwrite option not selected
        if not FilterFilesAlreadyExist(filter_description['name'], options.root_directory, options.overwrite):
            continue
        # Read JSON sub fields
        filter_inputs=[]
        # 'inputs' subfields first.
        # Read filter_description['inputs']
        # Checks content of dictionary with 'inputs' as a reference.
        # 'filter_inputs' is the output dictionary.
        if not ExtractDescriptionList(inputs, filter_description['inputs'],\
                                      filter_inputs, options.extra_verbose, options.not_implemented):
            continue
        # if 'inputs' specified, update 'number_of_input' with that value
        # if 'inputs' is specified, 'number_of_inputs' is typically initialized with '0'
        if len(filter_inputs):
            filter_description['number_of_inputs'] = len(filter_inputs)
        filter_members=[]
        if not ExtractDescriptionList(members, filter_description['members'],\
                                      filter_members, options.extra_verbose, options.not_implemented):
            continue
        # Add default members that are not included in the JSON description file
        filter_members += DefaultMembers[filter_description['template_code_filename']]
        # Extract measurements information
        filter_measurements=[]
        # Using "members" as a data structure example as 'measurements' and 'members' shoud contain the same data type
        if not ExtractDescriptionList(members, filter_description['measurements'],\
                                      filter_measurements, options.extra_verbose, options.not_implemented):
            continue
        # Read tests description
        filter_tests=[]
        if not ExtractDescriptionList(tests, filter_description['tests'],\
                                      filter_tests, options.extra_verbose, options.not_implemented):
            continue
        # Read tests settings
        # tests and settings are in the same order:
        # filter_tests[0]->filter_tests_settings[0]
        # A dictionary could have been used to contain filter_tests_settings
        # but most functions in this script used on the data containers expect
        # to work on lists.
        filter_test_settings=[]
        filter_test_measurements_results=[]
        for ii in range(len(filter_tests)):
            # Get test settings
            test_settings_description=[]
            current_settings = filter_tests[ii]['settings']
            if current_settings == []:
                test_settings_description = [{}]
            elif not ExtractDescriptionList(tests_settings, current_settings, test_settings_description,
                                          options.extra_verbose, options.not_implemented):
                filter_tests[ii]['error']=True
            # Get test measurements
            test_measurements_description=[]
            current_measurements = filter_tests[ii]['measurements_results']
            if current_measurements == []:
                test_measurements_description = [{}]
            elif not ExtractDescriptionList(tests_settings, current_measurements, test_measurements_description,
                                          options.extra_verbose, options.not_implemented):
                filter_tests[ii]['error']=True
            # Only append list of test settings if no error found in settings
            # The tests with errors in settings will be removed in the next step
            if 'error' not in filter_tests[ii]:
                filter_test_settings.append(test_settings_description)
                filter_test_measurements_results.append(test_measurements_description)
        # Remove tests that had errors in their settings
        filter_tests[:] = [x for x in filter_tests if 'error' not in x]
        # Since bad settings are not save, there should be the same number
        # of tests and test settings
        if len(filter_tests) != len(filter_test_settings):
            print("Different number of tests and settings.")
            print("filter_tests:%s"%(str(filter_tests)))
            print("filter_test_settings:%s"%(str(filter_test_settings)))
            print("skipping this filter")
            continue
        # Since bad measurements are not save, there should be the same number
        # of tests and test measurements
        if len(filter_tests) != len(filter_test_measurements_results):
            print("Different number of tests and measurements.")
            print("skipping this filter")
            continue
        # Verifies limitations
        if not options.disable_verifications:
            if not VerifyLimitations(general, [filter_description], options.verbose) or\
               not VerifyLimitations(inputs, filter_inputs, options.verbose) or\
               not VerifyLimitations(members, filter_members, options.verbose) or\
               not VerifyLimitations(members, filter_measurements, options.verbose) or\
               not VerifyLimitations(tests, filter_tests, options.verbose) or\
               not VerifyLimitations(tests_settings, filter_test_settings, options.verbose, True):
                print "VerifyLimitations Failed"
                continue
        # Filter lists in fields
        FilterFields(general, [filter_description])
        FilterFields(inputs, filter_inputs)
        FilterFields(members, filter_members)
        FilterFields(members, filter_measurements)
        FilterFields(tests, filter_tests)
        FilterFields(tests_settings, filter_test_settings, True)
        FilterFields(tests_settings, filter_test_measurements_results, True)
        # Check filter members type
        if not CheckTypeSupport(filter_members):
            print "CheckTypeSupport Failed"
            continue
        if not CheckITKTypeSupport(filter_members):
            print "CheckITKTypeSupport Failed"
            continue
        # Create Filter content
        # Initialization of replacement strings
        DREAM3DFilter={}
        InitializeParsingValues(DREAM3DFilter, filter_description)
        itk_include_file=GetDREAM3DITKInclude(filter_description['name'])
        # Find Module name in ITK_DIR
        DREAM3DFilter['ITKModule'] = FindModuleInIncludeFile(itk_include_file, options.itk_dir)
        #####
        include_list=[itk_include_file]+filter_description['include_files']
        member_include_list=[]
        for filter_member in filter_members:
            # Append Parameters
            DREAM3DFilter['Parameters'] += GetDREAM3DParameters(filter_member)
            # SetupFilterParameters
            [include, setup, limits] = GetDREAM3DSetupFilterParametersFromMembers(filter_member, DREAM3DFilter['FilterName'])
            DREAM3DFilter['SetupFilterParameters'] += '  '*bool(len(setup))+setup
            DREAM3DFilter['CheckIntegerEntry'] += '  '*bool(len(limits))+limits
            member_include_list.append(include)
            # ReadFilterParameters
            DREAM3DFilter['ReadFilterParameters'] += GetDREAM3DReadFilterParameters(filter_member)
            # initialization
            DREAM3DFilter['InitializationParameters'] += GetDREAM3DInitializationParameters(filter_member)
            # Description
            DREAM3DFilter['FilterParameterDescription'] += GetDREAM3DParameterDescription(filter_member) +'\n'
        #filter
        DREAM3DFilter['Filter']=ImplementFilter(filter_description, filter_members, filter_measurements)
        DREAM3DFilter['FilterInternal']=ImplementInternal(filter_description, 'this->filter')
        DREAM3DFilter['DataCheckInternal']=ImplementInternal(filter_description, 'this->dataCheck')
        #includes
        DREAM3DFilter['IncludeName']=FormatIncludes(include_list+member_include_list)
        DREAM3DFilter['TestsIncludeName']=FormatIncludes(member_include_list)
        # Implement tests
        for ii in range(len(filter_tests)):
            if not VerifyFilterParameterTypes(filter_members,filter_test_settings[ii]):
              print("Could not find all parameters types for tests_settings. This script will likely crash.")
            if not VerifyFilterParameterTypes(filter_measurements,filter_test_measurements_results[ii]):
              print("Could not find all parameters types for tests_measurements. This script will likely crash.")
            DREAM3DFilter['FilterTests'] += GetDream3DFilterTests(filter_description, filter_tests[ii], filter_test_settings[ii],filter_test_measurements_results[ii])
            DREAM3DFilter['RegisterTests'] += GetDream3DRegisterTests(filter_description, filter_tests[ii], filter_test_settings[ii])
        # Implement measurements get functions in header/${Parameters}
        for filter_measurement in filter_measurements:
            # Append Parameters
            DREAM3DFilter['Parameters'] += GetDREAM3DMeasurementParameters(filter_measurement)
        # Replace variables in template files
        ConfigureFiles('.h', template_directory, filters_output_directory, DREAM3DFilter, filter_description['template_code_filename'])
        ConfigureFiles('.cpp', template_directory, filters_output_directory, DREAM3DFilter, filter_description['template_code_filename'])
        testDirectory = os.path.join(options.root_directory, 'Test')
        # In SimpleITK, test rely on 'template_test_filename'. In this, we rely on 'template_code_filename'. This allows to include
        # definitions, enums,... that are included in SimpleITK and that would not be included here otherwise
        ConfigureFiles('Test.cpp', template_directory, testDirectory, DREAM3DFilter,  filter_description['template_code_filename'])
        ConfigureFiles('.md', template_directory, documentation_directory, DREAM3DFilter, "")  #
        # Append list of filters created
        filter_list.append(DREAM3DFilter['FilterName'])
    # Print manual step: Add created filters to CMakeLists
    print("Add these filters to  ITKImageProcessingFilters/SourceList.cmake and Test/CMakeLists.txt:")
    for filt in filter_list:
        print(filt)
    print("Update 'Test/TestFileLocations.h.in'")

if __name__=='__main__':
    main(sys.argv)
