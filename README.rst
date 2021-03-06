.. image::  https://raw.github.com/p3/regal/master/doc/regal.jpg
   :height: 128 px
   :width:  128 px
   :align: right

=====
Regal
=====

https://github.com/p3/regal

.. contents::

Summary
=======

OpenGL portability layer for OpenGL 2.x, 3.x, 4.x, Core contexts and ES 2.0

+---------------+----------------------------------------------------------------------+
|  **Warning**  |  Regal is pre-alpha software, it is available here for evaluation    |
|               |  and to play with the examples.  Don't use it in your projects yet.  |
|               |  You have been warned.                                               |
+---------------+----------------------------------------------------------------------+

Goals
=====

Regal is a portable OpenGL layer on top of existing OpenGL implementations.
Compile and link an app against Regal and deploy on various OpenGL implementations
including compatibility, core and ES 2.0 OpenGL contexts.

* Portability
    Consistent OpenGL API that runs on all major platforms:
    Windows, Linux, Mac, iOS, and Android

* Compatible
    Immediate mode, fixed function, GL_QUADS work everywhere, emulated as necessary.

* Modern OpenGL API
    Direct State Access (DSA) and Vertex Array Object (VAO), emulated as necessary.

* Open Source
    There is nothing behind the curtain.
    You can see what Regal is doing and change it if needed.

* Ease of Use
    Set breakpoints on OpenGL functions.
    Step into the code.
    Inspect state.

* Efficiency
    If it cannot be implemented efficiently in Regal, it is not universally supported.
    For example, tessellation support requires tessellation hardware.

Deployment
==========

* Most code remains unchanged.
* Optionally **#include <GL/Regal.h>**
* Link with **regal32.dll**, **libRegal.so** or **libRegal.dylib**

From an application developer's perspective, Regal just looks like an OpenGL implementation.
You link with it instead of your platform's OpenGL library or framework, and that's really
all you have to do to use Regal.  The rest of your code can remain unchanged.

Features
========

Emulation
---------

  - Fixed function texturing, lighting and fog.
  - Direct State Access extension

    http://www.opengl.org/registry/specs/EXT/direct_state_access.txt

  - GL_EXT_debug_marker extension

    http://www.khronos.org/registry/gles/extensions/EXT/EXT_debug_marker.txt

  - Fixed function and DSA emulation enabled by default, except when compatibility context
    with DSA extension detected.

  - Build-time configuration:

    - **REGAL_EMULATION**         -- Enable/disable the use of emulation
    - **REGAL_FORCE_EMULATION**   -- Force the use of emulation, even for compatibility contexts with DSA

  - Environment variable configuration:

    - **REGAL_EMULATION**         -- Enable/disable the use of emulation
    - **REGAL_FORCE_EMULATION**   -- Force the use of emulation, even for compatibility contexts with DSA

  - Runime configuration via GL_REGAL_enable_:

    - **GL_EMULATION_REGAL**      -- glEnable/glDisable emulation

OpenGL error checking
---------------------

  - Disabled by default.

  - Build-time configuration: **REGAL_ERROR**

  - Environment variable configuration: **REGAL_ERROR**

  - Runtime configuration via GL_REGAL_enable_: **GL_ERROR_REGAL**

Logging
-------

  Regal supports detailed logging for development purposes.

  - **Error**       -- Fatal and non-fatal Regal runtime errors.
  - **Warning**     -- Non-fatal Regal warnings.
  - **Info**        -- Informational messages.
  - **App**         -- Application calls into Regal.
  - **Driver**      -- Regal calls into OpenGL.
  - **Internal**    -- Internal Regal calls.
  - **Http**        -- HTTP traffic logging.

  Build-time configuration:

  - **REGAL_LOG_ERROR**
  - **REGAL_LOG_WARNING**
  - **REGAL_LOG_INFO**
  - **REGAL_LOG_APP**
  - **REGAL_LOG_DRIVER**
  - **REGAL_LOG_INTERNAL**
  - **REGAL_LOG_HTTP**

  * Debug mode: All logging supported with error, warning, info and http logging enabled.
  * Release mode: Support for application, driver and internal logging disabled by default.

  Environment variable configuration:

  - **REGAL_LOG_ERROR**
  - **REGAL_LOG_WARNING**
  - **REGAL_LOG_INFO**
  - **REGAL_LOG_APP**
  - **REGAL_LOG_DRIVER**
  - **REGAL_LOG_INTERNAL**
  - **REGAL_LOG_HTTP**
  - **REGAL_LOG_API**       --- Application + Driver OpenGL logging
  - **REGAL_LOG_NONE**      --- enable or disable all logging

  Environment variable lookup is globally disabled by defining **REGAL_NO_GETENV**
  at compile-time.

  Runtime configuration via GL_REGAL_log_ extension: (**glEnable/glDisable/glIsEnabled**)

  - **GL_LOG_ERROR_REGAL**
  - **GL_LOG_WARNING_REGAL**
  - **GL_LOG_INFO_REGAL**
  - **GL_LOG_APP_REGAL**
  - **GL_LOG_DRIVER_REGAL**
  - **GL_LOG_INTERNAL_REGAL**
  - **GL_LOG_HTTP_REGAL**

