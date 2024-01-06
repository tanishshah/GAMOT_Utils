## Installing the driver (or other nuget packages)

### Method 1. Terminal
1. dotnet new classlib --framework "netstandard<version>" -o <name>
2. cd <name>
3. dotnet add package <package name>
4. dotnet publish
5. copy the dlls to unity

### Method 2. From visual studio
1. Open Visual studio
2. Open a C# class file
3. Go to nuget package manager and install the package
4. Build
5. dotnet publish
6. Copy the dlls to unity