set(exe_name Monitor)
project(${exe_name} LANGUAGES CSharp)
include(CSharpUtilities)
set(${exe_name}_config
  App.config
  )
set(${exe_name}_designer
  Properties/AssemblyInfo.cs
  Properties/Resources.Designer.cs
  Properties/Resources.resx
  Properties/Settings.Designer.cs
  Properties/Settings.settings
  )
set(${exe_name}_xaml
  App.xaml
  App.xaml.cs
  MainWindow.xaml
  MainWindow.xaml.cs
  )
add_executable(${exe_name} ${${exe_name}_config} ${${exe_name}_designer} ${${exe_name}_xaml})
csharp_set_designer_cs_properties(${${exe_name}_designer})
csharp_set_xaml_cs_properties(${${exe_name}_xaml})
#source_group("" FILES ${${exe_name}_srcs})
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
