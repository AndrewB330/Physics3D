#version 450

const int LOD_SCALE[10] = int[10](1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9);

layout (location = 0) in vec3 worldPosition_i;
layout (location = 0) out vec4 color_o;

struct VoxelObjectData {
    float voxelSize;
    ivec3 dims;
    int maxLod;
    ivec3 lodOffsets[10];
    ivec3 dataOffset;

    mat4 transform;
    mat4 transformInv;
};

uniform VoxelObjectData currentObject;
uniform VoxelObjectData globalShadow;

uniform usampler3D dataTexture;
uniform mat4 objectToShadow;

uniform int index;

uniform mat4 gl_ModelViewMatrixInverse;

vec3 LIGHT = normalize(vec3(0.8,0.6,0.65));

ivec3 AXIS_INVERSED; // Contains 1, if axis is swapped for current ray cast, and 0 otherwise

bool HasVoxel(VoxelObjectData object, int level, ivec3 cell) {
    ivec3 uv = ((cell * (1 - AXIS_INVERSED) + ((object.dims - 1) - cell) * AXIS_INVERSED) >> level);
    int bit = (uv.x & 1) | ((uv.y & 1) << 1) | ((uv.z & 1) << 2);
    return bool((texelFetch(dataTexture, (uv >> 1) + object.lodOffsets[level], 0).r >> bit) & 1);
}

bool HasVoxelFast(VoxelObjectData object, int level, ivec3 cell) {
    ivec3 uv = ((cell * (1 - AXIS_INVERSED) + ((object.dims - 1) - cell) * AXIS_INVERSED) >> level);
    return texelFetch(dataTexture, uv + object.lodOffsets[level - 1], 0).r > 0;
}

uint GetVoxel(VoxelObjectData object, ivec3 cell) {
    ivec3 uv = cell * (1 - AXIS_INVERSED) + ((object.dims - cell) - 1) * AXIS_INVERSED;
    return texelFetch(dataTexture, uv + object.dataOffset, 0).r;
}

bool IsInside(VoxelObjectData object, ivec3 cell) {
    return all(greaterThanEqual(cell, ivec3(0))) && all(lessThan(cell, object.dims));
}

int LodMask(int lod) {
    return ~(LOD_SCALE[lod] - 1);
}

bool CanLodUp(ivec3 cell, int lod, int maxLod) {
    return lod < maxLod && any(bvec3((cell & LodMask(lod)) == cell));
}

vec4 ComputeAO(VoxelObjectData object, ivec3 cell, ivec3 dir1, ivec3 dir2) {
    float side1 = float(IsInside(object, cell + dir1) && HasVoxel(object, 0, cell + dir1));
    float side2 = float(IsInside(object, cell + dir2) && HasVoxel(object, 0, cell + dir2));
    float corner = float(IsInside(object, cell + dir1 + dir2) && HasVoxel(object, 0, cell + dir1 + dir2));
    return 1 - vec4(0.0, side1 * 0.5, (side1 + side2 + max(side1*side2, corner)) * 0.25, side2 * 0.5);
}

struct HitResult {
    bool hit; // hit ?
    vec3 pos; // hit position
    ivec3 cell; // hit voxel coordinates
    float depth; // distance from origin to hit
    ivec3 side; // side of hit ((1,0,0) from X axis side, (0,1,0) from Y axis side, (0,0,1) from Z axis side)
};

