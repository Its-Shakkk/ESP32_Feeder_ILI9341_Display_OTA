#ifndef ROLLBACK_MANAGER_H
#define ROLLBACK_MANAGER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

void rollback_manager_startup(void);
void rollback_manager_confirm_image(void);
void rollback_manager_force_rollback(void);
bool rollback_manager_is_pending_verify(void);

void rollback_manager_handle_failed_image(void);

#ifdef __cplusplus
}
#endif

#endif