/*
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
 */

/*

 Regal object renaming layer
 Cass Everitt

 */

#ifndef __REGAL_OBJ_H__
#define __REGAL_OBJ_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <map>

#include "RegalPrivate.h"
#include "RegalEmu.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct RegalName {
    GLuint app;
    GLuint drv;
    RegalName() : app( 0 ), drv( 0 ) {}
};

struct RegalNameTranslator {
    std::map< GLuint, RegalName > app2drv;
    std::map< GLuint, RegalName * > drv2app;
    void (REGAL_CALL *gen)( GLsizei n, GLuint * objs );
    void (REGAL_CALL *del)( GLsizei n, const GLuint * objs );

    RegalNameTranslator() : gen( NULL ), del ( NULL ) {
        drv2app[ 0 ] = & app2drv[ 0 ];  // special case 0
    }

    GLboolean IsObject( GLuint appName ) {
        return app2drv.count( appName ) > 0 ? GL_TRUE : GL_FALSE;
    }

    GLuint Gen() {
        RegalName name;
        gen( 1, & name.drv );
        // could be more clever here, and this could fail...
        name.app = name.drv;
        const GLuint searchLimit = 1000000000;
        for( GLuint i = 0; i < searchLimit; i++ ) {
            if( app2drv.count( name.app ) == 0 ) {
                break;
            }
            name.app++;
        }
        if( ( name.app - name.drv ) >= searchLimit ) {
            return 0;
        }
        app2drv[ name.app ] = name;
        drv2app[ name.drv ] = & app2drv[ name.app ];
        return name.app;
    }

    GLuint ToDriverName( GLuint appName ) {
        if( app2drv.count( appName ) == 0 ) {
            RegalName & name = app2drv[ appName ];
            name.app = appName;
            gen( 1, & name.drv );
            drv2app[ name.drv ] = &name;
        }
        return app2drv[ appName ].drv;
    }

    GLuint ToAppName( GLuint drvName ) {
        if( drv2app.count( drvName ) ) {
            RegalAssert( drv2app[ drvName ] != NULL );
            return drv2app[ drvName ]->app;
        }
        return 0;
    }

    void Delete( GLuint appName ) {
        if( appName == 0 || app2drv.count( appName ) == 0 ) {
            return;
        }
        RegalName n = app2drv[ appName ];
        app2drv.erase( n.app );
        RegalAssert( drv2app.count( n.drv ) != 0 );
        drv2app.erase( n.drv );
        del( 1, & n.drv );
    }
};



struct RegalObj : public RegalEmu {

    RegalNameTranslator bufferNames;
	RegalNameTranslator vaoNames;

    void Init( RegalContext * ctx ) {
        RegalEmuScopedActivate activate( ctx, this );
        bufferNames.gen = ctx->dispatcher.emulation.glGenBuffers;
        bufferNames.del = ctx->dispatcher.emulation.glDeleteBuffers;
		vaoNames.gen = ctx->dispatcher.emulation.glGenVertexArrays;
		vaoNames.del = ctx->dispatcher.emulation.glDeleteVertexArrays;
    }

	void BindBuffer( RegalContext * ctx, GLenum target, GLuint bufferBinding ) {
		DispatchTable & tbl = ctx->dispatcher.emulation;
		tbl.glBindBuffer( target, bufferNames.ToDriverName( bufferBinding ) );
	}

	void GenBuffers( RegalContext * ctx, GLsizei n, GLuint * buffers ) {
		UNUSED_PARAMETER(ctx);
		for( int i = 0; i < n; i++ ) {
			buffers[ i ] = bufferNames.Gen();
		}
	}

	void DeleteBuffers( RegalContext * ctx, GLsizei n, const GLuint * buffers ) {
		UNUSED_PARAMETER(ctx);
		for( int i = 0; i < n; i++ ) {
			bufferNames.Delete( buffers[ i ] );
		}
	}

    GLboolean IsBuffer( RegalContext * ctx, GLuint appName ) {
		UNUSED_PARAMETER(ctx);
        return bufferNames.IsObject( appName );
    }


	void BindVertexArray( RegalContext * ctx, GLuint vao ) {
		DispatchTable & tbl = ctx->dispatcher.emulation;
		tbl.glBindVertexArray( vaoNames.ToDriverName( vao ) );
	}

	void GenVertexArrays( RegalContext * ctx, GLsizei n, GLuint * vaos ) {
		UNUSED_PARAMETER(ctx);
		for( int i = 0; i < n; i++ ) {
			vaos[ i ] = vaoNames.Gen();
		}
	}

	void DeleteVertexArrays( RegalContext * ctx, GLsizei n, const GLuint * vaos ) {
		UNUSED_PARAMETER(ctx);
		for( int i = 0; i < n; i++ ) {
			vaoNames.Delete( vaos[ i ] );
		}
	}

    GLboolean IsVertexArray( RegalContext * ctx, GLuint appName ) {
		UNUSED_PARAMETER(ctx);
        return vaoNames.IsObject( appName );
    }



};

REGAL_NAMESPACE_END

#endif // ! __REGAL_VAO_H__







