#version 330
// Inputs
in vec3 world_position;
// Outputs
out vec4 color;
out vec4 shadow;
out vec4 normal;
// Uniforms
uniform samplerCube sky_box;

uniform int tick;
uniform sampler2D blue_noise;
uniform sampler2D palette;
// Transformation
uniform mat4 model_view_matrix_inv;

uniform sampler3D voxels[16];// voxels with lod levels
uniform ivec3 grid_size;// size of voxel grid on 0-LOD
uniform int max_lod = 2;

// Constants
const float VOXEL_SIZE = 0.1;
const int LOD_POWER = 1;// power of 2 of compression factor for LODs
const int LOD_STEP = 1 << LOD_POWER;// (2^LOD_POWER) compression factor for LODs

const int LOD_SCALE[10] = int[10](// precomputed lod scales
1 << (LOD_POWER * 0),
1 << (LOD_POWER * 1),
1 << (LOD_POWER * 2),
1 << (LOD_POWER * 3),
1 << (LOD_POWER * 4),
1 << (LOD_POWER * 5),
1 << (LOD_POWER * 6),
1 << (LOD_POWER * 7),
1 << (LOD_POWER * 8),
1 << (LOD_POWER * 9)
);

const vec3 GLOBAL_LIGHT = normalize(vec3(0.4, -1.0, +0.7));

ivec3 AXIS_IVERSED;// Contains 1, if axis is swapped for current ray cast, and 0 otherwise

int getVoxel(ivec3 cell) {
    ivec3 uv = cell * (1 - AXIS_IVERSED) + ((grid_size - cell) - 1) * AXIS_IVERSED;
    return int(texelFetch(voxels[0], uv, 0).r * 255 + 1e-3);
}

bool hasVoxel(int level, ivec3 cell) {
    ivec3 uv = (cell * (1 - AXIS_IVERSED) + ((grid_size - cell) - 1) * AXIS_IVERSED) >> (LOD_POWER * level);
    return texelFetch(voxels[level], uv, 0).r > 0;
}

vec4 computeAo(ivec3 cell, ivec3 dir1, ivec3 dir2) {
    float side1 = float(hasVoxel(0, cell + dir1));
    float side2 = float(hasVoxel(0, cell + dir2));
    float corner = float(hasVoxel(0, cell + dir1 + dir2));
    return 1 - vec4(0.0, side1 * 0.5, (side1 + side2 + max(side1*side2, corner)) * 0.25, side2 * 0.5);
}

int lod_mask(int lod) {
    return ~(LOD_SCALE[lod] - 1);
}

bool can_lod_up(ivec3 cell, int lod) {
    return lod < max_lod && any(((cell & lod_mask(lod)) == cell));
}

const float g = 1.22074408460575947536;
const vec3 G = vec3(1/g, 1/(g*g), 1/(g*g*g));

vec3 getNoise(int offset) {
    return mod(texelFetch(blue_noise, ivec2(mod(gl_FragCoord.xy, 1024)), 0).rgb + G * (tick + offset), 1);
}

