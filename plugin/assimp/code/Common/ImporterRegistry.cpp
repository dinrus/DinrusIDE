﻿/*
---------------------------------------------------------------------------
Open Asset Import Library (assimp)
---------------------------------------------------------------------------

Copyright (c) 2006-2019, assimp team



All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

/** @file ImporterRegistry.cpp

Central registry for all importers available. Do not edit this file
directly (unless you are adding new loaders), instead use the
corresponding preprocessor flag to selectively disable formats.
*/

#include <vector>
#include <assimp/BaseImporter.h>

// ------------------------------------------------------------------------------------------------
// Importers
// (include_new_importers_here)
// ------------------------------------------------------------------------------------------------
#ifndef ASSIMP_BUILD_NO_X_IMPORTER
#   include "X/XFileImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_AMF_IMPORTER
#   include "AMF/AMFImporter.hpp"
#endif
#ifndef ASSIMP_BUILD_NO_3DS_IMPORTER
#   include "3DS/3DSLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_MD3_IMPORTER
#   include "MD3/MD3Loader.h"
#endif
#ifndef ASSIMP_BUILD_NO_MDL_IMPORTER
#   include "MDL/MDLLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_MD2_IMPORTER
#   include "MD2/MD2Loader.h"
#endif
#ifndef ASSIMP_BUILD_NO_PLY_IMPORTER
#   include "Ply/PlyLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_ASE_IMPORTER
#   include "ASE/ASELoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_OBJ_IMPORTER
#   include "Obj/ObjFileImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_HMP_IMPORTER
#   include "HMP/HMPLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_SMD_IMPORTER
#   include "SMD/SMDLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_MDC_IMPORTER
#   include "MDC/MDCLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_MD5_IMPORTER
#   include "MD5/MD5Loader.h"
#endif
#ifndef ASSIMP_BUILD_NO_STL_IMPORTER
#   include "STL/STLLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_LWO_IMPORTER
#   include "LWO/LWOLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_DXF_IMPORTER
#   include "DXF/DXFLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_NFF_IMPORTER
#   include "NFF/NFFLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_RAW_IMPORTER
#   include "Raw/RawLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_SIB_IMPORTER
#   include "SIB/SIBImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_OFF_IMPORTER
#   include "OFF/OFFLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_AC_IMPORTER
#   include "AC/ACLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_BVH_IMPORTER
#   include "BVH/BVHLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_IRRMESH_IMPORTER
#   include "Irr/IRRMeshLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_IRR_IMPORTER
#   include "Irr/IRRLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_Q3D_IMPORTER
#   include "Q3D/Q3DLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_B3D_IMPORTER
#   include "B3D/B3DImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_COLLADA_IMPORTER
#   include "Collada/ColladaLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_TERRAGEN_IMPORTER
#   include "Terragen/TerragenLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_CSM_IMPORTER
#   include "CSM/CSMLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_3D_IMPORTER
#   include "Unreal/UnrealLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_LWS_IMPORTER
#   include "LWS/LWSLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_OGRE_IMPORTER
#   include "Ogre/OgreImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_OPENGEX_IMPORTER
#   include "OpenGEX/OpenGEXImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_MS3D_IMPORTER
#   include "MS3D/MS3DLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_COB_IMPORTER
#   include "COB/COBLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_BLEND_IMPORTER
#   include "Blender/BlenderLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_Q3BSP_IMPORTER
#   include "Q3BSP/Q3BSPFileImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_NDO_IMPORTER
#   include "NDO/NDOLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_IFC_IMPORTER
#   include "Importer/IFC/IFCLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_XGL_IMPORTER
#   include "XGL/XGLLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_FBX_IMPORTER
#   include "FBX/FBXImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_ASSBIN_IMPORTER
#   include "Assbin/AssbinLoader.h"
#endif
#ifndef ASSIMP_BUILD_NO_GLTF_IMPORTER
#   include "glTF/glTFImporter.h"
#   include "glTF2/glTF2Importer.h"
#endif
#ifndef ASSIMP_BUILD_NO_C4D_IMPORTER
#   include "C4D/C4DImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_3MF_IMPORTER
#   include "3MF/D3MFImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_X3D_IMPORTER
#   include "X3D/X3DImporter.hpp"
#endif
#ifndef ASSIMP_BUILD_NO_MMD_IMPORTER
#   include "MMD/MMDImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_M3D_IMPORTER
#   include "M3D/M3DImporter.h"
#endif
#ifndef ASSIMP_BUILD_NO_STEP_IMPORTER
#   include "Importer/StepFile/StepFileImporter.h"
#endif

