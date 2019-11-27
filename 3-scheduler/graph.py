import matplotlib.pyplot as plt

class Graph:
    def y_label(self, label):
        plt.ylabel(label)

    def x_label(self, label):
        plt.xlabel(label)

    def histogram(self, values):
        plt.hist(values, bins = 40, color='blue', edgecolor='black')

    def plot(self, x_values, y_values):
        plt.plot(x_values, y_values)

    def show(self):
        plt.show()