void main()
{
    vec3 ray_origin = (model_view_matrix_inv * vec4(world_position, 1)).xyz;
    vec3 eye_position = (model_view_matrix_inv * vec4(0, 0, 0, 1)).xyz;
    float initial_depth = length(ray_origin - eye_position);

    if (all(greaterThanEqual(eye_position, vec3(0))) && all(lessThanEqual(eye_position, grid_size * VOXEL_SIZE))) {
        ray_origin = eye_position;
        initial_depth = 0;
    }
    vec3 ray_direction = (model_view_matrix_inv * vec4(normalize(world_position), 0)).xyz;

    ray_origin /= VOXEL_SIZE;
    ray_origin += ray_direction * 1e-3;

    // ===== RAY TRACING IN VOXEL SPACE =====

    // Inversing everything so that ray goes only in positive direction (simplifies some formulas)
    ivec3 signs = ivec3(sign(ray_direction));
    AXIS_IVERSED = (1 - signs) >> 1;
    ray_origin = grid_size*0.5 + (ray_origin - grid_size * 0.5) * signs;
    ray_direction = abs(ray_direction);

    vec3 ray_direction_inversed = 1.0f / ray_direction;

    ivec3 cell = ivec3(floor(ray_origin));

    int lod = max_lod;

    int voxel = 0;
    vec3 hit = vec3(0);// time when we can hit a plane (Y-0-Z, X-0-Z, Y-0-X planes)
    ivec3 mask = ivec3(step(ray_origin.xyz, ray_origin.yzx) * step(ray_origin.xyz, ray_origin.zxy));


    vec3 shifted_ray_origin = ray_origin;// ray origin is shifted each step to reduce floating point errors

    for (int iteration = 0; iteration < 512; iteration++) {
        if (any(greaterThanEqual(cell, grid_size))) {
            break;
        }

        while (lod > 0 && hasVoxel(lod, cell)) {
            lod--;
        }

        if (lod == 0 && (voxel = getVoxel(cell)) > 0) break;

        //hit = ((cell & ~(1 << (LEVEL_POWER * level) - 1)) - pos + power[level]) * time_per_step;
        //hit0 = ;
        //hit0 = ((cell - shifted_ray_origin) + 1) * time_per_step;
        //ivec3 s = max(ivec3(floor(1 + 1e-8 + (dot(hit, mask) - hit0)/time_per_step)), ivec3(0));// + mask;

        hit = ((cell & lod_mask(lod)) - shifted_ray_origin + LOD_SCALE[lod]) * ray_direction_inversed;
        mask = ivec3(step(hit.xyz, hit.yzx) * step(hit.xyz, hit.zxy));
        shifted_ray_origin += dot(mask, hit) * ray_direction;

        cell = ivec3(shifted_ray_origin);
        //if (can_lod_up(cell, lod)) lod++;
        ivec3 c = mask * cell;
        int s = c.x + c.y + c.z;
        lod += (~(s >> lod))&1;
        lod += (~(s >> lod))&1;
        //if ((int(dot(mask, cell)) & (1 << lod)) == 0) lod++;
        //lod++;

        /*level = 0;
        for(;level + 1 < NUM_LEVELS && position[level + 1] != position[level] / LEVEL_STEP; level++) {
            position[level + 1] = position[level] / LEVEL_STEP;
        }*/
    }

    vec3 endpos = grid_size*0.5 + (shifted_ray_origin - grid_size * 0.5) * signs;

    if (voxel == 0) {
        color = vec4(0, 0, 0, 1);
        shadow = vec4(1, 1, 1, 1);
        normal = vec4(0, 0, 0, 0);
        gl_FragDepth = 0.999999;
        return;
    }

    vec3 n = -mask * signs;
    vec3 r = ray_direction * signs;
    vec3 d = r - 2 * n * dot(n, r);
    float spec = pow(max(dot(d, -GLOBAL_LIGHT), 0), 8);
    normal = vec4((n + 1) * 0.5, dot(n, endpos*VOXEL_SIZE - eye_position) / 1024.0);


    // Approximate ambient occlusion
    vec2 uv = mod(vec2(dot(mask * shifted_ray_origin.yzx, vec3(1.0)), dot(mask * shifted_ray_origin.zxy, vec3(1.0))), vec2(1.0));
    vec4 ambient = computeAo(cell - mask, mask.zxy * ivec3(step(0.5, uv.x) * 2 - 1), mask.yzx * ivec3(step(0.5, uv.y) * 2 - 1));
    vec2 corner_uv = abs(uv - 0.5) * 2;
    uv *= vec2(dot(mask, signs.yzx), dot(mask, signs.zxy));
    float interpAo = mix(mix(ambient.x, ambient.y, corner_uv.x), mix(ambient.w, ambient.z, corner_uv.x), corner_uv.y);
    interpAo = pow(interpAo, 1.0 / 2.0);

    float intensity = 0.7 + max(0, interpAo * 0.30);
    intensity = intensity * (0.9 + 0.1 * max(0.0, dot(mask*signs, GLOBAL_LIGHT))) + spec * 0.2;
    gl_FragDepth = (initial_depth + dot(shifted_ray_origin - ray_origin, ray_direction) * VOXEL_SIZE) / 1000;

    // ==== SHADING ====
    vec3 saved = grid_size*0.5 + (shifted_ray_origin - grid_size * 0.5) * signs;
    int shadows = 0;
    /*int casts = 1;
    for (int c = 0; c < casts; c++) {
        ray_direction = normalize(-GLOBAL_LIGHT * 6 + (getNoise(c)-0.5));
        shifted_ray_origin = saved + ray_direction * 1e-8;

        signs = ivec3(sign(ray_direction));
        AXIS_IVERSED = (1 - signs) >> 1;
        shifted_ray_origin = grid_size*0.5 + (shifted_ray_origin - grid_size * 0.5) * signs;
        ray_direction = abs(ray_direction);
        ray_direction_inversed = 1.0f / ray_direction;
        cell = ivec3(floor(shifted_ray_origin));
        mask = ivec3(0);

        bool shaded = false;

        vec3 time = (cell - shifted_ray_origin + 1) * ray_direction_inversed;
        for (int iteration = 0; iteration < 32; iteration++) {
            if (any(greaterThanEqual(cell, grid_size))) break;
            if (shaded = hasVoxel(0, cell)) break;

            cell += mask;
            time += vec3(mask) * ray_direction_inversed;
            mask = ivec3(step(time.xyz, time.yzx) * step(time.xyz, time.zxy));
        }

        shifted_ray_origin += ray_direction * dot(time, mask);

        float step = getNoise(3 + c).x / 5 + 0.3;

        for (int iteration = 0; iteration < 128; iteration++) {
            if (any(greaterThanEqual(cell, grid_size))) break;
            if (shaded = hasVoxel(0, cell)) break;
            shifted_ray_origin += step * ray_direction;
            cell = ivec3(shifted_ray_origin);
        }

        if (shaded) shadows++;
    }*/

    ivec2 p = ivec2(voxel-1, 0);
    color = vec4(texelFetch(palette, p, 0).rgb * pow(intensity, 2.2), 1);
    shadow = vec4(vec3(1) * (1.0 - shadows*0.5/1), 1);
}
