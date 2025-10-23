// Copyright 2024 Accenture.

#ifndef B0971672_51A7_41D7_8D9D_08F811278086
#define B0971672_51A7_41D7_8D9D_08F811278086


#include <config/ConfigIds.h>

namespace config
{

class BspSystem
: public ComponentBase<
      ScopeType,
      CtxId<Ctx::BSP>>
{
public:
    void init();
    void start();
    void stop();
};

} // namespace config


#endif /* B0971672_51A7_41D7_8D9D_08F811278086 */
