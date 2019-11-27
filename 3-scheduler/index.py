from statistics import mean
from scheduler import *
from graph import *

ITERATIONS_COUNT = 40

research_types = {
    '1': 'Mean awaiting time versus density',
    '2': 'Resource free time versus density',
    '3': 'Tasks amount versus awaiting time'
}

def print_help():
    print("""Task scheduler model. Corbato algorithm

Available research types:
    1 - Mean awaiting time versus density
    2 - Resource free time versus density
    3 - Tasks amount versus awaiting time
    """)

def get_research_type():
    return input('Enter research type: ')

def get_parameters():
    tasks_amount = input('Enter tasks amount: ')
    density = input('Enter tasks density: ')
    max_runtime = input('Enter maximum task run time: ')
    return int(tasks_amount), int(density), int(max_runtime)

def cli():
    print_help()
    research_type = get_research_type()

    print()
    print(research_types[research_type])

    if research_type == '1':
        # Mean awaiting time versus density
        #
        tasks_amount = int(input('Enter tasks amount: '))
        max_runtime = int(input('Enter maximum task run time: '))

        x_density = []
        y_mean_awaiting_time = []

        for i in range(ITERATIONS_COUNT):
            density = i + 1
            scheduler = Scheduler(tasks_amount, density, max_runtime)
            scheduler.init_timeline()
            scheduler.start()
            mean_awaiting_time = mean(scheduler.awaiting_times)
            x_density.append(density)
            y_mean_awaiting_time.append(mean_awaiting_time)

        graph = Graph()
        graph.x_label('Density')
        graph.y_label('Mean awaiting time')

        graph.plot(x_density, y_mean_awaiting_time)

        graph.show()

    elif research_type == '2':
        # Resource free time versus density
        #
        tasks_amount = int(input('Enter tasks amount: '))
        max_runtime = int(input('Enter maximum task run time: '))

        x_density = []
        y_free_time = []

        for i in range(ITERATIONS_COUNT):
            density = i + 1
            scheduler = Scheduler(tasks_amount, density, max_runtime)
            scheduler.init_timeline()
            scheduler.start()
            x_density.append(density)
            y_free_time.append(scheduler.resource.free_time)

        graph = Graph()
        graph.x_label('Density')
        graph.y_label('Resource free time')

        graph.plot(x_density, y_free_time)

        graph.show()

    elif research_type == '3':
        # Tasks amount versus awaiting time
        #
        tasks_amount = int(input('Enter tasks amount: '))
        density = int(input('Enter tasks density: '))
        max_runtime = int(input('Enter maximum task run time: '))

        scheduler = Scheduler(tasks_amount, density, max_runtime)
        scheduler.init_timeline()
        scheduler.start()

        graph = Graph()
        graph.x_label('Awaiting time')
        graph.y_label('Tasks amount')

        scheduler.awaiting_times.sort()
        graph.histogram(scheduler.awaiting_times)
        graph.show()

cli()




