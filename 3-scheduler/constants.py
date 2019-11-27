from math import ceil, log2

TIME_UNIT = 10 # 10 beats per one time unit

MAX_TASK_LENGTH = 1000
BYTES_TRANSFER_AMOUNT = 100 # 10 bytes can be transfered between RAM
                           # and external memory per one TIME_UNIT

MAX_QUEUES_COUNT = ceil(log2(MAX_TASK_LENGTH / BYTES_TRANSFER_AMOUNT + 1))

FLOOR_BASE = 20

