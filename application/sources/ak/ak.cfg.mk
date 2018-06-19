#sizeof common message pool.
COMMON_MSG_POOL_SIZE = -DAK_COMMON_MSG_POOL_SIZE=8

#sizeof data common message.
COMMON_MSG_DATA_SIZE = -DAK_COMMON_MSG_DATA_SIZE=64

#sizeof pure message pool.
PURE_MSG_POOL_SIZE = -DAK_PURE_MSG_POOL_SIZE=64

#sizeof dynamic message pool.
DYNAMIC_MSG_POOL_SIZE = -DAK_DYNAMIC_MSG_POOL_SIZE=8

#numbers of dynamic pool data unit.
DYNAMIC_DATA_POOL_SIZE = -DAK_DYNAMIC_DATA_POOL_SIZE=128

#sizeof dynamic pool data unit.
DYNAMIC_PDU_SIZE = -DAK_DYNAMIC_PDU_SIZE=4

#sizeof timer pool.
TIMER_POOL_SIZE = -DAK_TIMER_POOL_SIZE=16

#task objects log queue enable
TASK_OBJ_LOG_ENABLE = -DAK_TASK_OBJ_LOG_ENABLE

#irq objects log queue enable
IRQ_OBJ_LOG_ENABLE = -DAK_IRQ_OBJ_LOG_ENABLE

CFLAGS += \
	$(COMMON_MSG_POOL_SIZE) \
	$(COMMON_MSG_DATA_SIZE) \
	$(PURE_MSG_POOL_SIZE) \
	$(DYNAMIC_MSG_POOL_SIZE) \
	$(DYNAMIC_DATA_POOL_SIZE) \
	$(DYNAMIC_PDU_SIZE) \
	$(TIMER_POOL_SIZE) \
	$(TASK_OBJ_LOG_ENABLE) \
	$(IRQ_OBJ_LOG_ENABLE) \
