from math import ceil
from constants import *
from random import randint

class Task:
    done = False
    awaiting_time = 0

    def __init__(self, max_runtime):
        self.arrival = randint(0, TIME_UNIT - 1)
        self.length = randint(1, MAX_TASK_LENGTH)
        self.runtime = ceil(max_runtime * (self.length / MAX_TASK_LENGTH))
        self.priority = ceil(log2(self.length / BYTES_TRANSFER_AMOUNT + 1))
        self.quantum = (self.priority + 1) * 8 * TIME_UNIT

    def __str__(self):
        done = self.done
        arrival = self.arrival
        runtime = self.runtime
        length = self.length
        priority = self.priority
        return f'{arrival}\t{runtime}\t{length}\t{priority}\t\t{done}'
