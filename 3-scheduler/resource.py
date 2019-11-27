class Resource:
    busy = 0
    task = None
    free_time = 0
    remaining_tasks_amount = 0

    def __init__(self, tasks_amount):
        self.remaining_tasks_amount = tasks_amount

    def is_free(self):
        return self.task == None

    def is_busy(self):
        return self.task != None

    def init_tasks_amount(self, tasks_amount):
        self.remaining_tasks_amount = tasks_amount

    def execute(self, task):
        self.task = task
        self.busy = task.runtime \
            if task.runtime <= task.quantum \
            else task.quantum

    def make_beat(self):
        self.busy -= 1

        if self.task:
            self.task.runtime -= 1

        if self.is_free() and self.remaining_tasks_amount > 0:
            self.free_time += 1

        if self.busy == 0:
            task = self.task
            self.task = None

            if task.runtime == 0:
                task.done = True
                self.remaining_tasks_amount -= 1
                return task

            task.quantum *= 2
            task.priority += 1
            return task

    def interrupt(self, newTask):
        if self.task == None:
            return

        oldTask = self.task
        oldTask.quantum = self.busy

        self.execute(newTask)

        return oldTask
