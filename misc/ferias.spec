Summary: Ferias out of office milter
Name: ferias
Version: 0.1
Release: 1%{dist}
License: Apache License 2.0
Group: Applications/Libraries
Source: ferias.tgz
BuildRoot: /var/tmp/%{name}-root
Requires: curl
BuildRequires: make
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: automake
BuildRequires: libtool
BuildRequires: libcurl-devel
BuildRequires: sendmail-devel
Packager: Tomas Halman

%description
Ferias out of office milter

%prep
%setup -n ferias

%build
./configure --prefix=%{_prefix} --libdir=%{_libdir}
make

%install
make DESTDIR=$RPM_BUILD_ROOT install

%clean
/bin/rm -rf $RPM_BUILD_ROOT

%files
%{_sbindir}/ferias
%{_unitdir}/ferias.service

%pre
%service_add_pre ferias.service

%post
%service_add_post ferias.service

%preun
%service_del_preun ferias.service

%postun
%service_del_postun ferias.service
