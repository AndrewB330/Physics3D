#include <common/logging.hpp>
#include <common/bit_utils.hpp>
#include "voxel_texture_manager.hpp"


void VoxelTextureManager::RegisterVoxelObject(const VoxelObject &object) {
    if (lod_offsets.find(object.GetId()) != lod_offsets.end()) {
        return;
    }

    auto &offsets = lod_offsets[object.GetId()];

    if (object.HasData()) {
        data_offset[object.GetId()] = allocator.Allocate(object.GetDims());
        UpdateTexture(data_offset[object.GetId()], object.GetDims(), object.GetDataPointer());
    }

    for (int i = 0; i < object.GetFirstFullLod(); i++) {
        offsets.push_back(allocator.Allocate(object.GetLodDims(i)));
        UpdateTexture(offsets.back(), object.GetLodDims(i), object.GetLodDataPointer(i));
    }
}

void VoxelTextureManager::Init() {

    glGenTextures(1, &gl_texture_id);
    glActiveTexture(GL_TEXTURE0 + VOXEL_TEXTURE);
    glBindTexture(GL_TEXTURE_3D, gl_texture_id);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8UI, TEXTURE_DIMS.x, TEXTURE_DIMS.y, TEXTURE_DIMS.z, 0, GL_RED_INTEGER,
                 GL_UNSIGNED_BYTE,
                 nullptr);
    glActiveTexture(GL_TEXTURE0);
}

void VoxelTextureManager::UpdateTexture(const Vec3i &offset, Vec3i dims, const uint8_t *data) {
    glTextureSubImage3D(gl_texture_id, 0,
                        offset.x, offset.y, offset.z,
                        dims.x, dims.y, dims.z, GL_RED_INTEGER,
                        GL_UNSIGNED_BYTE, data);
}

std::vector<Vec3i> VoxelTextureManager::GetLodOffsets(int object_id) {
    return lod_offsets[object_id];
}

Vec3i VoxelTextureManager::GetDataOffset(int object_id) {
    return data_offset[object_id];
}

VoxelTextureManager::VoxelTextureManager() : allocator(TEXTURE_DIMS) {}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message );
}

void VoxelTextureManager::check() {
// During init, enable debug output
    glEnable ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

    /*std::vector<uint8_t> data(256*256*256);
    glGetTexImage(gl_texture_id, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, data.data());
    int x = 0;
    int y = 0;
    int z = 0;
    auto v = glGetError();
    auto val = data[x + y * 256 + z * 256 * 256];*/
}

TextureAllocator::TextureAllocator(const Vec3i &texture_dims, int additional_layers)
        :
        w(texture_dims.x), h(texture_dims.y), d(texture_dims.z),
        additional_h(additional_layers + texture_dims.y % CHUNK_SIZE),
        chunk_used((texture_dims - Vec3i(0, additional_h, 0)) / CHUNK_SIZE),
        voxel_used(Vec3i(w, additional_h, d)) {}

Vec3i TextureAllocator::Allocate(const Vec3i &dims) {
    if (dims.x <= CHUNK_SIZE && dims.y <= CHUNK_SIZE && dims.z <= CHUNK_SIZE &&
        (dims.x * dims.y * dims.z <= CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 0.5)) {
        for (int64_t i = 0; i < w - dims.x + 1; i++) {
            for (int64_t j = 0; j < additional_h - dims.y + 1; j++) {
                for (int64_t k = 0; k < d - dims.z + 1; k++) {
                    Vec3i offset = Vec3i(i, j, k);
                    Vec3i real_offset = Vec3i(i, j + (h - additional_h), k);
                    if (voxel_used.GetRange(offset, offset + dims) == 0) {
                        voxel_used.AddRange(offset, offset + dims, 1);
                        allocated[real_offset] = dims;
                        return real_offset;
                    }
                }
            }
        }
    }
    Vec3i chunk_dims = (dims + Vec3i(1, 1, 1) * (CHUNK_SIZE - 1)) / CHUNK_SIZE;
    for (int64_t i = 0; i < w / CHUNK_SIZE - chunk_dims.x + 1; i++) {
        for (int64_t j = 0; j < (h - additional_h) / CHUNK_SIZE - chunk_dims.y + 1; j++) {
            for (int64_t k = 0; k < d / CHUNK_SIZE - chunk_dims.z + 1; k++) {
                Vec3i offset = Vec3i(i, j, k);
                Vec3i real_offset = offset * CHUNK_SIZE;
                if (chunk_used.GetRange(offset, offset + chunk_dims) == 0) {
                    chunk_used.AddRange(offset, offset + chunk_dims, 1);
                    allocated[real_offset] = chunk_dims;
                    return real_offset;
                }
            }
        }
    }
    Logger::LogError("Allocation error! Out of memory.");
    return Vec3i();
}

void TextureAllocator::Free(const Vec3i &offset) {
    auto it = allocated.find(offset);
    if (it == allocated.end()) {
        return;
    }
    Vec3i dims = it->second;
    allocated.erase(it);
    if (offset.y >= h - additional_h) {
        Vec3i from = offset - Vec3i(0, h - additional_h, 0);
        voxel_used.AddRange(from, from + dims, -1);
    } else {
        Vec3i from = offset / CHUNK_SIZE;
        chunk_used.AddRange(from, from + dims, -1);
    }
}