namespace Assimp {

// ------------------------------------------------------------------------------------------------
void GetImporterInstanceList(std::vector< BaseImporter* >& out)
{
    // ----------------------------------------------------------------------------
    // Add an instance of each worker class here
    // (register_new_importers_here)
    // ----------------------------------------------------------------------------
    out.reserve(64);
#if (!defined ASSIMP_BUILD_NO_X_IMPORTER)
    out.push_back( new XFileImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_OBJ_IMPORTER)
    out.push_back( new ObjFileImporter());
#endif
#ifndef ASSIMP_BUILD_NO_AMF_IMPORTER
	out.push_back( new AMFImporter() );
#endif
#if (!defined ASSIMP_BUILD_NO_3DS_IMPORTER)
    out.push_back( new Discreet3DSImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_M3D_IMPORTER)
    out.push_back( new M3DImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_MD3_IMPORTER)
    out.push_back( new MD3Importer());
#endif
#if (!defined ASSIMP_BUILD_NO_MD2_IMPORTER)
    out.push_back( new MD2Importer());
#endif
#if (!defined ASSIMP_BUILD_NO_PLY_IMPORTER)
    out.push_back( new PLYImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_MDL_IMPORTER)
    out.push_back( new MDLImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_ASE_IMPORTER)
  #if (!defined ASSIMP_BUILD_NO_3DS_IMPORTER)
    out.push_back( new ASEImporter());
#  endif
#endif
#if (!defined ASSIMP_BUILD_NO_HMP_IMPORTER)
    out.push_back( new HMPImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_SMD_IMPORTER)
    out.push_back( new SMDImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_MDC_IMPORTER)
    out.push_back( new MDCImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_MD5_IMPORTER)
    out.push_back( new MD5Importer());
#endif
#if (!defined ASSIMP_BUILD_NO_STL_IMPORTER)
    out.push_back( new STLImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_LWO_IMPORTER)
    out.push_back( new LWOImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_DXF_IMPORTER)
    out.push_back( new DXFImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_NFF_IMPORTER)
    out.push_back( new NFFImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_RAW_IMPORTER)
    out.push_back( new RAWImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_SIB_IMPORTER)
    out.push_back( new SIBImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_OFF_IMPORTER)
    out.push_back( new OFFImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_AC_IMPORTER)
    out.push_back( new AC3DImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_BVH_IMPORTER)
    out.push_back( new BVHLoader());
#endif
#if (!defined ASSIMP_BUILD_NO_IRRMESH_IMPORTER)
    out.push_back( new IRRMeshImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_IRR_IMPORTER)
    out.push_back( new IRRImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_Q3D_IMPORTER)
    out.push_back( new Q3DImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_B3D_IMPORTER)
    out.push_back( new B3DImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_COLLADA_IMPORTER)
    out.push_back( new ColladaLoader());
#endif
#if (!defined ASSIMP_BUILD_NO_TERRAGEN_IMPORTER)
    out.push_back( new TerragenImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_CSM_IMPORTER)
    out.push_back( new CSMImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_3D_IMPORTER)
    out.push_back( new UnrealImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_LWS_IMPORTER)
    out.push_back( new LWSImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_OGRE_IMPORTER)
    out.push_back( new Ogre::OgreImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_OPENGEX_IMPORTER )
    out.push_back( new OpenGEX::OpenGEXImporter() );
#endif
#if (!defined ASSIMP_BUILD_NO_MS3D_IMPORTER)
    out.push_back( new MS3DImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_COB_IMPORTER)
    out.push_back( new COBImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_BLEND_IMPORTER)
    out.push_back( new BlenderImporter());
#endif
#if (!defined ASSIMP_BUILD_NO_Q3BSP_IMPORTER)
    out.push_back( new Q3BSPFileImporter() );
#endif
#if (!defined ASSIMP_BUILD_NO_NDO_IMPORTER)
    out.push_back( new NDOImporter() );
#endif
#if (!defined ASSIMP_BUILD_NO_IFC_IMPORTER)
    out.push_back( new IFCImporter() );
#endif
#if ( !defined ASSIMP_BUILD_NO_XGL_IMPORTER )
    out.push_back( new XGLImporter() );
#endif
#if ( !defined ASSIMP_BUILD_NO_FBX_IMPORTER )
    out.push_back( new FBXImporter() );
#endif
#if ( !defined ASSIMP_BUILD_NO_ASSBIN_IMPORTER )
    out.push_back( new AssbinImporter() );
#endif
#if ( !defined ASSIMP_BUILD_NO_GLTF_IMPORTER )
    out.push_back( new glTFImporter() );
    out.push_back( new glTF2Importer() );
#endif
#if ( !defined ASSIMP_BUILD_NO_C4D_IMPORTER )
    out.push_back( new C4DImporter() );
#endif
#if ( !defined ASSIMP_BUILD_NO_3MF_IMPORTER )
    out.push_back( new D3MFImporter() );
#endif
#ifndef ASSIMP_BUILD_NO_X3D_IMPORTER
    out.push_back( new X3DImporter() );
#endif
#ifndef ASSIMP_BUILD_NO_MMD_IMPORTER
    out.push_back( new MMDImporter() );
#endif
#ifndef ASSIMP_BUILD_NO_STEP_IMPORTER
    out.push_back(new StepFile::StepFileImporter());
#endif
}

/** will delete all registered importers. */
void DeleteImporterInstanceList(std::vector< BaseImporter* >& deleteList){
	for(size_t i= 0; i<deleteList.size();++i){
		delete deleteList[i];
		deleteList[i]=nullptr;
	}//for
}

} // namespace Assimp
