Name:           pello
Version:        0.1.1
Release:        1%{?dist}
Summary:				Hello World example implemented in Python

License:				GPLv3+
URL:						https://example.com/%{name}
# Since this source is unreachable, it seems that rpm looks in SOURCES locally
# first.
Source0:				https://example.com/%{name}/release/%{name}-%{version}.tar.gz

BuildRequires:	python
Requires:				python
Requires:				bash

BuildArch:			noarch

buildroot:			/Users/peter.chen/prototypes/autotools/rpmbuild/rpm_wd_structure/BUILDROOT/%{Name}-%{Version}-%{Release}

%description
The long-tail description for our Hello World Example implemented in python

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
python -m compileall pello.py

%install
# Install built software into BUILDROOT
# Python shebang line is removed after compilation. We use a bash script to run the
# python compiled code.

# Here we hardcode /usr/lib/. Alternatively for more portable spec, we can use 
# %{_libdir}
mkdir -p %{buildroot}/%{_bindr}
mkdir -p %{buildroot}/usr/lib/%{name}

cat > %{buildroot}%{_bindir}/%{name} << EOF
#!/bin/bash
/usr/bin/python /usr/lib/%{name}/%{name}.pyc
EOF

chmod 0755 %{buildroot}/%{_bindir}/%{name}

install -m 0644 %{name}.py* %{buildroot}/usr/lib/%{name}/

%files
# Files that would be provided and installed on target system
# %license is a builtin macro that tells rpm manager that this file is the license
# %dir specifies that this directory is "owned" by this package for target query
#			 purposes
# 
%license LICENSE
%dir /usr/lib/%{name}/
%{_bindir}/%{name}
/usr/lib/%{name}/%{name}.py*

%changelog
# NOTE: This is the log of changes to the pkg spec file, not to what changed in the
#				software package in terms of features.
# Each item in the entry can be multiline and must begin with '-'
# Each entry timestamp begins with '*'
* Tue May 31 2016 Adam Miller <maxamillion@fedoraproject.org>
- First bello package
- Example second item in the changelog for version-release 0.1-1
