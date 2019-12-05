from math import ceil
from random import randint
from constants import *
from resource import *
from task import *

def floor_to_base(val, base):
    return val - val % base

class Scheduler:
    resource = None
    awaiting_times = []

    def __init__(self, tasks_amount, arrival_rate, max_runtime):
        self.resource = Resource(tasks_amount)

        timeline_stock = tasks_amount * max_runtime

        self.tasks_amount = tasks_amount
        self.arrival_rate = arrival_rate
        self.max_runtime = max_runtime

        self.units_count = ceil(tasks_amount / arrival_rate)
        self.beats_count = self.units_count * TIME_UNIT + timeline_stock

        self.timeline = [[] for _ in range(self.beats_count)]
        self.queues = [[] for _ in range(MAX_QUEUES_COUNT)]

    def init_timeline(self):
        for unit_number in range(self.units_count):
            tasks_remaining = self.tasks_amount - unit_number * self.arrival_rate
            unit_tasks_amount = self.arrival_rate \
                if tasks_remaining > self.arrival_rate \
                else tasks_remaining

            for _ in range(unit_tasks_amount):
                task = Task(self.max_runtime)
                beat_number = task.arrival + unit_number * TIME_UNIT
                beat_tasks = self.timeline[beat_number]
                beat_tasks.append(task)

    def assign_task_to_queue(self, task):
        try:
            self.queues[task.priority - 1].append(task)
        except IndexError:
            self.queues.append([task])

    def pick_task(self):
        for queue_number, queue in enumerate(self.queues):
            if len(queue):
                return queue.pop(0)

    def process_beat(self, beat_tasks):
        task = self.resource.make_beat()

        # if there is a task that has not been finished within desired quantum
        #
        if task and task.done == False:
            self.assign_task_to_queue(task)
        elif task and task.done:
            self.awaiting_times.append(task.awaiting_time)

        # Assign beat tasks to queues according to Corbato algorithm
        #
        for task in beat_tasks:
#             # If there is more prior task - interrupt execution
#             #
#             if self.resource.is_busy() and task.priority > self.resource.task.priority:
#                 interrupted_task = self.resource.interrupt(task)
#                 # put interrupted_task back to queue
#                 #
#                 self.assign_task_to_queue(interrupted_task)
#             # put task to its queue
#             #
#             else:
                self.assign_task_to_queue(task)

        # if resource is free, take task from the most prior queue and execute
        #
        if (self.resource.is_free()):
            task = self.pick_task()
            if task:
                self.resource.execute(task)

    def start(self):
        for beat_tasks in self.timeline:
            self.process_beat(beat_tasks)
            self.increment_awaiting_time()

    def increment_awaiting_time(self):
        for queue in self.queues:
            for task in queue:
                task.awaiting_time += 1
