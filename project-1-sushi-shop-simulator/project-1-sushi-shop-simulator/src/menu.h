#ifndef __MENU_H__
#define __MENU_H__


/**
 * @brief Pratos do Sushi Shop.
*/
enum menu_item {
    Sushi=0,
    Dango=1,
    Ramen=2,
    Onigiri=3,
    Tofu=4,
};

#define SUSHI                    "🍣"
#define DANGO                    "🍡"
#define RAMEN                    "🍜"
#define ONIGIRI                  "🍙"
#define TOFU                     "🧈"

#define SUSHI_PREP_TIME          60000
#define DANGO_PREP_TIME          60000
#define RAMEN_PREP_TIME          60000
#define ONIGIRI_PREP_TIME        60000
#define TOFU_PREP_TIME           60000

#endif  //__MENU_H__
