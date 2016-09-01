#include "check.h"

/* 
 * To debug segmentation faults, set env var: CK_FORK=no
 *
 * */

Suite *handler_suite(void);

int main(void)
{
    Suite *s;
    SRunner *sr;
    int fails = 0;
    
    s = handler_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    fails += srunner_ntests_failed(sr);
    srunner_free(sr);

    return fails;
}
               



