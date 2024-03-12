# Maya C++ Plugin Test Example

## Introduction

This repository is designed to guide individuals with no or little experience in using Google Test in Visual Studio to test Maya C++ plugins. This guide assumes that you are working with Maya 2022.5 or superior, but the principles are applicable to other versions with some adjustments.

## Plugin Overview

The plugin featured in this repository is a simple utility designed for Autodesk Maya. It creates a custom Maya node that calculates the center point among a list of object positions. The positions are provided using the world matrix of each object. Although there are alternative methods to achieve this in Maya, the purpose of this plugin is to offer a straightforward example to demonstrate the testing process of Maya Python plugins with pytest and mocks.

This plugin serves as an illustrative example for those learning how to test Maya Python plugins, emphasizing the simplicity and directness of the process rather than the practicality of the plugin itself.

## Requirements

Before we dive into the configurations, ensure you have the following:

- Maya 2022.5 or your specific version installed.
- Visual Studio with C++ development tools installed.
- A basic understanding of C++ and Maya plugin development.

### Step 1: Install Google Test and Google Mock on the Test Project

2. **Nuget package install**: Use NuGet to install a Google Test package that preferably includes Google Mock, such as "google-testmock by Google Inc.".
3. **Verify Installation**: Write a simple test to ensure your setup is correctly configured:

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TestCaseName, MyTest) {
    ASSERT_EQ(2, 1 + 1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

4. **Build and Run**: Build your project and use the Test Explorer to confirm the test runs successfully. This step verifies that your setup is correct so far.

### Step 3: Linking Test and Plugin Projects

Linking your test project with the plugin project is mandatory for integrated testing.

#### 3.1 Project Configuration

For the test project to work with the Maya plugin, you must **update the plugin configurations and replicate the plugin project's configuration on the testing project** adding a few new steps:

- **Architecture**: Use only the x64 configuration. Avoid x86 to ensure compatibility with Maya.
- **General Settings**:
  - **Configuration Type**: Keep it as `.exe`. Unlike the plugin project, we do not select DLL because our test project is an executable application.
  - **Windows SDK Version**: Match the SDK version used in the plugin project to avoid compatibility issues.
  - **Platform Toolset**: Ensure this matches the plugin project to use the same compiler and toolchain.
- **C/C++ Settings**:
  - **Additional Include Directories**: Add the Maya SDK include directory (e.g., `C:\Path\to\Maya[version]\include`) just like the plugin project. This setting allows your test project to find Maya SDK headers.
  - **Multi-processor Compilation**: Set this to Yes (optional) to speed up build times.
- **Linker Settings**:
  - **Additional Library Directories**: Include paths to Maya SDK libraries (just like the plugin project) and add the directory containing your plugin's compiled `.lib` file. For example, add `C:\Path\to\the\plugin\built\file` alongside the Maya library paths.
  - **Additional Dependencies**: List all Maya libraries used by your plugin project (e.g., Foundation.lib, OpenMaya.lib, etc.) and add your plugin's `.lib` file. This file is found on the plugin build folder alongside with the .exe file. This step is critical for resolving external symbols referenced in your tests.

#### 3.2 Environment Variables Configuration

To help your test project find the required DLLs, create a `.runsettings` file with the necessary Maya bin folders:

```xml
<?xml version="1.0" encoding="utf-8"?>
<RunSettings>
  <RunConfiguration>
    <EnvironmentVariables>
      <PATH>C:\Path\to\Maya[version]\bin; C:\Path\to\Maya[version]\bin3;$(PATH)</PATH> 
    </EnvironmentVariables>
  </RunConfiguration>
</RunSettings>
```

- **Apply the .runsettings File**: Select this file in Visual Studio via Test > Configure Run Settings > Select Solution Wide runsettings File. This step ensures that the test runner includes Maya's directories in the PATH environment variable.

#### 3.3 Referencing the Implementation Project

An essential step in setting up your testing environment is to reference your implementation project from your test project. This connection ensures that your tests can directly use the classes and functions defined in your Maya plugin. Here's how to do it in Visual Studio:

1. **Right-click on your test project** in the Solution Explorer and select "Add" > "Reference...".
2. In the Reference Manager window, click on "Projects" on the left side. You'll see a list of projects within your current solution.
3. **Find your implementation project** in the list. This is the project where your Maya plugin is being developed.
4. **Check the box** next to your implementation project to create a reference. This action tells Visual Studio to link the two projects, allowing your test project to call into the implementation project.
5. Click "OK" to close the Reference Manager and apply your changes.

### Updating the Build Dependencies

After adding the reference, it's also a good idea to ensure that Visual Studio builds the projects in the correct order. 

1. **Right-click on your solution** in the Solution Explorer and select "Project Dependencies...".
2. You'll see a list of projects within your solution. **Select your test project** in the list.
3. In the dependencies list on the right, **check the box next to your implementation project**. This step ensures that Visual Studio builds the implementation project before attempting to build the test project, which is necessary for the test project to link correctly.

#### 3.4 Command Line Testing

For tests run outside Visual Studio, ensure the system's PATH environment variable includes Maya's bin directories. You might consider creating a batch file to automate this setup for command-line testing:

```batch
SET PATH=C:\Path\to\Maya[version]\bin; C:\Path\to\Maya[version]\bin3;%PATH%
your_test_executable.exe
```

This approach ensures a consistent environment for your tests, whether run from Visual Studio or the command line.

### Step 4: Writing and Running Your Tests

Now that your environment is set up and your projects are linked, it's time to write tests that utilize the OpenMaya libraries, mirroring the functionality used in your plugin.

#### 4.1 Creating a Test Function

Let's create a simple test function that uses the OpenMaya library to demonstrate how you can test your Maya plugin. This test will instantiate a class from your plugin (just to make sure that it will not throw errors) and perform a basic operation using the OpenMaya library.

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <maya/MVector.h>
#include "../MyMayaPlugin.h" // Adjust this include path to match your plugin's header file

TEST(MyPluginTests, TestVectorMagnitude) {
    MyMayaPlugin myPlugin; // Assuming MyMayaPlugin is a class from your plugin.

    MVector vector(1.0, 2.0, 3.0);
    double magnitude = vector.length(); // Use an OpenMaya class directly in your test
    
    ASSERT_NEAR(3.74, magnitude, 0.01); 
}
```

In this test, we instantiate myPlugin and an `MVector` object, which is part of Maya's OpenMaya API. We then calculate its magnitude and use `ASSERT_NEAR` to check if the magnitude is as expected, within a small margin of error.

#### 4.2 Verifying Tests in Test Explorer

After writing your test, build your test project in Visual Studio. To check if your tests are recognized:

1. Open **Test Explorer** by going to **Test > Test Explorer** in Visual Studio's menu.
2. After building, Test Explorer should refresh and display your newly created test under the test project's listing.
3. If you don't see your test, try rebuilding the solution or checking the output window for any build errors that might prevent the test from being discovered.

#### 4.3 Running the Tests

To run your tests:

1. In **Test Explorer**, right-click on your test or the test project and select **Run**.
2. Watch the Test Explorer window as it runs your selected tests. Passed tests will be marked with a green checkmark, while failed tests will show a red "x."
3. If a test fails, you can click on it in Test Explorer to view detailed output, including assertion failures and error messages, which can help with troubleshooting.

### Troubleshooting

If you encounter issues at any point, here are some troubleshooting tips:

- **Build Errors**: Double-check your project configurations and paths. Ensure that all paths to Maya SDK and libraries are correct and that both projects are targeting the same architecture (x64).
- **Tests Not Discovered or Running**: Verify that your test project correctly references the implementation project and that the `.runsettings` file is correctly configured and selected in Test > Configure Run Settings. Ensure your tests follow the Google Test naming conventions.
- **Linking Issues**: If you encounter linking errors related to Maya libraries or your plugin, confirm that the linker settings in your test project correctly include all necessary library paths and dependencies.
- **Runtime Errors Related to Maya DLLs**: Ensure that the PATH environment variable is correctly set up in your `.runsettings` file or your system environment variables, especially if running tests outside Visual Studio.
