#ifndef SHIELD_TYPES_H
#define SHIELD_TYPES_H

const time_t kIntervalOfSendinToEquip (1);

typedef enum  {
    UPPER   = 0,
    LOWER_B = 1,
    LOWER_A = 2
} FoldDscr_type;


typedef enum  {
    BUZ  = 0,
    AUGS = 1,
    PMU  = 2,
    KEGP = 3
} LinkPoint_type;

#endif // SHIELD_TYPES_H
