#include <string.h>
#include "check.h"  
#include "../../handler.c"

START_TEST(tc_handler)
{
    fail_if(1 == 0);
}
END_TEST

Suite *handler_suite(void)
{
    Suite *s = suite_create(" handler");

    TCase *TCase_handler = tcase_create("tc 1");
    tcase_add_test(TCase_handler, tc_handler);
    suite_add_tcase(s, TCase_handler);    

    return s;
}

