
-- depth prepass render stage
local depth_prepass = {
    kind = "geometry",
    output = {
        depth = true
    }
}

local render_stages = {

}

set_stages( render_stages )
