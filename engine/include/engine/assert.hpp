#define ENGINE_ASSERT(exp, msg) \
if (!(exp)) std::cerr << "[ENGINE ASSERT]" << msg << \
":" << __FILE__ << ":" << __LINE__ << std::endl
