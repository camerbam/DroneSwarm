set(exe_name Monitor)
if(WIN32)
  project(${exe_name} LANGUAGES CSharp)
  include(CSharpUtilities)
endif()
###################################
set(${exe_name}_config
  App.config
  )
list(APPEND ${exe_name}_libsrcs ${${exe_name}_config})
source_group(config FILES ${${exe_name}_config})
####################################
set(${exe_name}_designer
  Properties/AssemblyInfo.cs
  Properties/Resources.Designer.cs
  Properties/Resources.resx
  Properties/Settings.Designer.cs
  Properties/Settings.settings
  )
list(APPEND ${exe_name}_libsrcs ${${exe_name}_designer})
source_group(designer FILES ${${exe_name}_designer})
####################################
set(${exe_name}_xaml
  App.xaml
  App.xaml.cs
  MainWindow.xaml
  MainWindow.xaml.cs
  StringMsg.cs
  )
list(APPEND ${exe_name}_libsrcs ${${exe_name}_xaml})
source_group(xaml FILES ${${exe_name}_xaml})
####################################
if(WIN32)
  add_executable(${exe_name} ${${exe_name}_config} ${${exe_name}_designer} ${${exe_name}_xaml})
  csharp_set_designer_cs_properties(${${exe_name}_designer})
  csharp_set_xaml_cs_properties(${${exe_name}_xaml})
  set_property(SOURCE App.xaml PROPERTY VS_XAML_TYPE "ApplicationDefinition")
  set_property(TARGET ${exe_name} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
  set_property(TARGET ${exe_name} PROPERTY WIN32_EXECUTABLE TRUE)
  set_property(TARGET ${exe_name} PROPERTY VS_DOTNET_REFERENCES
  	"Microsoft.CSharp"
  	"PresentationCore"
  	"PresentationFramework"
  	"System"
  	"System.Core"
  	"System.Data"
  	"System.Data.DataSetExtensions"
  	"System.Net.Http"
  	"System.Xaml"
  	"System.Xml"
  	"System.Xml.Linq"
  	"WindowsBase")
  set_property(TARGET ${exe_name} 
    PROPERTY VS_PACKAGE_REFERENCES "Google.Protobuf_3.11.4"
    )	
endif()
xpSourceListAppend("${${exe_name}_libsrcs}")

