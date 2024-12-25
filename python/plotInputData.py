import matplotlib.pyplot as plt
import numpy as np

def load_data_from_file(file_path):
    data = []
    with open(file_path, 'r') as file:
        next(file)  # ignore 1st line
        for line in file:
            values = list(map(float, line.split()))
            if len(values) == 3:
                data.append(values)
    return data

def plot_data(data):
    
    P = [row[0] for row in data]
    F = [row[1] for row in data]
    IF = [row[2] for row in data]
    plt.plot(P, label=f'Press')
    plt.plot(F, label=f'Flow')
    plt.plot(IF, label=f'Cumul volume')
    plt.legend()
    plt.show()

if __name__ == "__main__":
    file_path = "inputData.txt"
    data = load_data_from_file(file_path)
    plot_data(data)