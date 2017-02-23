#%define debug_package 1

Summary:	nxlog is a modular, multi-threaded, high-performance log management solution
Name:		nxlog-ce
Version:	2.7.1189
Release:	1
License:	NXLog Public License
Group:		System Environment/Daemons
Source:		nxlog-ce-%{version}.tar.gz
Vendor:         nxsec.com
URL:		http://nxlog.org
BuildRequires:	apr-devel pcre-devel openssl-devel libdbi-devel expat-devel openssl-devel gcc libgcc perl
Requires:	apr >= 1.2, libdbi >= 0.8.1, openssl, pcre, expat, zlib
BuildRoot:	%{_tmppath}/%{name}-root

%description

%prep
%setup -q -n nxlog-ce-%{version}

%build
export RM="rm -Rf"
export PATH="/opt/freeware/bin:$PATH"

./configure $(EXTRA_CONFIGURE_OPTIONS) --prefix=/opt/freeware --libexecdir=/opt/freeware/libexec --libdir=/opt/freeware/lib/nxlog --with-cachedir=/var/spool/nxlog --with-configfile=/etc/nxlog.conf --with-pidfile=/var/run/nxlog/nxlog.pid
make
exit 0

%install
export RM="rm -Rf"
export PATH="/opt/freeware/bin:$PATH"

make DESTDIR=$RPM_BUILD_ROOT install
rm -f $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/extension/*.a
rm -f $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/extension/*.la
rm -f $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/input/*.a
rm -f $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/input/*.la
rm -f $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/processor/*.a
rm -f $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/processor/*.la
rm -f $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/output/*.a
rm -f $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/output/*.la
rm -f $RPM_BUILD_ROOT/opt/freeware/lib/nxlog/*.a
rm -f $RPM_BUILD_ROOT/opt/freeware/lib/nxlog/*.la

strip $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/extension/*
strip $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/input/*
strip $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/processor/*
strip $RPM_BUILD_ROOT/opt/freeware/libexec/nxlog/modules/output/*
strip $RPM_BUILD_ROOT/opt/freeware/bin/nxlog*

install -d -m 755 $RPM_BUILD_ROOT/etc/init.d
install -m 755 packaging/aix/init $RPM_BUILD_ROOT/etc/init.d/nxlog

install -d -m 1770 $RPM_BUILD_ROOT/var/run/nxlog
install -d -m 0770 $RPM_BUILD_ROOT/var/lib/nxlog/cert
install -d -m 0770 $RPM_BUILD_ROOT/var/spool/nxlog
install -d -m 0770 $RPM_BUILD_ROOT/var/log/nxlog
install -d -m 0775 $RPM_BUILD_ROOT/etc
install -m 664 packaging/aix/nxlog.conf $RPM_BUILD_ROOT/etc/nxlog.conf

%clean

#rm -rf ${RPM_BUILD_ROOT}

%files
%defattr(-, root, root)
/opt/freeware/bin/nxlog*
/opt/freeware/libexec/nxlog/modules/*
/opt/freeware/lib/nxlog/*
/opt/freeware/share/doc/nxlog-ce/*
/opt/freeware/share/nxlog-ce/*
%config /etc/init.d/nxlog
%config /etc/nxlog.conf

%attr(1770, nxlog, nxlog) /var/run/nxlog
%attr(0770, nxlog, nxlog) /var/lib/nxlog/cert
%attr(0770, nxlog, nxlog) /var/spool/nxlog
%attr(0770, nxlog, nxlog) /var/log/nxlog


%pre
lsgroup nxlog >/dev/null || mkgroup nxlog
lsuser nxlog >/dev/null || \
    useradd -g nxlog -d /var/spool/nxlog -s /bin/sh \
    -c "user for the nxlog log managment tool" nxlog

%post
if [ -x /usr/lib/lsb/install_initd ]; then
  /usr/lib/lsb/install_initd /etc/init.d/nxlog
elif [ -x /sbin/chkconfig ]; then
  /sbin/chkconfig --add nxlog
else
   for i in 2 3 4 5; do
        /usr/bin/ln -sf /etc/init.d/nxlog /etc/rc.d/rc${i}.d/S90nxlog
   done
   for i in 6; do
        /usr/bin/ln -sf /etc/init.d/nxlog /etc/rc.d/rc${i}.d/K10nxlog
   done
fi

chown -R nxlog:nxlog /var/lib/nxlog

%preun
#only on uninstall, not on upgrades.
if [ $1 = 0 ]; then
  /etc/init.d/nxlog stop  > /dev/null 2>&1
  if [ -x /usr/lib/lsb/remove_initd ]; then
    /usr/lib/lsb/install_initd /etc/init.d/nxlog
  elif [ -x /sbin/chkconfig ]; then
    /sbin/chkconfig --del nxlog
  else
    rm -f /etc/rc.d/rc?.d/???nxlog
  fi
fi
%changelog

