
//
// This source file is part of appleseed.
// Visit https://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2013 Francois Beaune, Jupiter Jazz Limited
// Copyright (c) 2014-2018 Francois Beaune, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

// appleseed.renderer headers.
#include "renderer/modeling/surfaceshader/isurfaceshaderfactory.h"
#include "renderer/modeling/surfaceshader/surfaceshader.h"

// appleseed.foundation headers.
#include "foundation/platform/compiler.h"
#include "foundation/utility/autoreleaseptr.h"
#include "foundation/utility/kvpair.h"

// appleseed.main headers.
#include "main/dllsymbol.h"

// Standard headers.
#include <cstddef>

// Forward declarations.
namespace foundation    { class Dictionary; }
namespace foundation    { class DictionaryArray; }
namespace renderer      { class AOVAccumulatorContainer; }
namespace renderer      { class ParamArray; }
namespace renderer      { class PixelContext; }
namespace renderer      { class ShadingContext; }
namespace renderer      { class ShadingPoint; }

namespace renderer
{

//
// A shader offering a variety of diagnostic modes.
//

class APPLESEED_DLLSYMBOL DiagnosticSurfaceShader
  : public SurfaceShader
{
  public:
    // Available shading modes.
    enum ShadingMode
    {
        Albedo,                     // shade according to the surface bsdf's albedo component
        Coverage,                   // shade according to pixel coverage
        Barycentric,                // shade according to barycentric coordinates
        UV,                         // shade according to UV coordinates
        Tangent,                    // shade according to the tangent vector
        Bitangent,                  // shade according to the bitangent vector
        GeometricNormal,            // shade according to the geometric normal
        ShadingNormal,              // shade according to the (possibly modified) shading normal
        OriginalShadingNormal,      // shade according to the original shading normal
        WorldSpacePosition,         // shade according to the world space position
        Sides,                      // shade according to the surface side
        Depth,                      // shade according to distance from camera
        ScreenSpaceWireframe,       // screen-space wireframe
        WorldSpaceWireframe,        // world-space wireframe
        AmbientOcclusion,           // ambient occlusion
        AssemblyInstances,          // assign a unique color to each assembly instance
        ObjectInstances,            // assign a unique color to each object instance
        Primitives,                 // assign a unique color to each primitive
        Materials,                  // assign a unique color to each material
        RaySpread,                  // shade according to the reflected ray spread
        FacingRatio,                // shade according to the facing ratio
        ShadingModeCount            // number of shading modes -- keep last
    };

    static const foundation::KeyValuePair<const char*, ShadingMode> ShadingModeValues[];
    static const foundation::KeyValuePair<const char*, const char*> ShadingModeNames[];

    // Constructor.
    DiagnosticSurfaceShader(
        const char*                 name,
        const ParamArray&           params);

    // Delete this instance.
    void release() override;

    // Return a string identifying the model of this surface shader.
    const char* get_model() const override;

    // Evaluate the shading at a given point.
    void evaluate(
        SamplingContext&            sampling_context,
        const PixelContext&         pixel_context,
        const ShadingContext&       shading_context,
        const ShadingPoint&         shading_point,
        AOVAccumulatorContainer&    aov_accumulators,
        ShadingResult&              shading_result) const override;

  private:
    ShadingMode m_shading_mode;
    double      m_ao_max_distance;
    size_t      m_ao_samples;

    void extract_parameters();

    static void set_result(
        const foundation::Color3f&  color,
        ShadingResult&              shading_result);

    static void set_result(
        const foundation::Color4f&  color,
        ShadingResult&              shading_result);

    static void set_result(
        const Spectrum&             value,
        ShadingResult&              shading_result);
};


//
// Diagnostic surface shader factory.
//

class APPLESEED_DLLSYMBOL DiagnosticSurfaceShaderFactory
  : public ISurfaceShaderFactory
{
  public:
    // Delete this instance.
    void release() override;

    // Return a string identifying this surface shader model.
    const char* get_model() const override;

    // Return metadata for this surface shader model.
    foundation::Dictionary get_model_metadata() const override;

    // Return metadata for the inputs of this surface shader model.
    foundation::DictionaryArray get_input_metadata() const override;

    // Create a new surface shader instance.
    foundation::auto_release_ptr<SurfaceShader> create(
        const char*         name,
        const ParamArray&   params) const override;
};

}   // namespace renderer