Multi-threading support
-----------------------

  - Enabled by default.

  - Build-time configuration: **REGAL_NO_TLS**

Spoofing OpenGL vendor, renderer, version and extension strings
---------------------------------------------------------------

  Build-time configuration

  - **REGAL_GL_VENDOR**
  - **REGAL_GL_RENDERER**
  - **REGAL_GL_VERSION**
  - **REGAL_GL_EXTENSION**

  Environment variable configuration:

  - **REGAL_GL_VENDOR**
  - **REGAL_GL_RENDERER**
  - **REGAL_GL_VERSION**
  - **REGAL_GL_EXTENSION**

  Environment variable lookup is globally disabled by defining **REGAL_NO_GETENV**
  at compile-time.

Browser-based interface
-----------------------

  - Enabled by default.

  - **REGAL_NO_HTTP**         --- Build-time and environment variable enable/disable.
  - **REGAL_HTTP_PORT**       --- Port for HTTP connections (8080 by default)
  - **REGAL_HTTP_LOG_LIMIT**  --- Number of lines of logging buffered

  - `http://127.0.0.1:8080/log`_                          --- View log
  - `http://127.0.0.1:8080/glEnable?GL_LOG_APP_REGAL`_    --- Enable/disable application logging
  - `http://127.0.0.1:8080/glEnable?GL_LOG_DRIVER_REGAL`_ --- Enable/disable driver logging
  - `http://127.0.0.1:8080`_                              --- State information

.. _http://127.0.0.1:8080/log:                          http://127.0.0.1:8080/log
.. _http://127.0.0.1:8080/glEnable?GL_LOG_APP_REGAL:    http://127.0.0.1:8080/glEnable?GL_LOG_APP_REGAL
.. _http://127.0.0.1:8080/glEnable?GL_LOG_DRIVER_REGAL: http://127.0.0.1:8080/glEnable?GL_LOG_DRIVER_REGAL
.. _http://127.0.0.1:8080:                              http://127.0.0.1:8080

Regal OpenGL extensions
-----------------------

  - GL_REGAL_error_string_ for GLU-style GLenum to error string conversion.
  - GL_REGAL_extension_query_ for GLEW-style extension checking.
  - GL_REGAL_log_ for Regal logging configuration.
  - GL_REGAL_enable_ for Regal feature configuration.

In Progress
===========

* Google Native Client (NaCL) 1st class support
* glPushAttrib emulation

Planned Features
================

* Debug label.
* Debug output.
* Web browser-based debugging tools.
* Display list emulation.

Limitations
===========

* Incomplete implementation, so far.
* Limited GLSL language support.
* GL_QUADS only works in immediate mode or with DrawArrays for ES and core profiles.

.. _GL_REGAL_error_string:    https://github.com/p3/regal/blob/master/doc/extensions/GL_REGAL_error_string.txt
.. _GL_REGAL_extension_query: https://github.com/p3/regal/blob/master/doc/extensions/GL_REGAL_extension_query.txt
.. _GL_REGAL_log:             https://github.com/p3/regal/blob/master/doc/extensions/GL_REGAL_log.txt
.. _GL_REGAL_enable:          https://github.com/p3/regal/blob/master/doc/extensions/GL_REGAL_enable.txt

Other Information
=================

* #AltDevBlogADay blog: `Bringing Regal OpenGL to Native Client <http://www.altdevblogaday.com/2012/09/04/bringing-regal-opengl-to-native-client/>`_.
* opengl.org forum thread: `Introducing Regal <http://www.opengl.org/discussion_boards/showthread.php/178678-Introducing-Regal?p=1241199>`_.
* Slides - `Regal for Software Development <http://www.slideshare.net/nigelstewart1/regal-tlf>`_.

.. raw:: html

    <iframe src="http://www.slideshare.net/slideshow/embed_code/13870512" width="597" height="486" frameborder="0" marginwidth="0" marginheight="0" scrolling="no" style="border:1px solid #CCC;border-width:1px 1px 0;margin-bottom:5px" allowfullscreen> </iframe>

License and Credits
===================

Regal code, API database and generators are `BSD <http://opensource.org/licenses/bsd-license.php>`_ licensed.

::

  Copyright (c) 2011 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGE.

Boost is licensed under `Boost Software License <http://www.boost.org/users/license.html>`_.

GLEW is licensed under `BSD and MIT License <http://glew.sourceforge.net/credits.html>`_.

GLU is licensed under `SGI FREE SOFTWARE LICENSE B <http://oss.sgi.com/projects/FreeB/>`_.

GLUT license:

::

  /* Copyright (c) Mark J. Kilgard, 1994, 1995, 1996, 1998. */

  /* This program is freely distributable without licensing fees  and is
     provided without guarantee or warrantee expressed or  implied. This
     program is -not- in the public domain. */

Dreamtorus example program is `public domain <http://unlicense.org/>`_.