HitResult RayTrace(VoxelObjectData object, vec3 ray_origin, vec3 ray_direction, bool data, int max_iterations) {
    // ray_direction should be positive, inverse axes if needed
    ivec3 signs = ivec3(sign(ray_direction + 1e-6));
    AXIS_INVERSED = (1 - signs) >> 1;
    ray_origin = object.dims * 0.5 + (ray_origin - object.dims * 0.5) * signs;
    ray_direction = normalize(abs(ray_direction) + 1e-6);

    vec3 ray_direction_inversed = 1.0f / ray_direction; // to speed up division
    vec3 time = vec3(0); // time when we can hit a plane (Y-0-Z, X-0-Z, Y-0-X planes)
    vec3 shifted_ray_origin = ray_origin; // ray origin is shifted each step to reduce floating point errors
    ivec3 cell = ivec3(floor(ray_origin));
    int lod = object.maxLod;
    bool hit = false;

    int iteration = 0;
    for (; iteration < max_iterations && all(lessThan(cell, object.dims)); iteration++) {

        while (lod > 0 && HasVoxelFast(object, lod, cell)) lod--;
        if (lod == 0 && (hit = HasVoxel(object, lod, cell))) break;

        time = ((cell | (LOD_SCALE[lod] - 1)) + 1 - shifted_ray_origin) * ray_direction_inversed;
        shifted_ray_origin += min(time.x, min(time.y, time.z)) * ray_direction;

        cell = ivec3(shifted_ray_origin);

        if (CanLodUp(cell, lod, object.maxLod)) lod++;
    }

    HitResult res;
    res.hit = hit;
    res.cell = cell;
    res.depth = dot(shifted_ray_origin - ray_origin, ray_direction);
    res.pos = shifted_ray_origin;
    res.side = iteration > 0 ?
        ivec3(step(time.xyz, time.yzx) * step(time.xyz, time.zxy)) :
        ivec3(step(ray_origin.xyz, ray_origin.yzx) * step(ray_origin.xyz, ray_origin.zxy));
    return res;
}

float ComputeAO(VoxelObjectData object, HitResult res) {
    vec3 pos = res.pos;
    ivec3 mask = res.side;
    ivec3 cell = res.cell;

    vec2 uv = mod(vec2(dot(mask * pos.yzx, vec3(1.0)), dot(mask * pos.zxy, vec3(1.0))), vec2(1.0));
    vec4 ambient = ComputeAO(object, cell - mask, mask.zxy * ivec3(step(0.5, uv.x) * 2 - 1), mask.yzx * ivec3(step(0.5, uv.y) * 2 - 1));
    vec2 corner_uv = abs(uv - 0.5) * 2;
    float interpAo = mix(mix(ambient.x, ambient.y, corner_uv.x), mix(ambient.w, ambient.z, corner_uv.x), corner_uv.y);
    return pow(interpAo, 0.5);
}

void main() {
    vec3 eye_position = (gl_ModelViewMatrixInverse * vec4(0, 0, 0, 1)).xyz;
    vec3 ray_origin = worldPosition_i;
    vec3 ray_direction = normalize(worldPosition_i - eye_position);

    float initial_depth = length(ray_origin - eye_position);

    if (all(greaterThanEqual(eye_position, vec3(-0.5))) && all(lessThanEqual(eye_position, currentObject.dims + vec3(0.5)))) {
        ray_origin = eye_position;
        initial_depth = 0;
    } else {
        ray_origin += ray_direction * 1e-3;// move ray origin a bit forward
    }

    HitResult hit = RayTrace(currentObject, ray_origin, ray_direction, true, 256);

    if (!hit.hit) {
        gl_FragDepth = 0.999999;
        color_o = vec4(0);
        return;
    }

    // Voxel based ambient occlusion
    float ambientOcclusion = ComputeAO(currentObject, hit);

    vec3 hitPos = currentObject.dims * 0.5 + (hit.pos - hit.side * 1e-3 - currentObject.dims * 0.5) * (1 - AXIS_INVERSED * 2);

    //HitResult shadow = RayTrace(globalShadow,  + lightDirection * 1e-6, lightDirection, false);

    HitResult shadowShort = RayTrace(currentObject, hitPos, LIGHT, false, 5);
    bool shaded = false;
    if (!shaded) {
        hitPos = currentObject.dims * 0.5 + (shadowShort.pos - currentObject.dims * 0.5) * (1 - AXIS_INVERSED * 2);
        HitResult shadow = RayTrace(globalShadow, (globalShadow.transformInv * (currentObject.transform * vec4(hitPos, 1))).xyz, LIGHT, false, 256);
        shaded = shadow.hit;
    }

    gl_FragDepth = (initial_depth + hit.depth) * currentObject.voxelSize / 1000;

    float intensity = (ambientOcclusion * 0.5 + 0.5) * (shaded ? 0.5 : 1.0);
    vec3 color = (index > 0 ? vec3(0.8,0.5,0.5) : vec3(1,1,1));
    color_o = vec4(color * intensity, 1);
}
