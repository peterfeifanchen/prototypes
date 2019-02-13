# Preamble Items
#===============
# Name of the package, should match SPEC file name
# Upstream version number of software, needed for dependency
# Number of times this was released. Reset to 1 after new version. (1%{?dist} to 
# increment this everytime the package is updated for any reason and reset to 1
# when Version increments)
# This is called the package N-V-R as the package filename is NAME-VERSION-RELEASE.
# (e.g. rpm -q python => python-2.7.5-34.e17.x86_64, NAME=python, VERSION=2.7.5, 
#  RELEASE=34.e17, x86_64 is the Arch, a side-effect of the build env)
Name:			hello-world
Version:	1
Release:	1

# Series of commands to prepare the software to be built. For example, decompressing
# source files in SourceN. This can be a shell script.
%prep
# No source files. Nothing here.

# Series of commands used to actually perform the build procedure. For example, 
# compile the software.
%build
cat > hello-world.sh << EOF
#!/usr/bin/bash
echo Hello World
EOF

# Series of commands for copying hte desired build artifacts from %builddir (where
# the bld happens, usually ~/rpmbuild/BUILD) to the %buildroot/BUILDROOT directory 
# (working directory of the rpmbuild, which in this case is rpm_wd_structure/).
#
# BUILDROOT acts as a root directory whose structure conforms to the target system's
# root filesystem hierarchy. This BUILDROOT is put into a cpio archive
%install
mkdir -p %{buildroot}/usr/bin/
install -m 755 hello-world.sh %{buildroot}/usr/bin/hello-world.sh

%clean

# Specify the name of the package
%package [NAME]
# Brief one line summary of pkg.
Summary:	Simplest RPM Package
# Licence of software being packaged, e.g., open-source license.
License:	GPLv3+

# Full URL for more information about the software pkg.
# URL:

# Path or URL to the compressed archive of unpatched source code. Meant to be stable
# upstream storage, not meant to be packager's local storage. 
# Source0:
# Source1: 
# ...

# Name of patches to apply to source code.
# Patch0:
# Patch1:
# ...

# Specifies Arch dependencies (e.g., x86_64, noarch). If no arch is specified, it 
# uses the arch of the build env. 
# BuildArch:
# ExcludeArch:

# Comma or whitespace separated or multiple BuildRequires list of packages required
# to build the program.
# BuildRequires:

# Comma or whitespace separated or multiple Requires list of packages required to
#	run the program.
# Requires:

# Brief description of the RPM. Multiline.
%description [NAME]
This is my first RPM package. It does nothing.

# List of files that will be installed in the end user's target system.
%files [NAME]
/usr/bin/hello-world.sh

# Record of changes that happened to the pkg b/w diff VERSION-RELEASE
%changelog
