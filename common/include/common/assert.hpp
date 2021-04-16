#include <iostream>

#define ENGINE_ASSERT(exp, msg) \
if (!(exp)) std::cerr << "[ENGINE ASSERT]" << msg << \
":" << __FILE__ << ":" << __LINE__ << std::endl

#define ENSURE_THAT(constraint) \
if (!(constraint)) std::cout << "[Constraint violated] \"" << #constraint << "\" is false" << std::endl << \
"Location: " << __FILE__ << ":" << __LINE__ << std::endl << std::endl;

#define ENSURE_THAT_EQ(expr, val) \
{ auto expr_res = expr; auto val_res = val;\
if (expr_res != val_res) std::cout << "[Constraint violated] " << #expr << " != " << #val << std::endl << \
<< "Values are " << expr_res << " and " << val_res << std::endl <<\
"Location: " << __FILE__ << ":" << __LINE__ << std::endl << std::endl;}

#define ENSURE_THAT_BETWEEN(expr, val_low, val_high) \
{ auto expr_res = (expr); auto val_low_res = (val_low); auto val_high_res = (val_high);\
if (expr_res < val_low_res || expr_res > val_high_res) \
std::cout << "[Constraint violated] " << #expr << " is out of range [" \
<< #val_low << ", " << #val_high << "]" << std::endl \
<< "Values are " << expr_res << ", " << val_low_res << " and " << val_high_res << std::endl <<\
"Location: " << __FILE__ << ":" << __LINE__ << std::endl << std::endl;}
