
local rendertarget = {
    name = "Deferred",
    fbs = {
        { 
            name="gbuffer_albedo",
            format="R8G8B8A8_UNORM"
        },
		{
            name="gbuffer_normal",
            format="R8G8_UNORM"
        },
		{
            name="gbuffer_roughness_metallic",
            format="R16G16_FLOAT"
        }
    }
}

local simple_layer_config = {
    { -- Populate gbuffers
        name = "gbuffer",
        render_targets="gbuffer0 gbuffer1",
        depth_stencil_target="ds_buffer",
        sort="FRONT_BACK",
        profiling_scope="gbuffer"
    },
    { -- Kick resource generator 'linearize_depth'
        name = "linearize_depth",
        resource_generator = "linearize_depth",
        profiling_scope="lighting&shadows" 
    },
    { -- Render decals affecting albedo term
        name = "decal_albedo",
        render_targets="gbuffer0",
        depth_stencil_target="ds_buffer",
        sort="BACK_FRONT",
        profiling_scope="decals"
    },
    { -- Kick resource generator 'deferred_shading'
        name = "deferred_shading", 
        resource_generator = "deferred_shading",
        profiling_scope="lighting&shadows" 
    }
}

begin_renderer()
set_rendertargets( rendertargets )
-- ...
end_renderer()