Name:           cello
Version:				1.0 
Release:        1%{?dist}
Summary:				Hello World example implemented in C

License:				GPLv3+
URL:						https://example.com/%{name}
# Since this source is unreachable, it seems that rpm looks in SOURCES locally
# first.
Source0:				https://example.com/%{name}/release/%{name}-%{version}.tar.gz

Patch0:					cello-output-first-patch.patch

BuildRequires:	gcc
BuildRequires:  make

buildroot:			/Users/peter.chen/prototypes/autotools/rpmbuild/rpm_wd_structure/BUILDROOT/%{Name}-%{Version}-%{Release}

%description
The long-tail description for our Hello World Example implemented in C

%prep
# Provided rpmbuild macro. The builtin %setup, like %patch, extracts the tar files
# in the SourceN, which creates a subdirectory. If the SourceN is not a compressed
# tar which creates subdirectories, you must pass '-c". By default, the subdirectory
# is first deleted and then recreated. To prevent this if you need, pass '-D'. For
# %patch, similarly, it works on the PatchN. Notation is simply %patch1,...
# Equivalently, this can be expressed as %patch -P N. To remove leading backslashes,
# "-p <# of backslashes>".
%setup -q
%patch0

%build
# We can just call make of the C program source. _smp_mflags specifies -j option for
# the Makefile for parallel make. We do not call the native builtin %configure as we
# do not have a configure script in the source.
make %{?_smp_mflags}

%install
# Install built software into BUILDROOT
# Use builtin %make_install macro as we have a Makefile. This is basically a wrapper
# for "make install". These macros can be found in /usr/lib/rpm/macros.
%make_install

%files
# Files that would be provided and installed on target system
# %license is a builtin macro that tells rpm manager that this file is the license
# %dir specifies that this directory is "owned" by this package for target query
#			 purposes
# %doc identifies the documentation 
%license LICENSE
%{_bindir}/%{name}

%changelog
# NOTE: This is the log of changes to the pkg spec file, not to what changed in the
#				software package in terms of features.
# Each item in the entry can be multiline and must begin with '-'
# Each entry timestamp begins with '*'
* Tue May 31 2016 Adam Miller <maxamillion@fedoraproject.org>
- First cello package
