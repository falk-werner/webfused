# webfused changelog

## 0.7.0 _(unknown)_

* __Chore:__ added changelog

## 0.6.0 _(Sat Nov 14 2020)_

*   __Feature:__ allow to specify mount options via config file
*   __Feature:__ make userdb optional
*   __Feature:__ reduce versions of depencies to required minimum
*   __Chore:__ updated dependencies (libwebsockets 4.1.3, libjansson 2.13.1, libfuse 3.10.0, webfuse 0.7.0)
*   __Chore:__ remove meson subprojects (use install_deps to fetch depedencies)
*   __Fix:__ ensure that syslog ident is valid while logger is active (invalid memory access) 

## 0.5.0 _(Sun Jul 19 2020)_

*   __Chore:__ updated depedencies (webfuse 0.5.0)

## 0.4.1 _(Sun Jul 05 2020)_

*   __Fix:__ removed gtest dependency, when built without tests

## 0.4.0 _(Sun Jul 05 2020)_

*   __Feature:__ allow to build without tests 
*   __Chore:__   updated dependencies (libwebsockets 4.0.0, webfuse 0.4.0)

## 0.3.0 _(Sat Jun 06 2020)_

*   __Chore:__ switched build system to meson

## 0.2.0 _(Fri Mar 20 2020)_

*   __Feature:__ enabled authentication (pam, userdb)
*   __Feature:__ use config file
*   __Feature:__ added syslog logger

## 0.1.0 _(Mon Mar 09 2020)_

*   __Feature:__ initial version

