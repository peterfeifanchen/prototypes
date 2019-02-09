Name:           bello
Version:        0.1
Release:        1%{?dist}
Summary:        Hello world script implemente din bash script

License:        GPLv3+
URL:						https://example.com/%{name}
# Since this source is unreachable, it seems that rpm looks in SOURCES locally
# first.
Source0:				https://example.com/%{name}/release/%{name}-%{version}.tar.gz	

Requires:				bash

BuildArch:			noarch

buildroot:			/Users/peter.chen/prototypes/autotools/rpmbuild/rpm_wd_structure/BUILDROOT/%{Name}-%{Version}-%{Release}

%description
Toy example with hello world bash script for package management

%prep
# Provided rpmbuild macro. The builtin %setup, like %patch, extracts the tar files
# in the SourceN, which creates a subdirectory. If the SourceN is not a compressed
# tar which creates subdirectories, you must pass '-c". By default, the subdirectory
# is first deleted and then recreated. To prevent this if you need, pass '-D'. For
# %patch, similarly, it works on the PatchN. Notation is simply %patch1,...
# Equivalently, this can be expressed as %patch -P N. To remove leading backslashes,
# "-p <# of backslashes>".
%setup -q

%build

%install
# Install built software into BUILDROOT
mkdir -p %{buildroot}/%
install -m 0755 %{name} %{buildroot}/%{_bindir}/%{name}


%files
# Files that would be provided and installed on target system
# %license is a builtin macro that tells rpm manager that this file is the license
%license LICENCE
%{_bindir}/%{name}

# NOTE: This is the log of changes to the pkg spec file, not to what changed in the
#				software package in terms of features.
# Each item in the entry can be multiline and must begin with '-'
# Each entry timestamp begins with '*'
%changelog
* Tue May 31 2016 Adam Miller <maxamillion@fedoraproject.org>
- First bello package
- Example second item in the changelog for version-release 0.1-1
