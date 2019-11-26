#include <lcom/lcf.h>
#include "interrupts.h"

bool notification_origin(message msg, enum dispositive d){
   if(!is_ipc_notify(ipc_status)) return false;
   if(!(_ENDPOINT_P(msg.m_source) == HARDWARE)) return false;
   switch (d) {
     case MOUSE:
        return msg.m_notify.interrupts & mouse_irq_set;
     case KEYBOARD:
        return msg.m_notify.interrupts & keyboard_irq_set;
     case TIMER:
        return msg.m_notify.interrupts & timer_irq_set;
   }
   return false;
 }
