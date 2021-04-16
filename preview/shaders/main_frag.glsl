#version 450

const int LOD_SCALE[10] = int[10](1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9);

layout (location = 0) in vec3 worldPosition_i;

layout (location = 0) out vec4 color_o;

uniform mat4 modelViewMat_u;

uniform float voxelSize_u;
uniform int maxLod_u;
uniform ivec3 gridDims_u;
uniform usampler3D voxels_u;
uniform ivec3 textureOffsets_u[10];

ivec3 AXIS_IVERSED;// Contains 1, if axis is swapped for current ray cast, and 0 otherwise

bool hasVoxel(int level, ivec3 cell) {
    if (level < 6) {
        level = 6;
    }
    ivec3 uv = ((cell * (1 - AXIS_IVERSED) + ((gridDims_u - cell) - 1) * AXIS_IVERSED) >> level);
    int bit = (uv.x & 1) + ((uv.y & 1) << 1) + ((uv.z & 1)  << 2);
    return bool((texelFetch(voxels_u, (uv >> 1) + textureOffsets_u[level], 0).r >> bit) & 1);
}

uint getVoxel(ivec3 cell) {
    return uint(hasVoxel(6, cell));
    ivec3 uv = cell * (1 - AXIS_IVERSED) + ((gridDims_u - cell) - 1) * AXIS_IVERSED;
    return texelFetch(voxels_u, uv + textureOffsets_u[0], 0).r;
}

int loadMask(int lod) {
    return ~(LOD_SCALE[lod] - 1);
}

bool canLodUp(ivec3 cell, int lod) {
    return lod < maxLod_u && any(bvec3((cell & loadMask(lod)) == cell));
}

bool canLodUp(int cell, int lod) {
    return lod < maxLod_u && (cell & loadMask(lod)) == cell;
}

vec4 computeAo(ivec3 cell, ivec3 dir1, ivec3 dir2) {
    float side1 = float(getVoxel(cell + dir1) > 0);
    float side2 = float(getVoxel(cell + dir2) > 0);
    float corner = float(getVoxel(cell + dir1 + dir2) > 0);
    return 1 - vec4(0.0, side1 * 0.5, (side1 + side2 + max(side1*side2, corner)) * 0.25, side2 * 0.5);
}

void main() {
    vec3 eye_position = inverse(mat3(modelViewMat_u)) * -modelViewMat_u[3].xyz;
    vec3 ray_origin = worldPosition_i;
    vec3 ray_direction = normalize(worldPosition_i - eye_position);

    float initial_depth = length(ray_origin - eye_position);

    if (all(greaterThanEqual(eye_position, vec3(-0.5))) && all(lessThanEqual(eye_position, gridDims_u + vec3(0.5)))) {
        ray_origin = eye_position;
        initial_depth = 0;
    } else {
        ray_origin += ray_direction * 1e-3;// move ray origin a bit forward
    }

    // RAY TRACING START
    ivec3 signs = ivec3(sign(ray_direction));
    AXIS_IVERSED = (1 - signs) >> 1;
    ray_origin = gridDims_u * 0.5 + (ray_origin - gridDims_u * 0.5) * signs;
    ray_direction = normalize(abs(ray_direction) + 1e-6);

    vec3 ray_direction_inversed = 1.0f / ray_direction;

    ivec3 cell = ivec3(floor(ray_origin));

    int lod = maxLod_u;

    uint voxel = 0;
    vec3 hit = vec3(0);// time when we can hit a plane (Y-0-Z, X-0-Z, Y-0-X planes)
    ivec3 mask = ivec3(step(ray_origin.xyz, ray_origin.yzx) * step(ray_origin.xyz, ray_origin.zxy));

    vec3 shifted_ray_origin = ray_origin;// ray origin is shifted each step to reduce floating point errors

    for (int iteration = 0; iteration < 256; iteration++) {
        if (any(greaterThanEqual(cell, gridDims_u))) {
            break;
        }

        while (lod > 0 && hasVoxel(lod, cell)) {
            lod--;
        }

        if (lod == 0 && (voxel = getVoxel(cell)) > 0) break;

        hit = ((cell & loadMask(lod)) - shifted_ray_origin + LOD_SCALE[lod]) * ray_direction_inversed;
        mask = ivec3(step(hit.xyz, hit.yzx) * step(hit.xyz, hit.zxy));
        shifted_ray_origin += dot(mask, hit) * ray_direction;

        cell = ivec3(shifted_ray_origin);

        ivec3 c = mask * cell;
        if (canLodUp(c.x + c.y + c.z, lod)) lod++;
    }

    vec2 uv = mod(vec2(dot(mask * shifted_ray_origin.yzx, vec3(1.0)), dot(mask * shifted_ray_origin.zxy, vec3(1.0))), vec2(1.0));
    vec4 ambient = computeAo(cell - mask, mask.zxy * ivec3(step(0.5, uv.x) * 2 - 1), mask.yzx * ivec3(step(0.5, uv.y) * 2 - 1));
    vec2 corner_uv = abs(uv - 0.5) * 2;
    uv *= vec2(dot(mask, signs.yzx), dot(mask, signs.zxy));
    float interpAo = mix(mix(ambient.x, ambient.y, corner_uv.x), mix(ambient.w, ambient.z, corner_uv.x), corner_uv.y);
    interpAo = pow(interpAo, 1.0 / 2.0);


    if (voxel > 0) {
        gl_FragDepth = (initial_depth + dot(shifted_ray_origin - ray_origin, ray_direction)) * voxelSize_u / 1000;
        color_o = vec4(interpAo * vec3(1) * 0.5 + 0.5, 1);
    } else {
        gl_FragDepth = 0.999999;
        if (texelFetch(voxels_u, textureOffsets_u[6] + ivec3(1, 0, 1), 0).r != 255) {
            color_o = vec4(1, 0, 0, 1);
        } else {
            color_o = vec4(0);
        }
    }
}
